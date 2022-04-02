target_include_directories(Io_Lib INTERFACE "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Source/Io>")

if(DEFINED BOARD_FILE)
    target_compile_definitions(Io_Lib INTERFACE BOARD_FILE="${BOARD_FILE}")
else()
    message(FATAL_ERROR "BOARD_FILE is not defined!")
endif()
