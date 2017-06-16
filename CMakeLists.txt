# primary source: https://cmake.org/cmake-tutorial/
cmake_minimum_required(VERSION 3.6)
project(gdtw)

# inform compiler libs
set(CMAKE_CXX_FLAGS "-std=c++11 -O2")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# configure header file to include CMAKE values
option(SINGLE_PRECISION "Represent data using single-precision floating-point" OFF)
configure_file (
  "${PROJECT_SOURCE_DIR}/config.hpp.in"
  "${PROJECT_BINARY_DIR}/config.hpp"
)

# find matlab
set(Matlab_ROOT_DIR "/Applications/MATLAB_R2017a.app")
find_package( Matlab COMPONENTS MX_LIBRARY )

if (Matlab_FOUND)
  message(STATUS "Found Matlab library")
  include_directories("${Matlab_INCLUDE_DIRS}")
endif()

# include boost
find_package(Boost 1.6 REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

if (UNIX) 
  #include GNU readline
  find_package(Readline REQUIRED)
elseif (WIN32) 
  set(Readline_INCLUDE_DIR "C:/wineditline/include")
  set(Readline_LIBRARY "C:/wineditline/lib64/libedit_static.a")
endif()

include_directories(${Readline_INCLUDE_DIR})

message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
message(STATUS "Boost_UNIT_TEST_FRAMEWORK_LIBRARY: ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}")
message(STATUS "Readline_INCLUDE_DIR: ${Readline_INCLUDE_DIR}")
message(STATUS "Readline_LIBRARY: ${Readline_LIBRARY}")
message(STATUS "Matlab_ROOT_DIR: ${Matlab_ROOT_DIR}")

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
