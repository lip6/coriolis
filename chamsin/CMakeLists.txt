project(CHAMSIN)

cmake_minimum_required(VERSION 2.4.0)

set(CMAKE_MODULE_PATH "$ENV{HURRICANE_TOP}/share/cmake_modules/"
                      "$ENV{HURRICANE_TOP}/share/cmake_modules/")

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
find_package(HURRICANE REQUIRED)
find_package(CORIOLIS REQUIRED)
find_package(Qt4 REQUIRED) # find and setup Qt4 for this project
find_package(LibXml2 REQUIRED)

add_subdirectory(src)
