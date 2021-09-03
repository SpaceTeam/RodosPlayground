cmake_minimum_required(VERSION 3.21)

include(cmake/in-source-guard.cmake)

project(
    rodos_playground
    VERSION 0.1.0
    DESCRIPTION "A project to play around with RODOS and all other tools and things we want to use for the COBC firmware."
    HOMEPAGE_URL " "
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    rodos_playground_lib OBJECT
    source/lib.cpp
)

target_include_directories(
    rodos_playground_lib
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(rodos_playground_lib PUBLIC cxx_std_20)

# ---- Declare executable ----

add_executable(rodos_playground_rodos_playground source/main.cpp)
add_executable(rodos_playground::rodos_playground ALIAS rodos_playground_rodos_playground)

set_target_properties(
    rodos_playground_rodos_playground PROPERTIES
    OUTPUT_NAME rodos_playground
    EXPORT_NAME rodos_playground
)

target_compile_features(rodos_playground_rodos_playground PRIVATE cxx_std_20)

target_link_libraries(rodos_playground_rodos_playground PRIVATE rodos_playground_lib)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT rodos_playground_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of rodos_playground"
  )
endif()

include(cmake/dev-mode.cmake)
