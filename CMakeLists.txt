cmake_minimum_required(VERSION 3.10.0)
project(particle_systems VERSION 0.1.0 LANGUAGES C)

set(PARTICLE_LIB_SRC src/MonteCarloOverlapRemover.c
                     src/particle.c
                     src/vectorAlgebra.c)

add_executable(particle_systems ${PARTICLE_LIB_SRC}
                                # examples/brownian_motion.c
                                examples/maxwell_boltzmann.c)

add_custom_command(TARGET particle_systems PRE_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy 
   ${CMAKE_SOURCE_DIR}/assets/weary-face-emoji-clipart-md.png 
   $<TARGET_FILE_DIR:particle_systems>
)                                

target_include_directories(particle_systems PRIVATE include)
target_link_libraries     (particle_systems PRIVATE raylib)
 