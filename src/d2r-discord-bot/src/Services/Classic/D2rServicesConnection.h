#pragma once

#include "Rpc/Service.h"

#include <external/v1/d2r_connection.pb.h>

namespace d2rc {

namespace d2r_connection_v1 = classic::protocol::external::v1::d2r_connection;

class D2rServicesConnectionServiceImpl : public d2r_connection_v1::D2rServicesConnectionService {
 protected:
  asio::awaitable<bgs::rpc::Response> AuthSession(std::shared_ptr<bgs::rpc::Channel> channel,
                                                  bgs::protocol::Header& header,
                                                  const d2r_connection_v1::AuthSessionRequest& request);

  asio::awaitable<bgs::rpc::Response> SessionUpdate(std::shared_ptr<bgs::rpc::Channel> channel,
                                                    bgs::protocol::Header& header,
                                                    const d2r_connection_v1::SessionUpdateRequest& request);

  asio::awaitable<bgs::rpc::Response> SessionDestroyed(std::shared_ptr<bgs::rpc::Channel> channel,
                                                       bgs::protocol::Header& header,
                                                       const d2r_connection_v1::SessionDestroyedRequest& request);

  asio::awaitable<bgs::rpc::Response> CookieUpdate(std::shared_ptr<bgs::rpc::Channel> channel,
                                                   bgs::protocol::Header& header,
                                                   const d2r_connection_v1::CookieUpdateRequest& request);

  asio::awaitable<bgs::rpc::Response> Ping(std::shared_ptr<bgs::rpc::Channel> channel, bgs::protocol::Header& header,
                                           bgs::protocol::NoData& request);

  asio::awaitable<bgs::rpc::Response> ServerPing(std::shared_ptr<bgs::rpc::Channel> channel,
                                                 bgs::protocol::Header& header,
                                                 const d2r_connection_v1::ServerPingRequest& request);
};

}  // namespace d2rc
