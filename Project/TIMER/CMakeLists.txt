cmake_minimum_required(VERSION 3.17)
project("focus_timer" VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)

set_target_properties(focus_timer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")