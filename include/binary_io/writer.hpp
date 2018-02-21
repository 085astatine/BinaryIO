#ifndef BINARY_IO_WRITER_HPP_
#define BINARY_IO_WRITER_HPP_

#include <cassert>
#include <type_traits>
#include "binary_io/structure.hpp"

namespace binary_io {
template <typename Structure>
class Writer {
  using structure = Structure;
  using kind = typename structure::kind;

  template<kind key>
  using element = typename structure::template element<key>;

 public:
  // constructor
  Writer(void* buffer_head,
         const std::size_t& buffer_size)
    : buffer_head_(buffer_head),
      buffer_size_(buffer_size) {
    assert(buffer_head_ || buffer_size_ == 0);
    assert(structure::bit_size() <= buffer_size_ * 8);
  }
  // setter
  template<kind key>
  void Set(
          const typename element<key>::value_type& value) {
    static_assert(key != kind::End, "End is reserved");
    static_assert(element<key>::key != kind::End, "invalid key");
    const auto bit_offset = structure::template bit_offset<key>();
    if (buffer_head_
        && (bit_offset + element<key>::bit_size <= buffer_size_ * 8)) {
      element<key>::Write(
              buffer_head_,
              buffer_size_,
              value,
              bit_offset);
    }
  }

 private:
  void* buffer_head_;
  std::size_t buffer_size_;
};
}  // namespace binary_io
#endif  // BINARY_IO_WRITER_HPP_
