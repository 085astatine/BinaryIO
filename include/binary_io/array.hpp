#ifndef BINARY_IO_ARRAY_HPP_
#define BINARY_IO_ARRAY_HPP_

#include <cassert>
#include <cstring>
#include <type_traits>
#include "binary_io/element.hpp"

namespace binary_io {
template <typename Enum, Enum key, typename Type, std::size_t N>
class Array: public Element<Enum, key, sizeof(Type) * N * 8, Type> {
  static constexpr std::size_t byte_size = sizeof(Type) * N;

 public:
  // read
  static void Read(
          const void* buffer_head,
          const std::size_t& bit_offset,
          Type* output_ptr) {
    assert(bit_offset % 8 == 0);
    if (output_ptr) {
      std::memmove(
              output_ptr,
              reinterpret_cast<const uint8_t*>(buffer_head) + bit_offset / 8,
              byte_size);
    }
  }
  // write
  static bool Write(
          void* buffer_head,
          const std::size_t& bit_offset,
          const Type* value_ptr) {
    assert(bit_offset % 8 == 0);
    if (value_ptr) {
      std::memmove(
              reinterpret_cast<uint8_t*>(buffer_head) + bit_offset / 8,
              value_ptr,
              byte_size);
      return true;
    }
    return false;
  }
};
}  // namespace binary_io
#endif  // BINARY_IO_ARRAY_HPP_
