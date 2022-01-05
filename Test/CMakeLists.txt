# Parent project does not export its library target, so this CML implicitly depends on being added
# from it, i.e. the testing is done only from the build tree and is not feasible from an install
# location

project(RodosPlaygroundTests LANGUAGES CXX)

add_executable(RodosPlayground_test Source/RodosPlayground_Test.cpp)
target_link_libraries(RodosPlayground_test PRIVATE RodosPlayground_lib)

add_test(NAME RodosPlayground_test COMMAND RodosPlayground_test)

add_folders(Test)
