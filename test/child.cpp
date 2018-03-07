#include <boost/test/unit_test.hpp>
#include <array>
#include "binary_io/child.hpp"
#include "binary_io/integer.hpp"
#include "binary_io/reader.hpp"
#include "binary_io/writer.hpp"
#include "./key.h"

namespace binary_io {
BOOST_AUTO_TEST_SUITE(child)
// uint8_t
BOOST_AUTO_TEST_CASE(uint8) {
  using InnerStructure = Structure<
          TestKey,
          Integer<TestKey, TestKey::Key00, uint8_t>,
          Integer<TestKey, TestKey::Key01, uint8_t>>;
  using TestStructure = Structure<
          TestKey,
          Child<TestKey, TestKey::Key00, InnerStructure>,
          Integer<TestKey, TestKey::Key01, uint8_t>>;
  // size
  BOOST_CHECK_EQUAL(TestStructure::bit_size(), 24);
  BOOST_CHECK_EQUAL(TestStructure::element_size(), 2);
}
BOOST_AUTO_TEST_SUITE_END()  // child
}  // namespace binary_io
