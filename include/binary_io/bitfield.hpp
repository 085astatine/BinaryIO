#ifndef BINARY_IO_BITFIELD_HPP_
#define BINARY_IO_BITFIELD_HPP_

#include <cassert>
#include <type_traits>
#include "binary_io/element.hpp"

namespace binary_io {
template <
        typename Enum,
        Enum key,
        std::size_t bit_size,
        typename Type,
        Type default_value_ = Type(0),
        Type min_value_ = Type(0),
        Type max_value_ = Type(-1)>
class Bitfield: public Element<Enum, key, bit_size, Type> {
  static_assert(std::is_unsigned<Type>::value, "invalid type");
  static_assert(min_value_ <= max_value_, "invalid range");
  static_assert(bit_size <= sizeof(Type) * 8, "Type is short of bits");

  static constexpr Type default_value = default_value_;
  static constexpr Type min_value = min_value_;
  static constexpr Type max_value = max_value_;

 public:
  // default value
  static constexpr Type DefaultValue() {
    return default_value;
  }
  // read
  static Type Read(
          const void* buffer_head,
          const std::size_t& bit_offset) {
    auto result = Type(0);
    for (int i = static_cast<int>(bit_size) - 1; i >= 0; --i) {
      // shift
      result <<= 1;
      // bit check
      const auto byte = *(reinterpret_cast<const uint8_t*>(buffer_head)
                          + (bit_offset + i) / 8);
      const uint8_t mask = 0x1 << (bit_offset + i) % 8;
      if ((byte & mask) != 0) {
        result += 0x1;
      }
    }
    return Normalize(result);
  }
  // write
  static void Write(
          void* buffer_head,
          const std::size_t& bit_offset,
          const Type& value) {
    const auto normalized_value = Normalize(value);
    for (std::size_t i = 0; i < bit_size; ++i) {
      // bit mask
      const auto value_byte =
              *(reinterpret_cast<const uint8_t*>(&normalized_value) + i / 8);
      const uint8_t value_mask = 0x01 << i % 8;
      // write
      if ((value_byte & value_mask) != 0) {
        auto* const write_byte_ptr =
                static_cast<uint8_t*>(buffer_head) + (bit_offset + i) / 8;
        const uint8_t write_mask = 0x1 << (bit_offset + i) % 8;
        *write_byte_ptr |= write_mask;
      }
    }
  }

 private:
  static constexpr Type Normalize(
          const Type& value) {
    return (min_value <= value && value <= max_value)
           ? value
           : DefaultValue();
  }
};
}  // namespace binary_io
#endif  // BINARY_IO_BITFIELD_HPP_
