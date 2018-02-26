#include <boost/test/unit_test.hpp>
#include <array>
#include "binary_io/bitfield.hpp"
#include "binary_io/reader.hpp"
#include "binary_io/writer.hpp"
#include "./key.h"

namespace binary_io {
BOOST_AUTO_TEST_SUITE(bitfield)
// less than min
BOOST_AUTO_TEST_CASE(less_than_min) {
  using TestStructure = Structure<
          TestKey,
          Bitfield<TestKey, TestKey::Key00, 6, uint8_t, 0x10, 0x0f, 0x3f>>;
  // read
  const std::array<uint8_t, 1> read_buffer = {{0x08}};
  Reader<TestStructure> reader(read_buffer.data(), read_buffer.size());
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key00>(), 0x10);
  // write
  std::array<uint8_t, 1> write_buffer = {};
  Writer<TestStructure> writer(write_buffer.data(), write_buffer.size());
  writer.Set<TestKey::Key00>(0x00);
  BOOST_CHECK(writer.IsAllWritten());
  const std::array<uint8_t, 1> write_expected = {{0x10}};
  BOOST_CHECK_EQUAL_COLLECTIONS(
          write_expected.begin(),
          write_expected.end(),
          write_buffer.begin(),
          write_buffer.end());
}
// greater than max
BOOST_AUTO_TEST_CASE(greater_than_max) {
  using TestStructure = Structure<
          TestKey,
          Bitfield<TestKey, TestKey::Key00, 5, uint8_t, 0x01, 0x00, 0x10>>;
  // read
  const std::array<uint8_t, 1> read_buffer = {{0x1f}};
  Reader<TestStructure> reader(read_buffer.data(), read_buffer.size());
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key00>(), 0x01);
  // write
  std::array<uint8_t, 1> write_buffer = {};
  Writer<TestStructure> writer(write_buffer.data(), write_buffer.size());
  writer.Set<TestKey::Key00>(0x1f);
  BOOST_CHECK(writer.IsAllWritten());
  const std::array<uint8_t, 1> write_expected = {{0x01}};
  BOOST_CHECK_EQUAL_COLLECTIONS(
          write_expected.begin(),
          write_expected.end(),
          write_buffer.begin(),
          write_buffer.end());
}
// uint8_t
BOOST_AUTO_TEST_CASE(unit8) {
  using TestStructure = Structure<
          TestKey,
          Bitfield<TestKey, TestKey::Key00, 1, uint8_t>,
          Bitfield<TestKey, TestKey::Key01, 1, uint8_t>,
          Bitfield<TestKey, TestKey::Key02, 2, uint8_t>,
          Bitfield<TestKey, TestKey::Key03, 4, uint8_t>,
          Bitfield<TestKey, TestKey::Key04, 3, uint8_t>,
          Bitfield<TestKey, TestKey::Key05, 5, uint8_t>>;
  // read
  const std::array<uint8_t, 2> read_buffer = {{0xd5, 0x3e}};
  const Reader<TestStructure> reader(read_buffer.data(), read_buffer.size());
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key00>(), 0x1);
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key01>(), 0x0);
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key02>(), 0x1);
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key03>(), 0xd);
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key04>(), 0x6);
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key05>(), 0x7);
  // write
  std::array<uint8_t, 2> write_buffer = {};
  Writer<TestStructure> writer(write_buffer.data(), write_buffer.size());
  writer.Set<TestKey::Key00>(0x1);
  writer.Set<TestKey::Key01>(0x0);
  writer.Set<TestKey::Key02>(0x1);
  writer.Set<TestKey::Key03>(0xd);
  writer.Set<TestKey::Key04>(0x6);
  writer.Set<TestKey::Key05>(0x7);
  BOOST_CHECK(writer.IsAllWritten());
  BOOST_CHECK_EQUAL_COLLECTIONS(
          read_buffer.begin(),
          read_buffer.end(),
          write_buffer.begin(),
          write_buffer.end());
}
// uint16_t
BOOST_AUTO_TEST_CASE(uint16) {
  using TestStructure = Structure<
          TestKey,
          Bitfield<TestKey, TestKey::Key00, 4, uint16_t>,
          Bitfield<TestKey, TestKey::Key01, 12, uint16_t>>;
  // read
  const std::array<uint8_t, 2> read_buffer = {{0xde, 0xad}};
  const Reader<TestStructure> reader(read_buffer.data(), read_buffer.size());
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key00>(), 0xe);
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key01>(), 0xadd);
  // write
  std::array<uint8_t, 2> write_buffer = {};
  Writer<TestStructure> writer(write_buffer.data(), write_buffer.size());
  writer.Set<TestKey::Key00>(0xe);
  writer.Set<TestKey::Key01>(0xadd);
  BOOST_CHECK(writer.IsAllWritten());
  BOOST_CHECK_EQUAL_COLLECTIONS(
          read_buffer.begin(),
          read_buffer.end(),
          write_buffer.begin(),
          write_buffer.end());
}
BOOST_AUTO_TEST_SUITE_END()  // bitfield
}  // namespace binary_io
