#!/usr/bin/env python


import re
import os
import sys
import string
import subprocess
import optparse


mangrovePattern = re.compile ( r".*mangrove.*" )
coriolisPattern = re.compile ( r".*coriolis.*" )


def stripPath ( pathName ):
  pathEnv = os.getenv ( pathName )
  if not pathEnv: return ""
  
  pathList     = string.split ( pathEnv, ':' )
  strippedList = []
  for pathElement in pathList:
    if     not mangrovePattern.match(pathElement) \
       and not coriolisPattern.match(pathElement):
        strippedList += [ pathElement ]

  if len(strippedList) == 0: return ""

  strippedEnv = strippedList[0]
  for pathElement in strippedList[1:]:
      strippedEnv += ":" + pathElement

  return strippedEnv


def guessOs ():
    osSLSoC5x_64     = re.compile (".*Linux.*el5.*x86_64.*")
    osSLSoC5x        = re.compile (".*Linux.*(el5|2.6.23.13.*SoC).*")
    osLinux_64       = re.compile (".*Linux.*x86_64.*")
    osLinux          = re.compile (".*Linux.*")
    osDarwin         = re.compile (".*Darwin.*")

    uname = subprocess.Popen ( ["uname", "-srm"], stdout=subprocess.PIPE )
    lines = uname.stdout.readlines()

    libDir="lib"
    if osSLSoC5x_64.match(lines[0]):
      osType = "Linux.SLSoC5x_64"
      libDir = "lib64"
    elif osSLSoC5x   .match(lines[0]): osType = "Linux.SLSoC5x"
    elif osLinux_64  .match(lines[0]):
      osType = "Linux.x86_64"
      libDir = "lib64"
    elif osLinux     .match(lines[0]): osType = "Linux.i386"
    elif osDarwin    .match(lines[0]): osType = "Darwin"
    else:
        uname = subprocess.Popen ( ["uname", "-sr"], stdout=subprocess.PIPE )
        osType = uname.stdout.readlines()[0][:-1]

       #print "[WARNING] Unrecognized OS: \"%s\"." % lines[0][:-1]
       #print "          (using: \"%s\")" % osType
    
    return (osType,libDir)
      



if __name__ == "__main__":

  (osDir,libDir) = guessOs()
  buildType      = "Release"
  linkType       = "Shared"
  rootDir        = os.getenv("HOME") + "/mangrove"
  sysCoriolisTop = "/asim/coriolis2"
  sysMangroveTop = "/asim/mangrove"

  parser = optparse.OptionParser ()  
 # Build relateds.
  parser.add_option ( "--release", action="store_true" ,                dest="release"  )
  parser.add_option ( "--debug"  , action="store_true" ,                dest="debug"    )
  parser.add_option ( "--static" , action="store_true" ,                dest="static"   )
  parser.add_option ( "--shared" , action="store_true" ,                dest="shared"   )
  parser.add_option ( "--root"   , action="store"      , type="string", dest="rootDir"  )
  ( options, args ) = parser.parse_args ()

  if options.release:  buildType = "Release"
  if options.debug:    buildType = "Debug"
  if options.static:   linkType  = "Static"
  if options.shared:   linkType  = "Shared"
  if options.rootDir:  rootDir   = options.rootDir

  strippedPath        = stripPath ( "PATH" )
  strippedLibraryPath = stripPath ( "LD_LIBRARY_PATH" )
  strippedPythonPath  = stripPath ( "PYTHONPATH" )

  buildDir            = buildType + "." + linkType
  mangroveTop         = "%s/%s/%s/install"         % ( rootDir, osDir, buildDir )
  strippedPath        = "%s/bin:%s/bin:%s/bin:%s"  % ( mangroveTop \
                                                     , sysMangroveTop
                                                     , sysCoriolisTop
                                                     , strippedPath )
  strippedLibraryPath = "%s/%s:%s/%s:%s/%s:%s"     % ( mangroveTop   , libDir
                                                     , sysMangroveTop, libDir
                                                     , sysCoriolisTop, libDir
                                                     , strippedLibraryPath )

  shellScript = \
"""
echo "Setting up Mangrove Environment (%(buildDir)s)";
PATH=%(PATH)s;
LD_LIBRARY_PATH=%(LD_LIBRARY_PATH)s;
PYTHONPATH=%(PYTHONPATH)s;
BOOTSTRAP_TOP=%(CORIOLIS_TOP)s;
CORIOLIS_TOP=%(CORIOLIS_TOP)s;
HURRICANE_TOP=%(CORIOLIS_TOP)s;
MANGROVE_TOP=%(MANGROVE_TOP)s;
export PATH LD_LIBRARY_PATH PYTHONPATH BOOTSTRAP_TOP CORIOLIS_TOP MANGROVE_TOP;
hash -r
"""

  print shellScript % { "PATH"            : strippedPath
                      , "LD_LIBRARY_PATH" : strippedLibraryPath
                      , "PYTHONPATH"      : strippedPythonPath
                      , "CORIOLIS_TOP"    : sysCoriolisTop
                      , "MANGROVE_TOP"    : mangroveTop
                      , "buildDir"        : buildDir
                      }
