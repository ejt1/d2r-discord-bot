#pragma once

#include "Rpc/Service.h"

#include <external/v1/limited_time_event.pb.h>

namespace d2rc {

namespace event_v1 = classic::protocol::external::v1::limited_time_event;

class LimitedTimeEventListenerImpl : public event_v1::LimitedTimeEventListener {
 protected:
  asio::awaitable<bgs::rpc::Response> SetLimitedTimeEventStatus(
      std::shared_ptr<bgs::rpc::Channel> channel, const bgs::protocol::Header& header,
      const event_v1::SetLimitedTimeEventStatusRequest& request) override;
};

}  // namespace d2rc
