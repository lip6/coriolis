# - Find the Nimbus includes and libraries.
# The following variables are set if Coriolis is found.  If NIMBUS is not
# found, NIMBUS_FOUND is set to false.
#  NIMBUS_FOUND        - True when the Coriolis include directory is found.
#  NIMBUS_INCLUDE_DIR - the path to where the Coriolis include files are.
#  NIMBUS_LIBRARIES - The path to where the Coriolis library files are.


SET(NIMBUS_INCLUDE_PATH_DESCRIPTION "directory containing the Nimbus include files. E.g /usr/local/include/coriolis or /asim/coriolis/include/coriolis")

SET(NIMBUS_DIR_MESSAGE "Set the NIMBUS_INCLUDE_DIR cmake cache entry to the ${NIMBUS_INCLUDE_PATH_DESCRIPTION}")

# don't even bother under WIN32
IF(UNIX)
  #
  # Look for an installation.
  #
  FIND_PATH(NIMBUS_INCLUDE_PATH NAMES nimbus/NimbusEngine.h PATHS
    # Look in other places.
    ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES include/coriolis2
    # Help the user find it if we cannot.
    DOC "The ${NIMBUS_INCLUDE_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(NIMBUS_LIBRARY_PATH
    NAMES nimbus
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib${LIB_SUFFIX}
    # Help the user find it if we cannot.
    DOC "The ${NIMBUS_INCLUDE_PATH_DESCRIPTION}"
  )

  SET_LIBRARIES_PATH(NIMBUS NIMBUS)
  HURRICANE_CHECK_LIBRARIES(NIMBUS)

ENDIF(UNIX)
