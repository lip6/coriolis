# - Find the Hurricane includes and libraries.
# The following variables are set if Hurricane is found.  If HURRICANE is not
# found, HURRICANE_FOUND is set to false.
#  HURRICANE_FOUND        - True when the Hurricane include directory is found.
#  HURRICANE_INCLUDE_DIR - the path to where the Hurricane include files are.
#  HURRICANE_LIBRARIES - The path to where the Hurricane library files are.


MACRO(SET_LIBRARIES_PATH configname library)
  SET(${configname}_FOUND        "NOTFOUND")

  IF(${library}_LIBRARY_PATH)
    SET(${configname}_FOUND "YES")
    SET(${configname}_INCLUDE_DIR ${${library}_INCLUDE_PATH})
    SET(${configname}_LIBRARIES ${${library}_LIBRARY_PATH} ${${configname}_LIBRARIES})
    MARK_AS_ADVANCED(${configname}_INCLUDE_DIR ${configname}_LIBRARIES)
  ENDIF(${library}_LIBRARY_PATH)

  IF(NOT ${library}_INCLUDE_PATH)
    SET(${configname}_FOUND        "NOTFOUND")
  ENDIF(NOT ${library}_INCLUDE_PATH)
ENDMACRO ( SET_LIBRARIES_PATH )

MACRO(HURRICANE_CHECK_LIBRARIES)
  IF(ARGC LESS 2)
    SET(REQUIRED ${argv0}_FIND_REQUIRED)
  ELSE(ARGC LESS 2)
    SET(REQUIRED ${argv1})
  ENDIF(ARGC LESS 2)
  IF(${argv0}_FOUND)
    IF(NOT ${argv0}_FIND_QUIETLY)
      IF(${argv0}_FOUND)
        MESSAGE(STATUS "Found ${argv0} : ${${argv0}_LIBRARIES}")
      ENDIF(${argv0}_FOUND)
    ENDIF(NOT ${argv0}_FIND_QUIETLY)
  ELSE(${argv0}_FOUND)
    IF(REQUIRED)
        MESSAGE(FATAL_ERROR "${argv0} was not found. ${${argv0}_DIR_MESSAGE}")
    ENDIF(REQUIRED)
  ENDIF(${argv0}_FOUND)
ENDMACRO(HURRICANE_CHECK_LIBRARIES)

MACRO(SET_LIB_LINK_MODE)
  IF(NOT BUILD_SHARED_LIBS)
  # check for qmake
    FIND_PROGRAM(QT_QMAKE_EXECUTABLE NAMES qmake-qt4 qmake PATHS
      /opt/qt4-static-4.3.2/bin
      NO_DEFAULT_PATH
      )
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

  SET(HURRICANE_DIR_SEARCH $ENV{HURRICANE_TOP})
  IF(HURRICANE_DIR_SEARCH)
    FILE(TO_CMAKE_PATH ${HURRICANE_DIR_SEARCH} HURRICANE_DIR_SEARCH)
  ENDIF(HURRICANE_DIR_SEARCH)

  #
  # Look for an installation.
  #
  FIND_PATH(HURRICANE_INCLUDE_PATH NAMES hurricane/Cell.h PATHS
    # Look in other places.
    ${HURRICANE_DIR_SEARCH}
    PATH_SUFFIXES include
    # Help the user find it if we cannot.
    DOC "${HURRICANE_INCLUDE_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(HURRICANE_LIBRARY_PATH
    NAMES hurricane
    PATHS ${HURRICANE_DIR_SEARCH}
    PATH_SUFFIXES lib
    # Help the user find it if we cannot.
    DOC "${HURRICANE_LIBRARY_PATH_DESCRIPTION}"
  )

  FIND_PATH(HURRICANE_VIEWER_INCLUDE_PATH
    NAMES hurricane/viewer/CellWidget.h
    PATHS ${HURRICANE_DIR_SEARCH}
    PATH_SUFFIXES include
    # Help the user find it if we cannot.
    DOC "${HURRICANE_LIBRARY_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(HURRICANE_VIEWER_LIBRARY_PATH
    NAMES viewer 
    PATHS ${HURRICANE_DIR_SEARCH}
    PATH_SUFFIXES lib
    # Help the user find it if we cannot.
    DOC "${HURRICANE_LIBRARY_PATH_DESCRIPTION}"
  )

  FIND_PATH(HURRICANE_PYTHON_INCLUDE_PATH
    NAMES hurricane/isobar/PyCell.h
    PATHS ${HURRICANE_DIR_SEARCH}
    PATH_SUFFIXES include
    # Help the user find it if we cannot.
    DOC "${HURRICANE_LIBRARY_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(HURRICANE_PYTHON_LIBRARY_PATH
    NAMES isobar 
    PATHS ${HURRICANE_DIR_SEARCH}
    PATH_SUFFIXES lib
    # Help the user find it if we cannot.
    DOC "${HURRICANE_LIBRARY_PATH_DESCRIPTION}"
  )

  SET_LIBRARIES_PATH(HURRICANE HURRICANE)
  SET_LIBRARIES_PATH(HURRICANE_GRAPHICAL HURRICANE_VIEWER)
  SET_LIBRARIES_PATH(HURRICANE_PYTHON HURRICANE_PYTHON)

  HURRICANE_CHECK_LIBRARIES(HURRICANE           )
  HURRICANE_CHECK_LIBRARIES(HURRICANE_GRAPHICAL ${HURRICANE_FIND_REQUIRED})
  HURRICANE_CHECK_LIBRARIES(HURRICANE_PYTHON    ${HURRICANE_FIND_REQUIRED})

ENDIF(UNIX)
