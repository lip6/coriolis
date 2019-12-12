# -*- explicit-buffer-name: "CMakeLists.txt<coloquinte>" -*-

 set(CMAKE_LEGACY_CYGWIN_WIN32 0)
 project(COLOQUINTE)

 set(ignoreVariables "${CMAKE_INSTALL_DIR}")

 option(BUILD_DOC      "Build the documentation (doxygen)" OFF)
 
 cmake_minimum_required(VERSION 2.8.9)

 list(INSERT CMAKE_MODULE_PATH 0 "${DESTDIR}$ENV{CORIOLIS_TOP}/share/cmake/Modules/")
 find_package(Bootstrap  REQUIRED)
 setup_project_paths(CORIOLIS)
 setup_qt()
 
 set_cmake_policies()
 setup_boost()
 
 find_package(Libexecinfo REQUIRED)
 find_package(Doxygen)

 if(WITH_OPENMP)
   find_package(OpenMP REQUIRED)
   add_definitions(${OpenMP_CXX_FLAGS}) 
   set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${OpenMP_CXX_FLAGS}")
   set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OpenMP_CXX_FLAGS}")
   set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_CXX_FLAGS}")
 endif()
 
 add_subdirectory(src)
 add_subdirectory(cmake_modules)
#add_subdirectory(doc)
