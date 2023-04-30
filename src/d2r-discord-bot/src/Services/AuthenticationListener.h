#pragma once

#include "Rpc/Service.h"

#include <bgs/low/pb/client/authentication_service.pb.h>

namespace d2rc {

namespace auth_v1 = bgs::protocol::authentication::v1;

class AuthenticationListenerImpl : public auth_v1::AuthenticationListener {
 protected:
  asio::awaitable<bgs::rpc::Response> OnLogonComplete(std::shared_ptr<bgs::rpc::Channel> channel,
                                                      const bgs::protocol::Header& header,
                                                      const auth_v1::LogonResult& request) override;

  asio::awaitable<::bgs::rpc::Response> OnLogonUpdate(std::shared_ptr<::bgs::rpc::Channel> channel,
                                                      const bgs::protocol::Header& header,
                                                      const auth_v1::LogonUpdateRequest& request) override;

  asio::awaitable<::bgs::rpc::Response> OnLogonQueueUpdate(std::shared_ptr<::bgs::rpc::Channel> channel,
                                                           const bgs::protocol::Header& header,
                                                           const auth_v1::LogonQueueUpdateRequest& request) override;

  asio::awaitable<::bgs::rpc::Response> OnLogonQueueEnd(std::shared_ptr<::bgs::rpc::Channel> channel,
                                                        const bgs::protocol::Header& header,
                                                        const ::bgs::protocol::NoData& request) override;
};

}  // namespace d2rc
