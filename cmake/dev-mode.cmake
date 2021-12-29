include(cmake/folders.cmake)

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(Test)
endif()

add_custom_target(
    run-hello-world
    COMMAND RodosPlayground_HelloWorld
    VERBATIM
)
add_dependencies(run-hello-world RodosPlayground_HelloWorld)

option(BUILD_MCSS_DOCS "Build documentation using Doxygen and m.css" OFF)
if(BUILD_MCSS_DOCS)
    include(cmake/docs.cmake)
endif()

option(ENABLE_COVERAGE "Enable coverage support separate from CTest's" OFF)
if(ENABLE_COVERAGE)
    include(cmake/coverage.cmake)
endif()

include(cmake/lint-targets.cmake)
include(cmake/spell-targets.cmake)

add_folders(Project)
