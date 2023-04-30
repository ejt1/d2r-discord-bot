#pragma once

#include "Request.h"
#include "Response.h"
#include "Transport/Connection.h"

#include <bgs/low/pb/client/rpc_types.pb.h>
#include <fmt/ranges.h>
#include <google/protobuf/message.h>

#include <functional>

namespace bgs {
namespace rpc {

using bgs::protocol::Header;

class Channel : public transport::Connection<Channel> {
 public:
  Channel(asio::io_context& io, const std::string& name);
  virtual ~Channel();

  asio::awaitable<void> Connect(const std::string& host, const std::string& port, const std::string& target = "/");

  virtual void OnRead(ByteBuffer&& buffer) override;
  virtual asio::awaitable<void> HandlePacket(Packet* message);

  template <typename ResponseType = void, typename CompletionToken = boost::asio::use_awaitable_t<>>
  auto CallMethod(uint32_t service_hash, uint32_t method_id, const google::protobuf::Message& message,
                  CompletionToken&& completion_token = {});

  void Respond(uint32_t service_hash, uint32_t method_id, uint32_t token, const google::protobuf::Message& message);

  rpc::Router& GetRouter();
  std::string& GetName();

 protected:
  virtual void ProcessHeader(Header& header, const google::protobuf::Message& message) {}

 protected:
  rpc::Router router_;
  std::string name_;

  uint32_t token_ = 1;
  std::map<uint32_t, move_only_function<void(Response&&)>> response_callbacks_;
};

template <typename ResponseType, typename CompletionToken>
inline auto Channel::CallMethod(uint32_t service_hash, uint32_t method_id, const google::protobuf::Message& message,
                                CompletionToken&& completion_token) {
  Request request(message);

  Header& header = request.Header();
  uint32_t t = token_++;
  header.set_token(t);
  header.set_service_id(service_hash);
  header.set_method_id(method_id);
  header.set_service_hash(service_hash);
  ProcessHeader(header, message);

  auto packet = request.Serialize();

  // queue message
  write_queue_.push_back(packet);

  return asio::async_initiate<CompletionToken, void(Response &&)>(
      [self = shared_from_this()](auto&& handler, uint32_t token) mutable {
        if constexpr (std::is_void_v<ResponseType>) {
          handler(ErrorCode::OK);
        } else {
          self->response_callbacks_.emplace(token, std::forward<decltype(handler)>(handler));
        }
      },
      completion_token, t);
}

}  // namespace rpc
}  // namespace bgs
