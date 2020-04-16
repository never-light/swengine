file(GLOB_RECURSE TESTS_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
list(REMOVE_ITEM TESTS_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/precompiled.h)

file(GLOB_RECURSE TESTS_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        )

SET(TESTS_SOURCES ${TESTS_SOURCES} ${TESTS_INCLUDES})

add_executable(tests ${TESTS_SOURCES})

target_include_directories(tests PUBLIC ../)
target_include_directories(tests PRIVATE ./)

target_link_libraries(tests ${CONAN_LIBS}
        engine)
