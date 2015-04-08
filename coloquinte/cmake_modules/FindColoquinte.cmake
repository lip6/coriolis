# - Find the Coloquinte includes and libraries.
# The following variables are set if Coriolis is found.  If COLOQUINTE is not
# found, COLOQUINTE_FOUND is set to false.
#  COLOQUINTE_FOUND       - True when the Coriolis include directory is found.
#  COLOQUINTE_INCLUDE_DIR - the path to where the Coriolis include files are.
#  COLOQUINTE_LIBRARIES   - The path to where the Coriolis library files are.


# =============================================================================
# Duplicated from <bootstrap> as <COLOQUINTE> is standalone.
#
# Setup the <PROJECT>_SEARCH_PATH.
#   Where to find includes & libraries.
#
 macro(setup_search_dir project)
   if( NOT("$ENV{${project}_TOP}" STREQUAL "") )
     message("-- ${project}_TOP is set to $ENV{${project}_TOP}")
     list(INSERT ${project}_DIR_SEARCH 0 "${DESTDIR}$ENV{${project}_TOP}")
   endif( NOT("$ENV{${project}_TOP}" STREQUAL "") )

   if( NOT("$ENV{${project}_USER_TOP}" STREQUAL "") )
     message("-- ${project}_USER_TOP is set to $ENV{${project}_USER_TOP}")
     list(INSERT ${project}_DIR_SEARCH 0 "${DESTDIR}$ENV{${project}_USER_TOP}")
   endif( NOT("$ENV{${project}_USER_TOP}" STREQUAL "") )

   LIST(REMOVE_DUPLICATES ${project}_DIR_SEARCH)
  
   message("-- Components of ${project}_DIR_SEARCH:")
   foreach(PATH ${${project}_DIR_SEARCH})
     message("--   ${PATH}")
   endforeach(PATH)
 endmacro(setup_search_dir project)


#
# Build <PROJECT>_INCLUDE_DIR & <PROJECT>_LIBRARIES and sets up <PROJECT>_FOUND
# Usage:  set_library_path(<PROJECT> <library>)
#
# May be used any number of time on the same <PROJECT> to create a list of
# <library>.
#
 macro(set_libraries_path configname library)
   set(${configname}_FOUND "NOTFOUND")
 
   if(${library}_LIBRARY_PATH)
     set(${configname}_FOUND "YES")
     set(${configname}_INCLUDE_DIR ${${library}_INCLUDE_PATH})
     set(${configname}_LIBRARIES ${${library}_LIBRARY_PATH} ${${configname}_LIBRARIES})
     mark_as_advanced(${configname}_INCLUDE_DIR ${configname}_LIBRARIES)
   endif(${library}_LIBRARY_PATH)
 
   if(NOT ${library}_INCLUDE_PATH)
     set(${configname}_FOUND "NOTFOUND")
   endif(NOT ${library}_INCLUDE_PATH)
 endmacro ( set_libraries_path )


#
# Checks if a set of libraries has been found, could be blocking or not.
# Usage: hurricane_check_libraries(<PROJECT> <REQUIRED>)
#
# If <REQUIRED> is ommitted, it is guessed from <PROJECT>_FIND_REQUIRED.
#
 macro(hurricane_check_libraries)
   if(ARGC LESS 2)
     set(REQUIRED ${ARGV0}_FIND_REQUIRED)
   else(ARGC LESS 2)
     set(REQUIRED ${ARGV1})
   endif(ARGC LESS 2)
   if(${ARGV0}_FOUND)
     if(NOT ${ARGV0}_FIND_QUIETLY)
       if(${ARGV0}_FOUND)
         message(STATUS "Found ${ARGV0}:")
         foreach(library ${${ARGV0}_LIBRARIES})
           message(STATUS "  ${library}")
         endforeach(library)
       endif(${ARGV0}_FOUND)
     endif(NOT ${ARGV0}_FIND_QUIETLY)
   else(${ARGV0}_FOUND)
     if(REQUIRED)
       message(FATAL_ERROR "${ARGV0} was not found. ${${ARGV0}_DIR_MESSAGE}")
     endif(REQUIRED)
   endif(${ARGV0}_FOUND)
 endmacro(hurricane_check_libraries)

# End of <bootstrap> duplication.
# =============================================================================

#find_package(Eigen3 REQUIRED)

 set(COLOQUINTE_INCLUDE_PATH_DESCRIPTION "directory containing the Coloquinte include files. E.g /usr/local/include/coloquinte or /soc/coriolis/include/Coloquinte")
 set(COLOQUINTE_DIR_MESSAGE "Set the COLOQUINTE_INCLUDE_DIR cmake cache entry to the ${COLOQUINTE_INCLUDE_PATH_DESCRIPTION}")
 
 # don't even bother under WIN32
 if(UNIX)
   setup_search_dir(IMPORTEDS)
   message(STATUS "IMPORTEDS_DIR_SEARCH: ${IMPORTEDS_DIR_SEARCH}")
   #
   # Look for an installation.
   #
   find_path(COLOQUINTE_INCLUDE_PATH NAMES coloquinte/circuit.hxx PATHS
     # Look in other places.
     ${IMPORTEDS_DIR_SEARCH}
     PATH_SUFFIXES include
     # Help the user find it if we cannot.
     DOC "The ${COLOQUINTE_INCLUDE_PATH_DESCRIPTION}"
   )
 
   find_library(COLOQUINTE_LIBRARY_PATH
     NAMES coloquinte
     PATHS ${IMPORTEDS_DIR_SEARCH}
     PATH_SUFFIXES lib${LIB_SUFFIX}
     # Help the user find it if we cannot.
     DOC "The ${COLOQUINTE_INCLUDE_PATH_DESCRIPTION}"
   )
 
   set(COLOQUINTE_FIND_QUIETLY FALSE)

   set_libraries_path(COLOQUINTE COLOQUINTE)
   hurricane_check_libraries(COLOQUINTE)

   if(COLOQUINTE_FOUND)
     add_definitions( -DHAVE_COLOQUINTE )
   endif()

   message(STATUS "COLOQUINTE_INCLUDE_PATH: ${COLOQUINTE_INCLUDE_PATH}")
   message(STATUS "COLOQUINTE_LIBRARY: ${COLOQUINTE_LIBRARY}")
 
 endif(UNIX)
