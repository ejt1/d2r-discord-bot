#pragma once

#include "BnetSession.h"

#include <vector>

namespace d2rc {

class BnetSession;

enum class RealmState {
  NotConnected,
  Connected,
  Authenticated,
};

class RealmSession : public bgs::rpc::Channel {
 public:
  RealmSession(std::shared_ptr<BnetSession> bgs_conn, asio::io_context& io, const std::string& name);
  virtual ~RealmSession();

  // D2rServicesConnection
  asio::awaitable<void> AuthSession(const std::string& web_credentials, const std::string& token);

  // GameVersion
  asio::awaitable<void> SetVersion(const std::string& version);

  // Leaderboard
  asio::awaitable<void> GetServerTime();

  // Accessors
  void SetHostname(const std::string& hostname);
  std::string& GetHostname();

  void SetWebCredentials(const std::string& web_credentials);
  std::string& GetWebCredentials();

  void SetToken(const std::string& token);
  std::string& GetToken();

  Region& GetRegion() {
    return bgs_conn_->GetRegion();
  }

 protected:
  void ProcessHeader(bgs::protocol::Header& header, const google::protobuf::Message& message) override;

 private:
  std::shared_ptr<BnetSession> bgs_conn_;
  std::string hostname_;
  std::string web_credentials_;
  std::string token_;
};

}  // namespace d2rc
