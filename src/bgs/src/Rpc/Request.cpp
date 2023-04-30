#include "Rpc/Request.h"

namespace bgs {
namespace rpc {

Request::Request(ByteBuffer& raw) : Packet(raw) {}

Request::Request(const Packet& packet) : Packet(packet) {}

Request::Request(const google::protobuf::Message& message) : Request({}, message) {}

Request::Request(bgs::protocol::Header&& header, const google::protobuf::Message& message) : Packet() {
  header_ = std::move(header);
  payload_.Resize(message.ByteSize());

  auto ptr = payload_.GetWritePointer();
  payload_.WriteCompleted(message.GetCachedSize());
  message.SerializePartialToArray(ptr, message.GetCachedSize());
}

}  // namespace rpc
}  // namespace bgs
