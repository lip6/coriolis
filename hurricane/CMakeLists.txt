project(HURRICANE)

cmake_minimum_required(VERSION 2.4.0)

set(CMAKE_MODULE_PATH "${HURRICANE_SOURCE_DIR}/cmake_modules/")

find_package(Qt4 REQUIRED) # find and setup Qt4 for this project
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
find_package(Doxygen)

add_subdirectory(src)
add_subdirectory(cmake_modules)
if(DOXYGEN_FOUND)
  add_subdirectory(doc)
endif(DOXYGEN_FOUND)
