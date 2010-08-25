#!/usr/bin/env python


import re
import os
import sys
import string
import subprocess


coriolisPattern = re.compile ( r".*coriolis.*" )


def stripPath ( pathName ):
  pathEnv = os.getenv ( pathName )
  if not pathEnv: return ""
  
  pathList     = string.split ( pathEnv, ':' )
  strippedList = []
  for pathElement in pathList:
    if not coriolisPattern.match(pathElement):
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

  (osDir,libDir)  = guessOs()
  buildType       = "Debug"
  linkType        = "Shared"
  coriolisVersion = None
    
  for argument in sys.argv:
    if   argument == "--v1":      coriolisVersion=1
    elif argument == "--v2":      coriolisVersion=2
    elif argument == "--debug":   buildType="Debug"
    elif argument == "--devel":   buildType="Debug"
    elif argument == "--release": buildType="Release"
    elif argument == "--shared":  linkType="Shared"
    elif argument == "--static":  linkType="Static"

  strippedPath        = stripPath ( "PATH" )
  strippedLibraryPath = stripPath ( "LD_LIBRARY_PATH" )
  strippedPythonPath  = stripPath ( "PYTHONPATH" )

  if coriolisVersion == 1:

    hurricaneTop = "%s/coriolis-1.x/coriolis/%s/install" % ( os.getenv("HOME"), osDir )
    buildDir     = None
    shellScript  = \
"""
echo "Switching to Coriolis 1.x";
PATH=%(PATH)s;
LD_LIBRARY_PATH=%(LD_LIBRARY_PATH)s;
PYTHONPATH=%(PYTHONPATH)s;
HURRICANE_TOP=%(HURRICANE_TOP)s;
CORIOLIS_TOP=%(HURRICANE_TOP)s;
export PATH LD_LIBRARY_PATH PYTHONPATH HURRICANE_TOP CORIOLIS_TOP;
hash -r;
if [ -f "%(HURRICANE_TOP)s/share/etc/coriolis.sh" ]; then
  . "%(HURRICANE_TOP)s/share/etc/coriolis.sh"
fi
    """

  elif coriolisVersion == 2:

    buildDir            = buildType + "." + linkType
    hurricaneTop        = "%s/coriolis-2.x/%s/%s/install" % ( os.getenv("HOME"), osDir, buildDir )
    strippedPath        = "%s/bin:%s" % ( hurricaneTop, strippedPath )
    strippedLibraryPath = "%s/%s:%s" % ( hurricaneTop, libDir, strippedLibraryPath )

    shellScript = \
"""
echo "Switching to Coriolis 2.x (%(buildDir)s)";
PATH=%(PATH)s;
LD_LIBRARY_PATH=%(LD_LIBRARY_PATH)s;
PYTHONPATH=%(PYTHONPATH)s;
HURRICANE_TOP=%(HURRICANE_TOP)s;
CORIOLIS_TOP=%(HURRICANE_TOP)s;
export PATH LD_LIBRARY_PATH PYTHONPATH HURRICANE_TOP CORIOLIS_TOP;
hash -r
"""

  if coriolisVersion:
    print shellScript % { "PATH"            : strippedPath
                        , "LD_LIBRARY_PATH" : strippedLibraryPath
                        , "PYTHONPATH"      : strippedPythonPath
                        , "HURRICANE_TOP"   : hurricaneTop
                        , "buildDir"        : buildDir
                        }
