
 project(VLSISAPD)

 cmake_minimum_required(VERSION 2.4.0)

 list(INSERT CMAKE_MODULE_PATH 0 "${DESTDIR}/$ENV{BOOTSTRAP_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
 list(INSERT CMAKE_MODULE_PATH 0 "${VLSISAPD_SOURCE_DIR}/cmake_modules/")

 set_cmake_policies()
 setup_boost(program_options python regex)

 find_package(LibXml2 REQUIRED)
 find_package(PythonSitePackages REQUIRED)
 find_package(PythonLibs REQUIRED)
 find_package(BISON REQUIRED)
 find_package(FLEX REQUIRED)
 find_package(Doxygen)

 add_subdirectory(src)
 add_subdirectory(cmake_modules)

 if(BUILD_DOC AND DOXYGEN_FOUND)
   add_subdirectory(doc)
   add_subdirectory(examples)
 endif(BUILD_DOC AND DOXYGEN_FOUND)
