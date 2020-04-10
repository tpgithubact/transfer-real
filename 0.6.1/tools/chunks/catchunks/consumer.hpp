#ifndef NDN_TOOLS_CHUNKS_CATCHUNKS_CONSUMER_HPP
#define NDN_TOOLS_CHUNKS_CATCHUNKS_CONSUMER_HPP

#include "discover-version.hpp"
#include "pipeline-interests.hpp"

#include <ndn-cxx/security/v2/validation-error.hpp>
#include <ndn-cxx/security/v2/validator.hpp>

namespace ndn {
namespace chunks {

/**
 * @brief Segmented version consumer
 *
 * Discover the latest version of the data published under a specified prefix, and retrieve all the
 * segments associated to that version. The segments are fetched in order and written to a
 * user-specified stream in the same order.
 */
class Consumer : noncopyable
{
public:
  class ApplicationNackError : public std::runtime_error
  {
  public:
    explicit
    ApplicationNackError(const Data& data)
      : std::runtime_error("Application generated Nack: " + boost::lexical_cast<std::string>(data))
    {
    }
  };

  class DataValidationError : public std::runtime_error
  {
  public:
    explicit
    DataValidationError(const security::v2::ValidationError& error)
      : std::runtime_error(boost::lexical_cast<std::string>(error))
    {
    }
  };

  /**
   * @brief Create the consumer
   */
  explicit
  Consumer(security::v2::Validator& validator, std::ostream& os = std::cout);

  /**
   * @brief Run the consumer
   */
  void
  run(unique_ptr<DiscoverVersion> discover, unique_ptr<PipelineInterests> pipeline);

private:
  void
  handleData(const Data& data);

PUBLIC_WITH_TESTS_ELSE_PRIVATE:
  void
  writeInOrderData();

private:
  security::v2::Validator& m_validator;
  std::ostream& m_outputStream;
  unique_ptr<DiscoverVersion> m_discover;
  unique_ptr<PipelineInterests> m_pipeline;
  uint64_t m_nextToPrint;

PUBLIC_WITH_TESTS_ELSE_PRIVATE:
  std::map<uint64_t, shared_ptr<const Data>> m_bufferedData;
};

} // namespace chunks
} // namespace ndn

#endif // NDN_TOOLS_CHUNKS_CATCHUNKS_CONSUMER_HPP
