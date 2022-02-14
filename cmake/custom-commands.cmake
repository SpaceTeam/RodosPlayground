function(objcopy_target target)
    get_target_property(output_name ${target} OUTPUT_NAME)
    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND "${CMAKE_OBJCOPY}" -O binary ${output_name} ${output_name}.bin
        BYPRODUCTS ${output_name}.bin
        COMMENT "Calling objcopy on ${output_name} to generate flashable ${output_name}.bin"
        VERBATIM
    )
endfunction()