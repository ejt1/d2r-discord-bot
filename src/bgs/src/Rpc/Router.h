#pragma once

#include "Request.h"
#include "Service.h"

#include <map>

namespace bgs {
namespace rpc {

class Channel;

class Router {
 public:
  Router();
  ~Router();

   template <typename T>
  void AddService() {
    // deleted in destructor :)
    T* service = new T{};
    services_[service->Hash()] = service;
  }

  asio::awaitable<Response> Process(std::shared_ptr<Channel> channel, Request&& request);

 private:
  std::map<uint32_t, Service*> services_;
};

}  // namespace rpc
}  // namespace bgs
