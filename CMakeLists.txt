cmake_minimum_required(VERSION 3.20)
project(tri-checker)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/release)

file(GLOB sources src/*.cpp)
include_directories(include)
add_executable(tri-checkers ${sources})

message(${CMAKE_SYSTEM_NAME})
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")

find_library(allegro NAMES liballegro.dll liballegro.dll.a REQUIRED)
find_library(allegro_image NAMES liballegro_image.dll liballegro_image.dll.a REQUIRED)
find_library(allegro_primitives NAMES liballegro_primitives.dll liballegro_primitives.dll.a REQUIRED)
find_library(allegro_font NAMES liballegro_font.dll liballegro_font.dll.a REQUIRED)
find_library(allegro_ttf NAMES liballegro_ttf.dll liballegro_ttf.dll.a REQUIRED)
set(allegro_libs ${allegro} ${allegro_image} ${allegro_primitives} ${allegro_font} ${allegro_ttf})
cmake_path(SET allegro_include NORMALIZE ${allegro}/../../include/)

elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")

find_library(allegro NAMES liballegro.so REQUIRED)
find_library(allegro_image NAMES liballegro_image.so REQUIRED)
find_library(allegro_primitives NAMES liballegro_primitives.so REQUIRED)
find_library(allegro_font NAMES liballegro_font.so REQUIRED)
find_library(allegro_ttf NAMES liballegro_ttf.so REQUIRED)
set(allegro_libs ${allegro} ${allegro_image} ${allegro_primitives} ${allegro_font} ${allegro_ttf})
cmake_path(SET allegro_include NORMALIZE ${allegro}/../../include/)

else()

find_package(allegro CONFIG REQUIRED)
set(allegro_include "${PACKAGE_PREFIX_DIR}/include")
set(allegro_lib_dir "${PACKAGE_PREFIX_DIR}/lib")
file(GLOB allegro_libs 
    ${allegro_lib_dir}/liballegro.*
    ${allegro_lib_dir}/liballegro_image.*
    ${allegro_lib_dir}/liballegro_primitives.*
    ${allegro_lib_dir}/liballegro_ttf.*
)

endif()

target_include_directories(tri-checkers PUBLIC ${allegro_include})
target_link_libraries(tri-checkers PUBLIC ${allegro_libs})

file(COPY assets/shaders DESTINATION release/assets)
file(COPY assets/textures DESTINATION release/assets)
file(COPY assets/font DESTINATION release/assets)
