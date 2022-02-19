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

set(TargetsList
    "HelloWorld;HelloUart;HelloGpio;HelloSpi;ResetCounter;HelloHeartbeat;Beacon;EduHeartbeat"
)

function(add_exec_output name)
    add_executable("RodosPlayground_${name}")
    set_target_properties("RodosPlayground_${name}" PROPERTIES OUTPUT_NAME "${name}")
endfunction()

foreach(TargetName ${TargetsList})
    add_exec_output(${TargetName})
endforeach()

add_subdirectory(Source)

# Automatically call objcopy on the executable targets after the build
if(CMAKE_SYSTEM_NAME STREQUAL Generic)
    include(cmake/custom-commands.cmake)

    foreach(TargetName ${TargetsList})
        objcopy_target(RodosPlayground_${TargetName})
    endforeach()

endif()

# --- Developer mode ---

if(NOT RodosPlayground_DEVELOPER_MODE)
    return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
    message(AUTHOR_WARNING "Developer mode is intended for developers of RodosPlayground")
endif()

include(cmake/dev-mode.cmake)
