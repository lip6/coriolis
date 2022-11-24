# - Find the Katabatic includes and libraries.
# The following variables are set if Coriolis is found.  If KATABATIC is not
# found, KATABATIC_FOUND is set to false.
#  KATABATIC_FOUND       - True when the Coriolis include directory is found.
#  KATABATIC_INCLUDE_DIR - the path to where the Coriolis include files are.
#  KATABATIC_LIBRARIES   - The path to where the Coriolis library files are.


SET(KATABATIC_INCLUDE_PATH_DESCRIPTION "directory containing the Katabatic include files. E.g /usr/local/include/coriolis2 or /asim/coriolis/include/coriolis2")

SET(KATABATIC_DIR_MESSAGE "Set the KATABATIC_INCLUDE_DIR cmake cache entry to the ${KATABATIC_INCLUDE_PATH_DESCRIPTION}")

# don't even bother under WIN32
IF(UNIX)
  #
  # Look for an installation.
  #
  FIND_PATH(KATABATIC_INCLUDE_PATH NAMES katabatic/KatabaticEngine.h PATHS
    # Look in other places.
    ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES include/coriolis2
    # Help the user find it if we cannot.
    DOC "The ${KATABATIC_INCLUDE_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(KATABATIC_LIBRARY_PATH
    NAMES katabatic
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib64 lib
    # Help the user find it if we cannot.
    DOC "The ${KATABATIC_INCLUDE_PATH_DESCRIPTION}"
  )

  SET_LIBRARIES_PATH(KATABATIC KATABATIC)
  HURRICANE_CHECK_LIBRARIES(KATABATIC)

ENDIF(UNIX)
