#ifndef BINARY_IO_WRITER_HPP_
#define BINARY_IO_WRITER_HPP_

#include <array>
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
      buffer_size_(buffer_size),
      written_flags{} {
    assert(buffer_head_ || buffer_size_ == 0);
    assert(structure::bit_size() <= buffer_size_ * 8);
  }
  // setter
  template<kind key, typename... Args>
  bool Set(Args&&... args) {
    static_assert(key != kind::End, "End is reserved");
    static_assert(element<key>::key != kind::End, "invalid key");
    const auto bit_offset = structure::template bit_offset<key>();
    if (buffer_head_
        && (bit_offset + element<key>::bit_size <= buffer_size_ * 8)) {
      if (element<key>::Write(
              buffer_head_,
              bit_offset,
              std::forward<Args>(args)...)) {
        SetWrittenFlag(structure::template element_index<key>(), true);
        return true;
      }
    }
    return false;
  }
  // setter: force
  template<kind key, typename... Args>
  typename std::enable_if<
          impl::is_default_value_defined<element<key>>::value,
          bool>::type
  ForceSet(Args&&... args) {
    if (Set<key>(std::forward<Args>(args)...)
        || SetDefaultValue<key>()) {
      return true;
    }
    return false;
  }
  // have been all element set
  bool IsAllSet() const {
    for (std::size_t i = 0; i < structure::element_size(); ++i) {
      if (!GetWrittenFlag(i)) {
        return false;
      }
    }
    return true;
  }

 private:
  // set default value
  template<kind key>
  bool SetDefaultValue() {
    static_assert(key != kind::End, "End is reserved");
    static_assert(element<key>::key != kind::End, "invalid key");
    const auto bit_offset = structure::template bit_offset<key>();
    if (buffer_head_
        && (bit_offset + element<key>::bit_size <= buffer_size_ * 8)) {
      if (element<key>::WriteDefaultValue(buffer_head_, bit_offset)) {
        SetWrittenFlag(structure::template element_index<key>(), true);
        return true;
      }
    }
    return false;
  }
  // get writtern flag
  bool GetWrittenFlag(const std::size_t& index) const {
    const uint8_t mask = 0x1 << index % 8;
    return (written_flags[index / 8] & mask) != 0;
  }
  // set written flag
  void SetWrittenFlag(
          const std::size_t& index,
          const bool& is_written) {
    const uint8_t mask = 0x1 << index % 8;
    if (is_written) {
      written_flags[index / 8] |= mask;
    } else {
      written_flags[index / 8] &= ~mask;
    }
  }

  void* buffer_head_;
  std::size_t buffer_size_;
  std::array<uint8_t,
             structure::element_size() / 8
             + ((structure::element_size() % 8 == 0)? 0: 1)> written_flags;
};
}  // namespace binary_io
#endif  // BINARY_IO_WRITER_HPP_
