#ifndef BINARY_IO_INTEGER_HPP_
#define BINARY_IO_INTEGER_HPP_

#include <limits>
#include <type_traits>
#include "binary_io/value.hpp"

namespace binary_io {
template <
        typename Enum,
        Enum key,
        typename Type,
        Type default_value_ = Type(0),
        Type min_value_ = std::numeric_limits<Type>::min(),
        Type max_value_ = std::numeric_limits<Type>::max()>
class Integer: public Value<Enum, key, Type> {
  static_assert(std::is_integral<Type>::value, "invalid Type");
  static_assert(min_value_ <= max_value_, "invalid range");
  using base = Value<Enum, key, Type>;
  static constexpr Type default_value = default_value_;
  static constexpr Type min_value = min_value_;
  static constexpr Type max_value = max_value_;

 public:
  static constexpr Type DefaultValue() {
    return default_value;
  }
  // read
  static Type Read(
          const void* buffer_head,
          const std::size_t& bit_offset) {
    return Normalize(base::Read(buffer_head, bit_offset));
  }
  // write
  static void Write(
          void* buffer_head,
          const std::size_t& bit_offset,
          const Type& value) {
    base::Write(buffer_head, bit_offset, Normalize(value));
  }

 private:
  // normalize
  static constexpr Type Normalize(
          const Type& value) {
    return (min_value <= value && value <= max_value)
           ? value
           : DefaultValue();
  }
};
}  // namespace binary_io
#endif  // BINARY_IO_INTEGER_HPP_
