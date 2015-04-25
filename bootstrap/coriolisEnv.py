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
    useDevtoolset2    = False
    osSlsoc7x_64      = re.compile (".*Linux.*el7.*x86_64.*")
    osSlsoc6x_64      = re.compile (".*Linux.*el6.*x86_64.*")
    osSlsoc6x         = re.compile (".*Linux.*(el|slsoc)6.*")
    osSLSoC5x_64      = re.compile (".*Linux.*el5.*x86_64.*")
    osSLSoC5x         = re.compile (".*Linux.*(el5|2.6.23.13.*SoC).*")
    osFedora_64       = re.compile (".*Linux.*fc.*x86_64.*")
    osFedora          = re.compile (".*Linux.*fc.*")
    osLinux_64        = re.compile (".*Linux.*x86_64.*")
    osLinux           = re.compile (".*Linux.*")
    osDarwin          = re.compile (".*Darwin.*")
    osUbuntu1004      = re.compile (".*Linux.*ubuntu.*")
    osUbuntu1004_64   = re.compile (".*Linux.*ubuntu.*x86_64.*")
    osFreeBSD8x_amd64 = re.compile (".*FreeBSD 8.*amd64.*")
    osFreeBSD8x_64    = re.compile (".*FreeBSD 8.*x86_64.*")
    osFreeBSD8x       = re.compile (".*FreeBSD 8.*")
    osCygwinW7_64     = re.compile (".*CYGWIN_NT-6\.1.*x86_64.*")
    osCygwinW7        = re.compile (".*CYGWIN_NT-6\.1.*i686.*")
    osCygwinW8_64     = re.compile (".*CYGWIN_NT-6\.[2-3].*x86_64.*")
    osCygwinW8        = re.compile (".*CYGWIN_NT-6\.[2-3].*i686.*")

    uname = subprocess.Popen ( ["uname", "-srm"], stdout=subprocess.PIPE )
    lines = uname.stdout.readlines()

    libDir="lib"
    if osSlsoc7x_64.match(lines[0]):
      osType         = "Linux.el7_64"
      libDir         = "lib64"
    elif osSlsoc6x_64.match(lines[0]):
      osType         = "Linux.slsoc6x_64"
      libDir         = "lib64"
      useDevtoolset2 = True
    elif osSlsoc6x.match(lines[0]):
      osType         = "Linux.slsoc6x"
      useDevtoolset2 = True
    elif osSLSoC5x_64.match(lines[0]):
      osType         = "Linux.SLSoC5x_64"
      libDir         = "lib64"
    elif osSLSoC5x.match(lines[0]):
      osType = "Linux.SLSoC5x"
    elif osFedora_64.match(lines[0]):
      osType = "Linux.fc_64"
      libDir = "lib64"
    elif osFedora.match(lines[0]):
      osType = "Linux.fc"
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
    elif osCygwinW7_64.match(lines[0]):
      osType = "Cygwin.W7_64"
      libDir = "lib64"
    elif osCygwinW7.match(lines[0]):
      osType = "Cygwin.W7"
    elif osCygwinW8_64.match(lines[0]):
      osType = "Cygwin.W8_64"
      libDir = "lib64"
    elif osCygwinW8.match(lines[0]):
      osType = "Cygwin.W8"
    else:
      uname = subprocess.Popen ( ["uname", "-sr"], stdout=subprocess.PIPE )
      osType = uname.stdout.readlines()[0][:-1]

      print "[WARNING] Unrecognized OS: \"%s\"." % lines[0][:-1]
      print "          (using: \"%s\")" % osType

    ldLibraryPath = os.getenv('LD_LIBRARY_PATH')
    if not ldLibraryPath or 'devtoolset' in ldLibraryPath: useDevtoolset2 = False

    if libDir == 'lib64' and not os.path.exists('/usr/lib64'):
      libDir = 'lib'
    
    return (osType,libDir,useDevtoolset2)
      



if __name__ == "__main__":

  osType,libDir,useDevtoolset2 = guessOs()
  buildType                    = "Release"
  linkType                     = "Shared"
  rootDir                      = None

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
    'echo "%(MESSAGE)s";'                                          \
    'echo "Switching to Coriolis 2.x (%(buildDir)s)";'             \
    'PATH="%(PATH)s";'                                             \
    'BOOTSTRAP_TOP="%(BOOTSTRAP_TOP)s";'                           \
    'CORIOLIS_TOP="%(CORIOLIS_TOP)s";'                             \
    'STRATUS_MAPPING_NAME="%(SYSCONF_DIR)s/stratus2sxlib.xml";'    \
    'export PATH BOOTSTRAP_TOP CORIOLIS_TOP STRATUS_MAPPING_NAME;'

  shellScriptCsh = \
    'echo "%(MESSAGE)s";'                              \
    'echo "Switching to Coriolis 2.x (%(buildDir)s)";' \
    'setenv PATH "%(PATH)s";'                          \
    'setenv BOOTSTRAP_TOP "%(BOOTSTRAP_TOP)s";'        \
    'setenv CORIOLIS_TOP "%(CORIOLIS_TOP)s";'          \
    'setenv STRATUS_MAPPING_NAME "%(SYSCONF_DIR)s/stratus2sxlib.xml";'

  buildDir  = buildType + "." + linkType
  scriptDir = os.path.dirname ( os.path.abspath(__file__) )
 #print "echo \"Script Location: %s\";" % scriptDir,
  if scriptDir.startswith("/etc/coriolis2"):
    coriolisTop  = "/usr"
    sysconfDir   = scriptDir
    shellMessage = "Using system-wide Coriolis 2 (/usr)"
  elif scriptDir.startswith("/etc/coriolis2"): 
    coriolisTop  = "/opt/rh/devtoolset-2/root/usr"
    sysconfDir   = scriptDir
    shellMessage = "Using system-wide devtoolset-2 Coriolis 2 (/opt/rh/devtoolset-2/root/usr)"
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

  if osType.startswith("Cygwin"):
    strippedPath = "%s/%s:%s" % ( coriolisTop, libDir, strippedPath )

  absLibDir           = "%s/%s"     % ( coriolisTop, libDir )
  strippedPath        = "%s/bin:%s" % ( coriolisTop, strippedPath )
  strippedLibraryPath = "%s:%s"     % ( absLibDir  , strippedLibraryPath )

  if not options.nopython:
    pyVersion = sys.version_info
    version   = "%d.%d" % (pyVersion[0],pyVersion[1])
    if    osType.startswith("Linux.SL") \
       or osType.startswith("Linux.sl") \
       or osType.startswith("Linux.el") \
       or osType.startswith("Linux.fc") \
       or osType.startswith("Darwin")   \
       or osType.startswith("Cygwin"):
      sitePackagesDir = "%s/python%s/site-packages" % (absLibDir,version)
    else:
      sitePackagesDir = "%s/python%s/dist-packages" % (absLibDir,version)

    strippedPythonPath = "%s:"                 % (sitePackagesDir) + strippedPythonPath
    strippedPythonPath = "%s/crlcore:"         % (sitePackagesDir) + strippedPythonPath
    strippedPythonPath = "%s/cumulus:"         % (sitePackagesDir) + strippedPythonPath
    strippedPythonPath = "%s/cumulus/plugins:" % (sitePackagesDir) + strippedPythonPath
    strippedPythonPath = "%s/stratus:"         % (sitePackagesDir) + strippedPythonPath

    shellScriptSh  += 'PYTHONPATH="%(PYTHONPATH)s";' \
                      'export PYTHONPATH;'
    shellScriptCsh += 'setenv PYTHONPATH "%(PYTHONPATH)s";'

    if osType == "Darwin":
      shellScriptSh  += 'DYLD_LIBRARY_PATH="%(LD_LIBRARY_PATH)s";' \
                        'export DYLD_LIBRARY_PATH;'
      shellScriptCsh += 'setenv DYLD_LIBRARY_PATH="%(LD_LIBRARY_PATH)s";' 
    else:
      shellScriptSh  += 'LD_LIBRARY_PATH="%(LD_LIBRARY_PATH)s";' \
                        'export LD_LIBRARY_PATH;'
      shellScriptCsh += 'setenv LD_LIBRARY_PATH="%(LD_LIBRARY_PATH)s";'
  
  shellScriptSh  += "hash -r;"
  shellScriptCsh += "rehash;"

  if options.csh: shellScript = shellScriptCsh
  else:           shellScript = shellScriptSh

  evalScript = shellScript % { "PATH"            : strippedPath
                             , "LD_LIBRARY_PATH" : strippedLibraryPath
                             , "PYTHONPATH"      : strippedPythonPath
                             , "BOOTSTRAP_TOP"   : coriolisTop
                             , "CORIOLIS_TOP"    : coriolisTop
                             , "SYSCONF_DIR"     : sysconfDir
                             , "MESSAGE"         : shellMessage
                             , "buildDir"        : buildDir
                             }
  if useDevtoolset2:
    evalScript = '%s scl enable devtoolset-2 ${SHELL}' % evalScript

  print evalScript
