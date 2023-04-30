#include "Services/AuthenticationListener.h"

#include "BnetSession.h"
#include "Client.h"
#include "ClientRepository.h"

#include <bgs/low/pb/client/api/client/v2/attribute_types.pb.h>
#include <bgs/low/pb/client/api/client/v2/game_utilities_service.pb.h>
#include <bgs/low/pb/client/session_service.pb.h>
#include <external/v1/d2r_connection.pb.h>

namespace d2rc {

asio::awaitable<bgs::rpc::Response> AuthenticationListenerImpl::OnLogonComplete(
    std::shared_ptr<bgs::rpc::Channel> channel, const bgs::protocol::Header& header,
    const auth_v1::LogonResult& request) {
  auto bgs_conn = std::dynamic_pointer_cast<BnetSession>(channel);
  co_await bgs_conn->OnLogonComplete(request);

  co_return bgs::rpc::Response();
}

asio::awaitable<::bgs::rpc::Response> AuthenticationListenerImpl::OnLogonUpdate(
    std::shared_ptr<::bgs::rpc::Channel> channel, const bgs::protocol::Header& header,
    const auth_v1::LogonUpdateRequest& request) {
  co_return bgs::rpc::Response();
}

asio::awaitable<::bgs::rpc::Response> AuthenticationListenerImpl::OnLogonQueueUpdate(
    std::shared_ptr<::bgs::rpc::Channel> channel, const bgs::protocol::Header& header,
    const auth_v1::LogonQueueUpdateRequest& request) {
  co_return bgs::rpc::Response();
}

asio::awaitable<::bgs::rpc::Response> AuthenticationListenerImpl::OnLogonQueueEnd(
    std::shared_ptr<::bgs::rpc::Channel> channel, const bgs::protocol::Header& header,
    const ::bgs::protocol::NoData& request) {
  co_return bgs::rpc::Response();
}

}  // namespace d2rc
