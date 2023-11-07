cmake_minimum_required(VERSION 3.26)
project(space_invaders)

set(CMAKE_CXX_STANDARD 17)

add_executable(space_invaders src/main.cpp src/Dependencies/cpp/ErrorHandling.cpp src/Dependencies/cpp/Rendering.cpp src/Dependencies/cpp/Game.cpp)

target_link_libraries(space_invaders -lfreeglut -lglew32 -lopengl32 -lglfw3)
