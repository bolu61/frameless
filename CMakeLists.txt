cmake_minimum_required(VERSION 3.10)

project(frameless)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# target
add_executable(frameless "src/frameless.cxx")
