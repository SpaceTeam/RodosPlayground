cmake_minimum_required(VERSION 3.22)

include(cmake/prelude.cmake)

project(
    RodosPlayground
    VERSION 0.1.0
    DESCRIPTION
        "A project to play around with RODOS and all other tools and things we want to use for the COBC firmware."
    HOMEPAGE_URL ""
    LANGUAGES CXX
)

docs_early_return()

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library and executable ----

add_library(RodosPlayground_lib OBJECT)

add_executable(RodosPlayground_HelloWorld)
set_target_properties(RodosPlayground_HelloWorld PROPERTIES OUTPUT_NAME HelloWorld)

add_executable(RodosPlayground_HelloUart)
set_target_properties(RodosPlayground_HelloUart PROPERTIES OUTPUT_NAME HelloUart)


add_executable(RodosPlayground_GPIO_test)
set_target_properties(RodosPlayground_GPIO_test PROPERTIES OUTPUT_NAME GPIO_test)

add_subdirectory(Source)

# Automatically call objcopy on the executable targets after the build

if(CMAKE_SYSTEM_NAME STREQUAL Generic)
    add_custom_command(
        TARGET RodosPlayground_HelloWorld
        POST_BUILD
        COMMAND "${CMAKE_OBJCOPY}" -O binary HelloWorld HelloWorld.bin
        BYPRODUCTS HelloWorld.bin
        COMMENT "Calling objcopy to generate flashable .bin file."
        VERBATIM
    )
endif()

# ---- Developer mode ----

if(NOT RodosPlayground_DEVELOPER_MODE)
    return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
    message(AUTHOR_WARNING "Developer mode is intended for developers of RodosPlayground")
endif()

include(cmake/dev-mode.cmake)
