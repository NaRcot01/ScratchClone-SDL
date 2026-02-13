cmake_minimum_required(VERSION 3.23)
project(ScratchClone_Unified)
set(CMAKE_CXX_STANDARD 17)


find_package(SDL2 REQUIRED HINTS "C:/SDL2/SDL2-2.30.0") 
find_package(SDL2_ttf REQUIRED HINTS "C:/SDL2/SDL2_ttf-2.22.0")


include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

add_executable(ScratchClone_Unified
    main.cpp BlockExecutor.cpp ui/BLOCK.h ui/sprite.cpp ui/sprite.h
    ui/stage.cpp ui/stage.h ui/sprite_panel.cpp ui/sprite_panel.h
    ui/topbar.cpp ui/topbar.h ui/topbar_button.cpp ui/topbar_button.h
    ui/property_panel.cpp ui/property_panel.h ui/font.cpp ui/font.h
    core/engine.cpp core/engine.h core/sdl_init.cpp core/sdl_init.h
    core/config.cpp core/config.h core/physics.cpp core/physics.h
    tools/tools.cpp tools/tools.h
)

target_link_libraries(ScratchClone_Unified
    ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES}
)
