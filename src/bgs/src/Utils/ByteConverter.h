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

/** ByteConverter reverse your byte order.  This is use
for cross platform where they have different endians.
*/

#include <algorithm>
#include <bit>

namespace bgs {

namespace ByteConverter {
template <size_t T>
inline void convert(char* val) {
  std::swap(*val, *(val + T - 1));
  convert<T - 2>(val + 1);
}

template <>
inline void convert<0>(char*) {}
template <>
inline void convert<1>(char*) {}  // ignore central byte

template <typename T>
inline void apply(T* val) {
  convert<sizeof(T)>((char*)(val));
}
}  // namespace ByteConverter

template <typename T>
inline void EndianConvert(T& val) {
  if constexpr (std::endian::native == std::endian::big) ByteConverter::apply<T>(&val);
}

template <typename T>
inline void EndianConvertReverse(T& val) {
  if constexpr (std::endian::native == std::endian::little) ByteConverter::apply<T>(&val);
}

template <typename T>
inline void EndianConvertPtr(void* val) {
  if constexpr (std::endian::native == std::endian::big) ByteConverter::apply<T>(val);
}

template <typename T>
inline void EndianConvertPtrReverse(void* val) {
  if constexpr (std::endian::native == std::endian::little) ByteConverter::apply<T>(val);
}

}  // namespace bgs
