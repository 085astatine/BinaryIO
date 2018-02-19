#ifndef BINARY_IO_STRUCTURE_HPP_
#define BINARY_IO_STRUCTURE_HPP_

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

namespace impl {
template <typename Enum>
struct DummyElement {
  static constexpr Enum key = Enum::End;
};
// Element Iterator: end
template <typename Enum, std::size_t offset_, typename...>
struct ElementIterator {
  struct DummyIterator {};
  using element = DummyElement<Enum>;
  using next = DummyIterator;
  static constexpr std::size_t offset = offset_;
  static constexpr bool is_end = true;
};
// Element Iterator
template <typename Enum, size_t offset_, typename Head, typename... Tail>
struct ElementIterator<Enum, offset_, Head, Tail...>:
        private ElementIterator<Enum, offset_ + Head::bit_number, Tail...> {
  using element = Head;
  using next = ElementIterator<Enum, offset_ + Head::bit_number, Tail...>;
  static constexpr std::size_t offset = offset_;
  static constexpr bool is_end = false;
};
// get offset
template <typename Enum, Enum key, typename Iterator, bool is_end>
struct get_offset;
// get offset: end
template <typename Enum, Enum key, typename Iterator>
struct get_offset<Enum, key, Iterator, true> {
  static constexpr std::size_t value = 0;
};
// get offset: impl
template <typename Enum, Enum key, typename Iterator>
struct get_offset<Enum, key, Iterator, false> {
  static constexpr std::size_t value =
        key == Iterator::element::key
        ? Iterator::offset
        : get_offset<
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
  using offset_t = impl::get_offset<Enum, key, iterator, iterator::is_end>;

  template <Enum key>
  static std::size_t offset() {
    static_assert(key != Enum::End, "End is reserved");
    static_assert(element<key>::type::key != Enum::End, "invalid key");
    return offset_t<key>::value;
  }

  static std::size_t size() {
    return offset_t<Enum::End>::value;
  }
};
}  // namespace binary_io
#endif  // BINARY_IO_STRUCTURE_HPP_
