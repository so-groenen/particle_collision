cmake_minimum_required(VERSION 3.10.0)
project(maxwell_boltzmann_distribution VERSION 0.1.0 LANGUAGES C)

 
add_executable(maxwell_boltzmann_distribution main.c)
                                # examples/maxwell_boltzmann.c)

add_custom_command(TARGET maxwell_boltzmann_distribution PRE_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy 
   ${CMAKE_CURRENT_SOURCE_DIR}/assets/ninepatch_button.png 
   $<TARGET_FILE_DIR:maxwell_boltzmann_distribution>
)                                
target_compile_features(maxwell_boltzmann_distribution PRIVATE c_std_99)
target_link_libraries  (maxwell_boltzmann_distribution PRIVATE ray::particle_lib)
 