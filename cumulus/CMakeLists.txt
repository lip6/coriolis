
 project(CUMULUS)
 
 cmake_minimum_required(VERSION 2.4.0)
 list(INSERT CMAKE_MODULE_PATH 0 "${DESTDIR}/$ENV{BOOTSTRAP_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
 setup_project_paths(VLSISAPD)
 setup_project_paths(CORIOLIS)
 
 set_cmake_policies()
 set_lib_link_mode()
 setup_sysconfdir("$ENV{CORIOLIS_TOP}")
 
 find_package(PythonLibs REQUIRED)
 find_package(PythonSitePackages REQUIRED)
 find_package(VLSISAPD REQUIRED)
 find_package(HURRICANE REQUIRED)
 find_package(CORIOLIS REQUIRED)
 
 add_subdirectory(src)
