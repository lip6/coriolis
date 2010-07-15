

#
# Setup CMake policies.
#
 macro(set_cmake_policies)
   if(COMMAND CMAKE_POLICY)
     cmake_policy(SET CMP0003 NEW)
     cmake_policy(SET CMP0005 NEW)
     if(NOT (CMAKE_VERSION VERSION_LESS 2.8.0))
       cmake_policy(SET CMP0014 OLD)
     endif(NOT (CMAKE_VERSION VERSION_LESS 2.8.0))
   endif(COMMAND CMAKE_POLICY)
 endmacro(set_cmake_policies)


#
# Specific Apple OSX setup
#
 macro(setup_apple)
   if(APPLE)
     execute_process(
       COMMAND sw_vers -productVersion
       OUTPUT_VARIABLE OSX_VERSION
       OUTPUT_STRIP_TRAILING_WHITESPACE)
     message(STATUS "OSX_VERSION='${OSX_VERSION}'")
     if(${OSX_VERSION} MATCHES "^10\\.[012345]\\.?")
       message(STATUS "OSX < 10.6")
     else(${OSX_VERSION} MATCHES "^10\\.[012345]\\.?")
       set(CMAKE_OSX_ARCHITECTURES "i386;ppc") # for QT4.5 32bits on snow leopard
     endif(${OSX_VERSION} MATCHES "^10\\.[012345]\\.?")
   endif(APPLE)
 endmacro(setup_apple)



#
# Adds -Wall to the C/C++ flags.
#
 set(CMAKE_C_FLAGS_DEBUG     "-g -Wall"           CACHE STRING "Debug options."   FORCE)
 set(CMAKE_CXX_FLAGS_DEBUG   "-g -Wall"           CACHE STRING "Debug options."   FORCE)
 set(CMAKE_C_FLAGS_RELEASE   "-O3 -DNDEBUG -Wall" CACHE STRING "Release options." FORCE)
 set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -Wall" CACHE STRING "Release options." FORCE)


#
# Adds to the CMAKE_MODULE_PATH directories guesseds from project
# environment variables <PROJECT>_USER_TOP and <PROJECT>_TOP.
#
 macro(SETUP_PROJECT_PATHS project)
   if( NOT("$ENV{${project}_TOP}" STREQUAL "") )
     message("-- ${project}_TOP is set to $ENV{${project}_TOP}")
     set(PROJECT_MODULE_PATH "${DESTDIR}$ENV{${project}_TOP}/share/cmake/Modules/")
     list(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_MODULE_PATH}")
   endif( NOT("$ENV{${project}_TOP}" STREQUAL "") )
 
   if( NOT("$ENV{${project}_USER_TOP}" STREQUAL "") )
     message("-- ${project}_USER_TOP is set to $ENV{${project}_USER_TOP}")
     set(PROJECT_MODULE_PATH "${DESTDIR}$ENV{${project}_USER_TOP}/share/cmake/Modules/")
     list(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_MODULE_PATH}")
   endif( NOT("$ENV{${project}_USER_TOP}" STREQUAL "") )
 
   list(REMOVE_DUPLICATES CMAKE_MODULE_PATH)
 endmacro(SETUP_PROJECT_PATHS project)


#
# Displays the contents of CMAKE_MODULE_PATH.
#
 macro(print_cmake_module_path)
   message("-- Components of CMAKE_MODULE_PATH:")
   foreach(PATH IN LISTS CMAKE_MODULE_PATH)
     message("--   ${PATH}")
   endforeach(PATH)
 endmacro(print_cmake_module_path)


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
         message(STATUS "Found ${ARGV0} : ${${ARGV0}_LIBRARIES}")
       endif(${ARGV0}_FOUND)
     endif(NOT ${ARGV0}_FIND_QUIETLY)
   else(${ARGV0}_FOUND)
     if(REQUIRED)
       message(FATAL_ERROR "${ARGV0} was not found. ${${ARGV0}_DIR_MESSAGE}")
     endif(REQUIRED)
   endif(${ARGV0}_FOUND)
 endmacro(hurricane_check_libraries)


#
# Perform some tweaks on shared/static linking.
#
 macro(set_lib_link_mode)
   if(NOT BUILD_SHARED_LIBS)
   # check for qmake
     find_program(QT_QMAKE_EXECUTABLE NAMES qmake-qt4 qmake PATHS
       /opt/qt4-static-4.3.2/bin
       NO_DEFAULT_PATH
       )
     message(STATUS "Building static libraries.")
   else(NOT BUILD_SHARED_LIBS)
     message(STATUS "Building dynamic libraries.")
   endif(NOT BUILD_SHARED_LIBS)
 endmacro(set_lib_link_mode)


#
# Find Boost, checking different versions.
#
 macro(setup_boost)
  #set(Boost_USE_STATIC_LIBS ON)
  #message(STATUS "Always uses Boost static libraries.")
   if(ARGC LESS 1)
     find_package(Boost 1.33.1 REQUIRED)
   else(ARGC LESS 1)
     find_package(Boost 1.35.0 COMPONENTS ${ARGV} system)
     if(NOT Boost_FOUND)
       find_package(Boost 1.33.1 COMPONENTS ${ARGV} REQUIRED)
     endif(NOT Boost_FOUND)
   endif(ARGC LESS 1)
   message(STATUS "Found Boost libraries ${Boost_LIB_VERSION} in ${Boost_INCLUDE_DIR}")
   foreach(LIBRARY IN LISTS Boost_LIBRARIES)
     message(STATUS "  ${LIBRARY}")
   endforeach(LIBRARY)
 endmacro(setup_boost)


#
# Guess the value of SYS_CONF_DIR according to INSTALLDIR.
# if INSTALLDIR is either /usr or /opt, we uses the system-wide /etc,
# otherwise we install under the tool INSTALLDIR/etc.
#
 macro(setup_sysconfdir INSTALLDIR)
   string(REGEX MATCH "^/usr" IS_USR INSTALLDIR)
   string(REGEX MATCH "^/opt" IS_OPT INSTALLDIR)
   if(IS_USR OR IS_OPT)
     message("-- Using system-wide /etc.")
     set(SYS_CONF_DIR "/etc" CACHE STRING "System configuration directory (/etc)" FORCE)
   else(IS_USR OR IS_OPT)
     message("-- Using install tree /etc.")
     set(SYS_CONF_DIR "etc" CACHE STRING "System configuration directory (/etc)" FORCE)
   endif(IS_USR OR IS_OPT)
 endmacro(setup_sysconfdir)
 

#
# Setup the <PROJECT>_SEARCH_PATH.
#   Where to find includes & libraries.
#
 MACRO(SETUP_SEARCH_DIR project)
   IF( NOT("$ENV{${project}_TOP}" STREQUAL "") )
     MESSAGE("-- ${project}_TOP is set to $ENV{${project}_TOP}")
     LIST(INSERT ${project}_DIR_SEARCH 0 "${DESTDIR}$ENV{${project}_TOP}")
   ENDIF( NOT("$ENV{${project}_TOP}" STREQUAL "") )

   IF( NOT("$ENV{${project}_USER_TOP}" STREQUAL "") )
     MESSAGE("-- ${project}_USER_TOP is set to $ENV{${project}_USER_TOP}")
     LIST(INSERT ${project}_DIR_SEARCH 0 "${DESTDIR}$ENV{${project}_USER_TOP}")
   ENDIF( NOT("$ENV{${project}_USER_TOP}" STREQUAL "") )

   LIST(REMOVE_DUPLICATES ${project}_DIR_SEARCH)
  
   MESSAGE("-- Components of ${project}_DIR_SEARCH:")
   FOREACH(PATH IN LISTS ${project}_DIR_SEARCH)
     MESSAGE("--   ${PATH}")
   ENDFOREACH(PATH)
 ENDMACRO(SETUP_SEARCH_DIR project)
