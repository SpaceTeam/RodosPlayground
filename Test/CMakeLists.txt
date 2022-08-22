# Only include tests when building for Linux
if(NOT (CMAKE_SYSTEM_NAME STREQUAL Linux))
    return()
endif()

project(RodosPlaygroundTests LANGUAGES CXX)

find_package_and_notify(Catch2)
find_package_and_notify(etl)
find_package_and_notify(type_safe)
find_rodos()

file(GLOB test_files
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    utility/*.cpp)

message("test-files : ${test_files}")

#foreach(test_file ${test_files})
#
#    # filename: Used to get name of expected output
#    get_filename_component(filename ${test_file} NAME_WE)
#    # test_name: replaced_problematic_symbols
#    string(REGEX REPLACE "[^A-Za-z0-9_]" "_" test_name ${test_file})
#
#    # Compile rodos test application
#    add_executable("${test_name}-bin" ${test_file} /home/jerome/Documents/ST/rodos_playground/Source/Lib.cpp)
#    target_include_directories("${test_name}-bin"
#        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
#    target_include_directories("${test_name}-bin"
#        PRIVATE /home/jerome/Documents/ST/rodos_playground/Source/)
#
#    target_link_libraries("${test_name}-bin" PUBLIC rodos::rodos)
#
#
#    # Command to run rodos test application, generating output files
#    # including diff; this also generates coverage information if
#    # coverage reporting is enabled.
#    add_custom_command(
#        OUTPUT
#        "${test_name}-bin.output"
#        "${test_name}-bin.output_trimmed"
#        "${test_name}-bin.expected_trimmed"
#        "${test_name}-bin.diff"
#        COMMAND
#        bash
#        ${CMAKE_CURRENT_SOURCE_DIR}/test-runner.sh
#        $<TARGET_FILE:${test_name}-bin>
#        ${CMAKE_CURRENT_SOURCE_DIR}/expected-outputs/${filename}.txt
#        DEPENDS
#        ${test_name}-bin test-runner.sh)
#    #${coverage_prepare_dep}) # dependency: coverage_prepare <-- run of test
#
#    list(APPEND diff_files "${test_name}-bin.diff")
#
#    add_custom_target(${test_name}-clean
#        COMMAND
#        ${CMAKE_COMMAND} -E remove -f
#        "${test_name}-bin.output"
#        "${test_name}-bin.output_trimmed"
#        "${test_name}-bin.expected_trimmed"
#        "${test_name}-bin.diff")
#
#    add_custom_target(${test_name}-run ALL DEPENDS "${test_name}-bin.diff")
#
#    add_test(
#        NAME 
#        ${test_name}-test
#        COMMAND 
#        diff ${test_name}-bin.output ${CMAKE_CURRENT_SOURCE_DIR}/expected-outputs/${filename}.txt 
#        )
#
#endforeach()


add_executable(RodosPlayground_Test)
get_target_property(include_dirs rodos::rodos INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(RodosPlayground_Test PRIVATE ${include_dirs})

#add_test(NAME ${test-name} COMMAND f)
#add_test(NAME fifo-test COMMAND false)
add_subdirectory(Source)

include(Catch)
catch_discover_tests(RodosPlayground_Test)
add_folders(Tests)
