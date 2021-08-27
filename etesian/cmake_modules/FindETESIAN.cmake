# - Find the Etesian includes and libraries.
# The following variables are set if Coriolis is found.  If ETESIAN is not
# found, ETESIAN_FOUND is set to false.
#  ETESIAN_FOUND       - True when the Coriolis include directory is found.
#  ETESIAN_INCLUDE_DIR - the path to where the Coriolis include files are.
#  ETESIAN_LIBRARIES   - The path to where the Coriolis library files are.

find_package(COLOQUINTE REQUIRED)

SET(ETESIAN_INCLUDE_PATH_DESCRIPTION "directory containing the Etesian include files. E.g /usr/local/include/coriolis2 or /asim/coriolis/include/coriolis2")

SET(ETESIAN_DIR_MESSAGE "Set the ETESIAN_INCLUDE_DIR cmake cache entry to the ${ETESIAN_INCLUDE_PATH_DESCRIPTION}")

# don't even bother under WIN32
IF(UNIX)
  
  SETUP_SEARCH_DIR(CORIOLIS)
  #
  # Look for an installation.
  #
  FIND_PATH(ETESIAN_INCLUDE_PATH NAMES etesian/EtesianEngine.h PATHS
    # Look in other places.
    ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES include/coriolis2
    # Help the user find it if we cannot.
    DOC "The ${ETESIAN_INCLUDE_PATH_DESCRIPTION}"
  )
	SET(ETESIAN_INCLUDE_PATH "${ETESIAN_INCLUDE_PATH};${COLOQUINTE_INCLUDE_DIR}")
	MESSAGE("djaa${ETESIAN_INCLUDE_PATH};${COLOQUINTE_INCLUDE_DIR}djau")

  FIND_LIBRARY(ETESIAN_LIBRARY_PATH
    NAMES etesian
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib64 lib
    # Help the user find it if we cannot.
    DOC "The ${ETESIAN_INCLUDE_PATH_DESCRIPTION}"
  )

  SET_LIBRARIES_PATH(ETESIAN ETESIAN)
  HURRICANE_CHECK_LIBRARIES(ETESIAN)

ENDIF(UNIX)
