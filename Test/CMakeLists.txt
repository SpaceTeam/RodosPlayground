project(Test LANGUAGES CXX VERSION 0.0.1)

# Do not try to include catch2 if arch is not 64 bits
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	find_package(Catch2 REQUIRED)
	add_executable(RodosPlayground_test test.cpp)
	target_link_libraries(RodosPlayground_test PRIVATE Catch2::Catch2WithMain)

	include(CTest)
	include(Catch)
	catch_discover_tests(RodosPlayground_test)
endif ()

message("CMake source dir is : ${CMAKE_SOURCE_DIR}")

# Find libraries
include(${CMAKE_SOURCE_DIR}/cmake/custom-commands.cmake)

find_package(Catch2 REQUIRED)
find_package_and_notify(etl)
find_package_and_notify(type_safe)

add_executable(RodosPlayground_test)
target_sources(RodosPlayground_test PRIVATE test.cpp UtilityMock.hpp)
#target_link_libraries(RodosPlayground_test Catch2::Catch2)
target_link_libraries(RodosPlayground_test PRIVATE Catch2::Catch2 etl type_safe)

include(CTest)
include(Catch)
catch_discover_tests(RodosPlayground_test)

