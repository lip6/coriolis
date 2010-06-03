PROJECT(NIMBUS)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4.0)

SET(CMAKE_C_FLAGS_DEBUG             "-pg -g -Wall" CACHE STRING "Debug options." FORCE)
SET(CMAKE_CXX_FLAGS_DEBUG           "-pg -g -Wall" CACHE STRING "Debug options." FORCE)
#SET(CMAKE_LINKER_FLAGS_DEBUG        "-pg"          CACHE STRING "Debug options." FORCE)
#SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG "-pg"          CACHE STRING "Debug options." FORCE)
#SET(CMAKE_MODULE_LINKER_FLAGS_DEBUG "-pg"          CACHE STRING "Debug options." FORCE)
#SET(CMAKE_EXE_LINKER_FLAGS_DEBUG    "-pg"          CACHE STRING "Debug options." FORCE)
SET(CMAKE_C_FLAGS_RELEASE           "-O3 -DNDEBUG -Wall" CACHE STRING "Release options." FORCE)
SET(CMAKE_CXX_FLAGS_RELEASE         "-O3 -DNDEBUG -Wall" CACHE STRING "Release options." FORCE)

IF(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

# This macro has to be included in all the tools CMakeLists.txt as it's
# the sole means of localizing other tools/projects.
MACRO(SETUP_PROJECT_PATHS project)
  IF( NOT("$ENV{${project}_TOP}" STREQUAL "") )
    MESSAGE("-- ${project}_TOP is set to $ENV{${project}_TOP}")
    SET(PROJECT_MODULE_PATH "${DESTDIR}$ENV{${project}_TOP}/share/cmake/Modules/")
    LIST(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_MODULE_PATH}")
  ENDIF( NOT("$ENV{${project}_TOP}" STREQUAL "") )
                
  IF( NOT("$ENV{${project}_USER_TOP}" STREQUAL "") )
    MESSAGE("-- ${project}_USER_TOP is set to $ENV{${project}_USER_TOP}")
    SET(PROJECT_MODULE_PATH "${DESTDIR}$ENV{${project}_USER_TOP}/share/cmake/Modules/")
    LIST(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_MODULE_PATH}")
  ENDIF( NOT("$ENV{${project}_USER_TOP}" STREQUAL "") )
                                
  LIST(REMOVE_DUPLICATES CMAKE_MODULE_PATH)
ENDMACRO(SETUP_PROJECT_PATHS project)
                                  
SETUP_PROJECT_PATHS(VLSISAPD)
SETUP_PROJECT_PATHS(CORIOLIS)
                                  
IF(BUILD_DOC)
  FIND_PACKAGE(Doxygen)
ENDIF(BUILD_DOC)
FIND_PACKAGE(Qt4 REQUIRED) # find and setup Qt4 for this project
FIND_PACKAGE(HURRICANE REQUIRED)
FIND_PACKAGE(CORIOLIS REQUIRED)

SET_LIB_LINK_MODE()

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(cmake_modules)
