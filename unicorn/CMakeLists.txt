# -*- mode: CMAKE explicit-buffer-name: "CMakeLists.txt<unicorn>" -*-

 set(CMAKE_LEGACY_CYGWIN_WIN32 0)
 project(UNICORN)

 option(USE_LIBBFD "Link with BFD libraries to print stack traces" OFF)
 
 cmake_minimum_required(VERSION 3.16)

 set(ignoreVariables "${CMAKE_INSTALL_DIR}")
 option(USE_LIBBFD     "Link with BFD libraries to print stack traces" OFF)

 list(INSERT CMAKE_MODULE_PATH 0 "${DESTDIR}$ENV{CORIOLIS_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
 setup_project_paths(CORIOLIS)
 
 set_cmake_policies()
 cmake_policy(SET CMP0054 NEW)
 setup_sysconfdir("${CMAKE_INSTALL_PREFIX}")
 setup_boost(program_options)
 setup_qt()
 setup_python()
 
 if (USE_LIBBFD)
   find_package(Libbfd)
 endif()
 find_package(BZip2              REQUIRED)
 find_package(PythonSitePackages REQUIRED)
 find_package(LEFDEF             REQUIRED)
 find_package(COLOQUINTE)
 find_package(FLUTE              REQUIRED)
 find_package(HURRICANE          REQUIRED)
 find_package(CORIOLIS           REQUIRED)
 find_package(ANABATIC           REQUIRED)
 find_package(KATANA             REQUIRED)
 #find_package(BORA               REQUIRED) # FIXME: make FindBORA.cmake
 find_package(ETESIAN            REQUIRED)
 #find_package(KNIK               REQUIRED)
 #find_package(KATABATIC          REQUIRED)
 #find_package(KITE               REQUIRED)
 #find_package(EQUINOX            REQUIRED)
 #find_package(SOLSTICE           REQUIRED)
 #find_package(TUTORIAL           REQUIRED) # FIXME: make FindTUTORIAL.cmake
 find_package(Doxygen)

 if(WITH_OPENMP)
    find_package(OpenMP REQUIRED)
    add_definitions(${OpenMP_CXX_FLAGS})
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_CXX_FLAGS}")
 endif(WITH_OPENMP)

 add_subdirectory(src)
 add_subdirectory(python)
 add_subdirectory(cmake_modules)
 if (BUILD_DOC)
   add_subdirectory(doc)
 endif()
