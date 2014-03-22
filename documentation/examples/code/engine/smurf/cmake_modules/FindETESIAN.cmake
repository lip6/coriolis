# - Find the Smurf includes and libraries.
# The following variables are set if Coriolis is found.  If SMURF is not
# found, SMURF_FOUND is set to false.
#  SMURF_FOUND       - True when the Coriolis include directory is found.
#  SMURF_INCLUDE_DIR - the path to where the Coriolis include files are.
#  SMURF_LIBRARIES   - The path to where the Coriolis library files are.


SET(SMURF_INCLUDE_PATH_DESCRIPTION "directory containing the Smurf include files. E.g /usr/local/include/coriolis or /asim/coriolis/include/coriolis")

SET(SMURF_DIR_MESSAGE "Set the SMURF_INCLUDE_DIR cmake cache entry to the ${SMURF_INCLUDE_PATH_DESCRIPTION}")

# don't even bother under WIN32
IF(UNIX)
  #
  # Look for an installation.
  #
  FIND_PATH(SMURF_INCLUDE_PATH NAMES smurf/SmurfEngine.h PATHS
    # Look in other places.
    ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES include/coriolis
    # Help the user find it if we cannot.
    DOC "The ${SMURF_INCLUDE_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(SMURF_LIBRARY_PATH
    NAMES smurf
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib${LIB_SUFFIX}
    # Help the user find it if we cannot.
    DOC "The ${SMURF_INCLUDE_PATH_DESCRIPTION}"
  )

  SET_LIBRARIES_PATH(SMURF SMURF)
  HURRICANE_CHECK_LIBRARIES(SMURF)

ENDIF(UNIX)
