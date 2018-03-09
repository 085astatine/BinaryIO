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
          Integer<TestKey, TestKey::Key00, uint8_t>,
          Child<TestKey, TestKey::Key01, InnerStructure>,
          Integer<TestKey, TestKey::Key02, uint8_t>>;
  // size
  BOOST_CHECK_EQUAL(TestStructure::bit_size(), 32);
  BOOST_CHECK_EQUAL(TestStructure::element_index<TestKey::Key00>(), 0);
  BOOST_CHECK_EQUAL(TestStructure::element_index<TestKey::Key01>(), 1);
  BOOST_CHECK_EQUAL(TestStructure::element_index<TestKey::Key02>(), 3);
  BOOST_CHECK_EQUAL(TestStructure::element_size(), 4);
  // read & write
  const std::array<uint8_t, 4> binary{{0x10, 0x32, 0x54, 0x76}};
  // read
  Reader<TestStructure> reader(binary.data(), binary.size());
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key00>(), 0x10);
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key01>()->Get<TestKey::Key00>(), 0x32);
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key01>()->Get<TestKey::Key01>(), 0x54);
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key02>(), 0x76);
  // write
  std::array<uint8_t, 4> write_buffer{};
  Writer<TestStructure> writer(write_buffer.data(), write_buffer.size());
  writer.Set<TestKey::Key00>(0x10);
  BOOST_CHECK(!writer.IsAllSet());
  writer.Set<TestKey::Key01>()->Set<TestKey::Key00>(0x32);
  BOOST_CHECK(!writer.IsAllSet());
  writer.Set<TestKey::Key01>()->Set<TestKey::Key01>(0x54);
  BOOST_CHECK(!writer.IsAllSet());
  writer.Set<TestKey::Key02>(0x76);
  BOOST_CHECK(writer.IsAllSet());
  BOOST_CHECK_EQUAL_COLLECTIONS(
          binary.begin(),
          binary.end(),
          write_buffer.begin(),
          write_buffer.end());
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
  using Inner2 = Structure<
          TestKey,
          Child<TestKey, TestKey::Key00, Inner1>,
          Integer<TestKey, TestKey::Key01, uint32_t>>;
  using TestStructure = Structure<
          TestKey,
          Child<TestKey, TestKey::Key00, Inner0>,
          Child<TestKey, TestKey::Key01, Inner1>,
          Child<TestKey, TestKey::Key02, Inner2>,
          Integer<TestKey, TestKey::Key03, uint8_t>>;
  // size
  BOOST_CHECK_EQUAL(TestStructure::bit_size(), 120);
  BOOST_CHECK_EQUAL(TestStructure::element_index<TestKey::Key00>(), 0);
  BOOST_CHECK_EQUAL(TestStructure::element_index<TestKey::Key01>(), 2);
  BOOST_CHECK_EQUAL(TestStructure::element_index<TestKey::Key02>(), 5);
  BOOST_CHECK_EQUAL(TestStructure::element_index<TestKey::Key03>(), 9);
  BOOST_CHECK_EQUAL(TestStructure::element_size(), 10);
  // read & write
  const std::array<uint8_t, 15> binary{{
          0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe,
          0x21, 0x43, 0x65, 0x87, 0xa9, 0xcb, 0xed}};
  // read
  Reader<TestStructure> reader(binary.data(), binary.size());
  BOOST_CHECK_EQUAL(
          reader.Get<TestKey::Key00>()->Get<TestKey::Key00>(),
          0x10);
  BOOST_CHECK_EQUAL(
          reader.Get<TestKey::Key00>()->Get<TestKey::Key01>(),
          0x32);
  BOOST_CHECK_EQUAL(
          reader.Get<TestKey::Key01>()
                ->Get<TestKey::Key00>()
                ->Get<TestKey::Key00>(),
          0x54);
  BOOST_CHECK_EQUAL(
          reader.Get<TestKey::Key01>()
                ->Get<TestKey::Key00>()
                ->Get<TestKey::Key01>(),
          0x76);
  BOOST_CHECK_EQUAL(
          reader.Get<TestKey::Key01>()
                ->Get<TestKey::Key01>(),
          0xba98);
  BOOST_CHECK_EQUAL(
          reader.Get<TestKey::Key02>()
                ->Get<TestKey::Key00>()
                ->Get<TestKey::Key00>()
                ->Get<TestKey::Key00>(),
          0xdc);
  BOOST_CHECK_EQUAL(
          reader.Get<TestKey::Key02>()
                ->Get<TestKey::Key00>()
                ->Get<TestKey::Key00>()
                ->Get<TestKey::Key01>(),
          0xfe);
  BOOST_CHECK_EQUAL(
          reader.Get<TestKey::Key02>()
                ->Get<TestKey::Key00>()
                ->Get<TestKey::Key01>(),
          0x4321);
  BOOST_CHECK_EQUAL(
          reader.Get<TestKey::Key02>()
                ->Get<TestKey::Key01>(),
          0xcba98765);
  BOOST_CHECK_EQUAL(
          reader.Get<TestKey::Key03>(),
          0xed);
  // write
  std::array<uint8_t, 15> write_buffer{};
  Writer<TestStructure> writer(write_buffer.data(), write_buffer.size());
  writer.Set<TestKey::Key00>()
        ->Set<TestKey::Key00>(0x10);
  writer.Set<TestKey::Key00>()
        ->Set<TestKey::Key01>(0x32);
  writer.Set<TestKey::Key01>()
        ->Set<TestKey::Key00>()
        ->Set<TestKey::Key00>(0x54);
  writer.Set<TestKey::Key01>()
        ->Set<TestKey::Key00>()
        ->Set<TestKey::Key01>(0x76);
  writer.Set<TestKey::Key01>()
        ->Set<TestKey::Key01>(0xba98);
  writer.Set<TestKey::Key02>()
        ->Set<TestKey::Key00>()
        ->Set<TestKey::Key00>()
        ->Set<TestKey::Key00>(0xdc);
  writer.Set<TestKey::Key02>()
        ->Set<TestKey::Key00>()
        ->Set<TestKey::Key00>()
        ->Set<TestKey::Key01>(0xfe);
  writer.Set<TestKey::Key02>()
        ->Set<TestKey::Key00>()
        ->Set<TestKey::Key01>(0x4321);
  writer.Set<TestKey::Key02>()
        ->Set<TestKey::Key01>(0xcba98765);
  BOOST_CHECK(!writer.IsAllSet());
  writer.Set<TestKey::Key03>(0xed);
  BOOST_CHECK(writer.IsAllSet());
  BOOST_CHECK_EQUAL_COLLECTIONS(
          binary.begin(),
          binary.end(),
          write_buffer.begin(),
          write_buffer.end());
}
BOOST_AUTO_TEST_SUITE_END()  // child
}  // namespace binary_io
