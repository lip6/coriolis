# - Find the Knik includes and libraries.
# The following variables are set if Coriolis is found.  If KNIK is not
# found, KNIK_FOUND is set to false.
#  KNIK_FOUND        - True when the Coriolis include directory is found.
#  KNIK_INCLUDE_DIR - the path to where the Coriolis include files are.
#  KNIK_LIBRARIES - The path to where the Coriolis library files are.


SET(KNIK_INCLUDE_PATH_DESCRIPTION "directory containing the Knik/flute include files. E.g /usr/local/include/coriolis or /asim/coriolis/include/coriolis")

SET(KNIK_DIR_MESSAGE "Set the KNIK_INCLUDE_DIR cmake cache entry to the ${KNIK_INCLUDE_PATH_DESCRIPTION}")

# don't even bother under WIN32
IF(UNIX)
  #
  # Look for an installation.
  #
  FIND_PATH(KNIK_INCLUDE_PATH NAMES knik/KnikEngine.h PATHS
    # Look in other places.
    ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES include/coriolis2
    # Help the user find it if we cannot.
    DOC "The ${KNIK_INCLUDE_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(KNIK_LIBRARY_PATH
    NAMES knik
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib64 lib
    # Help the user find it if we cannot.
    DOC "The ${KNIK_INCLUDE_PATH_DESCRIPTION}"
  )

  FIND_LIBRARY(FLUTE_LIBRARY_PATH
    NAMES flute
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib64 lib
    # Help the user find it if we cannot.
    DOC "The ${KNIK_INCLUDE_PATH_DESCRIPTION}"
  )

  SET_LIBRARIES_PATH(KNIK FLUTE)
  SET_LIBRARIES_PATH(KNIK KNIK)
  HURRICANE_CHECK_LIBRARIES(KNIK)

ENDIF(UNIX)
