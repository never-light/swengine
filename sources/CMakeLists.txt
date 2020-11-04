if(${CMAKE_GENERATOR} MATCHES "Unix Makefiles")
    # TODO: add wall and werror flags
elseif(MSVC)
    add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
    add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/W4>)
    add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/WX>)

    # Suppress "nonstandard extension used: nameless struct/union",
    # "nonstandard extension used : translation unit is empty",
    # "conditional expression is constant" warnings
    add_definitions(/wd4206 /wd4201 /wd4127 /wd5054)

    # Suppress "PDB was not found" linking warning
    add_link_options(/ignore:4099)
endif()

add_definitions(-DGLM_FORCE_SILENT_WARNINGS)

add_subdirectory(Engine)
add_subdirectory(Tests)
add_subdirectory(Game)
add_subdirectory(MeshTool)
