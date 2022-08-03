# Create our test executable
add_executable(RodosPlayground_test)

get_target_property(Rodos_include_dirs rodos::rodos INTERFACE_INCLUDE_DIRECTORIES)

target_sources(RodosPlayground_test PRIVATE test.cpp)
target_link_libraries(RodosPlayground_test PRIVATE Catch2::Catch2WithMain etl type_safe)

include(CTest)
include(Catch)
catch_discover_tests(RodosPlayground_test)
