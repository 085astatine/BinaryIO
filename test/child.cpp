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
  BOOST_CHECK_EQUAL(TestStructure::element_index<TestKey::Key00>(), 0);
  BOOST_CHECK_EQUAL(TestStructure::element_index<TestKey::Key01>(), 2);
  BOOST_CHECK_EQUAL(TestStructure::element_size(), 3);
  // read & write
  const std::array<uint8_t, 3> binary{{0x10, 0x32, 0x54}};
  // read
  Reader<TestStructure> reader(binary.data(), binary.size());
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key00>()->Get<TestKey::Key00>(), 0x10);
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key00>()->Get<TestKey::Key01>(), 0x32);
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key01>(), 0x54);
}
// nested
BOOST_AUTO_TEST_CASE(nested) {
  using Inner0 = Structure<
          TestKey,
          Integer<TestKey, TestKey::Key00, uint8_t>,
          Integer<TestKey, TestKey::Key01, uint8_t>>;
  using Inner1 = Structure<
          TestKey,
          Child<TestKey, TestKey::Key00, Inner0>,
          Integer<TestKey, TestKey::Key01, uint16_t>>;
  using TestStructure = Structure<
          TestKey,
          Child<TestKey, TestKey::Key00, Inner1>,
          Child<TestKey, TestKey::Key01, Inner0>,
          Integer<TestKey, TestKey::Key02, uint8_t>>;
  // size
  BOOST_CHECK_EQUAL(TestStructure::bit_size(), 56);
  BOOST_CHECK_EQUAL(TestStructure::element_index<TestKey::Key00>(), 0);
  BOOST_CHECK_EQUAL(TestStructure::element_index<TestKey::Key01>(), 3);
  BOOST_CHECK_EQUAL(TestStructure::element_index<TestKey::Key02>(), 5);
  BOOST_CHECK_EQUAL(TestStructure::element_size(), 6);
}
BOOST_AUTO_TEST_SUITE_END()  // child
}  // namespace binary_io
