cmake_minimum_required(VERSION 3.18)
project(tri-checkers)

file(GLOB sources src/*.cpp)

add_executable(tri-checkers ${sources})

include_directories(include)
include_directories(assets/textures)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    message("built on windows")
    find_library(allegro NAMES liballegro liballegro.dll.a REQUIRED)
    find_library(allegro_image NAMES liballegro_image.dll.a REQUIRED)
    find_library(allegro_main NAMES liballegro_main.dll.a REQUIRED)
    find_library(allegro_memfile NAMES liballegro_memfile.dll.a REQUIRED)
    
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    message("built on Linux")
    find_library(allegro NAMES liballegro.so REQUIRED)
    find_library(allegro_image NAMES liballegro_image.so REQUIRED)
    find_library(allegro_main NAMES liballegro_main.so REQUIRED)
    find_library(allegro_memfile NAMES liballegro_memfile.so REQUIRED)
    
else()
    message(WARNING "built on unsupported platform!")
    find_library(allegro NAMES liballegro REQUIRED)
    find_library(allegro_image NAMES liballegro_image REQUIRED)
    find_library(allegro_main NAMES liballegro_main REQUIRED)
    find_library(allegro_memfile NAMES liballegro_memfile REQUIRED)
endif()

target_include_directories(tri-checkers PUBLIC ${allegro}/../../include)

set(libraries ${allegro} ${allegro_main} ${allegro_memfile} ${allegro_image})

target_link_libraries(tri-checkers ${libraries})
