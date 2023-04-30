#include "Services/Classic/D2rServicesConnection.h"

#include "Rpc/Channel.h"

namespace d2rc {

asio::awaitable<bgs::rpc::Response> D2rServicesConnectionServiceImpl::AuthSession(
    std::shared_ptr<bgs::rpc::Channel> channel, bgs::protocol::Header& header,
    const d2r_connection_v1::AuthSessionRequest& request) {
  co_return bgs::rpc::ErrorCode::OK;
}

asio::awaitable<bgs::rpc::Response> D2rServicesConnectionServiceImpl::SessionUpdate(
    std::shared_ptr<bgs::rpc::Channel> channel, bgs::protocol::Header& header,
    const d2r_connection_v1::SessionUpdateRequest& request) {
  co_return bgs::rpc::ErrorCode::OK;
}

asio::awaitable<bgs::rpc::Response> D2rServicesConnectionServiceImpl::SessionDestroyed(
    std::shared_ptr<bgs::rpc::Channel> channel, bgs::protocol::Header& header,
    const d2r_connection_v1::SessionDestroyedRequest& request) {
  co_return bgs::rpc::ErrorCode::OK;
}

asio::awaitable<bgs::rpc::Response> D2rServicesConnectionServiceImpl::CookieUpdate(
    std::shared_ptr<bgs::rpc::Channel> channel, bgs::protocol::Header& header,
    const d2r_connection_v1::CookieUpdateRequest& request) {
  co_return bgs::rpc::ErrorCode::OK;
}

asio::awaitable<bgs::rpc::Response> D2rServicesConnectionServiceImpl::Ping(std::shared_ptr<bgs::rpc::Channel> channel,
                                                                           bgs::protocol::Header& header,
                                                                           bgs::protocol::NoData& request) {
  co_return bgs::rpc::ErrorCode::OK;
}

asio::awaitable<bgs::rpc::Response> D2rServicesConnectionServiceImpl::ServerPing(
    std::shared_ptr<bgs::rpc::Channel> channel, bgs::protocol::Header& header,
    const d2r_connection_v1::ServerPingRequest& request) {
  return asio::awaitable<bgs::rpc::Response>();
}

}  // namespace d2rc
