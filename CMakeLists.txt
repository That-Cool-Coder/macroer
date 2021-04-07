cmake_minimum_required(VERSION 3.10)

project(Macroer VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "src/*.cpp")
add_executable(Macroer ${SOURCES})

configure_file(src/config.hpp.in ../src/config.hpp)

target_include_directories(Macroer PUBLIC
                            "${PROJECT_BINARY_DIR}"
                            )