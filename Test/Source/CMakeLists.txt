target_sources(RodosPlayground_Test PRIVATE Test.cpp)
target_link_libraries(RodosPlayground_Test PRIVATE etl::etl Catch2::Catch2WithMain type_safe)
