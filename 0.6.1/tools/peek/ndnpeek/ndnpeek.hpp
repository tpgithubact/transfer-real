

#ifndef NDN_TOOLS_NDNPEEK_NDNPEEK_HPP
#define NDN_TOOLS_NDNPEEK_NDNPEEK_HPP

#include "core/common.hpp"
#include <ndn-cxx/link.hpp>

namespace ndn {
namespace peek {

/**
 * @brief options for NdnPeek
 */
struct PeekOptions
{
  // Interest construction options
  std::string name;
  bool canBePrefix = false;
  bool mustBeFresh = false;
  shared_ptr<Link> link;
  time::milliseconds interestLifetime = -1_ms;

  // output behavior options
  bool isVerbose = false;
  time::milliseconds timeout = -1_ms;
  bool wantPayloadOnly = false;
};

enum class ResultCode {
  NONE = -1,
  DATA = 0,
  NACK = 4,
  TIMEOUT = 3
};

class NdnPeek : boost::noncopyable
{
public:
  NdnPeek(Face& face, const PeekOptions& options);

  /**
   * @return the timeout
   */
  time::milliseconds
  getTimeout() const;

  /**
   * @return the result of Peek execution
   */
  ResultCode
  getResultCode() const;

  /**
   * @brief express the Interest
   * @note The caller must invoke face.processEvents() afterwards
   */
  void
  start();

private:
  Interest
  createInterest() const;

  /**
   * @brief called when a Data packet is received
   */
  void
  onData(const Data& data);

  /**
   * @brief called when a Nack packet is received
   */
  void
  onNack(const lp::Nack& nack);

private:
  Face& m_face;
  const PeekOptions& m_options;
  time::steady_clock::TimePoint m_expressInterestTime;
  time::milliseconds m_timeout;
  ResultCode m_resultCode;
};

} // namespace peek
} // namespace ndn

#endif // NDN_TOOLS_NDNPEEK_NDNPEEK_HPP
