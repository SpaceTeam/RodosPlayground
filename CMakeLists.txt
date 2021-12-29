cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    RodosPlayground
    VERSION 0.1.0
    DESCRIPTION "A project to play around with RODOS and all other tools and things we want to use for the COBC firmware."
    HOMEPAGE_URL ""
    LANGUAGES CXX
)

docs_early_return()

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    RodosPlayground_lib OBJECT
    source/lib.cpp
)

target_include_directories(
    RodosPlayground_lib ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(RodosPlayground_lib PUBLIC cxx_std_20)

# ---- Declare executable ----

add_executable(RodosPlayground_exe source/main.cpp)
add_executable(RodosPlayground::exe ALIAS RodosPlayground_exe)

set_target_properties(
    RodosPlayground_exe PROPERTIES
    OUTPUT_NAME RodosPlayground
    EXPORT_NAME exe
)

target_compile_features(RodosPlayground_exe PRIVATE cxx_std_20)

target_link_libraries(RodosPlayground_exe PRIVATE RodosPlayground_lib)

# ---- Developer mode ----

if(NOT RodosPlayground_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of RodosPlayground"
  )
endif()

include(cmake/dev-mode.cmake)
