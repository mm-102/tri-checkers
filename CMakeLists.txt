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
set(allegro_libs ${allegro} ${allegro_image} ${allegro_primitives})
cmake_path(SET allegro_include NORMALIZE ${allegro}/../../include/)

elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")

find_library(allegro NAMES liballegro.so REQUIRED)
find_library(allegro_image NAMES liballegro_image.so REQUIRED)
find_library(allegro_primitives NAMES liballegro_primitives.so REQUIRED)
set(allegro_libs ${allegro} ${allegro_image} ${allegro_primitives})
cmake_path(SET allegro_include NORMALIZE ${allegro}/../../include/)

else()

find_package(allegro CONFIG REQUIRED)
set(allegro_include "${PACKAGE_PREFIX_DIR}/include")
set(allegro_lib_dir "${PACKAGE_PREFIX_DIR}/lib")
file(GLOB allegro_libs 
    ${allegro_lib_dir}/liballegro.*
    ${allegro_lib_dir}/liballegro_image.*
    ${allegro_lib_dir}/liballegro_primitives.*
)

endif()

target_include_directories(tri-checkers PUBLIC ${allegro_include})
target_link_libraries(tri-checkers PUBLIC ${allegro_libs})



# message(${allegro} ${allegro_image} ${allegro_primitives} ${allegro_include})

file(COPY assets/shaders DESTINATION release/assets)
file(COPY assets/textures DESTINATION release/assets)
