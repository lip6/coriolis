
 project(CRLCORE)

 cmake_minimum_required(VERSION 2.4.0)

 OPTION(BUILD_DOC    "Build the documentation (latex+doxygen)" OFF)

 list(INSERT CMAKE_MODULE_PATH 0 "$ENV{BOOTSTRAP_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
 setup_project_paths(VLSISAPD)
 setup_project_paths(CORIOLIS)
 list(INSERT CMAKE_MODULE_PATH 0 "${CRLCORE_SOURCE_DIR}/cmake_modules/")
 print_cmake_module_path()

 setup_sysconfdir("$ENV{CORIOLIS_TOP}")
 set_cmake_policies()
 set_lib_link_mode()
 setup_boost(program_options filesystem python regex)

 find_package(LibXml2 REQUIRED)
 find_package(PythonLibs REQUIRED)
 find_package(PythonSitePackages REQUIRED)
 set(QT_USE_QTXML "true")
 find_package(Qt4 REQUIRED) # find and setup Qt4 for this project
 find_package(BISON REQUIRED)
 find_package(FLEX REQUIRED)
 find_package(LEFDEF)
 find_package(OPENACCESS)
 find_package(VLSISAPD REQUIRED)
 find_package(HURRICANE REQUIRED)
 if(BUILD_DOC)
   include(UseLATEX)
 endif(BUILD_DOC)
 
 add_subdirectory(src)
 add_subdirectory(etc)
 add_subdirectory(cmake_modules)

 if(BUILD_DOC)
  FIND_PACKAGE(Doxygen)
 endif(BUILD_DOC)

 if(BUILD_DOC)
   add_subdirectory(doc)
 endif(BUILD_DOC)
