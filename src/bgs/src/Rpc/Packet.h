#pragma once

#include "ErrorCode.h"
#include "Utils/ByteBuffer.h"
#include "Utils/ByteConverter.h"

#include <bgs/low/pb/client/rpc_types.pb.h>
#include <google/protobuf/message.h>

namespace bgs {
namespace rpc {

template <typename T>
concept IsProto = std::is_base_of<::google::protobuf::Message, T>::value;

class Packet {
 public:
  Packet() {}
  Packet(ByteBuffer& raw);
  Packet(::google::protobuf::Message& message);

  // helper functions
  bool IsRequest();
  bool IsResponse();
  uint32_t ServiceHash();
  uint32_t MethodId();

  // raw accessors
  bgs::protocol::Header& Header();
  ByteBuffer& Payload();

  // XXX: Optimal would be to move T to avoid double construction however the
  // protobuf version we use do not support this :(
  template <typename T>
  T Deserialize()
    requires IsProto<T>
  {
    T message{};
    if (!message.ParseFromArray(payload_.GetReadPointer(), payload_.GetActiveSize())) {
      throw std::runtime_error("failed to parse payload");
    }
    return message;
  }

  ByteBuffer Serialize() {
    header_.set_size(payload_.GetBufferSize());

    // compute header length
    header_len_ = header_.ByteSize();
    // reverse endian
    EndianConvertReverse(header_len_);

    // construct message buffer
    ByteBuffer packet(sizeof(header_len_) + header_.GetCachedSize() + payload_.GetBufferSize());

    // write header length
    packet << header_len_;

    // write header
    auto ptr = packet.GetWritePointer();
    packet.WriteCompleted(header_.GetCachedSize());
    header_.SerializePartialToArray(ptr, header_.GetCachedSize());

    // write message
    packet.Write(payload_.GetBasePointer(), payload_.GetBufferSize());

    return packet;
  }

 protected:
  uint16_t header_len_ = 0;
  bgs::protocol::Header header_;
  ByteBuffer payload_;
};

}  // namespace rpc
}  // namespace bgs
