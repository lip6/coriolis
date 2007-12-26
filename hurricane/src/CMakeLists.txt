project(HURRICANE)

cmake_minimum_required(VERSION 2.4.0)

set(CMAKE_MODULE_PATH "cmake_modules/")

find_package(Qt4 REQUIRED) # find and setup Qt4 for this project
FIND_PACKAGE(BISON REQUIRED)
FIND_PACKAGE(FLEX REQUIRED)

add_subdirectory(hurricane)
add_subdirectory(analogic)
add_subdirectory(figures)
