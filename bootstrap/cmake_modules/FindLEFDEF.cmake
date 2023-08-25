# - Find the LEF/DEF includes and libraries.
# The following variables are set if LEF/DEF is found.  If LEF/DEF is not
# found, LEFDEF_FOUND is set to false.
#  LEFDEF_FOUND       - True when LEF/DEF is found.
#  LEFDEF_INCLUDE_DIR - the path to where the LEF/DEF include files are.
#  LEFDEF_LIBRARIES   - The path to where the LEF/DEF library files are.
#
#  The DEF C API library:           LEFDEF_CDEF_LIBRARY
#                                   LEFDEF_CDEF_LIBRARY_RELEASE
#
#  The DEF C API library (zlib) :   LEFDEF_ZCDEF_LIBRARY
#                                   LEFDEF_ZCDEF_LIBRARY_RELEASE
#
#  The DEF C++ API library:         LEFDEF_DEF_LIBRARY
#                                   LEFDEF_DEF_LIBRARY_RELEASE
#
#  The DEF C++ API library (zlib) : LEFDEF_ZDEF_LIBRARY
#                                   LEFDEF_ZDEF_LIBRARY_RELEASE
#
#  The LEF C API library:           LEFDEF_CLEF_LIBRARY
#                                   LEFDEF_CLEF_LIBRARY_RELEASE
#
#  The LEF C API library (zlib) :   LEFDEF_ZCLEF_LIBRARY
#                                   LEFDEF_ZCLEF_LIBRARY_RELEASE
#
#  The LEF C++ API library:         LEFDEF_LEF_LIBRARY
#                                   LEFDEF_LEF_LIBRARY_RELEASE
#
#  The LEF C++ API library (zlib) : LEFDEF_ZLEF_LIBRARY
#                                   LEFDEF_ZLEF_LIBRARY_RELEASE

  SETUP_SEARCH_DIR(CORIOLIS)

 macro( _find_lefdef_lib varname libname )
   find_library( LEFDEF_${varname}_LIBRARY_RELEASE NAMES ${libname}       PATHS ${LEFDEF_LIBRARY_DIR} )
 
   if( LEFDEF_${varname}_LIBRARY_RELEASE )
     set( LEFDEF_${varname}_LIBRARY_DEBUG ${LEFDEF_${varname}_LIBRARY_RELEASE} CACHE STRING "Path to a library" FORCE )
   endif()
 
   if( LEFDEF_${varname}_LIBRARY_RELEASE )
     list( APPEND LEFDEF_LIBRARIES "optimized" ${LEFDEF_${varname}_LIBRARY_RELEASE}
                                   "debug"     ${LEFDEF_${varname}_LIBRARY_DEBUG} )
     set( LEFDEF_${varname}_LIBRARY_FOUND 1 )
     mark_as_advanced( LEFDEF_${varname}_LIBRARY_RELEASE
                       LEFDEF_${varname}_LIBRARY_DEBUG )
   else()
     set ( LEFDEF_FOUND "NO" )
   endif()
 endmacro()


 set( LEFDEF_INCLUDE_DIR_DESCRIPTION "directory containing the LEF/DEF include files. E.g /opt/lefdef-5.6/include" )
 set( LEFDEF_LIBRARY_DIR_DESCRIPTION "directory containing the LEF/DEF library files. E.g /opt/lefdef-5.6/lib" )
 set( LEFDEF_DIR_MESSAGE             "Set the LEFDEF_INCLUDE_DIR cmake cache entry to the ${LEFDEF_INCLUDE_DIR_DESCRIPTION}" )


# Don't even bother under Win32
 if( UNIX )
   set( LEFDEF_FOUND "YES" )
   set( LEFDEF_LIBRARIES           "" )
   set( LEFDEF_LIBRARY_SEARCH_PATH "" )
 
   find_path( LEFDEF_INCLUDE_DIR NAMES "defiDefs.hpp"
                                 PATHS ${LEFDEF_SEARCH_PATH}
                                 PATH_SUFFIXES "include/lefdef" "include"
                                 DOC "The ${LEFDEF_INCLUDE_DIR_DESCRIPTION}" )
   message( STATUS "LEFDEF_INCLUDE_DIR: ${LEFDEF_INCLUDE_DIR}" )
 
  FIND_LIBRARY( LEFDEF_LIBRARY_DIR
    NAMES def
    PATHS ${CORIOLIS_DIR_SEARCH}
    PATH_SUFFIXES lib64 lib
    # Help the user find it if we cannot.
    DOC "${LEFDEF_LIBRARY_DIR_DESCRIPTION}"
  )

   message( STATUS "LEFDEF_LIBRARY_DIR: ${LEFDEF_LIBRARY_DIR}" )
 
   if( LEFDEF_INCLUDE_DIR AND LEFDEF_LIBRARY_DIR )
    #_find_lefdef_lib(  "CDEF" "cdef"     )
    #_find_lefdef_lib( "ZCDEF" "cdefzlib" )
     _find_lefdef_lib(   "DEF"  "def"     )
     _find_lefdef_lib(  "ZDEF"  "defzlib" )
    #_find_lefdef_lib(  "CLEF" "clef"     )
    #_find_lefdef_lib( "ZCLEF" "clefzlib" )
     _find_lefdef_lib(   "LEF"  "lef"     )
     _find_lefdef_lib(  "ZLEF"  "lefzlib" )
   else()
     set( LEFDEF_FOUND "NO" )
   endif()
 
   if( LEFDEF_FOUND )
     add_definitions( -DHAVE_LEFDEF )
     if( NOT LEFDEF_FIND_QUIETLY )
       message( STATUS "Found LEF/DEF" )
     endif()
   else( LEFDEF_FOUND )
     set( LEFDEF_LIBRARIES   "" )
     set( LEFDEF_INCLUDE_DIR "" )
     set( LEFDEF_LIBRARY_DIR "" )
     if( LEFDEF_FIND_REQUIRED )
       message( STATUS "LEF/DEF libraries and/or includes NOT found!" )
     endif()
   endif( LEFDEF_FOUND )
 
   mark_as_advanced( LEFDEF_INCLUDE_DIR LEFDEF_LIBRARY_DIR LEFDEF_LIBRARIES )
 endif( UNIX )
