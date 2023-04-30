#include "Services/Classic/LimitedTimeEventListener.h"

#include "Rpc/Channel.h"

namespace d2rc {

asio::awaitable<bgs::rpc::Response> LimitedTimeEventListenerImpl::SetLimitedTimeEventStatus(
    std::shared_ptr<bgs::rpc::Channel> channel, const bgs::protocol::Header& header,
    const event_v1::SetLimitedTimeEventStatusRequest& request) {
  event_v1::SetLimitedTimeEventStatusResponse response;
  co_return bgs::rpc::Response(response);
}

}  // namespace d2rc
