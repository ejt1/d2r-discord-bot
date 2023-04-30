#include "BnetSession.h"

#include "Client.h"
#include "ClientRepository.h"
#include "Services/AuthenticationListener.h"
#include "Services/ConnectionService.h"
#include "Services/SessionListener.h"

namespace d2rc {

BnetSession::BnetSession(asio::io_context& io, const std::string& name, const Region& region)
    : bgs::rpc::Channel(io, name), region_(region) {
  router_.AddService<AuthenticationListenerImpl>();
  router_.AddService<ConnectionServiceImpl>();
  router_.AddService<SessionListenerImpl>();
}

BnetSession::~BnetSession() {}

void BnetSession::OnUpgrade(beast::websocket::request_type& req) {
  req.set(beast::http::field::sec_websocket_protocol, "v1.rpc.battle.net");
}

asio::awaitable<void> BnetSession::OnLogonComplete(const auth_v1::LogonResult result) {
  auto& account_info = GetAccountInfo();

  auto& account_id = result.account_id();
  account_info.AccountId = {account_id.high(), account_id.low()};
  auto& game_accounts = result.game_account_id();
  for (int i = 0; i < game_accounts.size(); ++i) {
    auto& game_account_id = game_accounts.Get(i);
    account_info.GameAccounts.push_back({game_account_id.high(), game_account_id.low()});
  }
  account_info.Email = result.email();
  auto& regions = result.available_region();
  for (int i = 0; i < regions.size(); ++i) {
    auto& region = regions.Get(i);
    account_info.AvailableRegions.push_back(region);
  }
  account_info.ConnectedRegion = result.connected_region();
  account_info.BattleTag = result.battle_tag();
  account_info.GeoIpCountry = result.geoip_country();

  SetSessionKey(result.session_key());

  // Generate web credentials and set it on the realm connection
  auto&& credentials = co_await GenerateWebCredentials('OSI');
  Client* client = ClientRepository::GetInstance()->Get(session_id_);
  auto realm = client->GetRealmConn();
  realm->SetWebCredentials(credentials.web_credentials());

  co_await CreateSession('OSI', 'Wn64', 'enUS');
}

asio::awaitable<auth_v1::GenerateWebCredentialsResponse> BnetSession::GenerateWebCredentials(uint32_t program_id) {
  bgs::protocol::authentication::v1::GenerateWebCredentialsRequest request;
  request.set_program(program_id);

  auth_v1::AuthenticationService_Stub stub(shared_from_this());
  auto&& response = co_await stub.GenerateWebCredentials(request);
  co_return response.Deserialize<auth_v1::GenerateWebCredentialsResponse>();
}

asio::awaitable<void> BnetSession::OnSessionCreated(const session_v1::SessionCreatedNotification& request) {
  // attribute = Attribute[]
  /*
  Attribute {
    name = server_instance,
    value = {
      string_value = Release
    }
  },
  Attribute {
    name = client_request,
    value = {
      string_value = classic.protocol.v1.d2r_connection.LoginRequest
    }
  }
  */
  // payload = Attribute
  /*
  Attribute {
    name = protobuf_request,
    value = {
      blob_value = d2r_connection.LoginRequest {
        program = 'OSI'
        version = 1.15.0.71776,
        platform = 'Wn64'
      }
    }
  }
  */
  game_utilities_v2::ProcessTaskRequest gu_request;

  {
    auto* attr = gu_request.add_attribute();
    attr->set_name("server_instance");

    auto* val = attr->mutable_value();
    val->set_string_value("Release");
  }
  {
    auto* attr = gu_request.add_attribute();
    attr->set_name("client_request");

    auto* val = attr->mutable_value();
    val->set_string_value("classic.protocol.v1.d2r_connection.LoginRequest");
  }

  d2r_connection_v1::LoginRequest login_request;
  login_request.set_program('OSI');
  login_request.set_version("1.15.0.73090");
  login_request.set_platform('Wn64');

  // write message
  bgs::ByteBuffer buf(login_request.ByteSize());
  auto ptr = buf.GetWritePointer();
  buf.WriteCompleted(login_request.GetCachedSize());
  login_request.SerializePartialToArray(ptr, login_request.GetCachedSize());

  {
    auto* payload = gu_request.add_payload();
    payload->set_name("protobuf_request");

    auto* val = payload->mutable_value();
    val->set_blob_value(buf.GetReadPointer(), buf.GetActiveSize());
  }

  Client* client = ClientRepository::GetInstance()->Get(session_id_);
  auto realm = client->GetRealmConn();

  game_utilities_v2::GameUtilitiesService_Stub gu_stub(shared_from_this());
  auto&& response = co_await gu_stub.ProcessTask(gu_request);

  auto res = response.Deserialize<game_utilities_v2::ProcessTaskResponse>();

  // a few sanity checks
  if (res.result_size() < 2) {
    throw std::out_of_range("res.result_size < 2");
  }
  auto& client_response = res.result(0);
  auto& protobuf_response = res.result(1);
  auto& val = protobuf_response.value();
  if (!val.has_string_value()) {
    throw std::runtime_error("!val.has_string_value");
  }

  classic::protocol::external::v1::d2r_connection::LoginResponse login_response;
  if (!login_response.ParseFromString(val.string_value())) {
    throw std::runtime_error("Failed to parse LoginResponse");
  }

  spdlog::get(GetName())->debug("LoginResponse {}", login_response.ShortDebugString());

  std::string hostname = login_response.hostname();
  // clean the hostname, could do with some regex here
  // remove 'wss://'
  hostname = hostname.substr(hostname.find_first_of("/") + 2);
  // remove trailing '/'
  hostname = hostname.substr(0, hostname.find_last_of("/"));
  realm->SetHostname(hostname);
  realm->SetToken(login_response.token());

  co_await realm->Connect(realm->GetHostname(), "443", "/OSI/v2/rpc/client");
  co_await realm->AuthSession(realm->GetWebCredentials(), realm->GetToken());
  co_await realm->SetVersion("1.5.73090");
  co_await realm->GetServerTime();
}

asio::awaitable<void> BnetSession::CreateSession(uint32_t program, uint32_t platform, uint32_t locale) {
  bgs::protocol::session::v1::CreateSessionRequest request;

  auto* identity = request.mutable_identity();

  auto* account = identity->mutable_account();
  account->set_id(account_info_.AccountId.low);

  auto* game_account = identity->mutable_game_account();
  game_account->set_id(account_info_.GameAccounts[0].GameAccountId.low);
  game_account->set_program(program);
  game_account->set_region(100);

  request.set_platform(platform);
  request.set_locale(locale);
  request.set_application_version(1);
  request.set_session_key(GetSessionKey());

  session_v1::SessionService_Stub stub(shared_from_this());
  co_await stub.CreateSession(request);
}

AccountInfo& BnetSession::GetAccountInfo() {
  return account_info_;
}

void BnetSession::SetSessionId(const std::string& id) {
  session_id_ = id;
}

std::string& BnetSession::GetSessionId() {
  return session_id_;
}

void BnetSession::SetSessionKey(const std::string& data) {
  session_key_ = {data.begin(), data.end()};
}

std::string& BnetSession::GetSessionKey() {
  return session_key_;
}

Region& BnetSession::GetRegion() {
  return region_;
}

}  // namespace d2rc
