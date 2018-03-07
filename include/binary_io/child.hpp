#ifndef BINARY_IO_CHILD_HPP_
#define BINARY_IO_CHILD_HPP_

#include <type_traits>
#include "binary_io/element.hpp"

namespace binary_io {
template <
        typename Enum,
        Enum key,
        typename Structure>
class Child: public Element<Enum, key, Structure::bit_size(), Structure> {};
}  // namespace binary_io
#endif  // BINARY_IO_CHILD_HPP_
