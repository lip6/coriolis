# -*- mode: CMAKE explicit-buffer-name: "CMakeLists.txt<hurricane>" -*-

 set(CMAKE_LEGACY_CYGWIN_WIN32 0)
 project(HURRICANE)

 option(BUILD_DOC "Build the documentation (doxygen)" OFF)

 cmake_minimum_required(VERSION 2.8.9)

 list(INSERT CMAKE_MODULE_PATH 0 "${DESTDIR}$ENV{CORIOLIS_TOP}/share/cmake/Modules")
 find_package(Bootstrap REQUIRED)
 setup_project_paths(CORIOLIS)
 list(INSERT CMAKE_MODULE_PATH 0 "${HURRICANE_SOURCE_DIR}/cmake_modules/")

 set_cmake_policies()
 set_lib_link_mode()
 setup_boost(regex)
 setup_qt()

 find_package(LibXml2            REQUIRED)
 find_package(BISON              REQUIRED)
 find_package(FLEX               REQUIRED)
 find_package(PythonLibs         REQUIRED)
 find_package(PythonSitePackages REQUIRED)
 find_package(VLSISAPD           REQUIRED)
 find_package(Libexecinfo        REQUIRED)
 
 add_subdirectory(src)
 add_subdirectory(cmake_modules)
#add_subdirectory(tests)

 if(BUILD_DOC)
   find_package(Doxygen)
   if(DOXYGEN_FOUND)
     add_subdirectory(doc)
   endif()
 endif()
 
#enable_testing()
#add_test(HurricaneTest ${PROJECT_BINARY_DIR}/tests/htest)
