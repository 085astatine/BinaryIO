#include <boost/test/unit_test.hpp>
#include <array>
#include "binary_io/float.hpp"
#include "binary_io/reader.hpp"
#include "binary_io/writer.hpp"
#include "./key.h"

namespace binary_io {
BOOST_AUTO_TEST_SUITE(floating_point)
// float
BOOST_AUTO_TEST_CASE(float32) {
  using TestStructure = Structure<
          TestKey,
          Float<TestKey, TestKey::Key00, float>>;
  // read
  const float pi = 3.1415926f;
  std::array<uint8_t, 4> read_buffer = {};
  *reinterpret_cast<float*>(read_buffer.data()) = pi;
  Reader<TestStructure> reader(read_buffer.data(), read_buffer.size());
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key00>(), pi);
  // write
  std::array<uint8_t, 4> write_buffer = {};
  Writer<TestStructure> writer(write_buffer.data(), write_buffer.size());
  writer.Set<TestKey::Key00>(pi);
  BOOST_CHECK(writer.IsAllSet());
  BOOST_CHECK_EQUAL_COLLECTIONS(
          read_buffer.begin(),
          read_buffer.end(),
          write_buffer.begin(),
          write_buffer.end());
}
// double
BOOST_AUTO_TEST_CASE(float64) {
  using TestStructure = Structure<
          TestKey,
          Float<TestKey, TestKey::Key00, double>>;
  // read
  const float pi = 3.141592653589793;
  std::array<uint8_t, 8> read_buffer = {};
  *reinterpret_cast<double*>(read_buffer.data()) = pi;
  Reader<TestStructure> reader(read_buffer.data(), read_buffer.size());
  BOOST_CHECK_EQUAL(reader.Get<TestKey::Key00>(), pi);
  // write
  std::array<uint8_t, 8> write_buffer = {};
  Writer<TestStructure> writer(write_buffer.data(), write_buffer.size());
  writer.Set<TestKey::Key00>(pi);
  BOOST_CHECK(writer.IsAllSet());
  BOOST_CHECK_EQUAL_COLLECTIONS(
          read_buffer.begin(),
          read_buffer.end(),
          write_buffer.begin(),
          write_buffer.end());
}
BOOST_AUTO_TEST_SUITE_END()  // float
}  // namespace binary_io
