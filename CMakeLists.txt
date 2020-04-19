cmake_minimum_required(VERSION 3.15)
project(StarWind)

set(CMAKE_CXX_STANDARD 20)
add_definitions(/MP)

include(build/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(sources)

