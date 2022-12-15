# -*- explicit-buffer-name: "CMakeLists.txt<etesian>" -*-

 set(CMAKE_LEGACY_CYGWIN_WIN32 0)
 project(ETESIAN)

 set(ignoreVariables "${CMAKE_INSTALL_DIR}" "${BUILD_DOC}")

#option(BUILD_DOC  "Build the documentation (doxygen)" OFF)
 option(USE_LIBBFD "Link with BFD libraries to print stack traces" OFF)
 
 cmake_minimum_required(VERSION 3.18)

 list(INSERT CMAKE_MODULE_PATH 0 "${DESTDIR}$ENV{CORIOLIS_TOP}/share/cmake/Modules/")
 find_package(Bootstrap  REQUIRED)
 setup_project_paths(CORIOLIS)
 setup_qt()
 
 set_cmake_policies()
 setup_boost(program_options)
 
 find_package(Python 3           REQUIRED COMPONENTS Interpreter Development.Module)
 find_package(PythonSitePackages REQUIRED)
 find_package(HURRICANE          REQUIRED)
#find_package(KATABATIC          REQUIRED)
 find_package(CORIOLIS           REQUIRED)
#find_package(KITE               REQUIRED)
 find_package(COLOQUINTE         REQUIRED)
 find_package(Libexecinfo        REQUIRED)
 find_package(Doxygen)
 
 add_subdirectory(src)
 add_subdirectory(cmake_modules)
#add_subdirectory(doc)
