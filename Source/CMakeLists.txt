target_sources(RodosPlayground_lib PRIVATE Lib.cpp)
target_include_directories(
    RodosPlayground_lib ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Source>"
)

if(NOT RODOS_PACKAGE_NAME)
    message(
        FATAL_ERROR
        "Variable 'RODOS_PACKAGE_NAME' is not defined! It must be set on the command line with "
        "-DRODOS_PACKAGE_NAME=<value>, in a toolchain file or in a preset. It is usually of the "
        "form 'rodos_<port_name>', e.g., 'rodos_linux-x86'.")
endif()
find_package(${RODOS_PACKAGE_NAME} REQUIRED 0.0.1)
get_target_property(rodos_location rodos::rodos LOCATION)
message("Found rodos: ${rodos_location}")

target_sources(RodosPlayground_HelloWorld PRIVATE HelloWorld.cpp)
target_link_libraries(
    RodosPlayground_HelloWorld
    PRIVATE
    rodos::rodos
    RodosPlayground_lib
)
