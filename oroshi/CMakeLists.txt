# -*- explicit-buffer-name: "CMakeLists.txt<oroshi>" -*-

 project(OROSHI)
 
 cmake_minimum_required(VERSION 2.4.0)
 option(USE_LIBBFD "Link with BFD libraries to print stack traces" OFF)

 set(ignoreVariables "${CMAKE_INSTALL_DIR}")
 
 list(INSERT CMAKE_MODULE_PATH 0 "$ENV{CORIOLIS_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
 setup_project_paths(CORIOLIS)
 
 set_cmake_policies()
 setup_sysconfdir("${CMAKE_INSTALL_PREFIX}")
 setup_boost(program_options filesystem python)
 
 if (USE_LIBBFD)
   find_package(Libbfd)
 endif()
 find_package(PythonLibs         2 REQUIRED)
 find_package(PythonSitePackages   REQUIRED)
 find_package(HURRICANE            REQUIRED)
 find_package(VLSISAPD             REQUIRED)
 find_package(CORIOLIS             REQUIRED)
 find_package(Doxygen)
 setup_qt()
 
#add_subdirectory(src)
 add_subdirectory(python)
#add_subdirectory(etc)
#add_subdirectory(cmake_modules)
 add_subdirectory(doc)
