
 project(ISPD)
 
 cmake_minimum_required(VERSION 2.4.0)

 list(INSERT CMAKE_MODULE_PATH 0 "$ENV{BOOTSTRAP_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
 setup_project_paths(VLSISAPD)
 setup_project_paths(CORIOLIS)
 
 set_cmake_policies()
 set_lib_link_mode()
#setup_apple()
 setup_boost(program_options filesystem python regex)
 
 find_package(LibXml2 REQUIRED)
 set(QT_USE_QTXML "true")
 find_package(Qt4 REQUIRED)
 find_package(PythonLibs REQUIRED)
 find_package(PythonSitePackages REQUIRED)
 find_package(VLSISAPD REQUIRED)
 find_package(HURRICANE REQUIRED)
 find_package(CORIOLIS REQUIRED)
 find_package(KNIK REQUIRED)
 find_package(LEFDEF REQUIRED)
#find_package(KATABATIC REQUIRED)
#find_package(KITE REQUIRED)
#find_package(EQUINOX REQUIRED)
#find_package(SOLSTICE REQUIRED)
 
 add_subdirectory(src)
