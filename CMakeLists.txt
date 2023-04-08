
cmake_minimum_required(VERSION 3.20)
project(fptf CXX C)
enable_testing()
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_subdirectory(src)
add_subdirectory(tests)