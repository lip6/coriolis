# - Find the Hurricane includes and libraries.
# The following variables are set if Hurricane is found.  If HURRICANE is not
# found, HURRICANE_FOUND is set to false.
#  HURRICANE_FOUND        - True when the Hurricane include directory is found.
#  HURRICANE_INCLUDE_DIR - the path to where the Hurricane include files are.
#  HURRICANE_LIBRARIES - The path to where the Hurricane library files are.


MACRO(SET_LIBRARIES_PATH configname library)
  SET(${configname}_FOUND "NOTFOUND")

  IF(${library}_LIBRARY_PATH)
    SET(${configname}_FOUND "YES")
    SET(${configname}_INCLUDE_DIR ${${library}_INCLUDE_PATH})
    SET(${configname}_LIBRARIES ${${library}_LIBRARY_PATH} ${${configname}_LIBRARIES})
    MARK_AS_ADVANCED(${configname}_INCLUDE_DIR ${configname}_LIBRARIES)
  ENDIF(${library}_LIBRARY_PATH)

  IF(NOT ${library}_INCLUDE_PATH)
    SET(${configname}_FOUND "NOTFOUND")
  ENDIF(NOT ${library}_INCLUDE_PATH)
ENDMACRO ( SET_LIBRARIES_PATH )

MACRO(HURRICANE_CHECK_LIBRARIES)
  IF(ARGC LESS 2)
    SET(REQUIRED ${ARGV0}_FIND_REQUIRED)
  ELSE(ARGC LESS 2)
    SET(REQUIRED ${ARGV1})
  ENDIF(ARGC LESS 2)
  IF(${ARGV0}_FOUND)
    IF(NOT ${ARGV0}_FIND_QUIETLY)
      IF(${ARGV0}_FOUND)
        MESSAGE(STATUS "Found ${ARGV0} : ${${ARGV0}_LIBRARIES}")
      ENDIF(${ARGV0}_FOUND)
    ENDIF(NOT ${ARGV0}_FIND_QUIETLY)
  ELSE(${ARGV0}_FOUND)
    IF(REQUIRED)
        MESSAGE(FATAL_ERROR "${ARGV0} was not found. ${${ARGV0}_DIR_MESSAGE}")
    ENDIF(REQUIRED)
  ENDIF(${ARGV0}_FOUND)
ENDMACRO(HURRICANE_CHECK_LIBRARIES)

MACRO(SET_LIB_LINK_MODE)
  IF(NOT BUILD_SHARED_LIBS)
  # check for qmake
    FIND_PROGRAM(QT_QMAKE_EXECUTABLE NAMES qmake-qt4 qmake PATHS
      /opt/qt4-static-4.3.2/bin
      NO_DEFAULT_PATH
      )
    SET(Boost_USE_STATIC_LIBS ON)
    MESSAGE(STATUS "Using Boost static libraries.")
    MESSAGE(STATUS "Building static libraries.")
  ELSE(NOT BUILD_SHARED_LIBS)
    MESSAGE(STATUS "Building dynamic libraries.")
  ENDIF(NOT BUILD_SHARED_LIBS)
ENDMACRO(SET_LIB_LINK_MODE)

SET(HURRICANE_INCLUDE_PATH_DESCRIPTION "The directory containing the Hurricane include files. E.g /usr/local/include or /asim/coriolis/include")
SET(HURRICANE_LIBRARY_PATH_DESCRIPTION "The directory containing the Hurricane library files. E.g /usr/local/lib or /asim/coriolis/lib")
SET(HURRICANE_DIR_MESSAGE              "Set the HURRICANE_INCLUDE_DIR cmake cache entry to the ${HURRICANE_INCLUDE_PATH_DESCRIPTION}")

# don't even bother under WIN32
IF(UNIX)
  # Setup the DIR_SEARCH_PATH.
  MACRO(SETUP_SEARCH_DIR project)
    IF( NOT("$ENV{${project}_TOP}" STREQUAL "") )
      MESSAGE("-- ${project}_TOP is set to $ENV{${project}_TOP}")
      LIST(INSERT ${project}_DIR_SEARCH 0 "${DESTDIR}$ENV{${project}_TOP}")
    ENDIF( NOT("$ENV{${project}_TOP}" STREQUAL "") )

    IF( NOT("$ENV{${project}_USER_TOP}" STREQUAL "") )
      MESSAGE("-- ${project}_USER_TOP is set to $ENV{${project}_USER_TOP}")
      LIST(INSERT ${project}_DIR_SEARCH 0 "${DESTDIR}$ENV{${project}_USER_TOP}")
    ENDIF( NOT("$ENV{${project}_USER_TOP}" STREQUAL "") )

    LIST(REMOVE_DUPLICATES ${project}_DIR_SEARCH)
   
    MESSAGE("-- Components of ${project}_DIR_SEARCH:")
    FOREACH(PATH IN LISTS ${project}_DIR_SEARCH)
      MESSAGE("--   ${PATH}")
    ENDFOREACH(PATH)
  ENDMACRO(SETUP_SEARCH_DIR project)
  
  SETUP_SEARCH_DIR(CORIOLIS)

  #
  # Look for an installation.
  #
  FIND_PATH(HURRICANE_INCLUDE_PATH NAMES hurricane/Cell.h PATHS
    # Look in other places.
    ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES include/coriolis2 include
    # Help the user find it if we cannot.
    DOC "${HURRICANE_INCLUDE_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(HURRICANE_LIBRARY_PATH
    NAMES hurricane
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib${LIB_SUFFIX}
    # Help the user find it if we cannot.
    DOC "${HURRICANE_LIBRARY_PATH_DESCRIPTION}"
  )

  FIND_PATH(HURRICANE_VIEWER_INCLUDE_PATH
    NAMES hurricane/viewer/CellWidget.h
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES include/coriolis2 include
    # Help the user find it if we cannot.
    DOC "${HURRICANE_LIBRARY_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(HURRICANE_VIEWER_LIBRARY_PATH
    NAMES viewer 
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib${LIB_SUFFIX}
    # Help the user find it if we cannot.
    DOC "${HURRICANE_LIBRARY_PATH_DESCRIPTION}"
  )

  FIND_PATH(HURRICANE_PYTHON_INCLUDE_PATH
    NAMES hurricane/isobar/PyCell.h
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES include/coriolis2 include
    # Help the user find it if we cannot.
    DOC "${HURRICANE_LIBRARY_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(HURRICANE_PYTHON_LIBRARY_PATH
    NAMES isobar 
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib${LIB_SUFFIX}
    # Help the user find it if we cannot.
    DOC "${HURRICANE_LIBRARY_PATH_DESCRIPTION}"
  )

  SET_LIBRARIES_PATH(HURRICANE HURRICANE)
  SET_LIBRARIES_PATH(HURRICANE_GRAPHICAL HURRICANE_VIEWER)
  SET_LIBRARIES_PATH(HURRICANE_PYTHON    HURRICANE_PYTHON)

  HURRICANE_CHECK_LIBRARIES(HURRICANE           )
  HURRICANE_CHECK_LIBRARIES(HURRICANE_GRAPHICAL ${HURRICANE_FIND_REQUIRED})
  HURRICANE_CHECK_LIBRARIES(HURRICANE_PYTHON    ${HURRICANE_FIND_REQUIRED})

#
# Adds -Wall to the C/C++ flags.
#
  SET(CMAKE_C_FLAGS_DEBUG     "-g -Wall" CACHE STRING "Debug options." FORCE)
  SET(CMAKE_CXX_FLAGS_DEBUG   "-g -Wall" CACHE STRING "Debug options." FORCE)
  SET(CMAKE_C_FLAGS_RELEASE   "-O3 -DNDEBUG -Wall" CACHE STRING "Release options." FORCE)
  SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -Wall" CACHE STRING "Release options." FORCE)

ENDIF(UNIX)
