#ifndef BINARY_IO_ELEMENT_HPP_
#define BINARY_IO_ELEMENT_HPP_

#include <type_traits>

namespace binary_io {
template <
        typename Enum,
        Enum key_,
        std::size_t bit_number_>
struct Element {
  static_assert(key_ != Enum::End, "End is reserved");
  static constexpr Enum key = key_;
  static constexpr size_t bit_number = bit_number_;
};
}  // namespace binary_io
#endif  // BINARY_IO_ELEMENT_HPP_
