include(CTest)

function(chip_8_test test_name)
    add_executable(${test_name}
        ${test_name}.cpp
        chip8_test.cpp
    )
    target_include_directories(${test_name} PRIVATE ${CHIP8_INCLUDE_DIRS})
    target_link_libraries(${test_name} PRIVATE chip-8 doctest::doctest)
    enable_warnings(${test_name})
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

set(test_list "clear;ret;jmp;movi;addi")
foreach (test_name ${test_list})
    chip_8_test(${test_name})
endforeach()
