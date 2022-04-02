function(objcopy_target target_name)
    get_target_property(output_name ${target_name} OUTPUT_NAME)
    add_custom_command(
        TARGET ${target_name}
        POST_BUILD
        COMMAND "${CMAKE_OBJCOPY}" -O binary ${output_name} ${output_name}.bin
        BYPRODUCTS ${output_name}.bin
        COMMENT "Calling objcopy on ${output_name} to generate flashable ${output_name}.bin"
        VERBATIM
    )
endfunction()

function(add_program program_name)
    add_executable(RodosPlayground_${program_name} ${ARGN})
    set_target_properties(RodosPlayground_${program_name} PROPERTIES OUTPUT_NAME ${program_name})

    if(CMAKE_SYSTEM_NAME STREQUAL Generic)
        # Automatically call objcopy on the executable targets after the build
        objcopy_target(RodosPlayground_${program_name})
    endif()
endfunction()
