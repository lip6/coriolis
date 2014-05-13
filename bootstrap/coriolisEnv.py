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
    osSlsoc6x_64      = re.compile (".*Linux.*el6.*x86_64.*")
    osSlsoc6x         = re.compile (".*Linux.*(el|slsoc)6.*")
    osSLSoC5x_64      = re.compile (".*Linux.*el5.*x86_64.*")
    osSLSoC5x         = re.compile (".*Linux.*(el5|2.6.23.13.*SoC).*")
    osLinux_64        = re.compile (".*Linux.*x86_64.*")
    osLinux           = re.compile (".*Linux.*")
    osDarwin          = re.compile (".*Darwin.*")
    osUbuntu1004      = re.compile (".*Linux.*ubuntu.*")
    osUbuntu1004_64   = re.compile (".*Linux.*ubuntu.*x86_64.*")
    osFreeBSD8x_amd64 = re.compile (".*FreeBSD 8.*amd64.*")
    osFreeBSD8x_64    = re.compile (".*FreeBSD 8.*x86_64.*")
    osFreeBSD8x       = re.compile (".*FreeBSD 8.*")

    uname = subprocess.Popen ( ["uname", "-srm"], stdout=subprocess.PIPE )
    lines = uname.stdout.readlines()

    libDir="lib"
    if osSlsoc6x_64.match(lines[0]):
      osType = "Linux.slsoc6x_64"
      libDir = "lib64"
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
    elif osFreeBSD8x_64.match(lines[0]):
      osType = "FreeBSD.8x.x86_64"
      libDir = "lib64"
    elif osFreeBSD8x_amd64.match(lines[0]):
      osType = "FreeBSD.8x.amd64"
      libDir = "lib64"
    elif osFreeBSD8x.match(lines[0]):
      osType = "FreeBSD.8x.i386"
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
  rootDir         = None

  parser = optparse.OptionParser ()  
 # Build relateds.
  parser.add_option ( "--csh"      , action="store_true" ,                dest="csh"      )
  parser.add_option ( "--release"  , action="store_true" ,                dest="release"  )
  parser.add_option ( "--debug"    , action="store_true" ,                dest="debug"    )
  parser.add_option ( "--devel"    , action="store_true" ,                dest="devel"    )
  parser.add_option ( "--static"   , action="store_true" ,                dest="static"   )
  parser.add_option ( "--shared"   , action="store_true" ,                dest="shared"   )
  parser.add_option ( "--chams"    , action="store_true" ,                dest="chams"    )
  parser.add_option ( "--no-python", action="store_true" ,                dest="nopython" )
  parser.add_option ( "--root"     , action="store"      , type="string", dest="rootDir"  )
  ( options, args ) = parser.parse_args ()

  if options.release:    buildType       = "Release"
  if options.debug:      buildType       = "Debug"
  if options.devel:      buildType       = "Debug"
  if options.static:     linkType        = "Static"
  if options.shared:     linkType        = "Shared"
  if options.rootDir:    rootDir         = options.rootDir

  strippedPath        = stripPath ( "PATH" )
  strippedLibraryPath = stripPath ( "LD_LIBRARY_PATH" )
  strippedPythonPath  = stripPath ( "PYTHONPATH" )

  shellScriptSh = \
    'echo "%(MESSAGE)s";\n'                                          \
    'echo "Switching to Coriolis 2.x (%(buildDir)s)";\n'             \
    'PATH="%(PATH)s";\n'                                             \
    'BOOTSTRAP_TOP="%(BOOTSTRAP_TOP)s";\n'                           \
    'CORIOLIS_TOP="%(CORIOLIS_TOP)s";\n'                             \
    'STRATUS_MAPPING_NAME="%(SYSCONF_DIR)s/stratus2sxlib.xml";\n'    \
    'export PATH BOOTSTRAP_TOP CORIOLIS_TOP STRATUS_MAPPING_NAME;\n' \
    'hash -r;\n'

  shellScriptCsh = \
    'echo "%(MESSAGE)s";\n'                              \
    'echo "Switching to Coriolis 2.x (%(buildDir)s)";\n' \
    'setenv PATH "%(PATH)s";\n'                          \
    'setenv BOOTSTRAP_TOP "%(BOOTSTRAP_TOP)s";\n'        \
    'setenv CORIOLIS_TOP "%(CORIOLIS_TOP)s";\n'          \
    'setenv STRATUS_MAPPING_NAME "%(SYSCONF_DIR)s/stratus2sxlib.xml";\n' \
    'rehash\n;'

  buildDir  = buildType + "." + linkType
  scriptDir = os.path.dirname ( os.path.abspath(__file__) )
 #print "echo \"Script Location: %s\";" % scriptDir,
  if scriptDir.startswith("/etc/coriolis2"):
    coriolisTop  = "/usr"
    sysconfDir   = scriptDir
    shellMessage = "Using system-wide Coriolis 2 (/usr)"
  elif     scriptDir.startswith("/users/outil/coriolis/coriolis-2.x/") \
        or scriptDir.startswith("/soc/coriolis2/"):
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

  if not options.nopython:
    pyVersion = sys.version_info
    version   = "%d.%d" % (pyVersion[0],pyVersion[1])
    if osType.startswith("Linux.SL") or osType.startswith("Linux.sl") or osType.startswith("Darwin"):
      sitePackagesDir = "%s/python%s/site-packages" % (absLibDir,version)
    else:
      sitePackagesDir = "%s/python%s/dist-packages" % (absLibDir,version)

    strippedPythonPath = "%s:"         % (sitePackagesDir) + strippedPythonPath
    strippedPythonPath = "%s/crlcore:" % (sitePackagesDir) + strippedPythonPath
    strippedPythonPath = "%s/cumulus:" % (sitePackagesDir) + strippedPythonPath
    strippedPythonPath = "%s/stratus:" % (sitePackagesDir) + strippedPythonPath

    shellScriptSh  += 'PYTHONPATH="%(PYTHONPATH)s";\n' \
                      'export PYTHONPATH;\n'
    shellScriptCsh += 'setenv PYTHONPATH "%(PYTHONPATH)s";'

    if osType == "Darwin":
      shellScriptSh  += 'DYLD_LIBRARY_PATH="%(LD_LIBRARY_PATH)s";\n' \
                        'export DYLD_LIBRARY_PATH;\n'
      shellScriptCsh += 'setenv DYLD_LIBRARY_PATH="%(LD_LIBRARY_PATH)s";\n' 
    else:
      shellScriptSh  += 'LD_LIBRARY_PATH="%(LD_LIBRARY_PATH)s";\n' \
                        'export LD_LIBRARY_PATH;\n'
      shellScriptCsh += 'setenv LD_LIBRARY_PATH="%(LD_LIBRARY_PATH)s";\n' 
  
  shellScriptSh  += "hash -r;\n"
  shellScriptCSh += "rehash;\n"

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
