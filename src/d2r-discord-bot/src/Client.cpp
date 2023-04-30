#include "Client.h"

#include "ClientRepository.h"
#include "Services/ConnectionService.h"

#include <bgs/low/pb/client/api/client/v2/attribute_types.pb.h>
#include <bgs/low/pb/client/api/client/v2/game_utilities_service.pb.h>
#include <external/v1/d2r_connection.pb.h>

static const char* regionHosts[]{"eu.actual.battle.net", "us.actual.battle.net", "kr.actual.battle.net"};
constexpr auto regionPort = "1119";

namespace d2rc {

Client::Client(asio::io_context& io, const std::string& name, const Region& region, const std::string& web_credentials)
    : io_(io),
      bgs_conn_(std::make_shared<BnetSession>(io, name, region)),
      realm_conn_(std::make_shared<RealmSession>(bgs_conn_, io, name)),
      web_credentials_(web_credentials) {}

asio::awaitable<void> Client::Start() {
  co_await Connect(bgs_conn_->GetRegion());
  co_await Logon(web_credentials_);

  // AuthenticationService::GenerateWebCredentials
  // SessionService::CreateSession

  asio::steady_timer keep_alive_timer(io_);
  for (;;) {
    keep_alive_timer.expires_after(std::chrono::seconds(45));

    boost::system::error_code ec;
    co_await keep_alive_timer.async_wait(redirect_error(asio::use_awaitable, ec));
    if (!ec) {
      co_await KeepAlive();
    }
  }
}

asio::awaitable<void> Client::Connect(const Region& region) {
  co_await bgs_conn_->Connect(regionHosts[static_cast<size_t>(region)], regionPort);

  // service_hash = 0x65446991
  // method_id = 1
  // client_sdk_info = ClientSdkVersionInfo
  /*
  ClientSdkVersionInfo {
    sdk_name = BGS C++ SDK,
    sdk_version = {
      major_version = 3,
      minor_version = 3,
      patch_version = 1,
      version_string = 3.3.1
    },
    protocol_version = {
      major_version = 3,
      minor_version = 4,
      patch_version = 1,
      version_string = 3.04.1-f8c9619017
    }
  }
  */
  connection_v1::ConnectRequest request;

  auto* sdk_info = request.mutable_client_sdk_info();
  sdk_info->set_sdk_name("BGS C++ SDK");

  auto* sdk_version = sdk_info->mutable_sdk_version();
  sdk_version->set_major_version(3);
  sdk_version->set_minor_version(3);
  sdk_version->set_patch_version(1);
  sdk_version->set_version_string("3.3.1");

  auto* protocol_version = sdk_info->mutable_protocol_version();
  protocol_version->set_major_version(3);
  protocol_version->set_minor_version(4);
  protocol_version->set_patch_version(1);
  protocol_version->set_version_string("3.04.1-f8c9619017");

  connection_v1::ConnectionService_Stub stub(bgs_conn_);
  auto&& conn_res = (co_await stub.Connect(request)).Deserialize<connection_v1::ConnectResponse>();

  // server_id = ProcessId
  /*
  ProcessId {
    label = 3290412870,
    epoch = 1670288647
  }
  // client_id = ProcessId
  /*
  ProcessId {
    label = 1650438,
    epoch = 1676464972
  }
  */
  // server_time = 1676464972452748
  // binary_content_handle_array = ConnectionMeteringContentHandles
  /*
  ConnectionMeteringContentHandles {
    content_handle = {
      region = 17749,
      usage = 1835365490,
      hash = bytes
      proto_url = https://prod.depot.battle.net/${hash}.${usage}
    }
  }
  */
  bgs_conn_->SetSessionId(conn_res.session_id());
  ClientRepository::GetInstance()->Add(conn_res.session_id(), this);
}

asio::awaitable<void> Client::Logon(const std::string& web_token) {
  // clang-format off
  // service_hash = 0xDECFC01
  // method_id = 1
  // program = OSI
  // platform = Wn64
  // locale = enUS
  // version = BGS C++ SDK - v0.0.0
  // application_version = 1
  // allow_logon_queue_notifications = true
  // cached_web_credentials = WEB_TOKEN
  // device_id = Client-Id string
  /* Client-Id, all values are fnv1a hashes
  {
    "RGKY" : 0, Identity, as stored in Computer\HKEY_CURRENT_USER\SOFTWARE\Blizzard Entertainment\Battle.net\Identity\Identity
    "CPGE" : 0,
    "ULNG" : 0, GetUserDefaultLangID()
    "SLNG" : 0, Probably GetSystemDefaultLangID()?
    "CNME" : 0,
    "UNME" : 0,
    "UTCO" : 0, GetTimeZoneInformation
    "CARC" : 0,
    "CREV" : 0,
    "CLVL" : 0,
    "PMEM" : 0,
    "PSZE" : 0,
    "OVER" : 0, RtlGetVersion %d.%d.%d.%d %s
    "CVRA" : 0,
    "CFTC" : 0,
    "CFTD" : 0,
    "CEFC" : 0,
    "CEFD" : 0,
    "CBRD" : 0,
    "CVEN" : 0,
    "ANME" : 0,
    "ADSC" : 0,
    "MAC"  : 0,
  }
  */
  // clang-format on
  bgs::protocol::authentication::v1::LogonRequest request;
  request.set_program("OSI");
  request.set_platform("Wn64");
  request.set_locale("enUS");
  request.set_version("BGS C++ SDK - v0.0.0");
  request.set_application_version(1);
  request.set_allow_logon_queue_notifications(true);
  request.set_cached_web_credentials(web_token);

  const std::string device_id(
      "{ \"RGKY\" : 1644511417, \"CPGE\" : 194820301, \"ULNG\" : 1102284757, "
      "\"SLNG\" : 1151054009, \"CNME\" : 1210692535, \"UNME\" : 1008421003, "
      "\"UTCO\" : 1641370113, \"CARC\" : 1150887219, \"CREV\" : 962424505, "
      "\"CLVL\" : 696789934, \"PMEM\" : 891351586, \"PSZE\" : 1034928998, "
      "\"OVER\" : 817894658, \"CVRA\" : 474293621, \"CFTC\" : 1456863947, "
      "\"CFTD\" : 1293597112, \"CEFC\" : 1294775605, \"CEFD\" : 1050801917, "
      "\"CBRD\" : 1311596431, \"CVEN\" : 1162415567, \"ANME\" : 685474281, "
      "\"ADSC\" : 1184678494, \"MAC\" : 915802451 }");
  request.set_device_id(device_id);

  auth_v1::AuthenticationService_Stub stub(bgs_conn_);
  co_await stub.Logon(request);
}

asio::awaitable<void> Client::KeepAlive() {
  bgs::protocol::NoData request;

  connection_v1::ConnectionService_Stub stub(bgs_conn_);
  co_await stub.KeepAlive(request);
}

}  // namespace d2rc
