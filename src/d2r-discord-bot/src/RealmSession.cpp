#include "RealmSession.h"

#include "Services/Classic/D2rServicesConnection.h"
#include "Services/Classic/DiabloCloneExternalListener.h"
#include "Services/Classic/GameVersionListener.h"
#include "Services/Classic/GenericResourceListener.h"
#include "Services/Classic/LeaderboardListener.h"
#include "Services/Classic/LimitedTimeEventListener.h"
#include "Services/Classic/TerrorZonesListener.h"

#ifdef WIN32
#include <Rpc.h>                    // CreateUuid
#pragma comment(lib, "Rpcrt4.lib")  // CreateUuid
#endif

namespace d2rc {

RealmSession::RealmSession(std::shared_ptr<BnetSession> bgs_conn, asio::io_context& io, const std::string& name)
    : bgs::rpc::Channel(io, name), bgs_conn_(bgs_conn) {
  router_.AddService<D2rServicesConnectionServiceImpl>();
  router_.AddService<DiabloCloneExternalListenerImpl>();
  router_.AddService<GenericResourceListenerImpl>();
  router_.AddService<LeaderboardServiceImpl>();
  router_.AddService<LimitedTimeEventListenerImpl>();
  router_.AddService<TerrorZonesListenerImpl>();
}

RealmSession::~RealmSession() {}

asio::awaitable<void> RealmSession::AuthSession(const std::string& web_credentials, const std::string& token) {
  auto& account_info = bgs_conn_->GetAccountInfo();
  auto& game_account = account_info.GameAccounts[0].GameAccountId;

  d2r_connection_v1::AuthSessionRequest request;
  request.set_session_key(token);
  request.set_program('OSI');
  request.set_version("1.5.73090");
  request.set_session_token("JMgPfro+DyPnrg+ze9qOmrX/8iU=");
  request.set_unknown6(0x30100);
  request.set_platform('Wn64');
  request.set_unknown8(1);
  request.set_unknown11("");
  request.set_unknown12("");

  auto* session_info = request.mutable_session_info();
  session_info->set_web_credentials(web_credentials);
  session_info->set_unknown2(0);
  session_info->set_platform('Wn64');
  session_info->set_unknown8(0);

  auto* aurora = session_info->mutable_aurora_account();
  aurora->set_account_high(account_info.AccountId.high);
  aurora->set_account_low(account_info.AccountId.low);
  aurora->set_game_account_high(game_account.high);
  aurora->set_game_account_low(game_account.low);

  d2r_connection_v1::D2rServicesConnectionService_Stub stub(shared_from_this());
  co_await stub.AuthSession(request);
}

asio::awaitable<void> RealmSession::SetVersion(const std::string& version) {
  game_version_v1::SetGameVersionRequest request;
  request.set_version("1.5.73090");

  game_version_v1::GameVersionService_Stub stub(shared_from_this());
  co_await stub.SetGameVersion(request);
}

asio::awaitable<void> RealmSession::GetServerTime() {
  leaderboard_v1::GetServerTimeRequest request;

  leaderboard_v1::LeaderboardService_Stub stub(shared_from_this());
  co_await stub.GetServerTime(request);
}

void RealmSession::SetHostname(const std::string& hostname) {
  hostname_ = hostname;
}

std::string& RealmSession::GetHostname() {
  return hostname_;
}

void RealmSession::SetWebCredentials(const std::string& web_credentials) {
  web_credentials_ = web_credentials;
}

std::string& RealmSession::GetWebCredentials() {
  return web_credentials_;
}

void RealmSession::SetToken(const std::string& token) {
  token_ = token;
}

std::string& RealmSession::GetToken() {
  return token_;
}

void RealmSession::ProcessHeader(bgs::protocol::Header& header, const google::protobuf::Message& message) {
  header.set_object_id(2525111537);
  header.set_size(message.ByteSize());
  header.set_status(0);
  header.set_is_response(false);
#ifdef WIN32
  // XXX: this is trash because UuidToString gives us lowercase
  UUID uuid{};
  UuidCreate(&uuid);
  char* uuid_str = nullptr;
  UuidToString(&uuid, reinterpret_cast<RPC_CSTR*>(&uuid_str));
  std::string uuid_final = uuid_str;
  RpcStringFree(reinterpret_cast<RPC_CSTR*>(&uuid_str));
  std::transform(uuid_final.begin(), uuid_final.end(), uuid_final.begin(), ::toupper);
  header.set_request_id(std::format("RT-{}", uuid_final));
#endif
}

}  // namespace d2rc
