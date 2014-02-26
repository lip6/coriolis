# -*- mode: CMAKE explicit-buffer-name: "CMakeLists.txt<hurricane>" -*-

 project(HURRICANE)

 option(BUILD_DOC "Build the documentation (doxygen)" OFF)

 cmake_minimum_required(VERSION 2.4.0)

 list(INSERT CMAKE_MODULE_PATH 0 "${DESTDIR}/$ENV{CORIOLIS_TOP}/share/cmake/Modules")
 find_package(Bootstrap REQUIRED)
 setup_project_paths(CORIOLIS)
 list(INSERT CMAKE_MODULE_PATH 0 "${HURRICANE_SOURCE_DIR}/cmake_modules/")

 set_cmake_policies()
 set_lib_link_mode()
 setup_boost(regex)

 set(QT_USE_QTXML "true")
 find_package(LibXml2 REQUIRED)
 find_package(Qt4 REQUIRED)
 find_package(BISON REQUIRED)
 find_package(FLEX REQUIRED)
 find_package(PythonLibs REQUIRED)
 find_package(PythonSitePackages REQUIRED)
 find_package(VLSISAPD REQUIRED)
 find_package(Libexecinfo REQUIRED)
 if(BUILD_DOC)
   find_package(Doxygen)
 endif(BUILD_DOC)
 
 add_subdirectory(src)
 add_subdirectory(cmake_modules)
#add_subdirectory(tests)
 if(BUILD_DOC AND DOXYGEN_FOUND)
   add_subdirectory(doc)
 endif(BUILD_DOC AND DOXYGEN_FOUND)
 
#enable_testing()
#add_test(HurricaneTest ${PROJECT_BINARY_DIR}/tests/htest)
