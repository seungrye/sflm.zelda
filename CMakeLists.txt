cmake_minimum_required(VERSION 3.12)
project(sfmlZelda)

set(CMAKE_CXX_STANDARD 14)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(sfmlZelda 
    src/level.cpp 
    src/main.cpp
    src/player.cpp
    src/settings.cpp
    src/support.cpp
    src/enemy.cpp
    src/tile.cpp)

target_link_libraries(sfmlZelda
    sfml-graphics sfml-window sfml-system
    stdc++fs)

