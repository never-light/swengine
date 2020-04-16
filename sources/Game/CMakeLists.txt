file(GLOB_RECURSE GAME_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
list(REMOVE_ITEM GAME_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/precompiled.h)

file(GLOB_RECURSE GAME_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        )

SET(GAME_SOURCES ${GAME_SOURCES} ${GAME_INCLUDES})

add_executable (game ${GAME_SOURCES})

target_include_directories(game PUBLIC ../)
target_include_directories(game PRIVATE ./)

target_link_libraries (game ${CONAN_LIBS}
        engine)
