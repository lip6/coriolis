# -*- explicit-buffer-name: "CMakeLists.txt<smurf>" -*-

 project(SMURF)

 option(BUILD_DOC      "Build the documentation (doxygen)" OFF)
 
 cmake_minimum_required(VERSION 3.16)

 list(INSERT CMAKE_MODULE_PATH 0 "${DESTDIR}$ENV{CORIOLIS_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
 setup_project_paths(CORIOLIS)
 
 set_cmake_policies()
 setup_boost(program_options python)
 setup_qt()
 
 find_package(PythonLibs         2 REQUIRED)
 find_package(PythonSitePackages REQUIRED)
 find_package(AXEP REQUIRED)
 find_package(VLSISAPD REQUIRED)
 find_package(LEFDEF REQUIRED)
 find_package(HURRICANE REQUIRED)
 find_package(CORIOLIS REQUIRED)
 find_package(Libexecinfo REQUIRED)
 
 add_subdirectory(src)
 add_subdirectory(cmake_modules)

 if(BUILD_DOC)
   find_package(Doxygen)
#  if(DOXYGEN_FOUND)
#    add_subdirectory(doc)
#  endif()
 endif()
