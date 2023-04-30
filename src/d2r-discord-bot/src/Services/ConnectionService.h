#pragma once

#include "Rpc/Service.h"

#include <bgs/low/pb/client/connection_service.pb.h>

namespace d2rc {

namespace connection_v1 = bgs::protocol::connection::v1;

class ConnectionServiceImpl : public connection_v1::ConnectionService {
 protected:
  asio::awaitable<bgs::rpc::Response> Echo(std::shared_ptr<bgs::rpc::Channel> channel,
                                           const bgs::protocol::Header& header,
                                           const connection_v1::EchoRequest& request) override;
};

}  // namespace d2rc
