# - Find the Kite includes and libraries.
# The following variables are set if Coriolis is found.  If KITE is not
# found, KITE_FOUND is set to false.
#  KITE_FOUND       - True when the Coriolis include directory is found.
#  KITE_INCLUDE_DIR - the path to where the Coriolis include files are.
#  KITE_LIBRARIES   - The path to where the Coriolis library files are.


SET(KITE_INCLUDE_PATH_DESCRIPTION "directory containing the Kite include files. E.g /usr/local/include/coriolis or /asim/coriolis/include/coriolis")

SET(KITE_DIR_MESSAGE "Set the KITE_INCLUDE_DIR cmake cache entry to the ${KITE_INCLUDE_PATH_DESCRIPTION}")

# don't even bother under WIN32
IF(UNIX)
  #
  # Look for an installation.
  #
  FIND_PATH(KITE_INCLUDE_PATH NAMES kite/KiteEngine.h PATHS
    # Look in other places.
    ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES include/coriolis
    # Help the user find it if we cannot.
    DOC "The ${KITE_INCLUDE_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(KITE_LIBRARY_PATH
    NAMES kite
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib64 lib
    # Help the user find it if we cannot.
    DOC "The ${KITE_INCLUDE_PATH_DESCRIPTION}"
  )

  SET_LIBRARIES_PATH(KITE KITE)
  HURRICANE_CHECK_LIBRARIES(KITE)

ENDIF(UNIX)
