cmake_minimum_required(VERSION 3.10)
# project
project("Binary Payload")
# option
add_definitions(
        -std=c++17
        -Wall
        -Wextra)
# show Setting
message("C Compiler ID: " ${CMAKE_C_COMPILER_ID})
message("CXX Compiler ID: " ${CMAKE_CXX_COMPILER_ID})
message("CMake System Name: " ${CMAKE_SYSTEM_NAME})
# boost
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(
        Boost REQUIRED
        unit_test_framework)
message("boost include dir: " ${Boost_INCLUDE_DIR})
message("boost unit test framework: " ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES})
# include directory
include_directories(
        ${Boost_INCLUDE_DIR}
        ${CMAKE_HOME_DIRECTORY}/include)
# test
add_executable(
        Test
        test.cpp
        test/bitfield.cpp
        test/structure.cpp)
target_link_libraries(
        Test
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES})
