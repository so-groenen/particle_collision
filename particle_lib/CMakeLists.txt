cmake_minimum_required(VERSION 3.10.0)
project(particle_lib VERSION 0.1.0 LANGUAGES C)

set(PARTICLE_LIB_SRC src/monte_carlo_particle_position_setter.c
                     src/particle.c
                     src/vector_algebra.c)

add_library(particle_lib STATIC ${PARTICLE_LIB_SRC})
add_library(ray::particle_lib ALIAS particle_lib)

target_compile_features   (particle_lib PRIVATE c_std_99)

target_include_directories(particle_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries     (particle_lib PUBLIC raylib)

 