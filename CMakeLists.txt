# primary source: https://cmake.org/cmake-tutorial/
cmake_minimum_required(VERSION 3.6)
project(gdtw)

# inform compiler libs
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++11 -stdlib=libc++ -Wall -g")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11 -stdlib=libc++ -O2")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# configure header file to include CMAKE values
option(SINGLE_PRECISION "Represent data using single-precision floating-point" OFF)
configure_file (
  "${PROJECT_SOURCE_DIR}/config.hpp.in"
  "${PROJECT_BINARY_DIR}/config.hpp"
)

# find matlab
set(Matlab_ROOT_DIR "/Applications/MATLAB_R2017a.app")
find_package( Matlab REQUIRED COMPONENTS MX_LIBRARY )
include_directories("${Matlab_INCLUDE_DIRS}")

# include boost
find_package(Boost 1.6 REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

# include GNU readline
find_package(Readline REQUIRED)
include_directories(${Readline_INCLUDE_DIRS})

# allow src files to see generated config.hpp
include_directories("${PROJECT_BINARY_DIR}")

# add distance library
include_directories("${PROJECT_SOURCE_DIR}/lib")
add_subdirectory(lib)

# add online gdtw
include_directories("${PROJECT_SOURCE_DIR}/online_gdtw")
add_subdirectory(online_gdtw)

# add test
enable_testing()
add_subdirectory(test)