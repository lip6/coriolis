# - Try to find the IO libraries
# Once done this will define
#
#  IO_FOUND - system has the IO library
#  IO_INCLUDE_DIR - the IO include directory
#  IO_LIBRARIES - The libraries needed to use IO

SET(IO_DIR_SEARCH $ENV{IO_TOP})

FIND_PATH(IO_INCLUDE_DIR
          NAMES io/agds/GdsLibrary.h 
          PATHS ${IO_DIR_SEARCH}
          PATH_SUFFIXES include
)

FIND_LIBRARY(IO_AGDS_LIBRARY
             NAMES agds
             PATHS ${IO_DIR_SEARCH}
             PATH_SUFFIXES lib
)


IF(IO_INCLUDE_DIR AND IO_AGDS_LIBRARY)
   SET(IO_FOUND TRUE)
   SET(IO_LIBRARIES ${IO_AGDS_LIBRARY})
ELSE(IO_INCLUDE_DIR AND IO_AGDS_LIBRARY)
   SET(IO_FOUND FALSE)
   SET(IO_LIBRARIES)
ENDIF(IO_INCLUDE_DIR AND IO_AGDS_LIBRARY)

IF (NOT IO_FOUND)
    SET(IO_MESSAGE
    "IO library was not found. Make sure IO_TOP env variable is set.")
    IF (NOT IO_FIND_QUIETLY)
        MESSAGE(STATUS "${IO_MESSAGE}")
    ELSE(NOT IO_FIND_QUIETLY)
        IF(IO_FIND_REQUIRED)
            MESSAGE(FATAL_ERROR "${IO_MESSAGE}")
        ENDIF(IO_FIND_REQUIRED)
    ENDIF(NOT IO_FIND_QUIETLY)
ELSE (NOT IO_FOUND)
    MESSAGE(STATUS "IO library was found in ${IO_DIR_SEARCH}")
ENDIF (NOT IO_FOUND)

