# Only include tests when building for Linux
if(NOT (CMAKE_SYSTEM_NAME STREQUAL Linux))
    return()
endif()

project(RodosPlaygroundTests LANGUAGES CXX)

find_package_and_notify(Catch2)
find_rodos()
find_package_and_notify(etl)
find_package_and_notify(type_safe)

add_executable(RodosPlayground_Test)

add_subdirectory(Source)

include(Catch)
catch_discover_tests(RodosPlayground_Test)

add_folders(Tests)
