#pragma once

#include "Packet.h"

namespace bgs {
namespace rpc {

class Response : public Packet {
 public:
  Response() : Packet(), has_response_(false) {}
  Response(ByteBuffer& buffer) : Packet(buffer) {}
  Response(const Packet& message) : Packet(message) {}
  Response(const ErrorCode& ec) : Packet() {
    header_.set_status(static_cast<uint32_t>(ec));
  }
  Response(::google::protobuf::Message& message) : Packet(message) {}

  bool HasResponse() {
    return has_response_;
  }

 private:
  // XXX: ugly
  bool has_response_ = true;
};

}  // namespace rpc
}  // namespace bgs
