target_sources(RodosPlayground_lib PRIVATE Lib.cpp)
target_include_directories(
    RodosPlayground_lib ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Source>"
)

target_sources(RodosPlayground_exe PRIVATE Main.cpp)
target_link_libraries(RodosPlayground_exe PRIVATE RodosPlayground_lib)
