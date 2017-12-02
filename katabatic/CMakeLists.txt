# -*- explicit-buffer-name: "CMakeLists.txt<katabatic>" -*-

 set(CMAKE_LEGACY_CYGWIN_WIN32 0)
 project(KATABATIC)

 option(BUILD_DOC      "Build the documentation (doxygen)" OFF)
 option(CHECK_DATABASE "Run database in full check mode (very slow)" OFF)
 
 cmake_minimum_required(VERSION 2.8.9)

 list(INSERT CMAKE_MODULE_PATH 0 "${DESTDIR}$ENV{CORIOLIS_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
 setup_project_paths(CORIOLIS)
 
 set_cmake_policies()
 setup_boost(program_options filesystem python regex)
 setup_qt()
 
 find_package(PythonLibs         2 REQUIRED)
 find_package(PythonSitePackages REQUIRED)
 find_package(VLSISAPD REQUIRED)
 find_package(HURRICANE REQUIRED)
 find_package(CORIOLIS REQUIRED)
 find_package(KNIK REQUIRED)
 
 add_subdirectory(src)
 add_subdirectory(cmake_modules)

 if(BUILD_DOC)
   find_package(Doxygen)
   if(DOXYGEN_FOUND)
     add_subdirectory(doc)
   endif()
 endif()
 
 if(CHECK_DATABASE)
   add_definitions(-DCHECK_DATABASE)
   message(STATUS "Checking database enabled (very slow).")
 endif(CHECK_DATABASE)
