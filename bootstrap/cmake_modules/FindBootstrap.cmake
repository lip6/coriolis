#
# Setup CMake policies.
#
 macro(set_cmake_policies)
   if(COMMAND CMAKE_POLICY)
     cmake_policy(SET CMP0003 NEW)
     cmake_policy(SET CMP0005 NEW)
     cmake_policy(SET CMP0079 NEW)
     cmake_policy(SET CMP0022 NEW)
     cmake_policy(SET CMP0060 NEW)
     cmake_policy(SET CMP0095 NEW)
    #if(NOT (CMAKE_VERSION VERSION_LESS 2.8.0))
    #  cmake_policy(SET CMP0014 OLD)
    #endif()
    #if(NOT (CMAKE_VERSION VERSION_LESS 2.8.12))
    #  cmake_policy(SET CMP0022 OLD)
    #endif()
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
 set(BUILD_SHARED_LIBS "ON")
#set(DEBUG_FLAGS "-g -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC")
 set(DEBUG_FLAGS "-g")
 set(CMAKE_CXX_STANDARD 17)
 if(CYGWIN)
   set(ADDITIONAL_FLAGS "-D_GLIBCXX_USE_C99")
   set(CXX_STANDARD "gnu++17")
 else()
   set(ADDTIONAL_FLAGS "")
   set(CXX_STANDARD "c++17")
 endif()

 set(LINK_UNRESOLVED "-Wl,--unresolved-symbols=ignore-all")
 if(APPLE)
   set(LINK_UNRESOLVED "-undefined dynamic_lookup")
 endif()
  

#set(CMAKE_C_FLAGS_DEBUG     "                     -Wall -fsanitize=address ${ADDTIONAL_FLAGS} ${DEBUG_FLAGS}" CACHE STRING "C Compiler Debug options."     FORCE)
 set(CMAKE_C_FLAGS_DEBUG     "                     -Wall                    ${ADDTIONAL_FLAGS} ${DEBUG_FLAGS}" CACHE STRING "C Compiler Debug options."     FORCE)
 set(CMAKE_C_FLAGS_RELEASE   "                     -Wall -O2                ${ADDTIONAL_FLAGS} -DNDEBUG"       CACHE STRING "C Compiler Release options."   FORCE)
#set(CMAKE_C_FLAGS_RELEASE   "                     -Wall -fsanitize=address ${ADDTIONAL_FLAGS} -DNDEBUG"       CACHE STRING "C Compiler Release options."   FORCE)
#set(CMAKE_CXX_FLAGS_DEBUG   "-std=${CXX_STANDARD} -Wall -fsanitize=address ${ADDTIONAL_FLAGS} ${DEBUG_FLAGS}" CACHE STRING "C++ Compiler Debug options."   FORCE)
 set(CMAKE_CXX_FLAGS_DEBUG   "-std=${CXX_STANDARD} -Wall                    ${ADDTIONAL_FLAGS} ${DEBUG_FLAGS}" CACHE STRING "C++ Compiler Debug options."   FORCE)
 set(CMAKE_CXX_FLAGS_RELEASE "-std=${CXX_STANDARD} -Wall -O2                ${ADDTIONAL_FLAGS} -DNDEBUG"       CACHE STRING "C++ Compiler Release options." FORCE)
#set(CMAKE_CXX_FLAGS_RELEASE "-std=${CXX_STANDARD} -Wall -fsanitize=address ${ADDTIONAL_FLAGS} -DNDEBUG"       CACHE STRING "C++ Compiler Release options." FORCE)


if ( NOT CMAKE_BUILD_TYPE )
    message( "Build Type not set, defaulting to Debug..." )
    set( CMAKE_BUILD_TYPE Debug )
endif()

#
# Adds to the CMAKE_MODULE_PATH directories guesseds from project
# environment variables <PROJECT>_USER_TOP and <PROJECT>_TOP.
#
 macro(setup_project_paths project)
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
 endmacro(setup_project_paths project)


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
#PYTHON_NEW  May be used any number of time on the same <PROJECT> to create a list of
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
   endif()
 endmacro()
#
# sets that a library is expected to have unresolved symbols
# Usage:  set_has_unresolved_symbols(<PROJECT>)
# 
# Should be used before set_libraries_path.
 macro(set_has_unresolved_symbols configname)
   if( TARGET ${configname} )
     set_target_properties( ${configname} PROPERTIES LINK_FLAGS ${LINK_UNRESOLVED} )
   else()
     set(${configname}_LIBRARIES ${LINK_UNRESOLVED} ${${configname}_LIBRARIES})
   endif()
 endmacro()

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


 macro(setup_boost)
  #set(Boost_USE_STATIC_LIBS ON)
  #message(STATUS "Always uses Boost static libraries.")
   if(ARGC LESS 1)
     find_package(Boost 1.35.0 REQUIRED)
   else(ARGC LESS 1)
     foreach(component ${ARGV})
       if(${component} STREQUAL "python")
         set(components ${components} ${component}39)
       else()
         set(components ${components} ${component})
       endif()
     endforeach()
     
     find_package(Boost 1.35.0 COMPONENTS ${components} system)
     if(NOT Boost_FOUND)
       find_package(Boost 1.35.0 COMPONENTS ${components} REQUIRED)
     endif(NOT Boost_FOUND)
   endif(ARGC LESS 1)
   message(STATUS "Found Boost includes ${Boost_LIB_VERSION} in ${Boost_INCLUDE_DIR}")
   message(STATUS "Found Boost libraries ${Boost_LIB_VERSION} in ${Boost_LIBRARY_DIRS}")
   foreach(LIBRARY ${Boost_LIBRARIES})
     message(STATUS "  ${LIBRARY}")
   endforeach(LIBRARY)
   add_definitions(-DBOOST_FILESYSTEM_VERSION=2) # for boost 1.46 and more we need to specify to use filesystem v2 and not v3
 endmacro(setup_boost)


#
# Setup Python, select detection depending on USE_MANYLINUX.
#
 macro(setup_python)
   set(pydevelArg "Development")
 
   if (USE_MANYLINUX)
     message(STATUS "Build for manylinux")
     set(pydevelArg "Development.Module")
   endif()
  find_package(Python 3 REQUIRED COMPONENTS Interpreter ${pydevelArg} )
 endmacro()


#
# Find Qt, the union of all the modules we need for the whole project.
#
 macro(setup_qt)
   if(WITH_QT4)
     message(STATUS "Attempt to find Qt 4...")
    # For Qt4.
    #set(QT_USE_QTXML "true")
     set(QT_USE_QTSVG "true")
     find_package(Qt4 REQUIRED)
     include(${QT_USE_FILE})
    # ${QT_QTSVG_LIBRARY}
     set(QtX_INCLUDE_DIRS ${QT_INCLUDE_DIR})
     set(QtX_LIBRARIES    ${QT_LIBRARIES})
   else()
     message(STATUS "Attempt to find Qt 5...")
    # For Qt5
     find_package(Qt5Core         REQUIRED)
     find_package(Qt5Gui          REQUIRED)
     find_package(Qt5Widgets      REQUIRED)
     find_package(Qt5Svg          REQUIRED)
     find_package(Qt5PrintSupport REQUIRED)
     set(CMAKE_AUTOMOC ON)
     set(QtX_INCLUDE_DIRS ${Qt5PrintSupport_INCLUDE_DIRS}
                          ${Qt5Widgets_INCLUDE_DIRS}
                          ${Qt5Svg_INCLUDE_DIRS}
                          ${Qt5Gui_INCLUDE_DIRS}
                          ${Qt5Core_INCLUDE_DIRS} )
     set(QtX_LIBRARIES    ${Qt5PrintSupport_LIBRARIES}
                          ${Qt5Widgets_LIBRARIES}
                          ${Qt5Gui_LIBRARIES}
                          ${Qt5Core_LIBRARIES} )
    #message(STATUS "QtX_INCLUDE_DIRS: ${QtX_INCLUDE_DIRS}")
    #message(STATUS "QtX_LIBRARIES: ${QtX_LIBRARIES}")
   endif()
 endmacro()

 macro(qtX_wrap_cpp variable)
   if (WITH_QT4)
     qt4_wrap_cpp(${variable} ${ARGN})
   else()
     qt5_wrap_cpp(${variable} ${ARGN})
   endif()
 endmacro()

 macro(qtX_add_resources variable)
   if (WITH_QT4)
     qt4_add_resources(${variable} ${ARGN})
   else()
     qt5_add_resources(${variable} ${ARGN})
   endif()
 endmacro()


#
# Find Qwt, correlated to the Qt version.
#
 macro(setup_qwt)
   find_package(PkgConfig)
   pkg_search_module(QWT REQUIRED Qt5Qwt6)
   if(QWT_FOUND)
     set( QWT_LIBRARY ${QWT_LINK_LIBRARIES} )
     set( QWT_INCLUDE_DIR ${QWT_CFLAGS} )
     if(NOT QWT_FIND_QUIETLY)
       message(STATUS "-- Found Qwt: libs:${QWT_LIBRARY} cflags:${QWT_INCLUDE_DIR}")
     endif()
   else()
     if(QWT_FIND_REQUIRED)
       message(FATAL_ERROR "-- Could not find Qwt")
     endif()
   endif()
 endmacro()


#
# Guess the value of SYS_CONF_DIR according to INSTALLDIR.
# if INSTALLDIR is either /usr or /opt, we uses the system-wide /etc,
# otherwise we install under the tool INSTALLDIR/etc.
#
 macro(setup_sysconfdir INSTALLDIR)
   message("-- Checking installation directory <${INSTALLDIR}>")
   string(REGEX MATCH "^/opt/rh/devtoolset-[0-9]+/root" IS_OPT_RH ${INSTALLDIR})
   if(IS_OPT_RH)
     message("-- Using devtoolset-X install tree ${IS_OPT_RH}/etc.")
     set(SYS_CONF_DIR ${IS_OPT_RH} CACHE STRING "System configuration directory (/etc)" FORCE)
   else()
     string(REGEX MATCH "^/usr$" IS_USR ${INSTALLDIR})
     string(REGEX MATCH "^/opt$" IS_OPT ${INSTALLDIR})
     if(IS_USR OR IS_OPT)
       message("-- Using system-wide /etc.")
       set(SYS_CONF_DIR "/etc" CACHE STRING "System configuration directory (/etc)" FORCE)
     endif()
   endif()

   if(NOT IS_OPT_RH AND NOT IS_USR AND NOT IS_OPT)
     message("-- Using install tree <prefix>/etc.")
     set(SYS_CONF_DIR "etc" CACHE STRING "System configuration directory (/etc)" FORCE)
   endif()
 endmacro()
 

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


#
# Build a Python extention module.
# Usage:
#   * clibSpec:  The C/C++ shared part of the Python module.
#                A four three list CLIB_NAME;version;soversion;
#                - CLIB_NAME: the name of the C/C++ shared library
#                - version:   the full version number (ex: 1.0).
#                - soversion: the shared version major (ex: 1).
#                If the C library must not be generated because it is
#                already included in another one, set to IGNORE.
#   * pymodule:  The name of the Python module (for "import PYMODULE").
#   * deplibs:   The list of dependencies.
#
 macro( add_python_module pyCpps pyIncludes argClibSpec pymodule deplibs inc_install_dir )
   set( pyDeplibs ${deplibs} )
  # Force the argument <argClibSpec> to be parsed as a list.
   set( clibSpec ${argClibSpec} )
   list( GET clibSpec 0 clib )

   message( STATUS ${clib} )
   if( NOT (${clib} STREQUAL "Do_not_generate_C_library") )
     list( LENGTH clibSpec clibLen )
     if( NOT (clibLen EQUAL 3) )
       message( FATAL_ERROR "python_module(): clibSpec doesnt't have exactly 3 elements (${clibSpec})." )
     endif()
     list( GET clibSpec 1 version )
     list( GET clibSpec 2 soversion )
     set( pyDeplibs ${clib} ${deplibs} )

     add_library( ${clib} ${pyCpps} ) 
     set_target_properties( ${clib}      PROPERTIES VERSION ${version} SOVERSION ${soversion})
     #target_compile_definitions( ${clib} PUBLIC Py_LIMITED_API=1)
     target_link_libraries( ${clib}      ${deplibs} )
     set_has_unresolved_symbols( ${clib} )
                 install( TARGETS      ${clib}  DESTINATION lib${LIB_SUFFIX} )
   endif()
  
                     set( pytarget     "${pymodule}_target" )

             add_library( ${pytarget}  MODULE ${pyCpps} ) 
   set_target_properties( ${pytarget}  PROPERTIES
                                       COMPILE_FLAGS "${COMPILE_FLAGS} -D__PYTHON_MODULE__=1"
                                       PREFIX        ""
                                       OUTPUT_NAME   ${pymodule}
                        )
		#target_compile_definitions( ${pytarget} PUBLIC Py_LIMITED_API=1)
   target_link_libraries( ${pytarget}  ${pyDeplibs} )
   set_has_unresolved_symbols( ${pytarget} )
                 install( TARGETS      ${pytarget}    DESTINATION ${Python_CORIOLISARCH} )
   if( NOT ("${pyIncludes}" STREQUAL "None") )
                 install( FILES        ${pyIncludes}  DESTINATION ${inc_install_dir} )
   endif()
 endmacro( add_python_module )


#
# Build a Python extention module (3rd version).
# Usage:
#   * pyCpps:          The list of C/C++ module files.
#   * pyIncludes:      The list of C/C++ header files (will be installed in
#                      "inc_install_dir").
#   * pymodule:        The name of the Python module (for "import PYMODULE").
#   * deplibs:         The list of dependencies.
#
 macro( add_python_module3 pyCpps pyIncludes pymodule deplibs inc_install_dir )
             add_library( ${pymodule}  MODULE ${pyCpps} ) 
   set_target_properties( ${pymodule}  PROPERTIES PREFIX "" )
   target_link_libraries( ${pymodule}  ${deplibs} )
   set_has_unresolved_symbols( ${pymodule} )
   # target_compile_definitions( ${pymodule} PUBLIC Py_LIMITED_API=1)

                 install( TARGETS      ${pymodule}    DESTINATION ${Python_CORIOLISARCH} )
   if( NOT ("${pyIncludes}" STREQUAL "None") )
                 install( FILES        ${pyIncludes}  DESTINATION ${inc_install_dir} )
   endif()
 endmacro( add_python_module3 )
