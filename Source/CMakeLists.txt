target_sources(RodosPlayground_lib PRIVATE Lib.cpp)
target_include_directories(
    RodosPlayground_lib ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Source>"
)

find_package(${RODOS_PACKAGE_NAME} REQUIRED 0.0.1)

target_sources(RodosPlayground_HelloWorld PRIVATE HelloWorld.cpp)
target_link_libraries(
    RodosPlayground_HelloWorld
    PRIVATE
    rodos::rodos
    RodosPlayground_lib
)
