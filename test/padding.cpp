#include <boost/test/unit_test.hpp>
#include <array>
#include "binary_io/bitfield.hpp"
#include "binary_io/reader.hpp"
#include "binary_io/structure.hpp"
#include "binary_io/writer.hpp"
#include "./key.h"

namespace binary_io {
BOOST_AUTO_TEST_SUITE(padding)
// padding
BOOST_AUTO_TEST_CASE(padding) {
  using TestStructure = Structure<
          TestKey,
          Bitfield<TestKey, TestKey::Key00, 1, uint8_t>,
          Padding<4>,
          Bitfield<TestKey, TestKey::Key01, 3, uint8_t>>;
  // offset
  BOOST_CHECK_EQUAL(TestStructure::bit_offset<TestKey::Key00>(), 0);
  BOOST_CHECK_EQUAL(TestStructure::bit_offset<TestKey::Key01>(), 5);
  BOOST_CHECK_EQUAL(TestStructure::bit_size(), 8);
  // index
  BOOST_CHECK_EQUAL(TestStructure::element_index<TestKey::Key00>(), 0);
  BOOST_CHECK_EQUAL(TestStructure::element_index<TestKey::Key01>(), 1);
  BOOST_CHECK_EQUAL(TestStructure::element_size(), 2);
  // read
  const std::array<uint8_t, 1> read_buffer = {{0xff}};
  const Reader<TestStructure> reader(read_buffer.data(), read_buffer.size());
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key00>(), 0x1);
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key01>(), 0x7);
  // write
  std::array<uint8_t, 1> write_buffer = {};
  Writer<TestStructure> writer(write_buffer.data(), write_buffer.size());
  writer.Set<TestKey::Key00>(0x1);
  writer.Set<TestKey::Key01>(0x7);
  BOOST_CHECK(writer.IsAllWritten());
  const std::array<uint8_t, 1> write_expected = {{0xe1}};
  BOOST_CHECK_EQUAL_COLLECTIONS(
          write_expected.begin(),
          write_expected.end(),
          write_buffer.begin(),
          write_buffer.end());
}
// padding only
BOOST_AUTO_TEST_CASE(padding_only) {
  using TestStructure = Structure<TestKey, Padding<8>>;
  BOOST_CHECK_EQUAL(TestStructure::bit_size(), 8);
  BOOST_CHECK_EQUAL(TestStructure::element_size(), 0);
}
BOOST_AUTO_TEST_SUITE_END()  // structure
}  // namespace binary_io
