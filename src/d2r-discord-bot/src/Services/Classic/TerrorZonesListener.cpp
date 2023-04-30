#include "Services/Classic/TerrorZonesListener.h"

#include "Rpc/Channel.h"
#include "Terrorized.h"

#include <fmt/ranges.h>

namespace d2rc {

// clang-format off
static std::map<uint32_t, std::string> s_LevelNameLookup{
  // Act 1
  {1, "Rogue Encampment"},
  {2, "Blood Moor"},
  {3, "Cold Plains"},
  {4, "Stony Field"},
  {5, "Dark Wood"},
  {6, "Black Marsh"},
  {7, "Tamoe Highland"},
  {8, "Den of Evil"},
  {9, "Cave 1"},
  {10, "Underground Passage 1"},
  {11, "Hole 1"},
  {12, "Pit 1"},
  {13, "Cave 2"},
  {14, "Underground Passage 2"},
  {15, "Hole 2"},
  {16, "Pit 2"},
  {17, "Burial Grounds"},
  {18, "Crypt"},
  {19, "Mausoleum"},
  {20, "Forgotten Tower"},
  {21, "Tower Cellar 1"},
  {22, "Tower Cellar 2"},
  {23, "Tower Cellar 3"},
  {24, "Tower Cellar 4"},
  {25, "Tower Cellar 5"},
  {26, "Monastery Gate"},
  {27, "Outer Cloister"},
  {28, "Barracks"},
  {29, "Jail 1"},
  {30, "Jail 2"},
  {31, "Jail 3"},
  {32, "Inner Cloister"},
  {33, "Cathedral 1"},
  {34, "Catacombs 1"},
  {35, "Catacombs 2"},
  {36, "Catacombs 3"},
  {37, "Catacombs 4"},
  {38, "Tristram"},
  {39, "The Secret Cow Level"},

  // Act 2
  {40, "Lut Gholein"},
  {41, "Rocky Waste"},
  {42, "Dry Hills"},
  {43, "Far Oasis"},
  {44, "Lost City"},
  {45, "Valley of Snakes"},
  {46, "Canyon of the Magi"},
  {47, "Sewers 1"},
  {48, "Sewers 2"},
  {49, "Sewers 3"},
  {50, "Harem 1"},
  {51, "Harem 2"},
  {52, "Palace Cellar 1"},
  {53, "Palace Cellar 2"},
  {54, "Palace Cellar 3"},
  {55, "Stony Tomb 1"},
  {56, "Halls of the Dead 1"},
  {57, "Halls of the Dead 2"},
  {58, "Claw Viper Temple 1"},
  {59, "Stony Tomb 2"},
  {60, "Halls of the Dead 3"},
  {61, "Claw Viper Temple 2"},
  {62, "Maggot Lair 1"},
  {63, "Maggot Lair 2"},
  {64, "Maggot Lair 3"},
  {65, "Ancient Tunnels"},
  {66, "Tal Rashas Tomb 1"},
  {67, "Tal Rashas Tomb 2"},
  {68, "Tal Rashas Tomb 3"},
  {69, "Tal Rashas Tomb 4"},
  {70, "Tal Rashas Tomb 5"},
  {71, "Tal Rashas Tomb 6"},
  {72, "Tal Rashas Tomb 7"},
  {73, "Tal Rashas Chamber"},
  {74, "Arcane Sanctuary"},

  // Act 3
  {75, "Kurast Docks"},
  {76, "Spider Forest"},
  {77, "Great Marsh"},
  {78, "Flayer Jungle"},
  {79, "Lower Kurast"},
  {80, "Kurast Bazaar"},
  {81, "Upper Kurast"},
  {82, "Kurast Causeway"},
  {83, "Travincal"},
  {84, "Archnid Lair"},
  {85, "Spider Cavern"},
  {86, "Swampy Pit 1"},
  {87, "Swampy Pit 2"},
  {88, "Flayer Dungeon 1"},
  {89, "Flayer Dungeon 2"},
  {90, "Swampy Pit 3"},
  {91, "Player Dungeon 3"},
  {92, "Sewers 1"},
  {93, "Sewers 2"},
  {94, "Ruined Temple"},
  {95, "Disused Fane"},
  {96, "Forgotten Reliquary"},
  {97, "Forgotten Temple"},
  {98, "Ruined Fane"},
  {99, "Disused Reliquary"},
  {100, "Durance of Hate 1"},
  {101, "Durance of Hate 2"},
  {102, "Durance of Hate 3"},

  // Act 4
  {103, "The Pandemonium Fortress"},
  {104, "Outer Stepps"},
  {105, "Plains of Despair"},
  {106, "City of the Damned"},
  {107, "River of Flame"},
  {108, "The Chaos Sanctuary"},

  // Act 5
  {109, "Harrogath"},
  {110, "The Bloody Foothills"},
  {111, "Frigid Highlands"},
  {112, "Arreat Plateau"},
  {113, "Crystalline Passage"},
  {114, "Frozen River"},
  {115, "Glacial Trail"},
  {116, "Drifter Cavern"},
  {117, "Frozen Tundra"},
  {118, "The Ancients Way"},
  {119, "Icy Cellar"},
  {120, "Arreat Summit"},
  {121, "Nihlathaks Temple"},
  {122, "Halls of Anguish"},
  {123, "Halls of Pain"},
  {124, "Halls of Vaught"},
  {125, "Abaddon"},
  {126, "Pit of Acheron"},
  {127, "Infernal Pit"},
  {128, "Worldstone Keep 1"},
  {129, "Worldstone Keep 2"},
  {130, "Worldstone Keep 3"},
  {131, "Throne of Destruction"},
  {132, "Worldstone Keep"},

  // Ubers
  {133, "Matrons Den (Uber)"},
  {134, "Forgotten Sands (Uber)"},
  {135, "Furnace of Pain (Uber)"},
  {136, "Trustram (Uber)"},
};
// clang-format on

asio::awaitable<bgs::rpc::Response> TerrorZonesListenerImpl::TerrorZonesMessage(
    std::shared_ptr<bgs::rpc::Channel> channel, const bgs::protocol::Header& header,
    const terror_v1::TerrorZonesMessageRequest& request) {
  std::vector<std::string> current_terrorized;
  std::vector<std::string> next_terrorized;

  for (size_t i = 0; i < request.current_size(); ++i) {
    current_terrorized.push_back(s_LevelNameLookup[request.current(i)]);
  }
  for (size_t i = 0; i < request.next_size(); ++i) {
    next_terrorized.push_back(s_LevelNameLookup[request.next(i)]);
  }
  if (request.minutes_remaining() == 0) {
    spdlog::get(channel->GetName())->info("New terrorized zones: {}", next_terrorized);
    Terrorized::GetInstance()->SetCurrent(next_terrorized);
    Terrorized::GetInstance()->SetNext({});
  } else {
    spdlog::get(channel->GetName())
        ->info("Terrorized zones for another {} minutes: {}", request.minutes_remaining(), current_terrorized);
    spdlog::get(channel->GetName())->info("Next zones: {}", next_terrorized);
    Terrorized::GetInstance()->SetCurrent(current_terrorized);
    Terrorized::GetInstance()->SetNext(next_terrorized);
  }

  bgs::protocol::NoData response;
  co_return bgs::rpc::Response(response);
}

}  // namespace d2rc
