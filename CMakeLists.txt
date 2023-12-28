cmake_minimum_required(VERSION 3.20)
project(tri-checkers)

file(GLOB sources src/*.cpp)

add_executable(tri-checkers ${sources})

include_directories(include)
include_directories(assets/textures)
# include_directories(assets/shaders)

find_package(allegro CONFIG REQUIRED)

set(libs ${allegro} ${allegro_main} ${allegro_image} ${allegro_primitives})

cmake_path(SET allegro_include NORMALIZE ${allegro}/../../include/)
target_include_directories(tri-checkers PUBLIC ${allegro_include})
target_link_libraries(tri-checkers ${libs})
