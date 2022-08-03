# Create our test executable
add_executable(RodosPlayground_test)

get_target_property(Rodos_include_dirs rodos::rodos INTERFACE_INCLUDE_DIRECTORIES)

target_sources(RodosPlayground_test PRIVATE test.cpp)
target_link_libraries(RodosPlayground_test PRIVATE Catch2::Catch2WithMain etl type_safe)

#target_include_directories(RodosPlayground_test PRIVATE "${CMAKE_SOURCE_DIR}/Source/Demo")
#target_include_directories(RodosPlayground_test PUBLIC ${Rodos_include_dirs})
#target_link_directories(RodosPlayground_test PUBLIC ${Rodos_include_dirs})
#
#get_target_property(RodosPlayground_test_include_dirs RodosPlayground_test INCLUDE_DIRECTORIES)
#message("Found Rodos include dirs: ${Rodos_include_dirs}")
#message("Found RodosPlayground_test include dirs: ${RodosPlayground_test_include_dirs}")

include(CTest)
include(Catch)
catch_discover_tests(RodosPlayground_test)
