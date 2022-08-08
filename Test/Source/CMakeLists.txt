add_executable(RodosPlayground_Test)

target_sources(RodosPlayground_Test PRIVATE Test.cpp)
target_link_libraries(RodosPlayground_Test PRIVATE etl::etl Catch2::Catch2WithMain type_safe)

include(CTest)
include(Catch)
catch_discover_tests(RodosPlayground_Test)
