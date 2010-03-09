# - Find the Equinox includes and libraries.
# The following variables are set if Coriolis is found.  If EQUINOX is not
# found, EQUINOX_FOUND is set to false.
#  EQUINOX_FOUND       - True when the Coriolis include directory is found.
#  EQUINOX_INCLUDE_DIR - the path to where the Coriolis include files are.
#  EQUINOX_LIBRARIES   - The path to where the Coriolis library files are.


SET(EQUINOX_INCLUDE_PATH_DESCRIPTION "directory containing the Equinox include files. E.g /usr/local/include/coriolis or /asim/coriolis/include/coriolis")

SET(EQUINOX_DIR_MESSAGE "Set the EQUINOX_INCLUDE_DIR cmake cache entry to the ${EQUINOX_INCLUDE_PATH_DESCRIPTION}")

# don't even bother under WIN32
IF(UNIX)

  SET(EQUINOX_DIR_SEARCH $ENV{CORIOLIS_TOP} $ENV{HURRICANE_TOP})
  #
  # Look for an installation.
  #
  FIND_PATH(EQUINOX_INCLUDE_PATH NAMES equinox/Equi.h PATHS
    # Look in other places.
    ${EQUINOX_DIR_SEARCH}
    PATH_SUFFIXES include/coriolis
    # Help the user find it if we cannot.
    DOC "The ${EQUINOX_INCLUDE_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(EQUINOX_LIBRARY_PATH
    NAMES equinox
    PATHS ${EQUINOX_DIR_SEARCH}
    PATH_SUFFIXES lib
    # Help the user find it if we cannot.
    DOC "The ${EQUINOX_INCLUDE_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(EQUINOX_STATIC_LIBRARY_PATH
    NAMES equinox-static
    PATHS ${EQUINOX_DIR_SEARCH}
    PATH_SUFFIXES lib
    # Help the user find it if we cannot.
    DOC "The ${EQUINOX_INCLUDE_PATH_DESCRIPTION}"
  )

  SET_LIBRARIES_PATH(EQUINOX EQUINOX)
  HURRICANE_CHECK_LIBRARIES(EQUINOX)

ENDIF(UNIX)

