# -*- mode: CMAKE explicit-buffer-name: "CMakeLists.txt<crlcore>" -*-

 set(CMAKE_LEGACY_CYGWIN_WIN32 0)
 project(CRLCORE)

 set(ignoreVariables "${CMAKE_INSTALL_DIR}")

 cmake_minimum_required(VERSION 3.16)

 OPTION(BUILD_DOC  "Build the documentation (latex+doxygen)"       OFF)
 option(USE_LIBBFD "Link with BFD libraries to print stack traces" OFF)

 list(INSERT CMAKE_MODULE_PATH 0 "${DESTDIR}$ENV{CORIOLIS_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
 setup_project_paths(CORIOLIS)
 list(INSERT CMAKE_MODULE_PATH 0 "${CRLCORE_SOURCE_DIR}/cmake_modules/")
 print_cmake_module_path()

 set_cmake_policies()
 check_distribution()
 setup_sysconfdir("${CMAKE_INSTALL_PREFIX}")
 setup_boost(program_options)
 setup_qt()
 setup_python()

 cmake_policy(SET CMP0054 NEW)

 if (USE_LIBBFD)
   find_package(Libbfd)
 endif()
 find_package(BZip2              REQUIRED)
 find_package(PythonSitePackages REQUIRED)
 find_package(BISON              REQUIRED)
 find_package(FLEX               REQUIRED)
 find_package(LEFDEF)
 find_package(OPENACCESS)
 find_package(HURRICANE          REQUIRED)
 find_package(Libexecinfo        REQUIRED)
#include(UseLATEX)
 find_package(Doxygen)
 
 add_subdirectory(src)
 add_subdirectory(python)
 add_subdirectory(cmake_modules)
 if (BUILD_DOC)
   add_subdirectory(doc)
 endif()
