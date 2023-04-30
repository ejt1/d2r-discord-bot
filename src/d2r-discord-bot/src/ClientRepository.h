#pragma once

#include <map>
#include <memory>
#include <string>

namespace d2rc {

class Client;

class ClientRepository {
  ClientRepository() {}
  ~ClientRepository() {}

 public:
  static ClientRepository* GetInstance() {
    static ClientRepository instance_;
    return &instance_;
  }

  Client* Add(const std::string& clientid, Client* client);
  Client* Remove(const std::string& clientid);

  Client* Get(const std::string& clientid);

 private:
  std::map<std::string, Client*> clients_;
};

}  // namespace d2rc
