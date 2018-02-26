#ifndef BINARY_IO_FLOAT_HPP_
#define BINARY_IO_FLOAT_HPP_

#include <type_traits>
#include "binary_io/value.hpp"

namespace binary_io {
template <typename Enum, Enum key, typename Type>
class Float: public Value<Enum, key, Type> {
  static_assert(std::is_floating_point<Type>::value, "invalid type");

 public:
  static constexpr Type DefaultValue() {
    return Type(0);
  }
};
}  // namespace binary_io
#endif  // BINARY_IO_FLOAT_HPP_
