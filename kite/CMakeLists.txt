
 project(KITE)

 option(BUILD_DOC      "Build the documentation (doxygen)" OFF)
 option(CHECK_DATABASE "Run database in full check mode (very slow)" OFF)
 
 cmake_minimum_required(VERSION 2.4.0)

 list(INSERT CMAKE_MODULE_PATH 0 "${DESTDIR}/$ENV{BOOTSTRAP_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
 setup_project_paths(VLSISAPD)
 setup_project_paths(CORIOLIS)
 
 set_cmake_policies()
 set_lib_link_mode()
 setup_boost(program_options filesystem python regex)
 
 find_package(LibXml2 REQUIRED)
 set(QT_USE_QTXML "true")
 find_package(Qt4 REQUIRED)
 find_package(PythonLibs REQUIRED)
 find_package(PythonSitePackages REQUIRED)
 find_package(VLSISAPD REQUIRED)
 find_package(LEFDEF REQUIRED)
 find_package(HURRICANE REQUIRED)
 find_package(CORIOLIS REQUIRED)
 find_package(KNIK REQUIRED)
 find_package(KATABATIC REQUIRED)
 if(BUILD_DOC)
   find_package(Doxygen)
 endif(BUILD_DOC)
 
 if(CHECK_DATABASE)
   add_definitions(-DCHECK_DATABASE)
 endif(CHECK_DATABASE)
 if(CHECK_DETERMINISM)
   add_definitions(-DCHECK_DETERMINISM)
 endif(CHECK_DETERMINISM)
 
 add_subdirectory(src)
 add_subdirectory(cmake_modules)
 if(BUILD_DOC AND DOXYGEN_FOUND)
   add_subdirectory(doc)
 endif(BUILD_DOC AND DOXYGEN_FOUND)
