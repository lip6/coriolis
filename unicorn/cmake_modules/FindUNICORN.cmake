# - Find the Unicorn includes and libraries.
# The following variables are set if Coriolis is found.  If UNICORN is not
# found, UNICORN_FOUND is set to false.
#  UNICORN_FOUND       - True when the Coriolis include directory is found.
#  UNICORN_INCLUDE_DIR - the path to where the Coriolis include files are.
#  UNICORN_LIBRARIES   - The path to where the Coriolis library files are.


SET(UNICORN_INCLUDE_PATH_DESCRIPTION "directory containing the Unicorn include files. E.g /usr/local/include/coriolis or /asim/coriolis/include/coriolis")

SET(UNICORN_DIR_MESSAGE "Set the UNICORN_INCLUDE_DIR cmake cache entry to the ${UNICORN_INCLUDE_PATH_DESCRIPTION}")

# don't even bother under WIN32
IF(UNIX)

  SET(UNICORN_DIR_SEARCH $ENV{CORIOLIS_TOP} $ENV{HURRICANE_TOP})
  #
  # Look for an installation.
  #
  FIND_PATH(UNICORN_INCLUDE_PATH NAMES unicorn/UnicornEngine.h PATHS
    # Look in other places.
    ${UNICORN_DIR_SEARCH}
    PATH_SUFFIXES include/coriolis
    # Help the user find it if we cannot.
    DOC "The ${UNICORN_INCLUDE_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(UNICORN_LIBRARY_PATH
    NAMES unicorn
    PATHS ${UNICORN_DIR_SEARCH}
    PATH_SUFFIXES lib
    # Help the user find it if we cannot.
    DOC "The ${UNICORN_INCLUDE_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(UNICORN_STATIC_LIBRARY_PATH
    NAMES unicorn-static
    PATHS ${UNICORN_DIR_SEARCH}
    PATH_SUFFIXES lib
    # Help the user find it if we cannot.
    DOC "The ${UNICORN_INCLUDE_PATH_DESCRIPTION}"
  )

  SET_LIBRARIES_PATH(UNICORN UNICORN)
  HURRICANE_CHECK_LIBRARIES(UNICORN)

ENDIF(UNIX)
