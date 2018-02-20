#ifndef BINARY_IO_STRUCTURE_HPP_
#define BINARY_IO_STRUCTURE_HPP_

#include <type_traits>
#include "binary_io/element.hpp"

namespace binary_io {
namespace impl {
template <typename Enum>
struct DummyElement {
  static constexpr Enum key = Enum::End;
};
// Element Iterator: end
template <typename Enum, std::size_t bit_offset_, typename...>
struct ElementIterator {
  struct DummyIterator {};
  using element = DummyElement<Enum>;
  using next = DummyIterator;
  static constexpr std::size_t bit_offset = bit_offset_;
  static constexpr bool is_end = true;
};
// Element Iterator
template <typename Enum, size_t bit_offset_, typename Head, typename... Tail>
struct ElementIterator<Enum, bit_offset_, Head, Tail...>:
        private ElementIterator<Enum, bit_offset_ + Head::bit_size, Tail...> {
  using element = Head;
  using next = ElementIterator<Enum, bit_offset_ + Head::bit_size, Tail...>;
  static constexpr std::size_t bit_offset = bit_offset_;
  static constexpr bool is_end = false;
};
// get bit offset
template <typename Enum, Enum key, typename Iterator, bool is_end>
struct get_bit_offset;
// get bit offset: end
template <typename Enum, Enum key, typename Iterator>
struct get_bit_offset<Enum, key, Iterator, true> {
  static constexpr std::size_t value = 0;
};
// get bit offset: impl
template <typename Enum, Enum key, typename Iterator>
struct get_bit_offset<Enum, key, Iterator, false> {
  static constexpr std::size_t value =
        key == Iterator::element::key
        ? Iterator::bit_offset
        : get_bit_offset<
                  Enum,
                  key,
                  typename Iterator::next,
                  Iterator::is_end>::value;
};
}  // namespace impl

template <typename Enum, typename... Args>
class Structure {
 private:
  using iterator = impl::ElementIterator<Enum, 0, Args...>;
  // get element: end
  template <Enum, typename...>
  struct get_element {
    using type = impl::DummyElement<Enum>;
  };
  // get element
  template <Enum key, typename Head, typename... Tail>
  struct get_element<key, Head, Tail...> {
    using type = typename std::conditional<
            key == Head::key,
            Head,
            typename get_element<key, Tail...>::type>
            ::type;
  };

 public:
  using kind = Enum;

  template <Enum key>
  using element = get_element<key, Args...>;

  template <Enum key>
  using bit_offset_t =
          impl::get_bit_offset<Enum, key, iterator, iterator::is_end>;

  template <Enum key>
  static std::size_t bit_offset() {
    static_assert(key != Enum::End, "End is reserved");
    static_assert(element<key>::type::key != Enum::End, "invalid key");
    return bit_offset_t<key>::value;
  }

  static std::size_t bit_size() {
    return bit_offset_t<Enum::End>::value;
  }
};
}  // namespace binary_io
#endif  // BINARY_IO_STRUCTURE_HPP_
