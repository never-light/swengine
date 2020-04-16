file(GLOB_RECURSE MESH_TOOL_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
list(REMOVE_ITEM MESH_TOOL_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/precompiled.h)

file(GLOB_RECURSE MESH_TOOL_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        )

SET(MESH_TOOL_SOURCES ${MESH_TOOL_SOURCES} ${MESH_TOOL_INCLUDES})

add_executable(mesh_tool ${MESH_TOOL_SOURCES})

target_include_directories(mesh_tool PUBLIC ../)
target_include_directories(mesh_tool PRIVATE ./)

target_link_libraries(mesh_tool ${CONAN_LIBS}
        engine)
