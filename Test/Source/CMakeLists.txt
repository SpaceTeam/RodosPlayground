add_executable(RodosPlayground_test)

target_sources(RodosPlayground_test PRIVATE test.cpp)
target_link_libraries(RodosPlayground_test PRIVATE etl::etl Catch2::Catch2WithMain type_safe)

include(CTest)
include(Catch)
catch_discover_tests(RodosPlayground_test)
