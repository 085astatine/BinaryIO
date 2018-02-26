#ifndef BINARY_IO_INTEGER_HPP_
#define BINARY_IO_INTEGER_HPP_

#include <type_traits>
#include "binary_io/value.hpp"

namespace binary_io {
template <
        typename Enum,
        Enum key,
        typename Type,
        Type default_value_ = Type(0)>
class Integer: public Value<Enum, key, Type> {
  static_assert(std::is_integral<Type>::value, "invalid Type");
  using base = Value<Enum, key, Type>;
  static constexpr Type default_value = default_value_;

 public:
  static constexpr Type DefaultValue() {
    return default_value;
  }
  // read
  static Type Read(
          const void* buffer_head,
          const std::size_t& bit_offset) {
    return base::Read(buffer_head, bit_offset);
  }
  // write
  static void Write(
          void* buffer_head,
          const Type& value,
          const std::size_t& bit_offset) {
    base::Write(buffer_head, value, bit_offset);
  }
};
}  // namespace binary_io
#endif  // BINARY_IO_INTEGER_HPP_
