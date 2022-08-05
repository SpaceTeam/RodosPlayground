add_executable(RodosPlayground_test)

target_sources(RodosPlayground_test PRIVATE test.cpp)
target_link_libraries(RodosPlayground_test PRIVATE etl::etl Catch2::Catch2WithMain type_safe)
#target_link_libraries(RodosPlayground_test PRIVATE etl Catch2::Catch2WithMain type_safe)

#get_target_property(rodos_include_dirs rodos::rodos INTERFACE_INCLUDE_DIRECTORIES)
#message("Found rodos include dirs: ${rodos_include_dirs}")
#target_include_directories(RodosPlayground_test PRIVATE ${rodos_include_dirs})

include(CTest)
include(Catch)
catch_discover_tests(RodosPlayground_test)
