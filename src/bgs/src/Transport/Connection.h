#pragma once

#include "Rpc/Request.h"
#include "Rpc/Response.h"
#include "Rpc/Router.h"
#include "Utils/ByteBuffer.h"
#include "Utils/ByteConverter.h"

#include <deque>

namespace bgs {
namespace transport {

template <typename T>
class Connection : public std::enable_shared_from_this<T> {
 public:
  Connection(asio::io_context& io) : tls_(asio::ssl::context::tlsv12_client), ws_(io, tls_) {}
  virtual ~Connection() {}

  asio::awaitable<void> Close() {
    co_await ws_.async_close(beast::websocket::close_code::normal, asio::use_awaitable);
  }

  virtual void OnUpgrade(beast::websocket::request_type& req) {}
  virtual void OnRead(ByteBuffer&& buffer) = 0;

 protected:
  asio::awaitable<void> Reader() {
    try {
      for (;;) {
        // This buffer will hold the incoming message
        beast::flat_buffer buffer;

        // Read a message
        boost::system::error_code ec;
        auto&& bytes = co_await ws_.async_read(buffer, redirect_error(asio::use_awaitable, ec));

        if (ec) {
          if (ec != asio::error::eof) {
            throw beast::system_error(ec);
          }
        }

        ByteBuffer packet(reinterpret_cast<const uint8_t*>(buffer.cdata().data()), buffer.size());
        OnRead(std::move(packet));
      }
    } catch (std::exception& e) {
      spdlog::error("Read error: {}", e.what());
      Close();
    }
  }

  asio::awaitable<void> Writer() {
    try {
      auto&& ex = co_await asio::this_coro::executor;
      boost::asio::steady_timer timer(ex);

      while (ws_.is_open()) {
        if (write_queue_.empty()) {
          boost::system::error_code ec;
          co_await timer.async_wait(redirect_error(asio::use_awaitable, ec));
        } else {
          ByteBuffer packet = write_queue_.front();
          co_await ws_.async_write(asio::buffer(packet.GetReadPointer(), packet.GetActiveSize()), asio::use_awaitable);
          write_queue_.pop_front();
        }
      }
    } catch (std::exception& e) {
      spdlog::error("Write error: {}", e.what());
      Close();
    }
  }

 protected:
  tls::context tls_;

  beast::websocket::stream<beast::ssl_stream<beast::tcp_stream>> ws_;
  std::deque<ByteBuffer> write_queue_;
};

}  // namespace transport
}  // namespace bgs
