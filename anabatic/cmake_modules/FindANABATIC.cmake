# - Find the Katabatic includes and libraries.
# The following variables are set if Coriolis is found.  If ANABATIC is not
# found, ANABATIC_FOUND is set to false.
#  ANABATIC_FOUND       - True when the Coriolis include directory is found.
#  ANABATIC_INCLUDE_DIR - the path to where the Coriolis include files are.
#  ANABATIC_LIBRARIES   - The path to where the Coriolis library files are.


SET(ANABATIC_INCLUDE_PATH_DESCRIPTION "directory containing the Katabatic include files. E.g /usr/local/include/coriolis2 or /asim/coriolis/include/coriolis2")

SET(ANABATIC_DIR_MESSAGE "Set the ANABATIC_INCLUDE_DIR cmake cache entry to the ${ANABATIC_INCLUDE_PATH_DESCRIPTION}")

# don't even bother under WIN32
IF(UNIX)
  #
  # Look for an installation.
  #
  FIND_PATH(ANABATIC_INCLUDE_PATH NAMES anabatic/AnabaticEngine.h PATHS
    # Look in other places.
    ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES include/coriolis2
    # Help the user find it if we cannot.
    DOC "The ${ANABATIC_INCLUDE_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(ANABATIC_LIBRARY_PATH
    NAMES anabatic
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib64 lib
    # Help the user find it if we cannot.
    DOC "The ${ANABATIC_INCLUDE_PATH_DESCRIPTION}"
  )

  SET_LIBRARIES_PATH(ANABATIC ANABATIC)
  HURRICANE_CHECK_LIBRARIES(ANABATIC)

ENDIF(UNIX)
