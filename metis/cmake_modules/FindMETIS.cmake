# - Find the Metis includes and libraries.
# The following variables are set if Coriolis is found.  If METIS is not
# found, METIS_FOUND is set to false.
#  METIS_FOUND        - True when the Coriolis include directory is found.
#  METIS_INCLUDE_DIR - the path to where the Coriolis include files are.
#  METIS_LIBRARIES - The path to where the Coriolis library files are.


SET(METIS_INCLUDE_PATH_DESCRIPTION "directory containing the Metis/hmetis include files. E.g /usr/local/include/coriolis or /asim/coriolis/include/coriolis")

SET(METIS_DIR_MESSAGE "Set the METIS_INCLUDE_DIR cmake cache entry to the ${METIS_INCLUDE_PATH_DESCRIPTION}")

# don't even bother under WIN32
IF(UNIX)
  #
  # Look for an installation.
  #
  FIND_PATH(METIS_INCLUDE_PATH NAMES metis/MetisEngine.h PATHS
    # Look in other places.
    ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES include/coriolis2
    # Help the user find it if we cannot.
    DOC "The ${METIS_INCLUDE_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(METIS_LIBRARY_PATH
    NAMES metis
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib${LIB_SUFFIX}
    # Help the user find it if we cannot.
    DOC "The ${METIS_INCLUDE_PATH_DESCRIPTION}"
  )

  SET(HMETIS_INCLUDE_PATH METIS_INCLUDE_PATH)

  FIND_LIBRARY(HMETIS_LIBRARY_PATH
    NAMES hmetis
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib${LIB_SUFFIX}
    # Help the user find it if we cannot.
    DOC "The ${METIS_INCLUDE_PATH_DESCRIPTION}"
  )

  SET_LIBRARIES_PATH(METIS METIS)
  SET_LIBRARIES_PATH(HMETIS HMETIS)
  HURRICANE_CHECK_LIBRARIES(METIS)

ENDIF(UNIX)
