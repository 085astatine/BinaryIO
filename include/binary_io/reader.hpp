#ifndef BINARY_IO_READER_HPP_
#define BINARY_IO_READER_HPP_

#include <cassert>
#include <type_traits>
#include "binary_io/structure.hpp"

namespace binary_io {
template<typename Structure>
class Reader {
  using structure = Structure;
  using kind = typename structure::kind;

  template<kind key>
  using element = typename structure::template element<key>;

 public:
  // constructor
  Reader(const void* buffer_head,
         const std::size_t& buffer_size)
    : buffer_head_(buffer_head),
      buffer_size_(buffer_size) {
    assert(buffer_head_ || buffer_size_ == 0);
    assert(structure::bit_size() <= buffer_size_ * 8);
  }
  // getter
  template<kind key>
  typename element<key>::value_type Get() const {
    static_assert(key != kind::End, "End is reserved");
    static_assert(element<key>::key != kind::End, "invalid key");
    const auto bit_offset = structure::template bit_offset<key>();
    if (buffer_head_
        && (bit_offset + element<key>::bit_size <= buffer_size_ * 8)) {
      return element<key>::Read(
              buffer_head_,
              structure::template bit_offset<key>());
    } else {
      return element<key>::DefaultValue();
    }
  }

 private:
  const void* buffer_head_;
  std::size_t buffer_size_;
};
}  // namespace binary_io
#endif  // BINARY_IO_READER_HPP_
