#ifndef BINARY_IO_BINARY_HPP_
#define BINARY_IO_BINARY_HPP_

#include <type_traits>
#include "binary_io/element.hpp"

namespace binary_io {
template <typename Enum, Enum key, std::size_t byte_size>
class Binary: public Element<Enum, key, byte_size * 8, void*> {
 public:
  // read
  static void Read(
          const void* buffer_head,
          const std::size_t& bit_offset,
          void* output_ptr) {
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
          const void* value_ptr) {
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
#endif  //  BINARY_IO_BINARY_HPP_
