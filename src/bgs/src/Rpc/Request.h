#pragma once

#include "Packet.h"

namespace bgs {
namespace rpc {

class Request : public Packet {
 public:
  Request(ByteBuffer& raw);
  Request(const Packet& packet);
  Request(const google::protobuf::Message& message);
  Request(bgs::protocol::Header&& header, const google::protobuf::Message& message);
};

}  // namespace rpc
}  // namespace bgs
