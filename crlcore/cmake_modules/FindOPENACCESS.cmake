# - Find the OpenAccess includes and libraries.
# x-----------------------------------------------------------------x
# |  This file is part of the hurricaneAMS Software.                |
# |  Copyright (c) UPMC/LIP6 2009-2010, All Rights Reserved         |
# | =============================================================== |
# |  Author      :                    Jean-Manuel Caba              |
# |  E-mail      :       Jean-Manuel.Caba@asim.lip6.fr              |
# | =============================================================== |
# |  Author      :                 Chistophe Alexandre              |
# |  E-mail      :   Christophe.Alexandre@asim.lip6.fr              |
# x-----------------------------------------------------------------x
# The following variables are set if OpenAccess is found.  If OpenAccess is not
# found, OA_FOUND is set to false.
#  OA_FOUND        - True when OpenAccess is found.
#  OA_INCLUDE_DIR - the path to where the OpenAccess include files are.
#  OA_LIBRARIES - The path to where the OpenAccess library files are.
#
#  In OpenAccess si2 version the base library (without the plugins) are
#  The OpenAccess oaCommon library: OA_COMMON_LIBRARY
#                                   OA_COMMON_LIBRARY_RELEASE
#                                   OA_COMMON_DEBUG
#
#  The OpenAccess oaBase library:   OA_BASE_LIBRARY
#                                   OA_BASE_LIBRARY_RELEASE
#                                   OA_BASE_DEBUG
#
#  The OpenAccess oaCM library:     OA_CM_LIBRARY
#                                   OA_CM_LIBRARY_RELEASE
#                                   OA_CM_DEBUG
#
#  The OpenAccess oaDM library:     OA_DM_LIBRARY
#                                   OA_DM_LIBRARY_RELEASE
#                                   OA_DM_DEBUG
#
#  The OpenAccess oaTech library:   OA_TECH_LIBRARY
#                                   OA_TECH_LIBRARY_RELEASE
#                                   OA_TECH_DEBUG
#
#  The OpenAccess oaDesign library: OA_DESIGN_LIBRARY
#                                   OA_DESIGN_LIBRARY_RELEASE
#                                   OA_DESIGN_DEBUG
#
#  The OpenAccess oaWafer library:  OA_WAFER_LIBRARY
#                                   OA_WAFER_LIBRARY_RELEASE
#                                   OA_WAFER_DEBUG
#
#  The OpenAccess oaPlugin library: OA_PLUGIN_LIBRARY
#                                   OA_PLUGIN_LIBRARY_RELEASE
#                                   OA_PLUGIN_DEBUG
#
# Cadence version of OpenAccess has also
#
#liboaCMExportSample.so 
#liboaCMTrackingSample.so 
#liboaDMFileSysBase.so 
#liboaDMFileSys.so 
#liboaDMTurboBase.so 
#liboaDMTurbo.so 
#liboaLangBase.so
#liboaLangInfo.so
#liboaNativeLibDef.so
#liboaNativeText.so
#liboaPcellCPP.so
#liboaPcellScript.so
#liboaRQCDS.so
#liboaRQHGCDS.so
#liboaRQXYTree.so
#liboaTclCommon.so
#liboaTclBase.so
#liboaTclPlugIn.so
#liboaTclEngine.so
#liboaTclHelp.so
#liboaTcl.so

MESSAGE(STATUS "Searching for OPENACCESS ...")
SET(OA_INCLUDE_PATH_DESCRIPTION "directory containing the OpenAccess include files. E.g /usr/local/include")

SET(OA_DIR_MESSAGE "Set the OA_INCLUDE_DIR cmake cache entry to the ${OA_INCLUDE_PATH_DESCRIPTION}")

# don't even bother under WIN32
IF(UNIX)

  SET(OA_DIR_SEARCH $ENV{OA_LIB_DIR})
  IF(OA_DIR_SEARCH)
    FILE(TO_CMAKE_PATH ${OA_DIR_SEARCH} OA_DIR_SEARCH)
  ENDIF(OA_DIR_SEARCH)

  #
  # Look for an installation.
  #
  FIND_PATH(OA_INCLUDE_PATH NAMES oa/oaDesign.h PATHS $ENV{OA_INCLUDE_DIR}
    PATH_SUFFIXES include
    # Help the user find it if we cannot.
    DOC "The ${OA_INCLUDE_PATH_DESCRIPTION}"
  )

  SET(OA_LIBRARY_DIR ${OA_DIR_SEARCH})

  # Set OA_OACOMMON_LIBRARY 
  FIND_LIBRARY(OA_OACOMMON_LIBRARY_RELEASE NAMES oaCommon PATHS ${OA_LIBRARY_DIR} NO_DEFAULT_PATH )
  FIND_LIBRARY(OA_OACOMMON_LIBRARY_DEBUG NAMES oaCommonD PATHS ${OA_LIBRARY_DIR} NO_DEFAULT_PATH)

  # Set OA_OABASE_LIBRARY 
  FIND_LIBRARY(OA_OABASE_LIBRARY_RELEASE NAMES oaBase PATHS ${OA_LIBRARY_DIR} NO_DEFAULT_PATH )
  FIND_LIBRARY(OA_OABASE_LIBRARY_DEBUG NAMES oaBaseD PATHS ${OA_LIBRARY_DIR} NO_DEFAULT_PATH)

  # Set OA_OACM_LIBRARY 
  FIND_LIBRARY(OA_OACM_LIBRARY_RELEASE NAMES oaCM PATHS ${OA_LIBRARY_DIR} NO_DEFAULT_PATH )
  FIND_LIBRARY(OA_OACM_LIBRARY_DEBUG NAMES oaCMD PATHS ${OA_LIBRARY_DIR} NO_DEFAULT_PATH)

  # Set OA_OADM_LIBRARY 
  FIND_LIBRARY(OA_OADM_LIBRARY_RELEASE NAMES oaDM PATHS ${OA_LIBRARY_DIR} NO_DEFAULT_PATH )
  FIND_LIBRARY(OA_OADM_LIBRARY_DEBUG NAMES oaDMD PATHS ${OA_LIBRARY_DIR} NO_DEFAULT_PATH)

  # Set OA_OAPLUGIN_LIBRARY 
  FIND_LIBRARY(OA_OAPLUGIN_LIBRARY_RELEASE NAMES oaPlugIn PATHS ${OA_LIBRARY_DIR} NO_DEFAULT_PATH )
  FIND_LIBRARY(OA_OAPLUGIN_LIBRARY_DEBUG NAMES oaPlugInD PATHS ${OA_LIBRARY_DIR} NO_DEFAULT_PATH)

  # Set OA_OATECH_LIBRARY 
  FIND_LIBRARY(OA_OATECH_LIBRARY_RELEASE NAMES oaTech PATHS ${OA_LIBRARY_DIR} NO_DEFAULT_PATH )
  FIND_LIBRARY(OA_OATECH_LIBRARY_DEBUG NAMES oaTechD PATHS ${OA_LIBRARY_DIR} NO_DEFAULT_PATH)

  # Set OA_OADESIGN_LIBRARY 
  FIND_LIBRARY(OA_OADESIGN_LIBRARY_RELEASE NAMES oaDesign PATHS ${OA_LIBRARY_DIR} NO_DEFAULT_PATH )
  FIND_LIBRARY(OA_OADESIGN_LIBRARY_DEBUG NAMES oaDesignD PATHS ${OA_LIBRARY_DIR} NO_DEFAULT_PATH)

  # Set OA_OAWAFER_LIBRARY 
  FIND_LIBRARY(OA_OAWAFER_LIBRARY_RELEASE NAMES oaWafer PATHS ${OA_LIBRARY_DIR} NO_DEFAULT_PATH )
  FIND_LIBRARY(OA_OAWAFER_LIBRARY_DEBUG NAMES oaWaferD PATHS ${OA_LIBRARY_DIR} NO_DEFAULT_PATH)

  ############################################
  #
  # Check the existence of the libraries.
  #
  ############################################

  MACRO (_OA_ADJUST_LIB_VARS basename)
      # if only the release version was found, set the debug variable also to the release version
      IF (OA_${basename}_LIBRARY_RELEASE AND NOT OA_${basename}_LIBRARY_DEBUG)
        SET(OA_${basename}_LIBRARY_DEBUG ${OA_${basename}_LIBRARY_RELEASE})
        SET(OA_${basename}_LIBRARY       ${OA_${basename}_LIBRARY_RELEASE})
        SET(OA_${basename}_LIBRARIES     ${OA_${basename}_LIBRARY_RELEASE})
      ENDIF (OA_${basename}_LIBRARY_RELEASE AND NOT OA_${basename}_LIBRARY_DEBUG)

      # if only the debug version was found, set the release variable also to the debug version
      IF (OA_${basename}_LIBRARY_DEBUG AND NOT OA_${basename}_LIBRARY_RELEASE)
        SET(OA_${basename}_LIBRARY_RELEASE ${OA_${basename}_LIBRARY_DEBUG})
        SET(OA_${basename}_LIBRARY         ${OA_${basename}_LIBRARY_DEBUG})
        SET(OA_${basename}_LIBRARIES       ${OA_${basename}_LIBRARY_DEBUG})
      ENDIF (OA_${basename}_LIBRARY_DEBUG AND NOT OA_${basename}_LIBRARY_RELEASE)
      IF (OA_${basename}_LIBRARY_DEBUG AND OA_${basename}_LIBRARY_RELEASE)
        # if the generator supports configuration types then set
        # optimized and debug libraries, or if the CMAKE_BUILD_TYPE has a value
        IF (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
          SET(OA_${basename}_LIBRARY       optimized ${OA_${basename}_LIBRARY_RELEASE} debug ${OA_${basename}_LIBRARY_DEBUG})
        ELSE(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
          # if there are no configuration types and CMAKE_BUILD_TYPE has no value
          # then just use the release libraries
          SET(OA_${basename}_LIBRARY       ${OA_${basename}_LIBRARY_RELEASE} )
        ENDIF(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
        SET(OA_${basename}_LIBRARIES       optimized ${OA_${basename}_LIBRARY_RELEASE} debug ${OA_${basename}_LIBRARY_DEBUG})
      ENDIF (OA_${basename}_LIBRARY_DEBUG AND OA_${basename}_LIBRARY_RELEASE)

      SET(OA_${basename}_LIBRARY ${OA_${basename}_LIBRARY} CACHE FILEPATH "The OpenAccess ${basename} library")

      IF (OA_${basename}_LIBRARY)
        SET(OA_${basename}_FOUND 1)
      ENDIF (OA_${basename}_LIBRARY)

    # Make variables changeble to the advanced user
    MARK_AS_ADVANCED(OA_${basename}_LIBRARY OA_${basename}_LIBRARY_RELEASE OA_${basename}_LIBRARY_DEBUG OA_${basename}_INCLUDE_DIR)
  ENDMACRO (_OA_ADJUST_LIB_VARS)

  _OA_ADJUST_LIB_VARS(OACOMMON)
  _OA_ADJUST_LIB_VARS(OABASE)
  _OA_ADJUST_LIB_VARS(OACM)
  _OA_ADJUST_LIB_VARS(OADM)
  _OA_ADJUST_LIB_VARS(OAPLUGIN)
  _OA_ADJUST_LIB_VARS(OATECH)
  _OA_ADJUST_LIB_VARS(OADESIGN)
  _OA_ADJUST_LIB_VARS(OAWAFER)

  IF (OA_INCLUDE_PATH AND OA_OACOMMON_LIBRARY AND OA_OABASE_LIBRARY 
      AND OA_OADM_LIBRARY AND OA_OATECH_LIBRARY AND OA_OADESIGN_LIBRARY 
      AND OA_OAWAFER_LIBRARY AND OA_OAPLUGIN_LIBRARY) 
    SET(OA_FOUND "YES")
    IF (NOT OA_FIND_QUIETLY)
      MESSAGE(STATUS "Found OpenAccess")
    ENDIF( NOT OA_FIND_QUIETLY)
    SET(OA_INCLUDE_DIR
      ${OA_INCLUDE_PATH}
    )
    SET(OA_LIBRARIES
      ${OA_OACOMMON_LIBRARY} 
      ${OA_OABASE_LIBRARY} 
      ${OA_OACM_LIBRARY}
      ${OA_OADM_LIBRARY}
      ${OA_OAPLUGIN_LIBRARY} 
      ${OA_OATECH_LIBRARY} 
      ${OA_OADESIGN_LIBRARY} 
      ${OA_OAWAFER_LIBRARY}
    )
  ELSE (OA_INCLUDE_PATH AND OA_LIBRARY_PATH)
    SET(OA_FOUND "NO")
    IF(OA_FIND_REQUIRED)
      MESSAGE( FATAL_ERROR "OpenAccess NOT found!")
    ELSE(OA_FIND_REQUIRED)
      MESSAGE( STATUS "OpenAccess NOT found!")
    ENDIF(OA_FIND_REQUIRED)
  ENDIF (OA_INCLUDE_PATH AND OA_OACOMMON_LIBRARY AND OA_OABASE_LIBRARY 
      AND OA_OADM_LIBRARY AND OA_OATECH_LIBRARY AND OA_OADESIGN_LIBRARY 
      AND OA_OAWAFER_LIBRARY AND OA_OAPLUGIN_LIBRARY) 

  IF(OA_FOUND)
    ADD_DEFINITIONS(-DHAVE_OPENACCESS)
  ENDIF(OA_FOUND)

ENDIF(UNIX)
