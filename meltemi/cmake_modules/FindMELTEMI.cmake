# - Find the Meltemi includes and libraries.
# The following variables are set if Coriolis is found.  If MELTEMI is not
# found, MELTEMI_FOUND is set to false.
#  MELTEMI_FOUND       - True when the Coriolis include directory is found.
#  MELTEMI_INCLUDE_DIR - the path to where the Coriolis include files are.
#  MELTEMI_LIBRARIES   - The path to where the Coriolis library files are.

find_package(COLOQUINTE REQUIRED)

SET(MELTEMI_INCLUDE_PATH_DESCRIPTION "directory containing the Meltemi include files. E.g /usr/local/include/coriolis2 or /asim/coriolis/include/coriolis2")

SET(MELTEMI_DIR_MESSAGE "Set the MELTEMI_INCLUDE_DIR cmake cache entry to the ${MELTEMI_INCLUDE_PATH_DESCRIPTION}")

# don't even bother under WIN32
IF(UNIX)
  
  SETUP_SEARCH_DIR(CORIOLIS)
  #
  # Look for an installation.
  #
  FIND_PATH(MELTEMI_INCLUDE_PATH NAMES meltemi/MeltemiEngine.h PATHS
    # Look in other places.
    ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES include/coriolis2
    # Help the user find it if we cannot.
    DOC "The ${MELTEMI_INCLUDE_PATH_DESCRIPTION}"
  )
	SET(MELTEMI_INCLUDE_PATH "${MELTEMI_INCLUDE_PATH};${COLOQUINTE_INCLUDE_DIR}")
	MESSAGE("djaa${MELTEMI_INCLUDE_PATH};${COLOQUINTE_INCLUDE_DIR}djau")

  FIND_LIBRARY(MELTEMI_LIBRARY_PATH
    NAMES meltemi
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib64 lib
    # Help the user find it if we cannot.
    DOC "The ${MELTEMI_INCLUDE_PATH_DESCRIPTION}"
  )

  SET_LIBRARIES_PATH(MELTEMI MELTEMI)
  HURRICANE_CHECK_LIBRARIES(MELTEMI)

ENDIF(UNIX)
