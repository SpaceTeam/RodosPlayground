target_sources(RodosPlayground_lib PRIVATE Lib.cpp)
target_include_directories(
    RodosPlayground_lib ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Source>"
)

set(RODOS_PACKAGE_NAME "rodos" CACHE STRING
    "Name of the Rodos package used when calling find_package()")

find_package(${RODOS_PACKAGE_NAME} REQUIRED 0.1.0)
get_target_property(rodos_location rodos::rodos LOCATION)
message("Found rodos: ${rodos_location}")

target_sources(RodosPlayground_HelloWorld PRIVATE HelloWorld.cpp)
target_link_libraries(
    RodosPlayground_HelloWorld
    PRIVATE
    rodos::rodos
    RodosPlayground_lib
)
