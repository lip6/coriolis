# -*- mode: CMAKE explicit-buffer-name: "CMakeLists.txt<crlcore>" -*-

 set(CMAKE_LEGACY_CYGWIN_WIN32 0)
 project(CRLCORE)

 cmake_minimum_required(VERSION 2.8.9)

 OPTION(BUILD_DOC "Build the documentation (latex+doxygen)" OFF)

 list(INSERT CMAKE_MODULE_PATH 0 "${DESTDIR}$ENV{CORIOLIS_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
 setup_project_paths(CORIOLIS)
 list(INSERT CMAKE_MODULE_PATH 0 "${CRLCORE_SOURCE_DIR}/cmake_modules/")
 print_cmake_module_path()

 set_cmake_policies()
 check_distribution()
 set_lib_link_mode()
 setup_sysconfdir("${CMAKE_INSTALL_PREFIX}")
 setup_boost(program_options python regex wave)
 setup_qt()

 find_package(LibXml2            REQUIRED)
 find_package(PythonLibs         REQUIRED)
 find_package(PythonSitePackages REQUIRED)
 find_package(BISON              REQUIRED)
 find_package(FLEX               REQUIRED)
 find_package(LEFDEF)
 find_package(OPENACCESS)
 find_package(VLSISAPD           REQUIRED)
 find_package(HURRICANE          REQUIRED)
 find_package(Libexecinfo        REQUIRED)
 
 add_subdirectory(src)
 add_subdirectory(etc)
 add_subdirectory(cmake_modules)

 if(BUILD_DOC)
#  include(UseLATEX)
   find_package(Doxygen)
   add_subdirectory(doc)
 endif()
