cmake_minimum_required(VERSION 3.12)
project(sfmlZelda)

set(CMAKE_CXX_STANDARD 23)

find_package(SFML 2.5 COMPONENTS graphics audio window system REQUIRED)

add_executable(sfmlZelda
    src/level.cpp
    src/main.cpp
    src/player.cpp
    src/settings.cpp
    src/support.cpp
    src/enemy.cpp
    src/upgrade.cpp
    src/entity.cpp
    src/pygame_adapter.cpp
    src/magic.cpp
    src/particles.cpp
    src/sprite_manager.cpp
    src/ui.cpp
    src/tile.cpp)

target_link_libraries(sfmlZelda
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    stdc++fs)

