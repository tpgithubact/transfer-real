

#include "ndnpeek.hpp"

namespace ndn {
namespace peek {

NdnPeek::NdnPeek(Face& face, const PeekOptions& options)
  : m_face(face)
  , m_options(options)
  , m_timeout(options.timeout)
  , m_resultCode(ResultCode::TIMEOUT)
{
  if (m_timeout < 0_ms) {
    m_timeout = m_options.interestLifetime < 0_ms ?
                DEFAULT_INTEREST_LIFETIME : m_options.interestLifetime;
  }
}

time::milliseconds
NdnPeek::getTimeout() const
{
  return m_timeout;
}

ResultCode
NdnPeek::getResultCode() const
{
  return m_resultCode;
}

void
NdnPeek::start()
{
  m_face.expressInterest(createInterest(),
                         bind(&NdnPeek::onData, this, _2),
                         bind(&NdnPeek::onNack, this, _2),
                         nullptr);
  m_expressInterestTime = time::steady_clock::now();
}

Interest
NdnPeek::createInterest() const
{
  Interest interest(m_options.name);
  interest.setCanBePrefix(m_options.canBePrefix);
  interest.setMustBeFresh(m_options.mustBeFresh);
  if (m_options.link != nullptr) {
    interest.setForwardingHint(m_options.link->getDelegationList());
  }
  if (m_options.interestLifetime >= 0_ms) {
    interest.setInterestLifetime(m_options.interestLifetime);
  }

  if (m_options.isVerbose) {
    std::cerr << "INTEREST: " << interest << std::endl;
  }

  return interest;
}

void
NdnPeek::onData(const Data& data)
{
  m_resultCode = ResultCode::DATA;

  if (m_options.isVerbose) {
    std::cerr << "DATA, RTT: "
              << time::duration_cast<time::milliseconds>(time::steady_clock::now() - m_expressInterestTime).count()
              << "ms" << std::endl;
  }

  if (m_options.wantPayloadOnly) {
    const Block& block = data.getContent();
    std::cout.write(reinterpret_cast<const char*>(block.value()), block.value_size());
  }
  else {
    const Block& block = data.wireEncode();
    std::cout.write(reinterpret_cast<const char*>(block.wire()), block.size());
  }
}

void
NdnPeek::onNack(const lp::Nack& nack)
{
  m_resultCode = ResultCode::NACK;
  lp::NackHeader header = nack.getHeader();

  if (m_options.isVerbose) {
    std::cerr << "NACK, RTT: "
              << time::duration_cast<time::milliseconds>(time::steady_clock::now() - m_expressInterestTime).count()
              << "ms" << std::endl;
  }

  if (m_options.wantPayloadOnly) {
    std::cout << header.getReason() << std::endl;
  }
  else {
    const Block& block = header.wireEncode();
    std::cout.write(reinterpret_cast<const char*>(block.wire()), block.size());
  }
}

} // namespace peek
} // namespace ndn
