# - Find the Katabatic includes and libraries.
# The following variables are set if Coriolis is found.  If FOEHN is not
# found, FOEHN_FOUND is set to false.
#  FOEHN_FOUND       - True when the Coriolis include directory is found.
#  FOEHN_INCLUDE_DIR - the path to where the Coriolis include files are.
#  FOEHN_LIBRARIES   - The path to where the Coriolis library files are.


SET(FOEHN_INCLUDE_PATH_DESCRIPTION "directory containing the Katabatic include files. E.g /usr/local/include/coriolis2 or /asim/coriolis/include/coriolis2")

SET(FOEHN_DIR_MESSAGE "Set the FOEHN_INCLUDE_DIR cmake cache entry to the ${FOEHN_INCLUDE_PATH_DESCRIPTION}")

# don't even bother under WIN32
IF(UNIX)
  #
  # Look for an installation.
  #
  FIND_PATH(FOEHN_INCLUDE_PATH NAMES anabatic/FoehnEngine.h PATHS
    # Look in other places.
    ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES include/coriolis2
    # Help the user find it if we cannot.
    DOC "The ${FOEHN_INCLUDE_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(FOEHN_LIBRARY_PATH
    NAMES anabatic
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib64 lib
    # Help the user find it if we cannot.
    DOC "The ${FOEHN_INCLUDE_PATH_DESCRIPTION}"
  )

  SET_LIBRARIES_PATH(FOEHN FOEHN)
  HURRICANE_CHECK_LIBRARIES(FOEHN)

ENDIF(UNIX)
