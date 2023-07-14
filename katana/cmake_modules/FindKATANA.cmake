# - Find the Katana includes and libraries.
# The following variables are set if Coriolis is found.  If KATANA is not
# found, KATANA_FOUND is set to false.
#  KATANA_FOUND       - True when the Coriolis include directory is found.
#  KATANA_INCLUDE_DIR - the path to where the Coriolis include files are.
#  KATANA_LIBRARIES   - The path to where the Coriolis library files are.


SET(KATANA_INCLUDE_PATH_DESCRIPTION "directory containing the Katana include files. E.g /usr/local/include/coriolis2 or /asim/coriolis/include/coriolis2")

SET(KATANA_DIR_MESSAGE "Set the KATANA_INCLUDE_DIR cmake cache entry to the ${KATANA_INCLUDE_PATH_DESCRIPTION}")

# don't even bother under WIN32
IF(UNIX)
  #
  # Look for an installation.
  #
  FIND_PATH(KATANA_INCLUDE_PATH NAMES katana/KatanaEngine.h PATHS
    # Look in other places.
    ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES include/coriolis2
    # Help the user find it if we cannot.
    DOC "The ${KATANA_INCLUDE_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(KATANA_LIBRARY_PATH
    NAMES katana
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib64 lib
    # Help the user find it if we cannot.
    DOC "The ${KATANA_INCLUDE_PATH_DESCRIPTION}"
  )

  SET_LIBRARIES_PATH(KATANA KATANA)
  HURRICANE_CHECK_LIBRARIES(KATANA)

ENDIF(UNIX)
