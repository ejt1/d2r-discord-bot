#pragma once

#include "Rpc/Service.h"

#include <bgs/low/pb/client/session_service.pb.h>

namespace d2rc {

namespace session_v1 = bgs::protocol::session::v1;

class SessionListenerImpl : public session_v1::SessionListener {
 protected:
  asio::awaitable<bgs::rpc::Response> OnSessionCreated(std::shared_ptr<bgs::rpc::Channel> channel,
                                                       const bgs::protocol::Header& header,
                                                       const session_v1::SessionCreatedNotification& request) override;
};

}  // namespace d2rc
