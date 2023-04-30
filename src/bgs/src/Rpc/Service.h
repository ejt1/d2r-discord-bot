#pragma once

#include "Request.h"
#include "Response.h"

namespace bgs {
namespace rpc {

class Channel;

class Service {
 public:
  virtual asio::awaitable<Response> CallMethod(std::shared_ptr<Channel> channel, Request& request) = 0;

  virtual const uint32_t Hash() const = 0;
};

}  // namespace rpc
}  // namespace bgs
