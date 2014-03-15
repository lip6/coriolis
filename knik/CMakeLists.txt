# -*- mode: CMAKE explicit-buffer-name: "CMakeLists.txt<knik>" -*-

 project(KNIK)

 cmake_minimum_required(VERSION 2.4.0)

 list(INSERT CMAKE_MODULE_PATH 0 "${DESTDIR}$ENV{CORIOLIS_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
 setup_project_paths(CORIOLIS)
 print_cmake_module_path()

 set_cmake_policies()
 set_lib_link_mode()
 setup_boost(program_options filesystem python regex)
 
 set(QT_USE_QTXML "true")
 find_package(Qt4 REQUIRED) # find and setup Qt4 for this project
 find_package(VLSISAPD REQUIRED)
 find_package(HURRICANE REQUIRED)
 find_package(CORIOLIS REQUIRED)
 
 add_subdirectory(src)
 add_subdirectory(cmake_modules)
