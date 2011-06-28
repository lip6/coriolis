#!/usr/bin/env python


import re
import os
import sys
import string
import subprocess
import optparse


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
    osSlsoc6x_64    = re.compile (".*Linux.*el6.*x86_64.*")
    osSlsoc6x       = re.compile (".*Linux.*el6.*")
    osSLSoC5x_64    = re.compile (".*Linux.*el5.*x86_64.*")
    osSLSoC5x       = re.compile (".*Linux.*(el5|2.6.23.13.*SoC).*")
    osLinux_64      = re.compile (".*Linux.*x86_64.*")
    osLinux         = re.compile (".*Linux.*")
    osDarwin        = re.compile (".*Darwin.*")
    osUbuntu1004    = re.compile (".*Linux.*ubuntu.*")
    osUbuntu1004_64 = re.compile (".*Linux.*ubuntu.*x86_64.*")

    uname = subprocess.Popen ( ["uname", "-srm"], stdout=subprocess.PIPE )
    lines = uname.stdout.readlines()

    libDir="lib"
    if osSlsoc6x_64.match(lines[0]):
      osType    = "Linux.slsoc6x_64"
      libSuffix = "64"
    elif osSlsoc6x.match(lines[0]): osType = "Linux.slsoc6x"
    elif osSLSoC5x_64.match(lines[0]):
      osType = "Linux.SLSoC5x_64"
      libDir = "lib64"
    elif osSLSoC5x.match(lines[0]):
      osType = "Linux.SLSoC5x"
    elif osUbuntu1004.match(lines[0]):
      osType = "Linux.Ubuntu1004"
    elif osUbuntu1004_64.match(lines[0]):
      osType = "Linux.Ubuntu1004_64"
      libDir = "lib64"
    elif osLinux_64.match(lines[0]):
      osType = "Linux.x86_64"
      libDir = "lib64"
    elif osLinux.match(lines[0]):
      osType = "Linux.i386"
    elif osDarwin.match(lines[0]):
      osType = "Darwin"
    else:
        uname = subprocess.Popen ( ["uname", "-sr"], stdout=subprocess.PIPE )
        osType = uname.stdout.readlines()[0][:-1]

       #print "[WARNING] Unrecognized OS: \"%s\"." % lines[0][:-1]
       #print "          (using: \"%s\")" % osType
    
    return (osType,libDir)
      



if __name__ == "__main__":

  (osType,libDir) = guessOs()
  buildType       = "Release"
  linkType        = "Shared"
  coriolisVersion = None
  rootDir         = None

  parser = optparse.OptionParser ()  
 # Build relateds.
  parser.add_option ( "--csh"    , action="store_true" ,                dest="csh"        )
  parser.add_option ( "--v1"     , action="store_true" ,                dest="v1"         )
  parser.add_option ( "--v2"     , action="store_true" ,                dest="v2"         )
  parser.add_option ( "--release", action="store_true" ,                dest="release"    )
  parser.add_option ( "--debug"  , action="store_true" ,                dest="debug"      )
  parser.add_option ( "--devel"  , action="store_true" ,                dest="devel"      )
  parser.add_option ( "--static" , action="store_true" ,                dest="static"     )
  parser.add_option ( "--shared" , action="store_true" ,                dest="shared"     )
  parser.add_option ( "--python" , action="store_true" ,                dest="python"     )
  parser.add_option ( "--root"   , action="store"      , type="string", dest="rootDir"    )
  ( options, args ) = parser.parse_args ()

  if options.v1:         coriolisVersion = 1
  if options.v2:         coriolisVersion = 2
  if options.release:    buildType       = "Release"
  if options.debug:      buildType       = "Debug"
  if options.devel:      buildType       = "Debug"
  if options.static:     linkType        = "Static"
  if options.shared:     linkType        = "Shared"
  if options.rootDir:    rootDir         = options.rootDir

  strippedPath        = stripPath ( "PATH" )
  strippedLibraryPath = stripPath ( "LD_LIBRARY_PATH" )
  strippedPythonPath  = stripPath ( "PYTHONPATH" )

  if coriolisVersion == 1:

    if not rootDir:
      rootDir = os.getenv("HOME") + "/coriolis-1.x"

    hurricaneTop = "%s/coriolis/%s/install" % ( rootDir, osType )
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

    buildDir  = buildType + "." + linkType
    scriptDir = os.path.dirname ( os.path.abspath(__file__) )
   #print "echo %s;" % scriptDir
    if scriptDir == "/etc/coriolis2":
      coriolisTop  = "/usr"
      sysconfDir   = scriptDir
      shellMessage = "Using system-wide Coriolis 2 (/usr)"
    elif scriptDir.startswith("/users/outil/coriolis/coriolis-2.x/"):
      coriolisTop  = "/soc/coriolis2"
      sysconfDir   = coriolisTop + "/etc/coriolis2"
      shellMessage = "Using SoC network-wide Coriolis 2 (/soc/coriolis2)"
    else:
      if not rootDir:
        rootDir = os.getenv("HOME") + "/coriolis-2.x"
      coriolisTop  = "%s/%s/%s/install" % ( rootDir, osType, buildDir )
      sysconfDir   = coriolisTop + "/etc/coriolis2"
      shellMessage = "Using user-selected Coriolis 2 (%s)" % rootDir

    absLibDir           = "%s/%s"     % ( coriolisTop, libDir )
    strippedPath        = "%s/bin:%s" % ( coriolisTop, strippedPath )
    strippedLibraryPath = "%s:%s"     % ( absLibDir  , strippedLibraryPath )

    if options.python:
      pyVersion = sys.version_info
      version   = "%d.%d" % (pyVersion[0],pyVersion[1])
      if osType.startswith("Linux.SL") or osType.startswith("Linux.sl"):
        sitePackagesDir = "%s/python%s/site-packages" % (absLibDir,version)
      else:
        sitePackagesDir = "%s/python%s/dist-packages" % (absLibDir,version)

      strippedPythonPath = "%s:"         % (sitePackagesDir) + strippedPythonPath
      strippedPythonPath = "%s/cumulus:" % (sitePackagesDir) + strippedPythonPath
      strippedPythonPath = "%s/stratus:" % (sitePackagesDir) + strippedPythonPath

    shellScriptSh = \
"""
echo "%(MESSAGE)s";
echo "Switching to Coriolis 2.x (%(buildDir)s)";
PATH="%(PATH)s";
LD_LIBRARY_PATH="%(LD_LIBRARY_PATH)s";
PYTHONPATH="%(PYTHONPATH)s";
BOOTSTRAP_TOP="%(BOOTSTRAP_TOP)s";
CORIOLIS_TOP="%(CORIOLIS_TOP)s";
STRATUS_MAPPING_NAME="%(SYSCONF_DIR)s/stratus2sxlib.xml";
export PATH LD_LIBRARY_PATH PYTHONPATH BOOTSTRAP_TOP CORIOLIS_TOP STRATUS_MAPPING_NAME;
hash -r
"""

    shellScriptCsh = \
"""
echo "%(MESSAGE)s";
echo "Switching to Coriolis 2.x (%(buildDir)s)";
setenv PATH "%(PATH)s";
setenv LD_LIBRARY_PATH "%(LD_LIBRARY_PATH)s";
setenv PYTHONPATH "%(PYTHONPATH)s";
setenv BOOTSTRAP_TOP "%(BOOTSTRAP_TOP)s";
setenv CORIOLIS_TOP "%(CORIOLIS_TOP)s";
setenv STRATUS_MAPPING_NAME "%(SYSCONF_DIR)s/stratus2sxlib.xml";
rehash
"""

  if coriolisVersion:
    if options.csh: shellScript = shellScriptCsh
    else:           shellScript = shellScriptSh

    print shellScript % { "PATH"            : strippedPath
                        , "LD_LIBRARY_PATH" : strippedLibraryPath
                        , "PYTHONPATH"      : strippedPythonPath
                        , "BOOTSTRAP_TOP"   : coriolisTop
                        , "CORIOLIS_TOP"    : coriolisTop
                        , "SYSCONF_DIR"     : sysconfDir
                        , "MESSAGE"         : shellMessage
                        , "buildDir"        : buildDir
                        }
