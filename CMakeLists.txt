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

# --- Declare targets ---

add_library(RodosPlayground_lib OBJECT)

add_executable(RodosPlayground_HelloWorld)
set_target_properties(RodosPlayground_HelloWorld PROPERTIES OUTPUT_NAME HelloWorld)

add_executable(RodosPlayground_HelloUart)
set_target_properties(RodosPlayground_HelloUart PROPERTIES OUTPUT_NAME HelloUart)

add_executable(RodosPlayground_HelloGpio)
set_target_properties(RodosPlayground_HelloGpio PROPERTIES OUTPUT_NAME HelloGpio)

add_executable(RodosPlayground_HelloSpi)
set_target_properties(RodosPlayground_HelloSpi PROPERTIES OUTPUT_NAME HelloSpi)

add_executable(RodosPlayground_ResetCounter)
set_target_properties(RodosPlayground_ResetCounter PROPERTIES OUTPUT_NAME ResetCounter)


add_subdirectory(Source)

# Automatically call objcopy on the executable targets after the build
if(CMAKE_SYSTEM_NAME STREQUAL Generic)
    include(cmake/custom-commands.cmake)

    objcopy_target(RodosPlayground_HelloWorld)
    objcopy_target(RodosPlayground_HelloUart)
    objcopy_target(RodosPlayground_HelloGpio)
    objcopy_target(RodosPlayground_HelloSpi)
endif()

# --- Developer mode ---

if(NOT RodosPlayground_DEVELOPER_MODE)
    return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
    message(AUTHOR_WARNING "Developer mode is intended for developers of RodosPlayground")
endif()

include(cmake/dev-mode.cmake)
