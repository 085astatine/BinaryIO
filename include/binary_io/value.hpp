#ifndef BINARY_IO_VALUE_HPP_
#define BINARY_IO_VALUE_HPP_

#include <boost/optional.hpp>
#include "binary_io/element.hpp"

namespace binary_io {
template <typename Enum, Enum key, typename Type>
class Value: public Element<Enum, key, sizeof(Type) * 8, Type> {
 public:
  // read
  static boost::optional<Type> Read(
          const void* buffer_head,
          const std::size_t& bit_offset) {
    assert(bit_offset % 8 == 0);
    const auto byte_size = sizeof(Type);
    Type result;
    auto* const write_ptr = reinterpret_cast<uint8_t*>(&result);
    for (std::size_t i = 0; i < byte_size; ++i) {
      write_ptr[i] = *(reinterpret_cast<const uint8_t*>(buffer_head)
                       + bit_offset / 8 + i);
    }
    return result;
  }
  // write
  static void Write(
          void* buffer_head,
          const std::size_t& bit_offset,
          const Type& value) {
    assert(bit_offset % 8 == 0);
    const auto byte_size = sizeof(Type);
    const auto* const read_ptr = reinterpret_cast<const uint8_t*>(&value);
    for (std::size_t i = 0; i < byte_size; ++i) {
      auto* const write_ptr =
              reinterpret_cast<uint8_t*>(buffer_head)
              + bit_offset / 8 + i;
      *write_ptr = *(read_ptr + i);
    }
  }
};
}  // namespace binary_io
#endif  // BINARY_IO_VALUE_HPP_
