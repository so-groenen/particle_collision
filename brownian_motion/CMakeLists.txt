cmake_minimum_required(VERSION 3.10.0)
project(brownian_motion VERSION 0.1.0 LANGUAGES C)

 
add_executable(brownian_motion main.c)
 
add_custom_command(TARGET brownian_motion PRE_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy 
   ${CMAKE_CURRENT_SOURCE_DIR}/assets/weary-face-emoji-clipart-md.png 
   $<TARGET_FILE_DIR:brownian_motion>)                                
              
target_compile_features   (brownian_motion PRIVATE c_std_99)
target_link_libraries     (brownian_motion PRIVATE ray::particle_lib)
 