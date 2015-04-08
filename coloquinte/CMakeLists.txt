# -*- explicit-buffer-name: "CMakeLists.txt<Coloquinte/src>" -*-

 project(Coloquinte)

 set(CMAKE_C_FLAGS_DEBUG     " -Wall -O2 -g"       CACHE STRING "C Compiler Debug options."   FORCE)
 set(CMAKE_C_FLAGS_RELEASE   " -Wall -O3 -fopenmp" CACHE STRING "C Compiler Release options." FORCE)
 set(CMAKE_CXX_FLAGS_DEBUG   " -Wall -Og -g"   CACHE STRING "C++ Compiler Debug options."   FORCE)
 set(CMAKE_CXX_FLAGS_RELEASE " -Wall -O3 -fopenmp" CACHE STRING "C++ Compiler Release options." FORCE)

 cmake_minimum_required(VERSION 2.4.0)

 add_definitions(-std=c++11)

 add_subdirectory(cmake_modules)
 add_subdirectory(src)

