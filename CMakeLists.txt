cmake_minimum_required(VERSION 2.8.12)

add_subdirectory(cgraph)

add_executable(main an.nv_194466.c)

target_link_libraries(main cgraph)
