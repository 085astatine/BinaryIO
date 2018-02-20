#include <boost/test/unit_test.hpp>
#include "binary_io/structure.hpp"

namespace binary_io {
BOOST_AUTO_TEST_SUITE(structure)
// offset
BOOST_AUTO_TEST_CASE(bit_offset_and_index) {
  enum class Key {
    Hoge,
    Fuga,
    Piyo,
    End,
  };
  using TestStructure = Structure<
          Key,
          Element<Key, Key::Hoge, 1, uint8_t>,
          Element<Key, Key::Fuga, 3, uint8_t>,
          Element<Key, Key::Piyo, 4, uint8_t>>;
  // offset
  BOOST_CHECK_EQUAL(TestStructure::bit_offset<Key::Hoge>(), 0);
  BOOST_CHECK_EQUAL(TestStructure::bit_offset<Key::Fuga>(), 1);
  BOOST_CHECK_EQUAL(TestStructure::bit_offset<Key::Piyo>(), 4);
  BOOST_CHECK_EQUAL(TestStructure::bit_size(), 8);
  // index
  BOOST_CHECK_EQUAL(TestStructure::element_index<Key::Hoge>(), 0);
  BOOST_CHECK_EQUAL(TestStructure::element_index<Key::Fuga>(), 1);
  BOOST_CHECK_EQUAL(TestStructure::element_index<Key::Piyo>(), 2);
  BOOST_CHECK_EQUAL(TestStructure::element_size(), 3);
}
// empty
BOOST_AUTO_TEST_CASE(empty) {
  enum class Key {
    End,
  };
  using TestStructure = Structure<Key>;
  BOOST_CHECK_EQUAL(TestStructure::bit_size(), 0);
  BOOST_CHECK_EQUAL(TestStructure::element_size(), 0);
}
BOOST_AUTO_TEST_SUITE_END()  // structure
}  // namespace binary_io
