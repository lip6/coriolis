# -*- mode: CMAKE explicit-buffer-name: "CMakeLists.txt<unicorn>" -*-

 set(CMAKE_LEGACY_CYGWIN_WIN32 0)
 project(UNICORN)
 
 cmake_minimum_required(VERSION 2.8.9)

 list(INSERT CMAKE_MODULE_PATH 0 "${DESTDIR}$ENV{CORIOLIS_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
 setup_project_paths(IMPORTEDS)
 setup_project_paths(CORIOLIS)
 
 set_cmake_policies()
 set_lib_link_mode()
 setup_sysconfdir("${CMAKE_INSTALL_PREFIX}")
 setup_boost(program_options python regex)
 setup_qt()
 
 find_package(LibXml2 REQUIRED)
 find_package(PythonLibs REQUIRED)
 find_package(PythonSitePackages REQUIRED)
 find_package(LEFDEF REQUIRED)
 find_package(COLOQUINTE)
 find_package(VLSISAPD REQUIRED)
 find_package(HURRICANE REQUIRED)
 find_package(CORIOLIS REQUIRED)
#find_package(NIMBUS REQUIRED)
#find_package(METIS REQUIRED)
#find_package(MAUKA REQUIRED)
 find_package(ETESIAN REQUIRED)
 find_package(KNIK REQUIRED)
 find_package(KATABATIC REQUIRED)
 find_package(KITE REQUIRED)
#find_package(EQUINOX REQUIRED)
#find_package(SOLSTICE REQUIRED)
 
 add_subdirectory(src)
 add_subdirectory(cmake_modules)

 if(BUILD_DOC)
   find_package(Doxygen REQUIRED)
   add_subdirectory(doc)
 endif(BUILD_DOC)
