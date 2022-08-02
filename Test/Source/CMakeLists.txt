# Create our test executable
add_executable(RodosPlayground_test)
target_sources(RodosPlayground_test PRIVATE test.cpp UtilityMock.hpp)
target_link_libraries(RodosPlayground_test PRIVATE Catch2::Catch2 etl type_safe)

include(CTest)
include(Catch)
catch_discover_tests(RodosPlayground_test)

