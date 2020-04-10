#include "consumer.hpp"

namespace ndn {
namespace chunks {

Consumer::Consumer(security::v2::Validator& validator, std::ostream& os)
  : m_validator(validator)
  , m_outputStream(os)
  , m_nextToPrint(0)
{
}

void
Consumer::run(unique_ptr<DiscoverVersion> discover, unique_ptr<PipelineInterests> pipeline)
{
  m_discover = std::move(discover);
  m_pipeline = std::move(pipeline);
  m_nextToPrint = 0;
  m_bufferedData.clear();

  m_discover->onDiscoverySuccess.connect([this] (const Data& data) {
    m_pipeline->run(data,
      [this] (const Data& data) { handleData(data); },
      [] (const std::string& msg) { BOOST_THROW_EXCEPTION(std::runtime_error(msg)); });
  });
  m_discover->onDiscoveryFailure.connect([] (const std::string& msg) {
    BOOST_THROW_EXCEPTION(std::runtime_error(msg));
  });
  m_discover->run();
}

void
Consumer::handleData(const Data& data)
{
  auto dataPtr = data.shared_from_this();

  m_validator.validate(data,
    [this, dataPtr] (const Data& data) {
      if (data.getContentType() == ndn::tlv::ContentType_Nack) {
        BOOST_THROW_EXCEPTION(ApplicationNackError(data));
      }

      // 'data' passed to callback comes from DataValidationState and was not created with make_shared
      m_bufferedData[getSegmentFromPacket(data)] = dataPtr;
      writeInOrderData();
    },
    [] (const Data&, const security::v2::ValidationError& error) {
      BOOST_THROW_EXCEPTION(DataValidationError(error));
    });
}

void
Consumer::writeInOrderData()
{
  for (auto it = m_bufferedData.begin();
       it != m_bufferedData.end() && it->first == m_nextToPrint;
       it = m_bufferedData.erase(it), ++m_nextToPrint) {
    const Block& content = it->second->getContent();
    m_outputStream.write(reinterpret_cast<const char*>(content.value()), content.value_size());
  }
}

} // namespace chunks
} // namespace ndn
