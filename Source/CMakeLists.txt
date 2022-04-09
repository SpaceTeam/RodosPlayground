# --- Find packages ---

find_package_and_notify(etl)
find_package_and_notify(type_safe)

find_rodos(0.1.0)

# --- Define target stuff ---

target_sources(RodosPlayground_Lib PRIVATE Lib.cpp)
target_include_directories(
    RodosPlayground_Lib ${warning_guard} PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Source>"
)
target_compile_features(RodosPlayground_Lib PUBLIC cxx_std_20)
target_link_libraries(RodosPlayground_Lib PUBLIC etl)

target_sources(RodosPlayground_HelloWorld PRIVATE HelloWorld.cpp)
target_link_libraries(RodosPlayground_HelloWorld PRIVATE rodos::rodos RodosPlayground_Lib)

# --- Add subdirectories ---

if(CMAKE_SYSTEM_NAME STREQUAL Generic)
    add_subdirectory(HardwareTests)
    add_subdirectory(Demo)
    add_subdirectory(Io)
    add_subdirectory(RodosWrappers)
endif()
