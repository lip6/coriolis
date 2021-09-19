# -*- mode: CMAKE explicit-buffer-name: "CMakeLists.txt<vlsisapd>" -*-

 set(CMAKE_LEGACY_CYGWIN_WIN32 0)
 project(VLSISAPD)

 cmake_minimum_required(VERSION 2.8.9)

 set(ignoreVariables "${CMAKE_INSTALL_DIR}")

 list(INSERT CMAKE_MODULE_PATH 0 "${DESTDIR}$ENV{CORIOLIS_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
 list(INSERT CMAKE_MODULE_PATH 0 "${VLSISAPD_SOURCE_DIR}/cmake_modules/")
 print_cmake_module_path()

 message(STATUS "Boost_NO_SYSTEM_PATHS: ${Boost_NO_SYSTEM_PATHS}")
 set_cmake_policies()
 setup_boost(program_options python)

 find_package(LibXml2 REQUIRED)
 find_package(Python 3 REQUIRED COMPONENTS Interpreter Development)
 find_package(PythonSitePackages REQUIRED)
 find_package(BISON REQUIRED)
 find_package(FLEX REQUIRED)
 find_package(Doxygen)

 add_subdirectory(src)
 add_subdirectory(cmake_modules)
 add_subdirectory(doc)
 add_subdirectory(examples)
