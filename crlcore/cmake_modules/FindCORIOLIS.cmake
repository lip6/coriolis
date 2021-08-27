# - Find the Coriolis includes and libraries.
# The following variables are set if Coriolis is found.
# If Coriolis is not found, CRLCORE_FOUND is set to false.
#  CRLCORE_FOUND       - True when the Coriolis include directory is found.
#  CRLCORE_INCLUDE_DIR - the path to where the Coriolis include files are.
#  CRLCORE_LIBRARIES   - The path to where the Coriolis library files are.

SET(CORIOLIS_INCLUDE_PATH_DESCRIPTION "directory containing the Coriolis include files. E.g /usr/local/include/coriolis2 or /asim/coriolis/include/coriolis2")
SET(CORIOLIS_DIR_MESSAGE "Set the CORIOLIS_INCLUDE_DIR cmake cache entry to the ${CORIOLIS_INCLUDE_PATH_DESCRIPTION}")

# don't even bother under WIN32
IF(UNIX)
  
  SETUP_SEARCH_DIR(CORIOLIS)
  #
  # Look for an installation.
  #
  FIND_PATH(CRLCORE_INCLUDE_PATH NAMES crlcore/ToolEngine.h PATHS
    # Look in other places.
    ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES include/coriolis2 include
    # Help the user find it if we cannot.
    DOC "The ${CORIOLIS_INCLUDE_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(CRLCORE_LIBRARY_PATH
    NAMES crlcore
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib64 lib
    # Help the user find it if we cannot.
    DOC "The ${CORIOLIS_INCLUDE_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(LIBMANAGER_LIBRARY_PATH
    NAMES libmanager
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib64 lib
    # Help the user find it if we cannot.
    DOC "The ${CORIOLIS_INCLUDE_PATH_DESCRIPTION}"
  )

  FIND_PATH(CORIOLIS_PYTHON_INCLUDE_PATH
    NAMES crlcore/PyAllianceFramework.h
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES include/coriolis2 include
    # Help the user find it if we cannot.
    DOC "${CORIOLIS_LIBRARY_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(CORIOLIS_PYTHON_LIBRARY_PATH
    NAMES pycrlcore 
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib64 lib
    # Help the user find it if we cannot.
    DOC "${CORIOLIS_LIBRARY_PATH_DESCRIPTION}"
  )

  SET_LIBRARIES_PATH(CORIOLIS CRLCORE)
  SET_LIBRARIES_PATH(CORIOLIS LIBMANAGER)
  SET_LIBRARIES_PATH(CORIOLIS_PYTHON CORIOLIS_PYTHON)

  HURRICANE_CHECK_LIBRARIES(CORIOLIS)
  HURRICANE_CHECK_LIBRARIES(CORIOLIS_PYTHON ${CORIOLIS_FIND_REQUIRED})

ENDIF(UNIX)

SET(CORIOLIS_INCLUDE_DIR "${CRLCORE_INCLUDE_PATH}")
