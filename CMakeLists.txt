cmake_minimum_required(VERSION 3.10)

project(macroer VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "src/*.cpp")
add_executable(macroer ${SOURCES})

if(WIN32 OR MSVC)
    find_path(PDC_INCLUDES curses.h)
    find_library(PDC_LIBS pdcurses REQUIRED)
    include_directories(${PDC_INCLUDES})
    target_link_libraries(${PROJECT_NAME} ${PDC_LIBS})
else()
    find_package(Curses REQUIRED)
    include_directories(${CURSES_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
endif()


configure_file(src/config.hpp.in ../src/config.hpp)

target_include_directories(macroer PUBLIC
    "${PROJECT_BINARY_DIR}"
)