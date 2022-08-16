set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

function(enable_warnings target)
    target_compile_options(${target} PRIVATE
        $<$<CXX_COMPILER_ID:GNU>: -Wall -Wextra -Wpedantic -fdiagnostics-color=always>
        $<$<CXX_COMPILER_ID:Clang>: -Wall -Wextra -Wpedantic -fcolor-diagnostics>
        $<$<CXX_COMPILER_ID:MSVC>: /W3>)
endfunction()
