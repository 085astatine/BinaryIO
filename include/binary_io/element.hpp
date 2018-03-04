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

  /*
  // default value
  static constexpr value_type DefaultValue();
  // read
  //   if DefaultValue() is not defined, return void
  static (boost::optional<value_type> or void) Read(
          const void* buffer_head,
          const std::size_t& bit_offset,
          args...);
  // write
  //   returns whether writing has been successful
  static bool Write(
          const void* buffer_head,
          const std::size_t& bit_offset,
          args...);
  */
};

template <std::size_t bit_size_>
class Padding {
 public:
  static constexpr std::size_t bit_size = bit_size_;
};
}  // namespace binary_io
#endif  // BINARY_IO_ELEMENT_HPP_
