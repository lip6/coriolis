# - Find the Hurricane includes and libraries.
# The following variables are set if Hurricane is found.  If HURRICANE is not
# found, HURRICANE_FOUND is set to false.
#  HURRICANE_FOUND        - True when the Hurricane include directory is found.
#  HURRICANE_INCLUDE_DIR - the path to where the Hurricane include files are.
#  HURRICANE_LIBRARIES - The path to where the Hurricane library files are.


SET(HURRICANE_INCLUDE_PATH_DESCRIPTION "The directory containing the Hurricane include files. E.g /usr/local/include or /asim/coriolis/include")
SET(HURRICANE_LIBRARY_PATH_DESCRIPTION "The directory containing the Hurricane library files. E.g /usr/local/lib or /asim/coriolis/lib")
SET(HURRICANE_DIR_MESSAGE              "Set the HURRICANE_INCLUDE_DIR cmake cache entry to the ${HURRICANE_INCLUDE_PATH_DESCRIPTION}")

# don't even bother under WIN32
IF(UNIX)
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

  SET_LIBRARIES_PATH(HURRICANE           HURRICANE)
  SET_LIBRARIES_PATH(HURRICANE_PYTHON    HURRICANE_PYTHON)
  SET_LIBRARIES_PATH(HURRICANE_GRAPHICAL HURRICANE_VIEWER)

  HURRICANE_CHECK_LIBRARIES(HURRICANE           )
  HURRICANE_CHECK_LIBRARIES(HURRICANE_GRAPHICAL ${HURRICANE_FIND_REQUIRED})
  HURRICANE_CHECK_LIBRARIES(HURRICANE_PYTHON    ${HURRICANE_FIND_REQUIRED})

ENDIF(UNIX)
