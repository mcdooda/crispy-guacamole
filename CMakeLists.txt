cmake_minimum_required(VERSION 2.6)

project(crispy-guacamole)

include_directories(flat-engine/src)

file(
    GLOB_RECURSE
    src
    src/*
    flat-engine/src/*
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -Wall -Werror -DFLAT_DEBUG")

add_executable(
    crispy-guacamole
    ${src}
)

target_link_libraries(
    crispy-guacamole
    lua5.2
    SDL2
    SDL2_image
    SDL2_mixer
    SDL2_ttf
    GL
    GLEW
)

