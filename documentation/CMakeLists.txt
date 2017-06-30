# -*- mode: CMAKE; explicit-buffer-name: "CMakeLists.txt<documentation>" -*-

 set(CMAKE_LEGACY_CYGWIN_WIN32 0)
 project(DOCUMENTATION)

 cmake_minimum_required(VERSION 2.4.0)

 OPTION(BUILD_DOC "Build the documentation (html+latex)" OFF)

 list(INSERT CMAKE_MODULE_PATH 0 "${DESTDIR}$ENV{CORIOLIS_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
 setup_project_paths(CORIOLIS)
 list(INSERT CMAKE_MODULE_PATH 0 "${CRLCORE_SOURCE_DIR}/cmake_modules/")
 print_cmake_module_path()

 set_cmake_policies()
 check_distribution()

#if(BUILD_DOC)
#  include(UseLATEX)
#endif(BUILD_DOC)
 
 add_subdirectory(examples)
 if(BUILD_DOC)
   add_subdirectory(etc)
   add_subdirectory(UsersGuide)
   add_subdirectory(PythonCpp)
 endif(BUILD_DOC)

 set ( htmlInstallDir  share/doc/coriolis2/ )
 set ( latexInstallDir share/doc/coriolis2/ )

 install ( FILES general-index.html DESTINATION ${htmlInstallDir} RENAME index.html )

