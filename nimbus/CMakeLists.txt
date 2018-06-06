# -*- mode: CMAKE explicit-buffer-name: "CMakeLists.txt<nimbus>" -*-

 set(CMAKE_LEGACY_CYGWIN_WIN32 0)
 project(NIMBUS)

 cmake_minimum_required(VERSION 2.8.9)

 list(INSERT CMAKE_MODULE_PATH 0 "${DESTDIR}$ENV{CORIOLIS_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
 setup_project_paths(CORIOLIS)
                                  
 set_cmake_policies()
 setup_boost(program_options filesystem python regex)
 setup_qt()
 
 find_package(PythonLibs         2 REQUIRED)
 find_package(PythonSitePackages REQUIRED)
 find_package(VLSISAPD           REQUIRED)
 find_package(HURRICANE          REQUIRED)
 find_package(CORIOLIS           REQUIRED)
 find_package(Doxygen)
 
 ADD_SUBDIRECTORY(src)
 ADD_SUBDIRECTORY(cmake_modules)
