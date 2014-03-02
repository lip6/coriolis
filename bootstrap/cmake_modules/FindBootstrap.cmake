

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
# Check for the Distribution: RedHat/Scientific/Ubuntu/OSX.
# Currently OSX is assimilated to RedHat.
#
 macro(check_distribution)
   if(NOT APPLE)
     execute_process(COMMAND "lsb_release" "-d" OUTPUT_VARIABLE LSB_RELEASE)
     if(LSB_RELEASE MATCHES "[Uu]buntu|[Dd]ebian")
       set(DISTRIBUTION "Debian" CACHE STRING "Distribution Type/Vendor" FORCE)
       add_definitions(-DDEBIAN)
     endif(LSB_RELEASE MATCHES "[Uu]buntu|[Dd]ebian")
     if(LSB_RELEASE MATCHES "Scientific|RedHat")
       set(DISTRIBUTION "RedHat" CACHE STRING "Distribution Type/Vendor" FORCE)
       add_definitions(-DREDHAT)
     endif(LSB_RELEASE MATCHES "Scientific|RedHat")
   else(NOT APPLE)
     set(DISTRIBUTION "RedHat" CACHE STRING "Distribution Type/Vendor" FORCE)
     add_definitions(-DREDHAT)
   endif(NOT APPLE)
   message("-- Distribution is ${DISTRIBUTION}")
 endmacro(check_distribution)

#
# Get the svn revision version and configure a svn.h.in file based on this version
# The include directory name is passed as argument
#
 macro(SETUP_SVNH includedir)
   find_package(Subversion REQUIRED)
   Subversion_WC_INFO(${CMAKE_CURRENT_SOURCE_DIR} SVNREV)
   set(SUBVERSION_REVISION ${SVNREV_WC_REVISION})
   configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/svn.h.in ${CMAKE_CURRENT_SOURCE_DIR}/${includedir}/svn.h)
 endmacro(SETUP_SVNH)


#
# Specific Apple OSX setup
#
#  This is no more needed since QT 4.6 is available for Mac OSX 10.6
# macro(setup_apple)
#   if(APPLE)
#     execute_process(
#       COMMAND sw_vers -productVersion
#       OUTPUT_VARIABLE OSX_VERSION
#       OUTPUT_STRIP_TRAILING_WHITESPACE)
#     message(STATUS "OSX_VERSION='${OSX_VERSION}'")
#     if(${OSX_VERSION} MATCHES "^10\\.[012345]\\.?")
#       message(STATUS "OSX < 10.6")
#     else(${OSX_VERSION} MATCHES "^10\\.[012345]\\.?")
#       set(CMAKE_OSX_ARCHITECTURES "i386;ppc") # for QT4.5 32bits on snow leopard
#     endif(${OSX_VERSION} MATCHES "^10\\.[012345]\\.?")
#   endif(APPLE)
# endmacro(setup_apple)



#
# Adds -Wall to the C/C++ flags.
#
 set(CMAKE_C_FLAGS_DEBUG                "-Wall -g"           CACHE STRING "C Compiler Debug options."   FORCE)
 set(CMAKE_C_FLAGS_RELEASE              "-Wall -O2 -DNDEBUG" CACHE STRING "C Compiler Release options." FORCE)
 set(CMAKE_CXX_FLAGS_DEBUG   "-std=c++0x -Wall -g"           CACHE STRING "C++ Compiler Debug options."   FORCE)
 set(CMAKE_CXX_FLAGS_RELEASE "-std=c++0x -Wall -O2 -DNDEBUG" CACHE STRING "C++ Compiler Release options." FORCE)


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
   foreach(PATH ${CMAKE_MODULE_PATH})
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
   foreach(LIBRARY ${Boost_LIBRARIES})
     message(STATUS "  ${LIBRARY}")
   endforeach(LIBRARY)
   add_definitions(-DBOOST_FILESYSTEM_VERSION=2) # for boost 1.46 and more we need to specify to use filesystem v2 and not v3
 endmacro(setup_boost)


#
# Guess the value of SYS_CONF_DIR according to INSTALLDIR.
# if INSTALLDIR is either /usr or /opt, we uses the system-wide /etc,
# otherwise we install under the tool INSTALLDIR/etc.
#
 macro(setup_sysconfdir INSTALLDIR)
   message("-- Checking installation directory <${INSTALLDIR}>")
   string(REGEX MATCH "^/usr" IS_USR ${INSTALLDIR})
   string(REGEX MATCH "^/opt" IS_OPT ${INSTALLDIR})
   if(IS_USR OR IS_OPT)
     message("-- Using system-wide /etc.")
     set(SYS_CONF_DIR "/etc" CACHE STRING "System configuration directory (/etc)" FORCE)
   else(IS_USR OR IS_OPT)
     message("-- Using install tree <prefix>/etc.")
     set(SYS_CONF_DIR "etc" CACHE STRING "System configuration directory (/etc)" FORCE)
   endif(IS_USR OR IS_OPT)
 endmacro(setup_sysconfdir)
 

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
# Setup the <PROJECT>_FOUND.
#   Set to TRUE if both includes & libraries have been found.
#
 macro(set_found project)
   if(${project}_INCLUDE_DIR AND ${project}_LIBRARY)
     set(${project}_FOUND TRUE)
   else(${project}_INCLUDE_DIR AND ${project}_LIBRARY)
     set(${project}_FOUND FALSE)
   endif(${project}_INCLUDE_DIR AND ${project}_LIBRARY)
 endmacro(set_found project)
