# - Find the FLUTE includes and libraries.
# The following variables are set if Coriolis is found.  If FLUTE is not
# found, FLUTE_FOUND is set to false.
#  FLUTE_FOUND        - True when the Coriolis include directory is found.
#  FLUTE_INCLUDE_DIR - the path to where the Coriolis include files are.
#  FLUTE_LIBRARIES - The path to where the Coriolis library files are.


set( FLUTE_INCLUDE_PATH_DESCRIPTION "The directory containing the FLUTE include files. E.g /usr/local/include/coriolis2/flute/3.1 or /soc/coriolis2/include/coriolis2/flute/3.1" )
set( FLUTE_DIR_MESSAGE              "Set the FLUTE_INCLUDE_DIR cmake cache entry to the ${FLUTE_INCLUDE_PATH_DESCRIPTION}" )

if( UNIX )
  #
  # Look for an installation.
  #
  find_path( FLUTE_INCLUDE_PATH NAMES         flute.h
                                PATHS         ${CORIOLIS_DIR_SEARCH}
                                PATH_SUFFIXES include/flute/3.1
                                DOC "${FLUTE_INCLUDE_PATH_DESCRIPTION}"
  )

  find_library( FLUTE_LIBRARY_PATH NAMES         flute
                                   PATHS         ${CORIOLIS_DIR_SEARCH}
                                   PATH_SUFFIXES lib64 lib
                                   DOC           "${FLUTE_INCLUDE_PATH_DESCRIPTION}"
  )

  set_libraries_path       (FLUTE FLUTE)
  hurricane_check_libraries(FLUTE)

endif( UNIX )
