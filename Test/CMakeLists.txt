# Parent project does not export its library target, so this CML implicitly depends on being added
# from it, i.e. the testing is done only from the build tree and is not feasible from an install
# location

project(RodosPlaygroundTests LANGUAGES CXX)

add_executable(RodosPlayground_Test Source/RodosPlayground_Test.cpp)
target_link_libraries(RodosPlayground_Test PRIVATE RodosPlayground_Lib)

add_test(NAME RodosPlayground_Test COMMAND RodosPlayground_Test)

add_folders(Test)
