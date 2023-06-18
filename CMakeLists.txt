cmake_minimum_required(VERSION 3.12)
project(Game)

set(CMAKE_CXX_STANDARD 14)

# SFML 경로를 수정해야 할 수도 있습니다.
#set(SFML_DIR "SFML 경로")

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(Game main.cpp)

target_link_libraries(Game sfml-graphics sfml-window sfml-system)

