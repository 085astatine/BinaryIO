#include <boost/test/unit_test.hpp>
#include <array>
#include "binary_io/array.hpp"
#include "binary_io/reader.hpp"
#include "binary_io/writer.hpp"
#include "./key.h"

namespace binary_io {
BOOST_AUTO_TEST_SUITE(array)
// uint8_t
BOOST_AUTO_TEST_CASE(uint8) {
  using TestStructure = Structure<
          TestKey,
          Array<TestKey, TestKey::Key00, uint8_t, 8>>;
  const std::array<uint8_t, 8> binary = {{
          0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe}};
  // read
  std::array<uint8_t, 8> read_buffer{};
  Reader<TestStructure> reader(binary.data(), binary.size());
  reader.Get<TestKey::Key00>(read_buffer.data());
  BOOST_CHECK_EQUAL_COLLECTIONS(
          binary.begin(),
          binary.end(),
          read_buffer.begin(),
          read_buffer.end());
  // write
  std::array<uint8_t, 8> write_buffer{};
  Writer<TestStructure> writer(write_buffer.data(), write_buffer.size());
  writer.Set<TestKey::Key00>(binary.data());
  BOOST_CHECK(writer.IsAllSet());
  BOOST_CHECK_EQUAL_COLLECTIONS(
          binary.begin(),
          binary.end(),
          write_buffer.begin(),
          write_buffer.end());
}
// uint16_t
BOOST_AUTO_TEST_CASE(uint16) {
  using TestStructure = Structure<
          TestKey,
          Array<TestKey, TestKey::Key00, uint16_t, 4>>;
  const std::array<uint8_t, 8> binary{
          {0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe}};
  const std::array<uint16_t, 4> expected{
          {0x3210, 0x7654, 0xba98, 0xfedc}};
  // read
  std::array<uint16_t, 4> read_buffer{};
  Reader<TestStructure> reader(binary.data(), binary.size());
  reader.Get<TestKey::Key00>(read_buffer.data());
  BOOST_CHECK_EQUAL_COLLECTIONS(
          expected.begin(),
          expected.end(),
          read_buffer.begin(),
          read_buffer.end());
  // write
  std::array<uint16_t, 4> write_buffer{};
  Writer<TestStructure> writer(
          write_buffer.data(),
          write_buffer.size() * sizeof(decltype(write_buffer)::value_type));
  writer.Set<TestKey::Key00>(expected.data());
  BOOST_CHECK(writer.IsAllSet());
  BOOST_CHECK_EQUAL_COLLECTIONS(
          expected.begin(),
          expected.end(),
          write_buffer.begin(),
          write_buffer.end());
}
// miss-alignment
BOOST_AUTO_TEST_CASE(miss_alignment) {
  using TestStructure = Structure<
          TestKey,
          Padding<8>,
          Array<TestKey, TestKey::Key00, uint16_t, 4>>;
  const std::array<uint8_t, 9> binary{
          {0xff, 0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe}};
  const std::array<uint16_t, 4> expected{
          {0x3210, 0x7654, 0xba98, 0xfedc}};
  // read
  std::array<uint16_t, 4> read_buffer{};
  Reader<TestStructure> reader(binary.data(), binary.size());
  reader.Get<TestKey::Key00>(read_buffer.data());
  BOOST_CHECK_EQUAL_COLLECTIONS(
          expected.begin(),
          expected.end(),
          read_buffer.begin(),
          read_buffer.end());
  // write
  std::array<uint8_t, 9> write_buffer{};
  Writer<TestStructure> writer(write_buffer.data(), write_buffer.size());
  writer.Set<TestKey::Key00>(expected.data());
  BOOST_CHECK(writer.IsAllSet());
  BOOST_CHECK_EQUAL(write_buffer.front(), 0x00);
  BOOST_CHECK_EQUAL_COLLECTIONS(
          binary.begin() + 1,
          binary.end(),
          write_buffer.begin() + 1,
          write_buffer.end());
}
BOOST_AUTO_TEST_SUITE_END()  // array
}  // namespace binary_io
