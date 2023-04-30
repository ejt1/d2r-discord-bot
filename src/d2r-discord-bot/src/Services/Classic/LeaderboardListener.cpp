#include "Services/Classic/LeaderboardListener.h"

#include "Rpc/Channel.h"

namespace d2rc {

asio::awaitable<bgs::rpc::Response> LeaderboardServiceImpl::GetLeaderboardIds(
    std::shared_ptr<bgs::rpc::Channel> channel, const bgs::protocol::Header& header,
    const leaderboard_v1::GetLeaderboardIdsRequest& request) {
  leaderboard_v1::GetLeaderboardIdsResponse response;
  co_return bgs::rpc::Response(response);
}

asio::awaitable<bgs::rpc::Response> LeaderboardServiceImpl::GetServerTime(
    std::shared_ptr<bgs::rpc::Channel> channel, const bgs::protocol::Header& header,
    const leaderboard_v1::GetServerTimeRequest& request) {
  leaderboard_v1::GetServerTimeResponse response;
  co_return bgs::rpc::Response(response);
}

asio::awaitable<bgs::rpc::Response> LeaderboardServiceImpl::SetSeasonInfo(
    std::shared_ptr<bgs::rpc::Channel> channel, const bgs::protocol::Header& header,
    const leaderboard_v1::SetSeasonInfoRequest& request) {
  leaderboard_v1::SetSeasonInfoResponse response;
  co_return bgs::rpc::Response(response);
}

}  // namespace d2rc
