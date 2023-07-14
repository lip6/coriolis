# -*- mode: CMAKE explicit-buffer-name: "CMakeLists.txt<lefdef>" -*-

 set(CMAKE_LEGACY_CYGWIN_WIN32 0)
 project(LEFDEF)

 cmake_minimum_required(VERSION 3.16)
 set(ignoreVariables "${BUILD_DOC} ${CMAKE_INSTALL_DIR}")
 option(USE_LIBBFD     "Link with BFD libraries to print stack traces" OFF)

 list(INSERT CMAKE_MODULE_PATH 0 "${DESTDIR}$ENV{CORIOLIS_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
 setup_project_paths(CORIOLIS)
 list(INSERT CMAKE_MODULE_PATH 0 "${CRLCORE_SOURCE_DIR}/cmake_modules/")
 print_cmake_module_path()

 set_cmake_policies()
 setup_sysconfdir("${CMAKE_INSTALL_PREFIX}")

 find_package(BISON              REQUIRED)
 find_package(FLEX               REQUIRED)
 
 add_subdirectory(src)
 add_subdirectory(doc)
