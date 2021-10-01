# -*- mode: CMAKE explicit-buffer-name: "CMakeLists.txt<equinox>" -*-

 set(CMAKE_LEGACY_CYGWIN_WIN32 0)
 project(EQUINOX)
 
 cmake_minimum_required(VERSION 2.8.9)

 set(ignoreVariables "${BUILD_DOC} ${CMAKE_INSTALL_DIR}")
 option(USE_LIBBFD     "Link with BFD libraries to print stack traces" OFF)
 
 list(INSERT CMAKE_MODULE_PATH 0 "${DESTDIR}$ENV{CORIOLIS_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
 setup_project_paths(CORIOLIS)
 
 set_cmake_policies()
 setup_boost(program_options)
 setup_qt()
 
 find_package(HURRICANE REQUIRED)
 find_package(CORIOLIS REQUIRED)
 
 add_subdirectory(src)
 add_subdirectory(cmake_modules)
