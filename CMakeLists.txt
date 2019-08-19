cmake_minimum_required(VERSION 3.0)
project(SpaceBoopers C CXX)

set(CMAKE_BUILD_TYPE Debug)

# Include SDL2 and other external libraries
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})
link_directories(${SDL2_LIBRARY_DIRS})

find_library(MATH_LIB m)
if(NOT MATH_LIB)
  message(FATAL_ERROR "math library not found")
endif()

add_executable(
	SpaceBoopers WIN32
	src/game.cpp
	src/bullet.cpp
	src/player.cpp
	src/shared.cpp
)

add_definitions(${SDL2_CFLAGS_OTHER})
target_link_libraries(SpaceBoopers ${SDL2_LIBRARIES} m)

