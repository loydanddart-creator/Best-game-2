cmake_minimum_required(VERSION 3.20)
project(unicorn C ASM)

add_executable(unicorn main.c)
target_link_libraries(unicorn PRIVATE psxgpu psxetc psxapi psxpad)
