# -*- explicit-buffer-name: "CMakeLists.txt<karakaze>" -*-

 project(KARAKAZE)

 cmake_minimum_required(VERSION 3.16)

 set(ignoreVariables "${BUILD_DOC}" "${CMAKE_INSTALL_DIR}")
 option(USE_LIBBFD     "Link with BFD libraries to print stack traces" OFF)

 list(INSERT CMAKE_MODULE_PATH 0 "$ENV{CORIOLIS_TOP}/share/cmake/Modules/")
 find_package(Bootstrap REQUIRED)
 setup_project_paths(CORIOLIS)
 print_cmake_module_path()
 set_cmake_policies()
 check_distribution()

 setup_sysconfdir("${CMAKE_INSTALL_PREFIX}")
 setup_python()
 find_package(PythonSitePackages REQUIRED)
 find_package(HURRICANE          REQUIRED)
 find_package(CORIOLIS           REQUIRED)
 find_package(KATANA             REQUIRED)
 find_package(ANABATIC           REQUIRED)
 #find_package(BORA               REQUIRED) # FIXME

#add_subdirectory(src)
 add_subdirectory(python)
#add_subdirectory(etc)
#add_subdirectory(cmake_modules)

