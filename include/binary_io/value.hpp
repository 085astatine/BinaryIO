#ifndef BINARY_IO_VALUE_HPP_
#define BINARY_IO_VALUE_HPP_

#include <cassert>
#include <cstring>
#include <boost/optional.hpp>
#include "binary_io/element.hpp"

namespace binary_io {
template <typename Enum, Enum key, typename Type>
class Value: public Element<Enum, key, sizeof(Type) * 8, Type> {
  static constexpr std::size_t byte_size = sizeof(Type);

 public:
  // read
  static boost::optional<Type> Read(
          const void* buffer_head,
          const std::size_t& bit_offset) {
    assert(bit_offset % 8 == 0);
    Type result;
    std::memmove(
            reinterpret_cast<uint8_t*>(&result),
            reinterpret_cast<const uint8_t*>(buffer_head) + bit_offset / 8,
            byte_size);
    return result;
  }
  // write
  static bool Write(
          void* buffer_head,
          const std::size_t& bit_offset,
          const Type& value) {
    assert(bit_offset % 8 == 0);
    std::memmove(
            reinterpret_cast<uint8_t*>(buffer_head) + bit_offset / 8,
            reinterpret_cast<const uint8_t*>(&value),
            byte_size);
    return true;
  }
};
}  // namespace binary_io
#endif  // BINARY_IO_VALUE_HPP_
