#include "ClientRepository.h"

namespace d2rc {

Client* ClientRepository::Add(const std::string& clientid, Client* client) {
  clients_[clientid] = client;
  return client;
}

Client* ClientRepository::Remove(const std::string& clientid) {
  auto it = clients_.find(clientid);
  if (it != clients_.end()) {
    Client* removed = (*it).second;
    clients_.erase(it);
    return removed;
  }
  return nullptr;
}

Client* ClientRepository::Get(const std::string& clientid) {
  return clients_[clientid];
}

}  // namespace d2rc
