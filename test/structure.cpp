#include <boost/test/unit_test.hpp>
#include "binary_io/structure.hpp"
#include "./key.h"

namespace binary_io {
BOOST_AUTO_TEST_SUITE(structure)
// offset
BOOST_AUTO_TEST_CASE(bit_offset_and_index) {
  using TestStructure = Structure<
          TestKey,
          Element<TestKey, TestKey::Key01, 1, uint8_t>,
          Element<TestKey, TestKey::Key02, 3, uint8_t>,
          Element<TestKey, TestKey::Key03, 4, uint8_t>>;
  // offset
  BOOST_CHECK_EQUAL(TestStructure::bit_offset<TestKey::Key01>(), 0);
  BOOST_CHECK_EQUAL(TestStructure::bit_offset<TestKey::Key02>(), 1);
  BOOST_CHECK_EQUAL(TestStructure::bit_offset<TestKey::Key03>(), 4);
  BOOST_CHECK_EQUAL(TestStructure::bit_size(), 8);
  // index
  BOOST_CHECK_EQUAL(TestStructure::element_index<TestKey::Key01>(), 0);
  BOOST_CHECK_EQUAL(TestStructure::element_index<TestKey::Key02>(), 1);
  BOOST_CHECK_EQUAL(TestStructure::element_index<TestKey::Key03>(), 2);
  BOOST_CHECK_EQUAL(TestStructure::element_size(), 3);
}
// empty
BOOST_AUTO_TEST_CASE(empty) {
  using TestStructure = Structure<TestKey>;
  BOOST_CHECK_EQUAL(TestStructure::bit_size(), 0);
  BOOST_CHECK_EQUAL(TestStructure::element_size(), 0);
}
BOOST_AUTO_TEST_SUITE_END()  // structure
}  // namespace binary_io
