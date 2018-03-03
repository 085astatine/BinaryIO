#include <boost/test/unit_test.hpp>
#include <array>
#include "binary_io/binary.hpp"
#include "binary_io/reader.hpp"
#include "binary_io/writer.hpp"
#include "./key.h"

namespace binary_io {
BOOST_AUTO_TEST_SUITE(binary)
// float
BOOST_AUTO_TEST_CASE(binary) {
  using TestStructure = Structure<
        TestKey,
        Binary<TestKey, TestKey::Key00, 8>>;
  const std::array<uint8_t, 8> binary = {{
          0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe}};
  // read
  std::array<uint8_t, 8> read_buffer = {};
  Reader<TestStructure> reader(binary.data(), binary.size());
  reader.Get<TestKey::Key00>(read_buffer.data());
  BOOST_CHECK_EQUAL_COLLECTIONS(
          binary.begin(),
          binary.end(),
          read_buffer.begin(),
          read_buffer.end());
  // write
  std::array<uint8_t, 8> write_buffer = {};
  Writer<TestStructure> writer(write_buffer.data(), write_buffer.size());
  writer.Set<TestKey::Key00>(binary.data());
  BOOST_CHECK(writer.IsAllSet());
  BOOST_CHECK_EQUAL_COLLECTIONS(
          binary.begin(),
          binary.end(),
          write_buffer.begin(),
          write_buffer.end());
}
BOOST_AUTO_TEST_SUITE_END()  // binary
}  // namespace binary_io
