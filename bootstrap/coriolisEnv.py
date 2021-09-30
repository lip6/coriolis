#!/usr/bin/env python3


import re
import os
import sys
import string
import subprocess
import optparse


reCoriolisPattern      = re.compile( r".*coriolis.*" )
reReleaseSharedPattern = re.compile( r".*Release\.Shared.*" )
reReleaseStaticPattern = re.compile( r".*Release\.Static.*" )
reDebugSharedPattern   = re.compile( r".*Debug\.Shared.*" )
reDebugStaticPattern   = re.compile( r".*Debug\.Static.*" )


def scrubPath ( pathName ):
    pathEnv = os.getenv( pathName )
    if not pathEnv: return ""
    pathList     = pathEnv.split( ':' )
    scrubbedList = []
    for pathElement in pathList:
        if    reCoriolisPattern     .match(pathElement) \
           or reReleaseSharedPattern.match(pathElement) \
           or reReleaseStaticPattern.match(pathElement) \
           or reDebugSharedPattern  .match(pathElement) \
           or reDebugStaticPattern  .match(pathElement):
            continue
        scrubbedList += [ pathElement ]
    if len(scrubbedList) == 0: return ""
    scrubbedEnv = scrubbedList[0]
    for pathElement in scrubbedList[1:]:
        scrubbedEnv += ":" + pathElement
    return scrubbedEnv


def guessOs ():
    useDevtoolset     = False
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
    osCygwinW10_64    = re.compile (".*CYGWIN_NT-10\.[0-3].*x86_64.*")
    osCygwinW10       = re.compile (".*CYGWIN_NT-10\.[0-3].*i686.*")

    uname = subprocess.Popen ( ["uname", "-srm"], stdout=subprocess.PIPE )
    lines = uname.stdout.readlines()

    line = lines[0].decode( 'ascii' )
    if   osSlsoc7x_64.match(line): osType = "Linux.el7_64"
    elif osSlsoc6x_64.match(line):
        osType         = "Linux.slsoc6x_64"
        useDevtoolset  = True
    elif osSlsoc6x.match(line):
        osType         = "Linux.slsoc6x"
        useDevtoolset  = True
    elif osSLSoC5x_64     .match(line): osType = "Linux.SLSoC5x_64"
    elif osSLSoC5x        .match(line): osType = "Linux.SLSoC5x"
    elif osFedora_64      .match(line): osType = "Linux.fc_64"
    elif osFedora         .match(line): osType = "Linux.fc"
    elif osUbuntu1004     .match(line): osType = "Linux.Ubuntu1004"
    elif osUbuntu1004_64  .match(line): osType = "Linux.Ubuntu1004_64"
    elif osLinux_64       .match(line): osType = "Linux.x86_64"
    elif osLinux          .match(line): osType = "Linux.i386"
    elif osFreeBSD8x_64   .match(line): osType = "FreeBSD.8x.x86_64"
    elif osFreeBSD8x_amd64.match(line): osType = "FreeBSD.8x.amd64"
    elif osFreeBSD8x      .match(line): osType = "FreeBSD.8x.i386"
    elif osDarwin         .match(line): osType = "Darwin"
    elif osCygwinW7_64    .match(line): osType = "Cygwin.W7_64"
    elif osCygwinW7       .match(line): osType = "Cygwin.W7"
    elif osCygwinW8_64    .match(line): osType = "Cygwin.W8_64"
    elif osCygwinW8       .match(line): osType = "Cygwin.W8"
    elif osCygwinW10_64   .match(line): osType = "Cygwin.W10_64"
    elif osCygwinW10      .match(line): osType = "Cygwin.W10"
    else:
        uname = subprocess.Popen ( ["uname", "-sr"], stdout=subprocess.PIPE )
        osType = uname.stdout.readlines()[0][:-1]

        print( "[WARNING] Unrecognized OS: \"{}\".".format( line[:-1] ))
        print( "          (using: \"{}\")".format( osType ))
    ldLibraryPath = os.getenv('LD_LIBRARY_PATH')
    if ldLibraryPath and 'devtoolset' in ldLibraryPath: useDevtoolset = False
    return ( osType, useDevtoolset )


def guessShell ():
   # This environement variable cannot be trusted as it is set once when
   # the user logs in. If aftewards it changes it that variable is *not*
   # affected :-(.
   #if os.environ.has_key('SHELL'): return os.environ['SHELL']
    psCommand     = subprocess.Popen ( ['ps', '-p', str(os.getppid()) ], stdout=subprocess.PIPE )
    shell         = psCommand.stdout.readlines()[1].decode('ascii')[:-1].split()[3].lstrip('-')
    whichCommand  = subprocess.Popen ( ['which', shell ], stdout=subprocess.PIPE )
    shellPath     = whichCommand.stdout.readlines()[0][:-1]
    isBourneShell = True
    cshBins       = [ '/usr/bin/tcsh'
                    , '/bin/tcsh'
                    , '/usr/pkg/bin/tcsh'
                    , '/usr/local/bin/tcsh'
                    , '/usr/bin/csh'
                    , '/bin/csh'
                    , '/usr/pkg/bin/csh'
                    , '/usr/local/bin/csh'
                    ]
    if shellPath in cshBins: isBourneShell = False
   #print( 'GUESSED SHELL: "{}"'.format( shellPath ))
    return shellPath, isBourneShell


if __name__ == "__main__":

  osType,useDevtoolset    = guessOs()
  buildType               = "Release"
  linkType                = "Shared"
  rootDir                 = None
  shellBin, isBourneShell = guessShell()

  parser = optparse.OptionParser()  
 # Build relateds.
  parser.add_option ( "--query-inst-root", action="store_true" ,                dest="queryInstRoot" )
  parser.add_option ( "--query-isys-root", action="store_true" ,                dest="queryISysRoot" )
  parser.add_option ( "--release"        , action="store_true" ,                dest="release"       )
  parser.add_option ( "--debug"          , action="store_true" ,                dest="debug"         )
  parser.add_option ( "--devel"          , action="store_true" ,                dest="devel"         )
  parser.add_option ( "--static"         , action="store_true" ,                dest="static"        )
  parser.add_option ( "--shared"         , action="store_true" ,                dest="shared"        )
  parser.add_option ( "--no-python"      , action="store_true" ,                dest="nopython"      )
  parser.add_option ( "--root"           , action="store"      , type="string", dest="rootDir"       )
  parser.add_option ( "--remove"         , action="store_true" ,                dest="remove"        )
  ( options, args ) = parser.parse_args()

  if options.release:    buildType = "Release"
  if options.debug:      buildType = "Debug"
  if options.devel:      buildType = "Debug"
  if options.static:     linkType  = "Static"
  if options.shared:     linkType  = "Shared"
  if options.rootDir:    rootDir   = options.rootDir

  scriptPath = os.path.abspath( os.path.dirname( sys.argv[0] ))
  if 'Debug.' in scriptPath: buildType = 'Debug'
  if not shellBin:
      print( 'echo "[ERROR] coriolisEnv.py was not able to guess/find the current shell interpeter."' )
      sys.exit( 1 )
  strippedPath        = scrubPath( "PATH" )
  strippedLibraryPath = scrubPath( "LD_LIBRARY_PATH" )
  strippedPythonPath  = scrubPath( "PYTHONPATH" )
  if options.remove:
      shellScript = 'echo "Removing Coriolis environment";'
      if osType == "Darwin":
          ldVar = 'DYLD_LIBRARY_PATH'
      else:
          ldVar = 'LD_LIBRARY_PATH'
      if isBourneShell:
          shellScript += 'export PATH={};hash -r;'.format(strippedPath)
          shellScript += 'BOOTSTRAP_TOP="";CORIOLIS_TOP="";export -n BOOTSTRAP_TOP CORIOLIS_TOP;'
          if strippedLibraryPath:
              shellScript += 'export {}={};'.format(ldVar, strippedLibraryPath)
          else:
              shellScript += '{0}=""; export -n {0};'.format(ldVar)
      else:
          shellScript += 'setenv PATH {};rehash;'.format(strippedPath)
          shellScript += 'unsetenv BOOTSTRAP_TOP CORIOLIS_TOP;'
          if strippedLibraryPath:
              shellScript += 'setenv {} {};'.format(ldVar, strippedLibraryPath)
          else:
              shellScript += 'unsetenv {};'.format(ldVar)
      print( shellScript )
      sys.exit(0)


  shellScriptSh = \
      'echo "%(MESSAGE)s";'                                          \
      'echo "Switching to Coriolis 2.x (%(buildDir)s)";'             \
      'PATH="%(PATH)s";'                                             \
      'BOOTSTRAP_TOP="%(BOOTSTRAP_TOP)s";'                           \
      'CORIOLIS_TOP="%(CORIOLIS_TOP)s";'                             \
      'export PATH BOOTSTRAP_TOP CORIOLIS_TOP STRATUS_MAPPING_NAME;'

#     'STRATUS_MAPPING_NAME="%(SYSCONF_DIR)s/stratus2sxlib.xml";'    \

  shellScriptCsh = \
      'echo "%(MESSAGE)s";'                              \
      'echo "Switching to Coriolis 2.x (%(buildDir)s)";' \
      'setenv PATH "%(PATH)s";'                          \
      'setenv BOOTSTRAP_TOP "%(BOOTSTRAP_TOP)s";'        \
      'setenv CORIOLIS_TOP "%(CORIOLIS_TOP)s";'

#     'setenv STRATUS_MAPPING_NAME "%(SYSCONF_DIR)s/stratus2sxlib.xml";' \

  reDevtoolset = re.compile( r'/opt/rh/devtoolset-(?P<version>\d+)/root/etc/coriolis2.*' )

  buildDir  = buildType + "." + linkType
  scriptDir = os.path.dirname ( os.path.abspath(__file__) )
 #print( "echo \"Script Location: {}\";".format( scriptDir ))
  if scriptDir.startswith("/etc/coriolis2"):
      coriolisTop  = "/usr"
      sysconfDir   = scriptDir
      shellMessage = "Using system-wide Coriolis 2 (/usr)"
  else:
      m = reDevtoolset.match( scriptDir )
      if m:
          coriolisTop  = "/opt/rh/devtoolset-%d/root/usr" % m.group('version')
          sysconfDir   = scriptDir
          shellMessage = "Using system-wide devtoolset-%(v)d Coriolis 2 (/opt/rh/devtoolset-%(v)d/root/usr)" \
                         % { 'v':m.group('version') }
      elif scriptDir.startswith(os.getenv("HOME")+"/nightly/coriolis-2.x/"): 
          rootDir      = os.getenv("HOME") + "/nightly/coriolis-2.x"
          coriolisTop  = "%s/%s/%s/install" % ( rootDir, osType, buildDir )
          sysconfDir   = scriptDir
          shellMessage = "Using Nightly build Coriolis 2 (%s)" % coriolisTop
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
      strippedPath = "%s/lib:%s" % ( coriolisTop, libDir, strippedPath )
  if not os.path.exists(coriolisTop):
      print( 'echo "[ERROR] coriolisEnv.py, top directory <%s> do not exists."'.format( coriolisTop ))
      sys.exit( 1 )
  for lib in [ 'lib64', 'lib' ]:
      libDir    = lib
      absLibDir = '{0}/{1}'.format( coriolisTop, lib )
      if os.path.isdir(absLibDir): break
      libDir    = None
  if libDir is None:
      print( 'echo "[ERROR] coriolisEnv.py, library directory not found."' )
      sys.exit( 1 )

  strippedPath        = "%s/bin:%s" % ( coriolisTop, strippedPath )
  strippedLibraryPath = "%s:%s"     % ( absLibDir  , strippedLibraryPath )
  if not options.nopython:
      pyVersion = sys.version_info
      version   = "%d.%d" % (pyVersion[0],pyVersion[1])
      sitePackagesDir = "sitePackageDir_has_been_not_found"
      for pyPackageDir in [ "%s/python%s.%s/site-packages" % (absLibDir,pyVersion[0],pyVersion[1])
                          , "%s/python%s.%s/dist-packages" % (absLibDir,pyVersion[0],pyVersion[1])
                          , "%s/%s.%s/site-packages"       % (absLibDir,pyVersion[0],pyVersion[1])
                          , "%s/python%s/site-packages"    % (absLibDir,pyVersion[0])
                          , "%s/python%s/dist-packages"    % (absLibDir,pyVersion[0])
                          , "%s/%s/site-packages"          % (absLibDir,pyVersion[0])
                          ]:
          if os.path.isdir(pyPackageDir):
            sitePackagesDir = pyPackageDir
            break
      strippedPythonPath = "%s:"                 % (sitePackagesDir) + strippedPythonPath
      strippedPythonPath = "%s/crlcore:"         % (sitePackagesDir) + strippedPythonPath
      strippedPythonPath = "%s/cumulus:"         % (sitePackagesDir) + strippedPythonPath
      strippedPythonPath = "%s/cumulus/plugins:" % (sitePackagesDir) + strippedPythonPath
      strippedPythonPath = "%s/stratus:"         % (sitePackagesDir) + strippedPythonPath
      strippedPythonPath = "%s:"                 % (sysconfDir)      + strippedPythonPath
      shellScriptSh  += 'PYTHONPATH="%(PYTHONPATH)s";' \
                        'export PYTHONPATH;'
      shellScriptCsh += 'setenv PYTHONPATH "%(PYTHONPATH)s";'
      if osType == "Darwin":
          shellScriptSh  += 'DYLD_LIBRARY_PATH="%(LD_LIBRARY_PATH)s";' \
                            'export DYLD_LIBRARY_PATH;'
          shellScriptCsh += 'setenv DYLD_LIBRARY_PATH "%(LD_LIBRARY_PATH)s";' 
      else:
          shellScriptSh  += 'LD_LIBRARY_PATH="%(LD_LIBRARY_PATH)s";' \
                            'export LD_LIBRARY_PATH;'
          shellScriptCsh += 'setenv LD_LIBRARY_PATH "%(LD_LIBRARY_PATH)s";'
  
  shellScriptSh  += "hash -r;"
  shellScriptCsh += "rehash;"
  if isBourneShell: shellScript = shellScriptSh
  else:             shellScript = shellScriptCsh
  if useDevtoolset:
      shellScript += \
        ' echo "Launching a devtoolset-8 subshell though scl (CTRL+D to exit).";' \
        ' scl enable devtoolset-8 %(SHELL)s'

  evalScript = shellScript % { "PATH"            : strippedPath
                             , "LD_LIBRARY_PATH" : strippedLibraryPath
                             , "PYTHONPATH"      : strippedPythonPath
                             , "BOOTSTRAP_TOP"   : coriolisTop
                             , "CORIOLIS_TOP"    : coriolisTop
                             , "SYSCONF_DIR"     : sysconfDir
                             , "MESSAGE"         : shellMessage
                             , "buildDir"        : buildDir
                             , 'SHELL'           : shellBin
                             }
  if options.queryISysRoot:
      print( '{}/{}'.format( rootDir, osType ))
      sys.exit( 0 )
  if options.queryInstRoot:
      print( coriolisTop )
      sys.exit( 0 )
  print( evalScript )
  sys.exit( 0 )
