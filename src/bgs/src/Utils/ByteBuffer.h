/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdexcept>
#include <vector>

namespace bgs {

class ByteBuffer {
 public:
  typedef std::vector<uint8_t>::size_type size_type;

  explicit ByteBuffer(std::size_t size = 4096) {
    m_data.resize(size);
  }

  explicit ByteBuffer(const uint8_t* data, std::size_t size = 4096) {
    m_data.resize(size);
    Write(data, size);
  }

  ByteBuffer(const ByteBuffer& right) noexcept : m_wpos(right.m_wpos), m_rpos(right.m_rpos), m_data(right.m_data) {}

  ByteBuffer(ByteBuffer&& right) noexcept : m_wpos(right.m_wpos), m_rpos(right.m_rpos), m_data(right.Move()) {}

  uint8_t& operator[](const size_t pos) {
    if (pos >= GetBufferSize()) throw std::out_of_range("position outside of data storage.");

    return m_data[pos];
  }

  const uint8_t& operator[](const size_t pos) const {
    if (pos >= GetBufferSize()) throw std::out_of_range("position outside of data storage.");

    return m_data[pos];
  }

  ByteBuffer& operator=(const ByteBuffer& right) {
    if (this != &right) {
      m_wpos = right.m_wpos;
      m_rpos = right.m_rpos;
      m_data = right.m_data;
    }

    return *this;
  }

  ByteBuffer& operator=(ByteBuffer&& right) noexcept {
    if (this != &right) {
      m_wpos = right.m_wpos;
      m_rpos = right.m_rpos;
      m_data = right.Move();
    }

    return *this;
  }

  ByteBuffer& operator<<(const int8_t val) {
    Write<int8_t>(val);

    return *this;
  }

  ByteBuffer& operator<<(const uint8_t val) {
    Write<int8_t>(val);

    return *this;
  }

  ByteBuffer& operator<<(const int16_t val) {
    Write<int16_t>(val);

    return *this;
  }

  ByteBuffer& operator<<(const uint16_t val) {
    Write<uint16_t>(val);

    return *this;
  }

  ByteBuffer& operator<<(const int32_t val) {
    Write<int32_t>(val);

    return *this;
  }

  ByteBuffer& operator<<(const uint32_t val) {
    Write<uint32_t>(val);

    return *this;
  }

  ByteBuffer& operator<<(const int64_t val) {
    Write<int64_t>(val);

    return *this;
  }

  ByteBuffer& operator<<(const uint64_t val) {
    Write<uint64_t>(val);

    return *this;
  }

  ByteBuffer& operator<<(const float val) {
    Write<float>(val);

    return *this;
  }

  ByteBuffer& operator<<(const double val) {
    Write<double>(val);

    return *this;
  }

  ByteBuffer& operator<<(const std::string& val) {
    Write(reinterpret_cast<const uint8_t*>(val.c_str()), val.size());
    Write<uint8_t>(0);  // null-terminating

    return *this;
  }

  ByteBuffer& operator<<(const char* val) {
    size_t len = (val ? strlen(val) : 0);

    Write(reinterpret_cast<const uint8_t*>(val), len);
    Write<uint8_t>(0);  // null-terminating

    return *this;
  }

  ByteBuffer& operator<<(const std::wstring& val) {
    Write(reinterpret_cast<const uint8_t*>(val.c_str()), val.size());
    Write<uint8_t>(0);  // null-terminating

    return *this;
  }

  ByteBuffer& operator>>(int8_t& val) {
    val = Read<int8_t>();

    return *this;
  }

  ByteBuffer& operator>>(uint8_t& val) {
    val = Read<uint8_t>();

    return *this;
  }

  ByteBuffer& operator>>(int16_t& val) {
    val = Read<int16_t>();

    return *this;
  }

  ByteBuffer& operator>>(uint16_t& val) {
    val = Read<uint16_t>();

    return *this;
  }

  ByteBuffer& operator>>(int32_t& val) {
    val = Read<int32_t>();

    return *this;
  }

  ByteBuffer& operator>>(uint32_t& val) {
    val = Read<uint32_t>();

    return *this;
  }

  ByteBuffer& operator>>(int64_t& val) {
    val = Read<int64_t>();

    return *this;
  }

  ByteBuffer& operator>>(uint64_t& val) {
    val = Read<uint64_t>();

    return *this;
  }

  ByteBuffer& operator>>(float& val) {
    val = Read<float>();

    return *this;
  }

  ByteBuffer& operator>>(double& val) {
    val = Read<double>();

    return *this;
  }

  ByteBuffer& operator>>(std::string& val) {
    val.clear();

    while (m_rpos < m_data.size()) {
      char c = Read<char>();
      if (c == 0) break;
      val += c;
    }

    return *this;
  }

  ByteBuffer& operator>>(std::wstring& val) {
    val.clear();

    while (m_rpos + 1 < m_data.size()) {
      wchar_t c = Read<wchar_t>();
      if (c == 0) break;
      val += c;
    }

    return *this;
  }

  std::vector<uint8_t>&& Move() noexcept {
    m_wpos = 0;
    m_rpos = 0;

    return std::move(m_data);
  }

  /**
   * Reset the read-write positions to 0.
   */
  void Reset() {
    m_wpos = 0;
    m_rpos = 0;
  }

  /**
   * Clears the content of the buffer, this will also reset the read-write
   * positions.
   */
  void Clear() {
    Reset();
    m_data.clear();
  }

  /**
   * Resizes the buffer, this will also reset the read-write positions.
   *
   * @param size_t new_size
   */
  void Resize(size_type bytes) {
    Reset();
    m_data.resize(bytes);
  }

  /**
   * Discard read bytes from the buffer and moves buffer to the start of read
   * position.
   *
   * @param size_t num_bytes
   */
  size_type Normalize() {
    size_type discarded_bytes = (size_type)GetReadPointer() - (size_type)GetBasePointer();

    if (m_rpos) {
      if (m_rpos != m_wpos) memmove(GetBasePointer(), GetReadPointer(), GetActiveSize());

      m_wpos -= m_rpos;
      m_rpos = 0;
      return discarded_bytes;
    }

    return discarded_bytes;
  }

  /**
   * Checks and resizes the buffer if its full.
   */
  void EnsureFreeSpace() {
    if (GetRemainingSpace() == 0) m_data.resize(m_data.size() * 3 / 2);
  }

  void Peek(uint8_t* data, std::size_t size) {
    if (GetActiveSize() >= size) memcpy(data, GetReadPointer(), size);
  }

  void Read(uint8_t* data, std::size_t size) {
    if (GetActiveSize() >= size) {
      memcpy(data, GetReadPointer(), size);
      ReadCompleted(size);
    }
  }

  void Write(const uint8_t* data, std::size_t size) {
    if (GetRemainingSpace() >= size) {
      memcpy(GetWritePointer(), data, size);
      WriteCompleted(size);
    }
  }

  template <typename T>
  T Peek() {
    std::size_t size = sizeof(T);
    T val;

    Peek(reinterpret_cast<uint8_t*>(&val), size);

    return val;
  }

  template <typename T>
  T Read() {
    std::size_t size = sizeof(T);
    T val;

    Read(reinterpret_cast<uint8_t*>(&val), size);

    return val;
  }

  template <typename T>
  void Write(T data) {
    std::size_t size = sizeof(T);

    Write(reinterpret_cast<const uint8_t*>(&data), size);
  }

  /**
   * Moves the read position forward.
   *
   * @param size_t num_bytes
   */
  void ReadCompleted(size_type bytes) {
    if (GetActiveSize() >= bytes) m_rpos += bytes;
  }

  /**
   * Moves the write position forward.
   *
   * @param size_t num_bytes
   */
  void WriteCompleted(size_type bytes) {
    if (GetRemainingSpace() >= bytes) m_wpos += bytes;
  }

  /**
   * Gets the active bytes that are remaining to read.
   */
  size_type GetActiveSize() const {
    return m_wpos - m_rpos;
  }

  /**
   * Gets the size remaining from write position.
   */
  size_type GetRemainingSpace() const {
    return m_data.size() - m_wpos;
  }

  /**
   * Gets the total buffer size.
   */
  size_type GetBufferSize() const {
    return m_data.size();
  }

  /**
   * Gets the base buffer pointer.
   */
  uint8_t* GetBasePointer() {
    return m_data.data();
  }

  /**
   * Gets the buffer pointer relative to write position.
   */
  uint8_t* GetReadPointer() {
    return GetBasePointer() + m_rpos;
  }

  /**
   * Gets the buffer pointer relative to write position.
   */
  uint8_t* GetWritePointer() {
    return GetBasePointer() + m_wpos;
  }

  std::vector<uint8_t>& data() {
    return m_data;
  }

 private:
  size_type m_wpos = 0;
  size_type m_rpos = 0;
  std::vector<uint8_t> m_data;
};

}  // namespace bgs
