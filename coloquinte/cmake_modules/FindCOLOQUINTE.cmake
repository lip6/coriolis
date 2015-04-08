# - Find the Coloquinte includes and libraries.
# The following variables are set if Coriolis is found.  If COLOQUINTE is not
# found, COLOQUINTE_FOUND is set to false.
#  COLOQUINTE_FOUND       - True when the Coriolis include directory is found.
#  COLOQUINTE_INCLUDE_DIR - the path to where the Coriolis include files are.
#  COLOQUINTE_LIBRARIES   - The path to where the Coriolis library files are.


SET(COLOQUINTE_INCLUDE_PATH_DESCRIPTION "directory containing the Coloquinte include files. E.g /usr/local/include/coriolis or /asim/coriolis/include/coriolis")

SET(COLOQUINTE_DIR_MESSAGE "Set the COLOQUINTE_INCLUDE_DIR cmake cache entry to the ${COLOQUINTE_INCLUDE_PATH_DESCRIPTION}")

# don't even bother under WIN32
IF(UNIX)
  #
  # Look for an installation.
  #
  FIND_PATH(COLOQUINTE_INCLUDE_PATH NAMES coloquinte/netlist.hxx PATHS
    # Look in other places.
    ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES include/coriolis2
    # Help the user find it if we cannot.
    DOC "The ${COLOQUINTE_INCLUDE_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(COLOQUINTE_LIBRARY_PATH
    NAMES coloquinte
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib${LIB_SUFFIX}
    # Help the user find it if we cannot.
    DOC "The ${COLOQUINTE_INCLUDE_PATH_DESCRIPTION}"
  )

  SET_LIBRARIES_PATH(COLOQUINTE COLOQUINTE)
  HURRICANE_CHECK_LIBRARIES(COLOQUINTE)

ENDIF(UNIX)
