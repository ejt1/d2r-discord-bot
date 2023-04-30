#include "Channel.h"

#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace bgs {
namespace rpc {

//

Channel::Channel(asio::io_context& io, const std::string& name) : transport::Connection<Channel>(io), name_(name) {
  if (!spdlog::get(name)) {
    // create logger for the connection
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_st>());
    sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_st>(name + ".txt", 23, 59));
    auto combined_logger = std::make_shared<spdlog::logger>(name, begin(sinks), end(sinks));
    spdlog::initialize_logger(combined_logger);
  }
}

Channel::~Channel() {}

asio::awaitable<void> Channel::Connect(const std::string& host, const std::string& port, const std::string& target) {
  auto ex = co_await asio::this_coro::executor;
  tcp::resolver resolver(ex);

  spdlog::get(name_)->debug("Connecting to {}:{}", host, port);

  const auto results = co_await resolver.async_resolve(host, port, asio::use_awaitable);

  beast::get_lowest_layer(ws_).expires_after(std::chrono::seconds(30));

  // connect
  auto ep = co_await beast::get_lowest_layer(ws_).async_connect(results, asio::use_awaitable);

  // tls handshake
  co_await ws_.next_layer().async_handshake(tls::stream_base::client, asio::use_awaitable);

  beast::get_lowest_layer(ws_).expires_never();

  // websocket handshake
  ws_.set_option(beast::websocket::stream_base::decorator(
      [self = shared_from_this()](beast::websocket::request_type& req) { self->OnUpgrade(req); }));
  ws_.set_option(beast::websocket::stream_base::timeout::suggested(beast::role_type::client));
  ws_.binary(true);

  std::string http_host = host + ':' + std::to_string(ep.port());
  co_await ws_.async_handshake(http_host, target, asio::use_awaitable);

  asio::co_spawn(
      ws_.get_executor(), [self = shared_from_this()] { return self->Reader(); }, asio::detached);

  asio::co_spawn(
      ws_.get_executor(), [self = shared_from_this()] { return self->Writer(); }, asio::detached);

  spdlog::get(name_)->info("Connected to {}:{}", host, port);
}

void Channel::OnRead(ByteBuffer&& buffer) {
  // heap allocated due to a bug with moving packet to HandlePacket
  // HandlePacket is responsible for deallocation!
  Packet* packet = new Packet(buffer);

  asio::co_spawn(ws_.get_executor(), this->HandlePacket(packet), asio::detached);
}

asio::awaitable<void> Channel::HandlePacket(Packet* message) {
  // wrap heap pointer to make sure it is deallocated properly if something throws
  std::unique_ptr<Packet> packet(message);

  if (packet->IsRequest()) {
    Request request(*packet);
    Response&& response = co_await router_.Process(shared_from_this(), std::move(request));
    if (!response.HasResponse()) {
      co_return;
    }

    auto& req_hdr = request.Header();
    auto& res_hdr = response.Header();

    if (req_hdr.service_id() > 0xFE) {
      res_hdr.set_service_id(req_hdr.service_id());
    } else {
      res_hdr.set_service_id(0xFE);
    }
    res_hdr.set_token(req_hdr.token());
    res_hdr.set_method_id(req_hdr.method_id());
    res_hdr.set_service_hash(req_hdr.service_hash());
    res_hdr.set_object_id(2525111537);
    res_hdr.set_is_response(true);
    ByteBuffer response_buf = response.Serialize();
    write_queue_.push_back(std::move(response_buf));
  } else {
    auto& header = packet->Header();
    if (response_callbacks_.contains(header.token())) {
      if (header.status() != 0) {
        spdlog::error("header.status() == {} ({})", to_string(static_cast<ErrorCode>(header.status())),
                      header.status());
      }
      Response response(*packet);
      response_callbacks_[header.token()](std::move(response));
      response_callbacks_.erase(header.token());
    } else {
      spdlog::error("Ignoring response with token {}", header.token());
    }
  }
}

void Channel::Respond(uint32_t service_hash, uint32_t method_id, uint32_t token,
                      const google::protobuf::Message& message) {
  Header header;
  header.set_service_id(0xFE);
  header.set_token(token);
  header.set_method_id(method_id);
  header.set_service_hash(service_hash);

  // size doesn't seem to be set by D2R but we do it here for good measure
  header.set_size(message.ByteSize());

  // compute header length
  uint16_t header_size = header.ByteSize();
  // reverse endian
  EndianConvertReverse(header_size);

  // construct message buffer
  ByteBuffer packet(sizeof(header_size) + header.GetCachedSize() + message.GetCachedSize());

  // write header length
  packet << header_size;

  // write header
  auto ptr = packet.GetWritePointer();
  packet.WriteCompleted(header.GetCachedSize());
  header.SerializePartialToArray(ptr, header.GetCachedSize());

  // write message
  ptr = packet.GetWritePointer();
  packet.WriteCompleted(message.GetCachedSize());
  message.SerializePartialToArray(ptr, message.GetCachedSize());

  // queue message
  write_queue_.push_back(packet);
}

rpc::Router& Channel::GetRouter() {
  return router_;
}

std::string& Channel::GetName() {
  return name_;
}

}  // namespace rpc
}  // namespace bgs
