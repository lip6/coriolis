cmake_minimum_required(VERSION 3.16)
project(MELTEMI)

set(ignoreVariables "${CMAKE_INSTALL_DIR}" "${BUILD_DOC}")


list(INSERT CMAKE_MODULE_PATH 0
     "${DESTDIR}$ENV{CORIOLIS_TOP}/share/cmake/Modules/")
find_package(Bootstrap REQUIRED)
setup_project_paths(CORIOLIS)
setup_qt()

set_cmake_policies()
setup_boost(program_options)
setup_python()

find_package(PythonSitePackages REQUIRED)
find_package(HURRICANE REQUIRED)
find_package(CORIOLIS REQUIRED)
find_package(COLOQUINTE REQUIRED)
find_package(ETESIAN REQUIRED)
find_package(ANABATIC REQUIRED)
find_package(KATANA REQUIRED)
find_package(Libexecinfo REQUIRED)
find_package(Doxygen)

add_subdirectory(src)
add_subdirectory(cmake_modules)
