# - Find the Coriolis includes and libraries.
# The following variables are set if Coriolis is found.  If CORIOLIS is not
# found, CORIOLIS_FOUND is set to false.
#  CORIOLIS_FOUND        - True when the Coriolis include directory is found.
#  CORIOLIS_INCLUDE_DIR - the path to where the Coriolis include files are.
#  CORIOLIS_LIBRARIES - The path to where the Coriolis library files are.


SET(CORIOLIS_INCLUDE_PATH_DESCRIPTION "directory containing the Coriolis include files. E.g /usr/local/include/coriolis or /asim/coriolis/include/coriolis")

SET(CORIOLIS_DIR_MESSAGE "Set the CORIOLIS_INCLUDE_DIR cmake cache entry to the ${CORIOLIS_INCLUDE_PATH_DESCRIPTION}")

# don't even bother under WIN32
IF(UNIX)

  SET(CORIOLIS_DIR_SEARCH $ENV{CORIOLIS_TOP} $ENV{HURRICANE_TOP})
  #
  # Look for an installation.
  #
  FIND_PATH(CRLCORE_INCLUDE_PATH NAMES crlcore/ToolEngine.h PATHS
    # Look in other places.
    ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES include/coriolis
    # Help the user find it if we cannot.
    DOC "The ${CORIOLIS_INCLUDE_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(CRLCORE_LIBRARY_PATH
    NAMES crlcore
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib
    # Help the user find it if we cannot.
    DOC "The ${CORIOLIS_INCLUDE_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(CRLCORE_STATIC_LIBRARY_PATH
    NAMES crlcore-static
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib
    # Help the user find it if we cannot.
    DOC "The ${CORIOLIS_INCLUDE_PATH_DESCRIPTION}"
  )

  SET_LIBRARIES_PATH(CORIOLIS CRLCORE)
  HURRICANE_CHECK_LIBRARIES(CORIOLIS)

ENDIF(UNIX)
