target_include_directories(
	RodosWrappers_Lib INTERFACE "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Source/RodosWrappers>"
)
target_compile_features(RodosWrappers_Lib INTERFACE cxx_std_20)
target_link_libraries(RodosWrappers_Lib INTERFACE etl)
