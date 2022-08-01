cmake_minimum_required(VERSION 3.22)

project(Test LANGUAGES CXX VERSION 0.0.1)

find_package(Catch2 REQUIRED)
add_executable(RodosPlayground_test test.cpp)
target_link_libraries(RodosPlayground_test PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(RodosPlayground_test)
