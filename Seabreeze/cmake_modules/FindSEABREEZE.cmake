# - Find the Seabreeze includes and libraries.
# The following variables are set if Coriolis is found.  If KATABATIC is not
# found, KATABATIC_FOUND is set to false.
#  KATABATIC_FOUND       - True when the Coriolis include directory is found.
#  KATABATIC_INCLUDE_DIR - the path to where the Coriolis include files are.
#  KATABATIC_LIBRARIES   - The path to where the Coriolis library files are.


SET(SEABREEZE_INCLUDE_PATH_DESCRIPTION "directory containing the Katabatic include files. E.g /usr/local/include/coriolis2 or /asim/coriolis/include/coriolis2")

SET(SEABREEZE_DIR_MESSAGE "Set the SEABREEZE_INCLUDE_DIR cmake cache entry to the ${SEABREEZE_INCLUDE_PATH_DESCRIPTION}")

# don't even bother under WIN32
IF(UNIX)
  #
  # Look for an installation.
  #
  FIND_PATH(SEABREEZE_INCLUDE_PATH NAMES katabatic/KatabaticEngine.h PATHS
    # Look in other places.
    ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES include/coriolis2
    # Help the user find it if we cannot.
    DOC "The ${SEABREEZE_INCLUDE_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(SEABREEZE_LIBRARY_PATH
    NAMES seabreeze
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib64 lib
    # Help the user find it if we cannot.
    DOC "The ${SEABREEZE_INCLUDE_PATH_DESCRIPTION}"
  )

  SET_LIBRARIES_PATH(SEABREEZE SEABREEZE)
  HURRICANE_CHECK_LIBRARIES(SEABREEZE)

ENDIF(UNIX)

