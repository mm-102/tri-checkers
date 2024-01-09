cmake_minimum_required(VERSION 3.20)
project(tri-checker)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/release)

file(GLOB sources src/*.cpp)
include_directories(include)
add_executable(tri-checkers ${sources})

find_package(allegro CONFIG REQUIRED)
set(allegro_include "${PACKAGE_PREFIX_DIR}/include")
set(allegro_lib_dir "${PACKAGE_PREFIX_DIR}/lib")
file(GLOB allegro_libs 
    ${allegro_lib_dir}/liballegro.*
    ${allegro_lib_dir}/liballegro_image.*
    ${allegro_lib_dir}/liballegro_primitives.*
)
target_include_directories(tri-checkers PUBLIC ${allegro_include})
target_link_libraries(tri-checkers PUBLIC ${allegro_libs})

file(COPY assets/shaders DESTINATION release/assets)
file(COPY assets/textures DESTINATION release/assets)
