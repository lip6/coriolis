project(HURRICANE)

cmake_minimum_required(VERSION 2.4.0)

set(CMAKE_MODULE_PATH "${HURRICANE_SOURCE_DIR}/cmake_modules/")

find_package(Qt4 REQUIRED) # find and setup Qt4 for this project
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

add_subdirectory(hurricane)
add_subdirectory(analogic)
add_subdirectory(viewer)
add_subdirectory(editor)

set(DEST_DIR "$ENV{DESTDIR}")
configure_file(${HURRICANE_SOURCE_DIR}/hurricane.pc.cmake hurricane.pc @ONLY)
install(FILES ${HURRICANE_BINARY_DIR}/hurricane.pc DESTINATION /share/pkgconfig) 
