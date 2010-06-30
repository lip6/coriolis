PROJECT(testOAWrapper)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4.0)

SET(CMAKE_MODULE_PATH "$ENV{CORIOLIS_USER_TOP}/share/cmake/Modules/"
                      "$ENV{CORIOLIS_TOP}/share/cmake/Modules/"
                      "$ENV{IO_USER_TOP}/share/cmake/Modules/"
                      "$ENV{IO_TOP}/share/cmake/Modules/"
                      "$ENV{CHAMS_USER_TOP}/share/cmake/Modules/"
                      "$ENV{CHAMS_TOP}/share/cmake/Modules/")

FIND_PACKAGE(HURRICANE REQUIRED)
FIND_PACKAGE(CORIOLIS REQUIRED)
FIND_PACKAGE(VLSISAPD REQUIRED)
FIND_PACKAGE(HURRICANEAMS REQUIRED)
FIND_PACKAGE(AMSCORE REQUIRED)
FIND_PACKAGE(LibXml2 REQUIRED)
FIND_PACKAGE(PythonLibs REQUIRED)

ADD_SUBDIRECTORY(src)
