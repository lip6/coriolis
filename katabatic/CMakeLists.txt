
 project(KATABATIC)

 option(BUILD_DOC      "Build the documentation (doxygen)" OFF)
 option(CHECK_DATABASE "Run database in full check mode (very slow)" OFF)
 
 cmake_minimum_required(VERSION 2.4.0)

 list(INSERT CMAKE_MODULE_PATH 0 "$ENV{BOOTSTRAP_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
 setup_project_paths(VLSISAPD)
 setup_project_paths(CORIOLIS)
 
 set_cmake_policies()
 set_lib_link_mode()
 setup_boost(program_options filesystem python regex)
 
 set(QT_USE_QTXML "true")
 find_package(Qt4 REQUIRED)
 find_package(VLSISAPD REQUIRED)
 find_package(HURRICANE REQUIRED)
 find_package(CORIOLIS REQUIRED)
 find_package(KNIK REQUIRED)
 if(BUILD_DOC)
   find_package(Doxygen)
 ENDIF(BUILD_DOC)
 
 add_subdirectory(src)
 add_subdirectory(cmake_modules)
 if(BUILD_DOC AND DOXYGEN_FOUND)
   add_subdirectory(doc)
 endif(BUILD_DOC AND DOXYGEN_FOUND)
 
 if(CHECK_DATABASE)
   add_definitions(-DCHECK_DATABASE)
   message(STATUS "Checking database enabled (very slow).")
 endif(CHECK_DATABASE)
 if(CHECK_DETERMINISM)
   add_definitions(-DCHECK_DETERMINISM)
   message(STATUS "Checking determinism enabled.")
 endif(CHECK_DETERMINISM)
