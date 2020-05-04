if(${CMAKE_GENERATOR} MATCHES "Unix Makefiles")
    # TODO: add wall and werror flags
elseif(MSVC)
    add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
    add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/W4>)
#    add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/WX>)
endif()

add_subdirectory(Engine)
add_subdirectory(MeshTool)
add_subdirectory(Tests)
add_subdirectory(Game)
