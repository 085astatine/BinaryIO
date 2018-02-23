#ifndef BINARY_IO_ELEMENT_HPP_
#define BINARY_IO_ELEMENT_HPP_

#include <type_traits>

namespace binary_io {
template <
        typename Enum,
        Enum key_,
        std::size_t bit_size_,
        typename Type>
class Element {
  static_assert(key_ != Enum::End, "End is reserved");

 public:
  using value_type = Type;
  static constexpr Enum key = key_;
  static constexpr size_t bit_size = bit_size_;

  // default value: be redefined with inheritance class
  static constexpr value_type DefaultValue();
  // read: be redefined with inheritance class
  static value_type Read(
          const void* buffer_head,
          const std::size_t& buffer_size,
          const std::size_t& bit_offset);
  // write: be redefined with inheritance class
  static void Write(
          const void* buffer_head,
          const std::size_t& buffer_size,
          const value_type& value,
          const std::size_t& bit_offset);
};

template <std::size_t bit_size_>
class Padding {
 public:
  static constexpr std::size_t bit_size = bit_size_;
};
}  // namespace binary_io
#endif  // BINARY_IO_ELEMENT_HPP_
