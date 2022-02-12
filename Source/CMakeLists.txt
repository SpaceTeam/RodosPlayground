# ---- Find packages ----

find_package(etl REQUIRED)
get_target_property(etl_include_dirs etl INTERFACE_INCLUDE_DIRECTORIES)
message("Found ETL include dirs: ${etl_include_dirs}")

set(RODOS_PACKAGE_NAME
    "rodos"
    CACHE STRING "Name of the Rodos package used when calling find_package()"
)
find_package(${RODOS_PACKAGE_NAME} REQUIRED 0.1.0)
get_target_property(rodos_location rodos::rodos LOCATION)
message("Found rodos: ${rodos_location}")

# ---- Define target stuff ----

target_sources(RodosPlayground_lib PRIVATE Lib.cpp)
target_include_directories(
    RodosPlayground_lib ${warning_guard} PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Source>"
)
target_compile_features(RodosPlayground_lib PUBLIC cxx_std_20)
target_link_libraries(RodosPlayground_lib PUBLIC etl)

target_sources(RodosPlayground_HelloWorld PRIVATE HelloWorld.cpp)
target_link_libraries(RodosPlayground_HelloWorld PRIVATE rodos::rodos RodosPlayground_lib)

add_subdirectory(HardwareTests)
