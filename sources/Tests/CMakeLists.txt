# Build game as static library
file(GLOB_RECURSE GAME_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/../Game/*.h)
list(REMOVE_ITEM GAME_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/../Game/precompiled.h)

file(GLOB_RECURSE GAME_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../Game/*.cpp
        )

SET(GAME_SOURCES ${GAME_SOURCES} ${GAME_INCLUDES})

add_library(game_static ${GAME_SOURCES})

target_include_directories(game_static PUBLIC ../)
target_include_directories(game_static PRIVATE ../Game/)

target_compile_definitions(game_static PUBLIC GAME_AS_STATIC_LIB)

target_link_libraries(game_static ${CONAN_LIBS}
        engine)

# Build tests

file(GLOB_RECURSE TESTS_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

file(GLOB_RECURSE TESTS_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        )

SET(TESTS_SOURCES ${TESTS_SOURCES} ${TESTS_INCLUDES})

add_executable(tests ${TESTS_SOURCES})

target_include_directories(tests PUBLIC ../)
target_include_directories(tests PRIVATE ./)

target_link_libraries(tests ${CONAN_LIBS}
        engine
        game_static)
