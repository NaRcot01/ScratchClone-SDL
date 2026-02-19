cmake_minimum_required(VERSION 3.23)
project(FoP_Project_Clion)

set(CMAKE_CXX_STANDARD 23)

add_executable(FoP_Project_Clion main.cpp core/sdl_init.cpp core/sdl_init.h core/engine.cpp core/engine.h ui/stage.cpp ui/stage.h core/config.h ui/sprite.cpp ui/sprite.h ui/sprite_panel.cpp ui/sprite_panel.h core/config.cpp ui/topbar_button.h ui/topbar.h ui/topbar.cpp ui/topbar_button.cpp ui/font.cpp ui/font.h ui/property_panel.cpp ui/property_panel.h tools/tools.cpp tools/tools.h tools/tinyfiledialogs.c tools/tinyfiledialogs.h ui/library_panel.cpp ui/library_panel.h ui/block.cpp ui/block.h ui/script_area.cpp ui/script_area.h ui/block_palette.h ui/block_palette.cpp ui/control_panel.cpp ui/control_panel.h core/block_executer.h core/block_executer.cpp ui/background_panel.h ui/background_panel.cpp ui/backdrop.cpp ui/backdrop.h)
target_link_libraries(${PROJECT_NAME} -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer)