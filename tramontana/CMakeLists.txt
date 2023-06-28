# -*- explicit-buffer-name: "CMakeLists.txt<tramontana>" -*-

 set(CMAKE_LEGACY_CYGWIN_WIN32 0)
 project(TRAMONTANA)

 set(ignoreVariables "${BUILD_DOC} ${CMAKE_INSTALL_DIR}")

 option(BUILD_DOC      "Build the documentation (doxygen)" OFF)
 option(USE_LIBBFD     "Link with BFD libraries to print stack traces" OFF)
 
 cmake_minimum_required(VERSION 3.16)

 list(INSERT CMAKE_MODULE_PATH 0 "${DESTDIR}$ENV{CORIOLIS_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
 setup_project_paths(CORIOLIS)
 
 set_cmake_policies()
 setup_boost()
 setup_qt()
 
 find_package(Python 3 REQUIRED COMPONENTS Interpreter Development)
 find_package(PythonSitePackages REQUIRED)
 find_package(HURRICANE REQUIRED)
 find_package(CORIOLIS REQUIRED)
 find_package(Doxygen)
 
 add_subdirectory(src)
#add_subdirectory(cmake_modules)
#add_subdirectory(doc)
