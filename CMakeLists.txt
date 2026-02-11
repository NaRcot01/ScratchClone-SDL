cmake_minimum_required(VERSION 3.20)
project(ScratchClone_SDL)

set(CMAKE_CXX_STANDARD 17)

add_executable(ScratchClone_SDL
        main.cpp
        BlockExecutor.cpp
        BLOCK.h
        Sprite.h
)