cmake_minimum_required(VERSION 3.27)
project(bc)


set( GCC_COVERAGE_COMPILE_FLAGS "-Wall" )
add_subdirectory(calc)


add_executable(bc main.c)
target_include_directories(bc PRIVATE include)
target_link_libraries(bc calc)
