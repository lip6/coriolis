# -*- mode: CMAKE explicit-buffer-name: "CMakeLists.txt<stratus1>" -*-

 set(CMAKE_LEGACY_CYGWIN_WIN32 0)
 project(STRATUS1)
 
 # option(BUILD_DOC "Build the documentation (latex2html)" OFF)

 cmake_minimum_required(VERSION 2.4.0)
 list(INSERT CMAKE_MODULE_PATH 0 "${DESTDIR}$ENV{CORIOLIS_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
 setup_project_paths(CORIOLIS)
 
 find_package(Bootstrap REQUIRED)
 set_cmake_policies()
# The flaw is in UseLATEX.cmake.
 cmake_policy(SET CMP0002 OLD)
 setup_sysconfdir("${CMAKE_INSTALL_PREFIX}")
 
 find_package(PythonLibs         2 REQUIRED)
 find_package(PythonSitePackages REQUIRED)
 find_package(VLSISAPD REQUIRED)
 find_package(HURRICANE REQUIRED)
 find_package(CORIOLIS REQUIRED)
 
 add_subdirectory(src)
 add_subdirectory(etc)
