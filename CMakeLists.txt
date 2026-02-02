cmake_minimum_required(VERSION 3.23)
project(FoP_Project_Clion)

set(CMAKE_CXX_STANDARD 23)

add_executable(FoP_Project_Clion main.cpp core/sdl_init.cpp core/sdl_init.h core/engine.cpp core/engine.h ui/stage.cpp ui/stage.h core/config.cpp core/config.h)
target_link_libraries(${PROJECT_NAME} -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer)