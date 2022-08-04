project(Test LANGUAGES CXX VERSION 0.0.1)

message("Architecture(sizeofVoidP) : ${CMAKE_SIZEOF_VOID_P}")
include(${CMAKE_SOURCE_DIR}/cmake/custom-commands.cmake)

# Find libraries
find_package(Catch2 REQUIRED)
find_package(etl)
find_package_and_notify(type_safe)

# Do not include tests when we are not building for Linux
if(NOT (CMAKE_SYSTEM_NAME STREQUAL Generic))
	add_subdirectory(Source)
endif()
