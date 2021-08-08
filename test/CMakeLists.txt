# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(rodos_playgroundTests LANGUAGES CXX)

add_executable(rodos_playground_test source/rodos_playground_test.cpp)
target_link_libraries(rodos_playground_test PRIVATE rodos_playground_lib)
target_compile_features(rodos_playground_test PRIVATE cxx_std_20)

add_test(NAME rodos_playground_test COMMAND rodos_playground_test)
