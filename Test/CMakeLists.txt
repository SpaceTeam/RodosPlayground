project(Test LANGUAGES CXX VERSION 0.0.1)

# Find libraries
include(${CMAKE_SOURCE_DIR}/cmake/custom-commands.cmake)

find_package(Catch2 REQUIRED)
find_package_and_notify(etl)
find_package_and_notify(type_safe)
find_rodos(0.1.0)

if(NOT (CMAKE_SYSTEM_NAME STREQUAL Generic))
	add_subdirectory(Source)
endif()


