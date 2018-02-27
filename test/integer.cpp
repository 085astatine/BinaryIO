#include <boost/test/unit_test.hpp>
#include <array>
#include "binary_io/integer.hpp"
#include "binary_io/reader.hpp"
#include "binary_io/writer.hpp"
#include "./key.h"

#include <iostream>

namespace binary_io {
BOOST_AUTO_TEST_SUITE(integer)
// less than min
BOOST_AUTO_TEST_CASE(less_than_min) {
  using TestStructure = Structure<
          TestKey,
          Integer<TestKey, TestKey::Key00, int8_t, -1, 0, 127>>;
  // read
  const std::array<uint8_t, 1> read_buffer = {{static_cast<uint8_t>(-2)}};
  Reader<TestStructure> reader(read_buffer.data(), read_buffer.size());
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key00>(), -1);
  // write
  std::array<uint8_t, 1> write_buffer = {};
  Writer<TestStructure> writer(write_buffer.data(), write_buffer.size());
  writer.Set<TestKey::Key00>(-2);
  BOOST_CHECK(writer.IsAllSet());
  const std::array<uint8_t, 1> write_expected  = {{static_cast<uint8_t>(-1)}};
  BOOST_CHECK_EQUAL_COLLECTIONS(
          write_expected.begin(),
          write_expected.end(),
          write_buffer.begin(),
          write_buffer.end());
}
// grater than max
BOOST_AUTO_TEST_CASE(greater_than_max) {
  using TestStructure = Structure<
          TestKey,
          Integer<TestKey, TestKey::Key00, uint8_t, 0, 0, 1>>;
  // read
  const std::array<uint8_t, 1> read_buffer = {{2}};
  Reader<TestStructure> reader(read_buffer.data(), read_buffer.size());
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key00>(), 0);
  // write
  std::array<uint8_t, 1> write_buffer = {};
  Writer<TestStructure> writer(write_buffer.data(), write_buffer.size());
  writer.Set<TestKey::Key00>(2);
  BOOST_CHECK(writer.IsAllSet());
  const std::array<uint8_t, 1> write_expected  = {{0}};
  BOOST_CHECK_EQUAL_COLLECTIONS(
          write_expected.begin(),
          write_expected.end(),
          write_buffer.begin(),
          write_buffer.end());
}
// uint8_t
BOOST_AUTO_TEST_CASE(uint8) {
  using TestStructure = Structure<
          TestKey,
          Integer<TestKey, TestKey::Key00, uint8_t>>;
  // read
  const std::array<uint8_t, 1> read_buffer = {{42}};
  const Reader<TestStructure> reader(read_buffer.data(), read_buffer.size());
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key00>(), 42);
  // write
  std::array<uint8_t, 1>  write_buffer = {};
  Writer<TestStructure> writer(write_buffer.data(), write_buffer.size());
  writer.Set<TestKey::Key00>(42);
  BOOST_CHECK(writer.IsAllSet());
  BOOST_CHECK_EQUAL_COLLECTIONS(
          read_buffer.begin(),
          read_buffer.end(),
          write_buffer.begin(),
          write_buffer.end());
}
// int8_t
BOOST_AUTO_TEST_CASE(int8) {
  using TestStructure = Structure<
          TestKey,
          Integer<TestKey, TestKey::Key00, int8_t>>;
  // read
  std::array<uint8_t, 1> read_buffer = {};
  *reinterpret_cast<uint8_t*>(read_buffer.data()) = -42;
  const Reader<TestStructure> reader(read_buffer.data(), read_buffer.size());
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key00>(), -42);
  // write
  std::array<uint8_t, 1>  write_buffer = {};
  Writer<TestStructure> writer(write_buffer.data(), write_buffer.size());
  writer.Set<TestKey::Key00>(-42);
  BOOST_CHECK(writer.IsAllSet());
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
          Integer<TestKey, TestKey::Key00, uint16_t>>;
  // read
  const std::array<uint8_t, 2> read_buffer = {{0xfe, 0xca}};
  const Reader<TestStructure> reader(read_buffer.data(), read_buffer.size());
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key00>(), 0xcafe);
  // write
  std::array<uint8_t, 2>  write_buffer = {};
  Writer<TestStructure> writer(write_buffer.data(), write_buffer.size());
  writer.Set<TestKey::Key00>(0xcafe);
  BOOST_CHECK(writer.IsAllSet());
  BOOST_CHECK_EQUAL_COLLECTIONS(
          read_buffer.begin(),
          read_buffer.end(),
          write_buffer.begin(),
          write_buffer.end());
}
// int16_t
BOOST_AUTO_TEST_CASE(int16) {
  using TestStructure = Structure<
          TestKey,
          Integer<TestKey, TestKey::Key00, int16_t>>;
  // read
  std::array<uint8_t, 2> read_buffer = {};
  *reinterpret_cast<int16_t*>(read_buffer.data()) = -12345;
  const Reader<TestStructure> reader(read_buffer.data(), read_buffer.size());
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key00>(), -12345);
  // write
  std::array<uint8_t, 2>  write_buffer = {};
  Writer<TestStructure> writer(write_buffer.data(), write_buffer.size());
  writer.Set<TestKey::Key00>(-12345);
  BOOST_CHECK(writer.IsAllSet());
  BOOST_CHECK_EQUAL_COLLECTIONS(
          read_buffer.begin(),
          read_buffer.end(),
          write_buffer.begin(),
          write_buffer.end());
}
// uint32_t
BOOST_AUTO_TEST_CASE(uint32) {
  using TestStructure = Structure<
          TestKey,
          Integer<TestKey, TestKey::Key00, uint32_t>>;
  // read
  const std::array<uint8_t, 4> read_buffer = {{0xef, 0xbe, 0xad, 0xde}};
  const Reader<TestStructure> reader(read_buffer.data(), read_buffer.size());
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key00>(), 0xdeadbeef);
  // write
  std::array<uint8_t, 4>  write_buffer = {};
  Writer<TestStructure> writer(write_buffer.data(), write_buffer.size());
  writer.Set<TestKey::Key00>(0xdeadbeef);
  BOOST_CHECK(writer.IsAllSet());
  BOOST_CHECK_EQUAL_COLLECTIONS(
          read_buffer.begin(),
          read_buffer.end(),
          write_buffer.begin(),
          write_buffer.end());
}
// int32_t
BOOST_AUTO_TEST_CASE(int32) {
  using TestStructure = Structure<
          TestKey,
          Integer<TestKey, TestKey::Key00, int32_t>>;
  // read
  std::array<uint8_t, 4> read_buffer = {};
  *reinterpret_cast<int32_t*>(read_buffer.data()) = -1234567890;
  const Reader<TestStructure> reader(read_buffer.data(), read_buffer.size());
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key00>(), -1234567890);
  // write
  std::array<uint8_t, 4>  write_buffer = {};
  Writer<TestStructure> writer(write_buffer.data(), write_buffer.size());
  writer.Set<TestKey::Key00>(-1234567890);
  BOOST_CHECK(writer.IsAllSet());
  BOOST_CHECK_EQUAL_COLLECTIONS(
          read_buffer.begin(),
          read_buffer.end(),
          write_buffer.begin(),
          write_buffer.end());
}
// uint64_t
BOOST_AUTO_TEST_CASE(uint64) {
  using TestStructure = Structure<
          TestKey,
          Integer<TestKey, TestKey::Key00, uint64_t>>;
  // read
  const std::array<uint8_t, 8> read_buffer =
          {{0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe}};
  const Reader<TestStructure> reader(read_buffer.data(), read_buffer.size());
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key00>(), 0xfedcba9876543210);
  // write
  std::array<uint8_t, 8> write_buffer = {};
  Writer<TestStructure> writer(write_buffer.data(), write_buffer.size());
  writer.Set<TestKey::Key00>(0xfedcba9876543210);
  BOOST_CHECK(writer.IsAllSet());
  BOOST_CHECK_EQUAL_COLLECTIONS(
          read_buffer.begin(),
          read_buffer.end(),
          write_buffer.begin(),
          write_buffer.end());
}
// int64_t
BOOST_AUTO_TEST_CASE(int64) {
  using TestStructure = Structure<
          TestKey,
          Integer<TestKey, TestKey::Key00, int64_t>>;
  // read
  std::array<uint8_t, 8> read_buffer = {};
  *reinterpret_cast<int64_t*>(read_buffer.data()) = -1234567890987654321;
  const Reader<TestStructure> reader(read_buffer.data(), read_buffer.size());
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key00>(), -1234567890987654321);
  // write
  std::array<uint8_t, 8>  write_buffer = {};
  Writer<TestStructure> writer(write_buffer.data(), write_buffer.size());
  writer.Set<TestKey::Key00>(-1234567890987654321);
  BOOST_CHECK(writer.IsAllSet());
  BOOST_CHECK_EQUAL_COLLECTIONS(
          read_buffer.begin(),
          read_buffer.end(),
          write_buffer.begin(),
          write_buffer.end());
}
// mix-in
BOOST_AUTO_TEST_CASE(mix_in) {
  using TestStructure = Structure<
          TestKey,
          Integer<TestKey, TestKey::Key00, uint8_t>,
          Integer<TestKey, TestKey::Key01, uint16_t>,
          Integer<TestKey, TestKey::Key02, uint32_t>,
          Integer<TestKey, TestKey::Key03, uint64_t>>;
  // read
  const std::array<uint8_t, 15> read_buffer = {{
          0x10,
          0x32, 0x10,
          0x76, 0x54, 0x32, 0x10,
          0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10}};
  const Reader<TestStructure> reader(read_buffer.data(), read_buffer.size());
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key00>(), 0x10);
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key01>(), 0x1032);
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key02>(), 0x10325476);
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key03>(), 0x1032547698badcfe);
  // write
  std::array<uint8_t, 15> write_buffer = {};
  Writer<TestStructure> writer(write_buffer.data(), write_buffer.size());
  writer.Set<TestKey::Key00>(0x10);
  writer.Set<TestKey::Key01>(0x1032);
  writer.Set<TestKey::Key02>(0x10325476);
  writer.Set<TestKey::Key03>(0x1032547698badcfe);
  BOOST_CHECK(writer.IsAllSet());
  BOOST_CHECK_EQUAL_COLLECTIONS(
          read_buffer.begin(),
          read_buffer.end(),
          write_buffer.begin(),
          write_buffer.end());
}
BOOST_AUTO_TEST_SUITE_END()  // integer
}  // namespace binary_io
