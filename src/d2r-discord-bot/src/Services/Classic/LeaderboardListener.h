#pragma once

#include "Rpc/Service.h"

#include <external/v1/leaderboard.pb.h>

namespace d2rc {

namespace leaderboard_v1 = classic::protocol::external::v1::leaderboard;

class LeaderboardServiceImpl : public leaderboard_v1::LeaderboardService {
 protected:
  asio::awaitable<bgs::rpc::Response> GetLeaderboardIds(
      std::shared_ptr<bgs::rpc::Channel> channel, const bgs::protocol::Header& header,
      const leaderboard_v1::GetLeaderboardIdsRequest& request) override;

  asio::awaitable<bgs::rpc::Response> GetServerTime(std::shared_ptr<bgs::rpc::Channel> channel,
                                                    const bgs::protocol::Header& header,
                                                    const leaderboard_v1::GetServerTimeRequest& request) override;

  asio::awaitable<bgs::rpc::Response> SetSeasonInfo(std::shared_ptr<bgs::rpc::Channel> channel,
                                                    const bgs::protocol::Header& header,
                                                    const leaderboard_v1::SetSeasonInfoRequest& request) override;
};

}  // namespace d2rc
