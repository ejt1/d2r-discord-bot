#pragma once

#include "Services/AuthenticationListener.h"
#include "Services/Classic/D2rServicesConnection.h"
#include "Services/SessionListener.h"

#include <Rpc/Channel.h>

#include <vector>

// XXX: move to Services/??/GameUtilitiesService.h
#include <bgs/low/pb/client/api/client/v2/game_utilities_service.pb.h>

namespace d2rc {

namespace game_utilities_v2 = bgs::protocol::game_utilities::v2::client;

struct EntityId {
  uint64_t high;
  uint64_t low;
};

struct GameAccountInfo {
  EntityId GameAccountId;
};

struct AccountInfo {
  EntityId AccountId;
  std::vector<GameAccountInfo> GameAccounts;
  std::string Email;
  std::vector<uint32_t> AvailableRegions;
  uint32_t ConnectedRegion;
  std::string BattleTag;
  std::string GeoIpCountry;
};

enum class BgsState {
  NotConnected,
  Connected,
  Authenticated,
};

enum class Region { EU, US, KR };

class BnetSession : public bgs::rpc::Channel {
 public:
  BnetSession(asio::io_context& io, const std::string& name, const Region& region);
  virtual ~BnetSession();

  // override for WebSocket upgrade
  void OnUpgrade(beast::websocket::request_type& req) override;

  // Authentication
  asio::awaitable<void> OnLogonComplete(const auth_v1::LogonResult result);
  asio::awaitable<auth_v1::GenerateWebCredentialsResponse> GenerateWebCredentials(uint32_t program_id);

  // Session
  asio::awaitable<void> OnSessionCreated(const session_v1::SessionCreatedNotification& request);
  asio::awaitable<void> CreateSession(uint32_t program, uint32_t platform, uint32_t locale);

  // accessors
  AccountInfo& GetAccountInfo();
  void SetSessionId(const std::string& id);
  std::string& GetSessionId();
  void SetSessionKey(const std::string& data);
  std::string& GetSessionKey();
  Region& GetRegion();

 private:
  AccountInfo account_info_;
  std::string session_id_;
  std::string session_key_;
  Region region_;
};

}  // namespace d2rc
