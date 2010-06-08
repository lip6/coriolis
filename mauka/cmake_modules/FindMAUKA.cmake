# - Find the Mauka includes and libraries.
# The following variables are set if Coriolis is found.  If MAUKA is not
# found, MAUKA_FOUND is set to false.
#  MAUKA_FOUND        - True when the Coriolis include directory is found.
#  MAUKA_INCLUDE_DIR - the path to where the Coriolis include files are.
#  MAUKA_LIBRARIES - The path to where the Coriolis library files are.


SET(MAUKA_INCLUDE_PATH_DESCRIPTION "directory containing the Mauka/flute include files. E.g /usr/local/include/coriolis or /asim/coriolis/include/coriolis")

SET(MAUKA_DIR_MESSAGE "Set the MAUKA_INCLUDE_DIR cmake cache entry to the ${MAUKA_INCLUDE_PATH_DESCRIPTION}")

# don't even bother under WIN32
IF(UNIX)
  #
  # Look for an installation.
  #
  FIND_PATH(MAUKA_INCLUDE_PATH NAMES mauka/MaukaEngine.h PATHS
    # Look in other places.
    ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES include/coriolis2
    # Help the user find it if we cannot.
    DOC "The ${MAUKA_INCLUDE_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(MAUKA_LIBRARY_PATH
    NAMES mauka
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib${LIB_SUFFIX}
    # Help the user find it if we cannot.
    DOC "The ${MAUKA_INCLUDE_PATH_DESCRIPTION}"
  )

  SET_LIBRARIES_PATH(MAUKA MAUKA)
  HURRICANE_CHECK_LIBRARIES(MAUKA)

ENDIF(UNIX)
