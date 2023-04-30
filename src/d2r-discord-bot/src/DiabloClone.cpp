#include "DiabloClone.h"

namespace d2rc {

using namespace classic::protocol::external::v1::diablo_clone;

// clang-format off
static std::map<GameType, std::string> s_GameTypeNameLookup{
  { GameType::Classic,                    "Classic Softcore (Non ladder)"},
  { GameType::ClassicHardcore,            "Classic Hardcore (Non ladder)"},
  { GameType::ClassicLadder,              "Classic Softcore (Ladder)"},
  { GameType::ClassicLadderHardcore,      "Classic Hardcore (Ladder)"},
  { GameType::ExpansionNonLadder,         "Expansion Softcore (Non ladder)"},
  { GameType::ExpansionNonLadderHardcore, "Expansion Hardcore (Non ladder)"},
  { GameType::ExpansionLadder,            "Expansion Softcore (Ladder)"},
  { GameType::ExpansionLadderHardcore,    "Expansion Hardcore (Ladder)"},
};

static std::map<Region, std::string> s_RegionLookup{
  { Region::EU, "Europe" },
  { Region::US, "United States" },
  { Region::KR, "Asia" },
};
// clang-format on

std::string DiabloClone::GetStatus(const Region& region) {
  std::unique_lock<std::mutex> lock(uniq_mtx_);
  std::stringstream ss;

  auto& status = status_[static_cast<size_t>(region)];

  if (!status.initialized) {
    ss << "Diablo clone progression for " << s_RegionLookup[region] << " has not been collected yet.";
    return ss.str();
  }

  std::time_t t = std::chrono::system_clock::to_time_t(status.last_update);
  ss << "Diablo clone progression for " << s_RegionLookup[region] << ", last updated " << std::ctime(&t) << std::endl;

  for (size_t i = 0; i < 8; ++i) {
    ss << s_GameTypeNameLookup[static_cast<GameType>(i)] << " " << status.progression[i] << "/5\n";
  }

  auto time_since_update =
      std::chrono::duration_cast<std::chrono::hours>(std::chrono::system_clock::now() - status.last_update).count();
  if (time_since_update > 7 * 24) {
    ss << "\n_Time since last update for this region is very old and may "
          "suggest the scraper is offline._\n\n";
  }
  return ss.str();
}

void DiabloClone::SetStatus(const Region& region,
                            classic::protocol::external::v1::diablo_clone::GameType gametype, uint32_t progress) {
  std::unique_lock<std::mutex> lock(uniq_mtx_);

  auto& status = status_[static_cast<size_t>(region)];
  status.last_update = std::chrono::system_clock::now();
  status.initialized = true;
  status.progression[gametype] = progress;
}

}  // namespace d2rc
