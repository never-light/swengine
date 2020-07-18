set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
set(BUILD_SHARED_LIBS TRUE)

file(GLOB_RECURSE ENGINE_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
list(REMOVE_ITEM ENGINE_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/precompiled.h)

file(GLOB_RECURSE ENGINE_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.c
        )

SET(ENGINE_SOURCES ${ENGINE_SOURCES} ${ENGINE_INCLUDES} Exceptions/exceptions.h)

add_library(engine STATIC ${ENGINE_SOURCES})

target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        ../../ThirdParty/OpenGL)

target_link_libraries(engine ${ENGINE_LIBS})

target_precompile_headers(engine PUBLIC precompiled.h)

install(TARGETS engine DESTINATION lib)
install(FILES ${ENGINE_INCLUDES} DESTINATION include/engine)
