#pragma once

#include "BnetSession.h"
#include "RealmSession.h"

#include <memory>

namespace d2rc {

class Client : public std::enable_shared_from_this<Client> {
 public:
  Client(asio::io_context& io, const std::string& name, const Region& region, const std::string& web_credentials);

  asio::awaitable<void> Start();

  asio::awaitable<void> Connect(const Region& region);
  asio::awaitable<void> Logon(const std::string& web_token);
  asio::awaitable<void> KeepAlive();

  std::shared_ptr<BnetSession> GetBgsConn() {
    return bgs_conn_;
  }

  std::shared_ptr<RealmSession> GetRealmConn() {
    return realm_conn_;
  }

 private:
  asio::io_context& io_;
  std::shared_ptr<BnetSession> bgs_conn_;
  std::shared_ptr<RealmSession> realm_conn_;
  std::string web_credentials_;
};

}  // namespace d2rc
