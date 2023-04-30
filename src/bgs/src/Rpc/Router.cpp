#include "Rpc/Router.h"

#include "Channel.h"

namespace bgs {
namespace rpc {

Router::Router() {}

Router::~Router() {
  for (auto& [hash, service] : services_) {
    delete service;
    service = nullptr;
  }
}

asio::awaitable<Response> Router::Process(std::shared_ptr<Channel> channel, Request&& request) {
  uint32_t service_hash = request.ServiceHash();
  if (services_.contains(service_hash)) {
    co_return co_await services_[service_hash]->CallMethod(channel, std::ref(request));
  } else {
    spdlog::error("No service registered for service hash {:08X}", service_hash);
    co_return ErrorCode::RPC_INVALID_SERVICE;
  }
}

}  // namespace rpc
}  // namespace bgs
