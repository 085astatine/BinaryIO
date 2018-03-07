#ifndef BINARY_IO_STRUCTURE_HPP_
#define BINARY_IO_STRUCTURE_HPP_

#include <type_traits>
#include "binary_io/element.hpp"

namespace binary_io {
namespace impl {
// utility for std::enable_if
extern void* enabler;
// Dummy Element
template <typename Enum>
struct DummyElement {
  static constexpr Enum key = Enum::End;
};
// check if the Element has DefaultValue()
template<typename T>
class is_default_value_defined {
  template<typename U>
  static auto check(U) -> decltype(U::DefaultValue(), std::true_type{});
  static std::false_type check(...);

 public:
  static constexpr bool value = decltype(check(std::declval<T>()))::value;
};
// check if the Element is Structure
template<typename T>
class is_structure {
  template <typename U>
  static auto check(U) -> decltype(U::element_size(), std::true_type{});
  static std::false_type check(...);

 public:
  static constexpr bool value = decltype(check(std::declval<T>()))::value;
};
// Element Iterator: end
template <
        typename Enum,
        std::size_t index_,
        std::size_t bit_offset_,
        typename...>
struct ElementIterator {
  struct DummyIterator {};
  using element = DummyElement<Enum>;
  using next = DummyIterator;
  static constexpr std::size_t index = index_;
  static constexpr std::size_t bit_offset = bit_offset_;
};
// Element Iterator
template <
        typename Enum,
        size_t index_,
        size_t bit_offset_,
        typename Head,
        typename... Tail>
struct ElementIterator<Enum, index_, bit_offset_, Head, Tail...> {
  using element = Head;
  using next = ElementIterator<
          Enum,
          index_ + 1,
          bit_offset_ + Head::bit_size,
          Tail...>;
  static constexpr std::size_t index = index_;
  static constexpr std::size_t bit_offset = bit_offset_;
};
// Element Iterator: padding
template <
        typename Enum,
        size_t index,
        size_t bit_offset,
        size_t padding_bit_size,
        typename... Tail>
struct ElementIterator<
            Enum,
            index,
            bit_offset,
            Padding<padding_bit_size>,
            Tail...> :
        public ElementIterator<
                    Enum,
                    index,
                    bit_offset + Padding<padding_bit_size>::bit_size,
                    Tail...> {};
// get bit offset
template <
        typename Enum, Enum key, typename Iterator,
        typename std::enable_if<Iterator::element::key == Enum::End>::type *&
                    = enabler>
constexpr std::size_t get_bit_offset() {
  return Iterator::bit_offset;
}
template <
        typename Enum, Enum key, typename Iterator,
        typename std::enable_if<Iterator::element::key != Enum::End>::type *&
                    = enabler>
constexpr std::size_t get_bit_offset() {
  return key == Iterator::element::key
         ? Iterator::bit_offset
         : get_bit_offset<Enum, key, typename Iterator::next>();
}
// get index
template <
        typename Enum, Enum key, typename Iterator,
        typename std::enable_if<Iterator::element::key == Enum::End>::type *&
                    = enabler>
constexpr std::size_t get_index() {
  return Iterator::index;
}
template <
        typename Enum, Enum key, typename Iterator,
        typename std::enable_if<Iterator::element::key != Enum::End>::type *&
                    = enabler>
constexpr std::size_t get_index() {
  return key == Iterator::element::key
         ? Iterator::index
         : get_index<Enum, key, typename Iterator::next>();
}
}  // namespace impl

template <typename Enum, typename... Args>
class Structure {
 private:
  using iterator_begin = impl::ElementIterator<Enum, 0, 0, Args...>;
  // get element: end
  template <Enum, typename...>
  struct get_element {
    using type = impl::DummyElement<Enum>;
  };
  // get element
  template <Enum key, typename Head, typename... Tail>
  struct get_element<key, Head, Tail...> {
    static_assert(key != Head::key
                  || get_element<key, Tail...>::type::key == Enum::End,
                  "dupricate key");
    using type = typename std::conditional<
            key == Head::key,
            Head,
            typename get_element<key, Tail...>::type>
            ::type;
  };
  // get element: padding
  template <Enum key, std::size_t padding_bit_size, typename... Tail>
  struct get_element<key, Padding<padding_bit_size>, Tail...> {
    using type = typename get_element<key, Tail...>::type;
  };

 public:
  using kind = Enum;

  template <Enum key>
  using element = typename get_element<key, Args...>::type;

  template <Enum key>
  static constexpr std::size_t bit_offset() {
    static_assert(key != Enum::End, "End is reserved");
    static_assert(element<key>::key != Enum::End, "invalid key");
    return impl::get_bit_offset<Enum, key, iterator_begin>();
  }

  static constexpr std::size_t bit_size() {
    return impl::get_bit_offset<Enum, Enum::End, iterator_begin>();
  }

  static constexpr std::size_t byte_size() {
    return bit_size() / 8 + ((bit_size() % 8 == 0)? 0: 1);
  }

  template <Enum key>
  static constexpr std::size_t element_index() {
    static_assert(key != Enum::End, "End is reserved");
    static_assert(element<key>::key != Enum::End, "invalid key");
    return impl::get_index<Enum, key, iterator_begin>();
  }

  static constexpr std::size_t element_size() {
    return impl::get_index<Enum, Enum::End, iterator_begin>();
  }
};
}  // namespace binary_io
#endif  // BINARY_IO_STRUCTURE_HPP_
