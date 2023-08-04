# -*- mode: CMAKE explicit-buffer-name: "CMakeLists.txt<hurricane>" -*-

 set(CMAKE_LEGACY_CYGWIN_WIN32 0)
 project(HURRICANE)

 set(ignoreVariables "${CMAKE_INSTALL_DIR}")

 option(BUILD_DOC  "Build the documentation (doxygen)" OFF)
 option(USE_LIBBFD "Link with BFD libraries to print stack traces" OFF)

 cmake_minimum_required(VERSION 3.16)

 list(INSERT CMAKE_MODULE_PATH 0 "${DESTDIR}$ENV{CORIOLIS_TOP}/share/cmake/Modules")
 find_package(RapidJSON REQUIRED)
 find_package(Bootstrap REQUIRED)
 setup_project_paths(CORIOLIS)
 list(INSERT CMAKE_MODULE_PATH 0 "${HURRICANE_SOURCE_DIR}/cmake_modules/")

 set_cmake_policies()
 cmake_policy(SET CMP0054 NEW)
 setup_boost(program_options)
 setup_qt()
 setup_python()

 find_package(BZip2              REQUIRED)
 find_package(BISON              REQUIRED)
 find_package(FLEX               REQUIRED)
 find_package(PythonSitePackages REQUIRED)
 find_package(Libexecinfo        REQUIRED)
 if (USE_LIBBFD)
   find_package(Libbfd)
 endif()
 find_package(Doxygen)
 
 add_subdirectory(src)
 add_subdirectory(cmake_modules)
 if (BUILD_DOC)
   add_subdirectory(doc)
 endif()
#add_subdirectory(tests)
 
#enable_testing()
#add_test(HurricaneTest ${PROJECT_BINARY_DIR}/tests/htest)

