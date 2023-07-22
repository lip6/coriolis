# -*- explicit-buffer-name: "CMakeLists.txt<foehn>" -*-

 set(CMAKE_LEGACY_CYGWIN_WIN32 0)
 project(FOEHN)

 option(CHECK_DATABASE "Run database in full check mode (very slow)" OFF)
 option(USE_LIBBFD     "Link with BFD libraries to print stack traces" OFF)
 
 cmake_minimum_required(VERSION 3.16)

 set(ignoreVariables "${BUILD_DOC} ${CMAKE_INSTALL_DIR}")

 list(INSERT CMAKE_MODULE_PATH 0 "${DESTDIR}$ENV{CORIOLIS_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
 setup_project_paths(CORIOLIS)
 
 set_cmake_policies()
 setup_boost(program_options)
 setup_qt()
 setup_python()
 
 find_package(Libexecinfo        REQUIRED)
 find_package(PythonSitePackages REQUIRED)
 find_package(LEFDEF             REQUIRED)
 find_package(FLUTE              REQUIRED)
 find_package(HURRICANE          REQUIRED)
 find_package(CORIOLIS           REQUIRED)
 
 add_subdirectory(src)
 add_subdirectory(cmake_modules)
