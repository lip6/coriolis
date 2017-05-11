# -*- mode: CMAKE explicit-buffer-name: "CMakeLists.txt<mauka>" -*-

 set(CMAKE_LEGACY_CYGWIN_WIN32 0)
 project(MAUKA)

 cmake_minimum_required(VERSION 2.8.9)

 list(INSERT CMAKE_MODULE_PATH 0 "${DESTDIR}$ENV{CORIOLIS_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
 setup_project_paths(CORIOLIS)
 print_cmake_module_path()

 set_cmake_policies()
 set_lib_link_mode()
 setup_boost(program_options filesystem regex python)
 setup_qt()

 find_package(PythonLibs         2.7 REQUIRED)
 find_package(PythonSitePackages REQUIRED)
 find_package(VLSISAPD           REQUIRED)
 find_package(HURRICANE          REQUIRED)
 find_package(CORIOLIS           REQUIRED)
 find_package(NIMBUS             REQUIRED)
 find_package(METIS              REQUIRED)
 
 add_subdirectory(src)
 add_subdirectory(cmake_modules)
