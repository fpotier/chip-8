function(copy_dll exe dll_list)
    foreach (dll ${dll_list})
        if (WIN32 AND ${BUILD_SHARED_LIBS})
            add_custom_command(TARGET ${exe} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:${dll}>
                $<TARGET_FILE_DIR:${exe}>
            )
        endif()
    endforeach()
endfunction()
