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
  using base = Element<Enum, key, bit_size, Type>;

  static constexpr typename base::value_type default_value = default_value_;
  static constexpr typename base::value_type min_value = min_value_;
  static constexpr typename base::value_type max_value = max_value_;

 public:
  // default value
  static constexpr typename base::value_type DefaultValue() {
    return default_value;
  }
  // read
  static typename base::value_type Read(
          const void* buffer_head,
          const std::size_t& bit_offset) {
    auto result = typename base::value_type(0);
    for (int i = static_cast<int>(base::bit_size) - 1; i >= 0; --i) {
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
          const typename base::value_type& value,
          const std::size_t& bit_offset) {
    const auto normalized_value = Normalize(value);
    for (std::size_t i = 0; i < base::bit_size; ++i) {
      // bit mask
      const auto value_byte =
              *(reinterpret_cast<const uint8_t*>(&normalized_value) + i / 8);
      const uint8_t value_mask = 0x01 << i % 8;
      // write
      if ((value_byte & value_mask) != 0) {
        auto write_byte_ptr =
                static_cast<uint8_t*>(buffer_head) + (bit_offset + i) / 8;
        const uint8_t write_mask = 0x1 << (bit_offset + i) % 8;
        *write_byte_ptr |= write_mask;
      }
    }
  }

 private:
  static constexpr typename base::value_type Normalize(
          const typename base::value_type& value) {
    return (min_value <= value && value <= max_value)
           ? value
           : DefaultValue();
  }
};
}  // namespace binary_io
#endif  // BINARY_IO_BITFIELD_HPP_
