#include "pipeline-interests-fixed-window.hpp"
#include "data-fetcher.hpp"

namespace ndn {
namespace chunks {

PipelineInterestsFixedWindow::PipelineInterestsFixedWindow(Face& face, const Options& options)
  : PipelineInterests(face)
  , m_options(options)
  , m_hasFailure(false)
{
  m_segmentFetchers.resize(m_options.maxPipelineSize);
}

PipelineInterestsFixedWindow::~PipelineInterestsFixedWindow()
{
  cancel();
}

void
PipelineInterestsFixedWindow::doRun()
{
  // if the FinalBlockId is unknown, this could potentially request non-existent segments
  for (size_t nRequestedSegments = 0;
       nRequestedSegments < m_options.maxPipelineSize;
       ++nRequestedSegments) {
    if (!fetchNextSegment(nRequestedSegments))
      // all segments have been requested
      break;
  }
}

bool
PipelineInterestsFixedWindow::fetchNextSegment(std::size_t pipeNo)
{
  if (isStopping())
    return false;

  if (m_hasFailure) {
    onFailure("Fetching terminated but no final segment number has been found");
    return false;
  }

  uint64_t nextSegmentNo = getNextSegmentNo();
  if (m_hasFinalBlockId && nextSegmentNo > m_lastSegmentNo)
    return false;

  // send interest for next segment
  if (m_options.isVerbose)
    std::cerr << "Requesting segment #" << nextSegmentNo << std::endl;

  Interest interest(Name(m_prefix).appendSegment(nextSegmentNo));
  interest.setInterestLifetime(m_options.interestLifetime);
  interest.setMustBeFresh(m_options.mustBeFresh);
  interest.setMaxSuffixComponents(1);

  auto fetcher = DataFetcher::fetch(m_face, interest,
                                    m_options.maxRetriesOnTimeoutOrNack,
                                    m_options.maxRetriesOnTimeoutOrNack,
                                    bind(&PipelineInterestsFixedWindow::handleData, this, _1, _2, pipeNo),
                                    bind(&PipelineInterestsFixedWindow::handleFail, this, _2, pipeNo),
                                    bind(&PipelineInterestsFixedWindow::handleFail, this, _2, pipeNo),
                                    m_options.isVerbose);

  BOOST_ASSERT(!m_segmentFetchers[pipeNo].first || !m_segmentFetchers[pipeNo].first->isRunning());
  m_segmentFetchers[pipeNo] = make_pair(fetcher, nextSegmentNo);

  return true;
}

void
PipelineInterestsFixedWindow::doCancel()
{
  for (auto& fetcher : m_segmentFetchers) {
    if (fetcher.first)
      fetcher.first->cancel();
  }

  m_segmentFetchers.clear();
}

void
PipelineInterestsFixedWindow::handleData(const Interest& interest, const Data& data, size_t pipeNo)
{
  if (isStopping())
    return;

  BOOST_ASSERT(data.getName().equals(interest.getName()));

  if (m_options.isVerbose)
    std::cerr << "Received segment #" << getSegmentFromPacket(data) << std::endl;

  onData(data);

  if (!m_hasFinalBlockId && data.getFinalBlock()) {
    m_lastSegmentNo = data.getFinalBlock()->toSegment();
    m_hasFinalBlockId = true;

    for (auto& fetcher : m_segmentFetchers) {
      if (fetcher.first == nullptr)
        continue;

      if (fetcher.second > m_lastSegmentNo) {
        // stop trying to fetch segments that are beyond m_lastSegmentNo
        fetcher.first->cancel();
      }
      else if (fetcher.first->hasError()) { // fetcher.second <= m_lastSegmentNo
        // there was an error while fetching a segment that is part of the content
        return onFailure("Failure retrieving segment #" + to_string(fetcher.second));
      }
    }
  }

  BOOST_ASSERT(m_nReceived > 0);
  if (m_hasFinalBlockId &&
      static_cast<uint64_t>(m_nReceived - 1) >= m_lastSegmentNo) { // all segments have been received
    if (!m_options.isQuiet) {
      printSummary();
    }
  }
  else {
    fetchNextSegment(pipeNo);
  }
}

void PipelineInterestsFixedWindow::handleFail(const std::string& reason, std::size_t pipeNo)
{
  if (isStopping())
    return;

  // if the failed segment is definitely part of the content, raise a fatal error
  if (m_hasFinalBlockId && m_segmentFetchers[pipeNo].second <= m_lastSegmentNo)
    return onFailure(reason);

  if (!m_hasFinalBlockId) {
    bool areAllFetchersStopped = true;
    for (auto& fetcher : m_segmentFetchers) {
      if (fetcher.first == nullptr)
        continue;

      // cancel fetching all segments that follow
      if (fetcher.second > m_segmentFetchers[pipeNo].second) {
        fetcher.first->cancel();
      }
      else if (fetcher.first->isRunning()) { // fetcher.second <= m_segmentFetchers[pipeNo].second
        areAllFetchersStopped = false;
      }
    }

    if (areAllFetchersStopped) {
      onFailure("Fetching terminated but no final segment number has been found");
    }
    else {
      m_hasFailure = true;
    }
  }
}

} // namespace chunks
} // namespace ndn
