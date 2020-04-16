add_subdirectory(Engine)
add_subdirectory(MeshTool)
add_subdirectory(Tests)
add_subdirectory(Game)

add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>
        )
