# -*- explicit-buffer-name: "CMakeLists.txt<karakaze>" -*-

 project(KARAKAZE)

 cmake_minimum_required(VERSION 2.8.9)

 set(ignoreVariables "${BUILD_DOC}")

 list(INSERT CMAKE_MODULE_PATH 0 "$ENV{CORIOLIS_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
 setup_project_paths(CORIOLIS)
 print_cmake_module_path()
 set_cmake_policies()
 check_distribution()

 setup_sysconfdir("${CMAKE_INSTALL_PREFIX}")
 setup_qt()
 find_package(PythonLibs         2 REQUIRED)
 find_package(PythonSitePackages REQUIRED)
 find_package(HURRICANE          REQUIRED)
 find_package(CORIOLIS           REQUIRED)

#add_subdirectory(src)
 add_subdirectory(python)
#add_subdirectory(etc)
#add_subdirectory(cmake_modules)

