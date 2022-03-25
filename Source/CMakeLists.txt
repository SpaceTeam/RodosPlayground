# --- Find packages ---

# Maybe hide this in a find_package_and_notify() function or something
find_package(etl REQUIRED)
get_target_property(etl_include_dirs etl INTERFACE_INCLUDE_DIRECTORIES)
message("Found ETL include dirs: ${etl_include_dirs}")

# Maybe hide this in a find_rodos() function
set(RODOS_PACKAGE_NAME
    "rodos"
    CACHE STRING "Name of the Rodos package used when calling find_package()"
)
find_package(${RODOS_PACKAGE_NAME} REQUIRED 0.1.0)
get_target_property(rodos_location rodos::rodos LOCATION)
message("Found rodos: ${rodos_location}")

# --- Define target stuff ---

target_sources(RodosPlayground_Lib PRIVATE Lib.cpp)
target_include_directories(
    RodosPlayground_Lib ${warning_guard} PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Source>"
)
target_compile_features(RodosPlayground_Lib PUBLIC cxx_std_20)
target_link_libraries(RodosPlayground_Lib PUBLIC etl)

target_sources(RodosPlayground_HelloWorld PRIVATE HelloWorld.cpp)
target_link_libraries(RodosPlayground_HelloWorld PRIVATE rodos::rodos RodosPlayground_Lib)

target_include_directories(Io_Lib INTERFACE "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Source/Io>")
# Define BOARD based on the port file,
# e.g. nucleo.cmake -> Board is set to "Nucleo.hpp"
cmake_path(GET CMAKE_BINARY_DIR STEM BOARD)
define_board_macro("${BOARD}")

# --- Add subdirectories ---

if(CMAKE_SYSTEM_NAME STREQUAL Generic)
    add_subdirectory(HardwareTests)
    add_subdirectory(Demo)
endif()
