#include "Rpc/Packet.h"

namespace bgs {
namespace rpc {

Packet::Packet(ByteBuffer& raw) {
  raw >> header_len_;
  EndianConvertReverse(header_len_);
  if (header_len_ > raw.GetActiveSize()) {
    throw std::out_of_range("header length larger than buffer");
  }

  // read header
  if (!header_.ParsePartialFromArray(raw.GetReadPointer(), header_len_)) {
    throw std::runtime_error("Failed to parse header");
  }
  raw.ReadCompleted(header_len_);
  spdlog::trace("header: {}", header_.ShortDebugString());

  // read payload
  if (raw.GetActiveSize() > 0) {
    payload_.Write(raw.GetReadPointer(), raw.GetActiveSize());
  }
}

Packet::Packet(::google::protobuf::Message& message) {
  payload_.Resize(message.ByteSize());
  message.SerializePartialToArray(payload_.GetWritePointer(), payload_.GetRemainingSpace());
  payload_.WriteCompleted(message.GetCachedSize());
}

bool Packet::IsRequest() {
  return header_.has_is_response() ? !header_.is_response() : (header_.service_id() & 0xFE) == 0;
}

bool Packet::IsResponse() {
  return !IsRequest();
}

uint32_t Packet::ServiceHash() {
  return header_.has_service_hash() ? header_.service_hash() : header_.service_id();
}

uint32_t Packet::MethodId() {
  return header_.method_id();
}

bgs::protocol::Header& Packet::Header() {
  return header_;
}

ByteBuffer& Packet::Payload() {
  return payload_;
}

}  // namespace rpc
}  // namespace bgs
