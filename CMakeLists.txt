cmake_minimum_required(VERSION 3.10)
project(tri-checkers)

file(GLOB sources src/*.cpp)

add_executable(tri-checkers ${sources})

include_directories(include)
include_directories(assets/textures)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    message("built on windows")
    target_include_directories(tri-checkers PUBLIC C:/MinGW/include)
    find_library(allegro NAMES liballegro.dll.a)
    find_library(allegro_image NAMES liballegro_image.dll.a)
    find_library(allegro_main NAMES liballegro_main.dll.a)
    find_library(allegro_memfile NAMES liballegro_memfile.dll.a)

elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    message("built on Linux")
    find_library(allegro NAMES liballegro.so)
    find_library(allegro_image NAMES liballegro_image.so)
    find_library(allegro_main NAMES liballegro_main.so)
    find_library(allegro_memfile NAMES liballegro_memfile.so)

else()
    message(WARNING "built on unsupported platform!")
    find_library(allegro NAMES liballegro)
    find_library(allegro_image NAMES liballegro_image)
    find_library(allegro_main NAMES liballegro_main)
    find_library(allegro_memfile NAMES liballegro_memfile)
endif()

set(libraries ${allegro} ${allegro_main} ${allegro_memfile} ${allegro_image})

target_link_libraries(tri-checkers ${libraries})
