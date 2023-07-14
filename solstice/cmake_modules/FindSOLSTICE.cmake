# - Find the Solstice includes and libraries.
# The following variables are set if Coriolis is found.  If SOLSTICE is not
# found, SOLSTICE_FOUND is set to false.
#  SOLSTICE_FOUND       - True when the Coriolis include directory is found.
#  SOLSTICE_INCLUDE_DIR - the path to where the Coriolis include files are.
#  SOLSTICE_LIBRARIES   - The path to where the Coriolis library files are.


SET(SOLSTICE_INCLUDE_PATH_DESCRIPTION "directory containing the Solstice include files. E.g /usr/local/include/coriolis2 or /asim/coriolis/include/coriolis2")

SET(SOLSTICE_DIR_MESSAGE "Set the SOLSTICE_INCLUDE_DIR cmake cache entry to the ${SOLSTICE_INCLUDE_PATH_DESCRIPTION}")

# don't even bother under WIN32
IF(UNIX)
  #
  # Look for an installation.
  #
  FIND_PATH(SOLSTICE_INCLUDE_PATH NAMES solstice/Brick.h PATHS
    # Look in other places.
    ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES include/coriolis2
    # Help the user find it if we cannot.
    DOC "The ${SOLSTICE_INCLUDE_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(SOLSTICE_LIBRARY_PATH
    NAMES solstice
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib64 lib
    # Help the user find it if we cannot.
    DOC "The ${SOLSTICE_INCLUDE_PATH_DESCRIPTION}"
  )

  SET_LIBRARIES_PATH(SOLSTICE SOLSTICE)
  HURRICANE_CHECK_LIBRARIES(SOLSTICE)

ENDIF(UNIX)
