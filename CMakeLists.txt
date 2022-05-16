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
include(cmake/custom-commands.cmake)

# --- Declare targets ---

add_library(RodosPlayground_Lib OBJECT)
add_library(RodosWrappers_Lib INTERFACE)
add_library(Io_Lib INTERFACE)

add_program(HelloWorld)

if(CMAKE_SYSTEM_NAME STREQUAL Generic)
    add_program(HelloUart)
    add_program(HelloGpio)
    add_program(HelloSpi)
    add_program(Beacon)
    add_program(EduHeartbeat)
    add_program(EduData)
    add_program(CommandParser)
    add_program(Demo)
endif()

add_subdirectory(Source)

# --- Developer mode ---

if(NOT RodosPlayground_DEVELOPER_MODE)
    return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
    message(AUTHOR_WARNING "Developer mode is intended for developers of RodosPlayground")
endif()

include(cmake/dev-mode.cmake)
