
 project(METIS)

 cmake_minimum_required(VERSION 2.4.0)

 list(INSERT CMAKE_MODULE_PATH 0 "$ENV{BOOTSTRAP_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
 setup_project_paths(VLSISAPD)
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
 find_package(NIMBUS REQUIRED)
 
 set(HMETIS_INCLUDE_PATH METIS_INCLUDE_PATH)
 find_library(HMETIS_LIBRARY_PATH
   NAMES hmetis
   PATHS ${CORIOLIS_DIR_SEARCH} /opt/coriolis
   PATH_SUFFIXES lib${LIB_SUFFIX}
   # Help the user find it if we cannot.
   DOC "The hmetis static library"
 )
 set_libraries_path(HMETIS HMETIS)
 
 add_subdirectory(src)
 add_subdirectory(cmake_modules)
