#!/usr/bin/env python3
#
# -*- mode:Python -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2008-2021, All Rights Reserved
#
# +-----------------------------------------------------------------+ 
# |                   C O R I O L I S                               |
# |          T o o l c h a i n   B u i l d e r                      |
# |                                                                 |
# |  Authors     :                    Jean-Paul Chaput              |
# |                                      Damien Dupuis              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :   "./ccb.py"                                     |
# +-----------------------------------------------------------------+

showTrace = True

try:
    import sys
    import os.path
    import optparse
    import traceback
    import distutils.sysconfig
    import subprocess
    import re
except ImportError as e:
    module = str(e).split()[-1]
    print( '[ERROR] The "{}" python module or symbol cannot be loaded.'.format(module) )
    print( '        Please check your standard Python installation, it may have problems.' )
    quit()


def safeImport ( moduleName, symbol=None ):
    try:
        module = __import__( moduleName, globals(), locals(), symbol )
    except ImportError as e:
        print( '[ERROR] The "{}" python module or symbol cannot be loaded.'.format(moduleName) )
        print( '        Please check the integrity of the "coriolis/boostrap" package.' )
        if showTrace: traceback.print_tb(sys.exc_info()[2])
        sys.exit(1)
    except Exception as e:
        print( '[ERROR] An exception occured while importing module "{}". Is is a bug,'.format(moduleName) )
        print( '        you may want to report it...' )
        print( '        {}'.format(e) )
        if showTrace: traceback.print_tb(sys.exc_info()[2])
        sys.exit(2)
    if symbol: return module.__dict__[symbol]
    return module


def checkCMake ():
    child = subprocess.Popen ( ["which", "cmake"], stdout=subprocess.PIPE, stderr=subprocess.PIPE )
    (pid,status) = os.waitpid ( child.pid, 0 )
    status >>= 8
    if status != 0:
        print( '[ERROR] The "cmake" program has not been found, please install it.' )
        sys.exit(1)


def guessOs ():
    libDir            = 'lib'
    osEL9             = re.compile (".*Linux.*(el9|al9).*x86_64.*")
    osSlsoc7x_64      = re.compile (".*Linux.*(el7|slsoc7).*x86_64.*")
    osSlsoc6x_64      = re.compile (".*Linux.*(el6|slsoc6).*x86_64.*")
    osSlsoc6x         = re.compile (".*Linux.*(el6|slsoc6).*")
    osSLSoC5x_64      = re.compile (".*Linux.*el5.*x86_64.*")
    osSLSoC5x         = re.compile (".*Linux.*(el5|2.6.23.13.*SoC).*")
    osFedora_64       = re.compile (".*Linux.*fc.*x86_64.*")
    osFedora          = re.compile (".*Linux.*fc.*")
    osLinux_64        = re.compile (".*Linux.*x86_64.*")
    osLinux           = re.compile (".*Linux.*")
    osFreeBSD8x_amd64 = re.compile (".*FreeBSD 8.*amd64.*")
    osFreeBSD8x_64    = re.compile (".*FreeBSD 8.*x86_64.*")
    osFreeBSD8x       = re.compile (".*FreeBSD 8.*")
    osDarwin          = re.compile (".*Darwin.*")
    osCygwinW7_64     = re.compile (".*CYGWIN_NT-6\.1.*x86_64.*")
    osCygwinW7        = re.compile (".*CYGWIN_NT-6\.1.*i686.*")
    osCygwinW8_64     = re.compile (".*CYGWIN_NT-6\.[2-3].*x86_64.*")
    osCygwinW8        = re.compile (".*CYGWIN_NT-6\.[2-3].*i686.*")
    osCygwinW10_64    = re.compile (".*CYGWIN_NT-10\.[0-3].*x86_64.*")
    osCygwinW10       = re.compile (".*CYGWIN_NT-10\.[0-3].*i686.*")

    uname = subprocess.Popen ( ["uname", "-srm"], stdout=subprocess.PIPE )
    lines = uname.stdout.readlines()
    line  = lines[0].decode( 'ascii' )
    if osEL9.match(line):
        osType = "Linux.el9"
        libDir = "lib64"
    elif osSlsoc7x_64.match(line):
        osType = "Linux.el7_64"
        libDir = "lib64"
    elif osSlsoc6x_64.match(line):
        osType = "Linux.slsoc6x_64"
        libDir = "lib64"
    elif osSlsoc6x.match(line):
        osType = "Linux.slsoc6x"
    elif osSLSoC5x_64.match(line):
        osType = "Linux.SLSoC5x_64"
        libDir = "lib64"
    elif osSLSoC5x .match(line):
        osType = "Linux.SLSoC5x"
    elif osFedora_64.match(line):
        osType = "Linux.fc_64"
        libDir = "lib64"
    elif osFedora .match(line):
        osType = "Linux.fc"
    elif osLinux_64.match(line):
        osType = "Linux.x86_64"
        libDir = "lib64"
    elif osLinux .match(line):
        osType = "Linux.i386"
    elif osDarwin.match(line):
        osType = "Darwin"
    elif osFreeBSD8x_amd64.match(line):
        osType = "FreeBSD.8x.amd64"
        libDir = "lib64"
    elif osFreeBSD8x_64.match(line):
        osType = "FreeBSD.8x.x86_64"
        libDir = "lib64"
    elif osFreeBSD8x.match(line):
        osType = "FreeBSD.8x.i386"
    elif osCygwinW7_64.match(line):
        osType = "Cygwin.W7_64"
        libDir = "lib64"
    elif osCygwinW7.match(line):
        osType = "Cygwin.W7"
    elif osCygwinW8_64.match(line):
        osType = "Cygwin.W8_64"
        libDir = "lib64"
    elif osCygwinW8.match(line):
        osType = "Cygwin.W8"
    elif osCygwinW10_64.match(line):
        osType = "Cygwin.W10_64"
        libDir = "lib64"
    elif osCygwinW10.match(line):
        osType = "Cygwin.W10"
    else:
        uname = subprocess.Popen ( ["uname", "-sr"], stdout=subprocess.PIPE )
        osType = uname.stdout.readlines()[0][:-1]
        print( '[WARNING] Unrecognized OS: "{}".'.format(lines[0][:-1]) )
        print( '          (using: "{}")'.format(osType) )
    return osType, libDir


def guessPythonSitePackage ():
    pathes = distutils.sysconfig.get_python_lib().split('/')[-2:]
    return os.path.join( pathes[0], pathes[1] )


def autoLocate ():
    osType, libDir = guessOs()
    print( 'Building for target: "{}"'.format(osType) )
    print( 'Making an educated guess to locate myself:' )
    sitePackage = guessPythonSitePackage()
    builderDir = None
    locations  = [ os.path.abspath(os.path.dirname(sys.argv[0]))
                 , os.environ['HOME']+'/coriolis-2.x/src/coriolis/bootstrap'
                 , os.environ['HOME']+'/coriolis/src/coriolis/bootstrap'
                 , '/users/outil/coriolis/coriolis-2.x/src/coriolis/bootstrap'
                 , os.environ['HOME']+'/coriolis-2.x/'+osType+'/Release.Shared/install/'+libDir+'/'+sitePackage
                 , '/users/outil/coriolis/coriolis-2.x/'+osType+'/Release.Shared/install/'+libDir+'/'+sitePackage
                 ]
    for location in locations:
      print( '  "{}" '.format(location), end='' )
      if os.path.isfile(location + '/builder/__init__.py'):
          if not builderDir:
              builderDir = location
              print( '(Found*)' )
          else:
              print( '(Found)' )
      else:
          print( '(No)' )
    if not builderDir:
      print( '[ERROR] Failed to locate the builder modules in any of the normal pathes.' )
      print( '        Please check your Coriolis/Bootsrap installation.' )
      if showTrace: traceback.print_tb(sys.exc_info()[2])
      sys.exit(1)
    sys.path.insert( 0, builderDir )
    return


# ------------------------------------------------------------------- 
# CCB Main Part.

autoLocate()
checkCMake()

parser = optparse.OptionParser ()  
parser.add_option ( "-g", "--gui"            , action="store_true" ,                dest="gui"            , help="Lauch the graphical interface." )
# Build relateds.                                                                                         
parser.add_option ( "-c", "--conf"           ,                       type="string", dest="conf"           , help="Fichier de configuration." )
parser.add_option (       "--show-conf"      , action="store_true" ,                dest="showConf"       , help="Display the Project/Tools configuration, then exit." )
parser.add_option ( "-q", "--quiet"          , action="store_true" ,                dest="quiet"          , help="Do not print all the informative messages." )
parser.add_option ( "-r", "--release"        , action="store_true" ,                dest="release"        , help="Build a <Release> aka optimized version." )
parser.add_option ( "-d", "--debug"          , action="store_true" ,                dest="debug"          , help="Build a <Debug> aka (-g) version." )
parser.add_option ( "-s", "--static"         , action="store_true" ,                dest="static"         , help="Try to link statically, as much as possible." )
parser.add_option (       "--doc"            , action="store_true" ,                dest="doc"            , help="Enable the documentation building (uses with -j1)." )
parser.add_option ( "-v", "--verbose"        , action="store_true" ,                dest="verboseMakefile", help="Tells CMake to print all compilation commands." )
parser.add_option (       "--root"           , action="store"      , type="string", dest="rootDir"        , help="The root directory (default: <~/coriolis-2.x/>)." )
parser.add_option (       "--no-build"       , action="store_true" ,                dest="noBuild"        , help="Do *not* build anything (by default: build)." )
parser.add_option (       "--no-cache"       , action="store_true" ,                dest="noCache"        , help="Remove previous CMake cache before building." )
parser.add_option (       "--rm-build"       , action="store_true" ,                dest="rmBuild"        , help="Remove previous build directoty before building." )
parser.add_option (       "--macports"       , action="store_true" ,                dest="macports"       , help="Build against MacPorts." )
parser.add_option (       "--devtoolset"     , action="store"      , type="int"   , dest="devtoolset"     , help="Build against TUV Dev Toolset N." )
parser.add_option (       "--llvm-toolset"   , action="store"      , type="int"   , dest="llvmtoolset"    , help="Build against TUV Dev LLVM Toolset N." )
parser.add_option (       "--qt5"            , action="store_true" ,                dest="qt5"            , help="Build against Qt 5 (default: Qt 4)." )
parser.add_option (       "--bfd"            , action="store_true" ,                dest="bfd"            , help="Build against Qt 5 (default: Qt 4)." )
parser.add_option (       "--openmp"         , action="store_true" ,                dest="openmp"         , help="Enable the use of OpenMP in Gcc." )
parser.add_option (       "--ninja"          , action="store_true" ,                dest="ninja"          , help="Use Ninja instead of UNIX Makefile." )
parser.add_option (       "--clang"          , action="store_true" ,                dest="clang"          , help="Force use of Clang C/C++ compiler instead of system default." )
parser.add_option (       "--make"           , action="store"      , type="string", dest="makeArguments"  , help="Arguments to pass to make (ex: \"-j4 install\")." )
parser.add_option (       "--project"        , action="append"     , type="string", dest="projects"       , help="The name of a project to build (may appears any number of time)." )
parser.add_option ( "-t", "--tool"           , action="append"     , type="string", dest="tools"          , help="The name of a tool to build (may appears any number of time)." )
# SVN repository relateds.                
# Have to be ported to Git.               
#parser.add_option ( "--svn-tag"              , action="store"      , type="string", dest="svnTag"         , help="Explicitly select a SVN tag (for SVN related commands)." )
#parser.add_option ( "--svn-method"           , action="store"      , type="string", dest="svnMethod"      , help="Allows to sets the svn checkout method (svn+ssh://coriolis.soc.lip6.fr)." )
#parser.add_option ( "--svn-status"           , action="store_true"                , dest="svnStatus"      , help="Check status against the SVN repository." )
#parser.add_option ( "--svn-diff"             , action="store_true"                , dest="svnDiff"        , help="Perform a diff against the SVN repository." )
#parser.add_option ( "--svn-update"           , action="store_true"                , dest="svnUpdate"      , help="Update to the latest SVN version *or* the one given by svn-tag." )
#parser.add_option ( "--svn-checkout"         , action="store_true"                , dest="svnCheckout"    , help="Checkout the latest SVN version *or* the one given by svn-tag." )
# Miscellaneous.                                                 
parser.add_option ( "--user-tarball"         , action="store_true"                , dest="userTarball"    , help="Regenerate a tarball from checked out sources (in <root>/tarball/." )
parser.add_option ( "--tarball"              , action="store_true"                , dest="tarball"        , help="Regenerate a tarball (in <root>/tarball/." )
parser.add_option ( "--rpm"                  , action="store_true"                , dest="doRpm"          , help="Regenerate RPM packages." )
parser.add_option ( "--deb"                  , action="store_true"                , dest="doDeb"          , help="Regenerate Debian/Ubuntu packages." )
# Katabatic/Kite specific options.                                                                        
parser.add_option ( "-D", "--check-db"       , action="store_true"                , dest="checkDb"        , help="Enable Katabatic/Kite data-base checking (*very* slow)." )
parser.add_option ( "-u", "--check-deter"    , action="store_true"                , dest="checkDeterminism", help="Enable Katabatic/Kite determinism checking (*very* slow)." )
(options, args) = parser.parse_args ()

if options.gui:
    ErrorMessage = safeImport( 'builder'           , 'ErrorMessage' )
    QApplication = safeImport( 'PyQt4.QtGui'       , 'QApplication' )
    BuilderGui   = safeImport( 'builder.BuilderGui', 'BuilderGui'   )
    try:
        app = QApplication( sys.argv )
        app.setOrganizationName  ( 'Sorbonne Universite' )
        app.setOrganizationDomain( 'lip6.fr' )
        app.setApplicationName   ( 'CoriolisBuilder' )
        gui = BuilderGui( options.conf )
        gui.show()
        rcode = app.exec_()
        sys.exit( rcode )
    except ErrorMessage as e:
        print( e )
        if showTrace: traceback.print_tb(sys.exc_info()[2])
        sys.exit(2)
    except Exception as e:
        print( '[ERROR] An exception occured while running the Qt application.' )
        print( '        {}'.format(e) )
        if showTrace: traceback.print_tb(sys.exc_info()[2])
        sys.exit(2)
else:
    ErrorMessage = safeImport( 'builder'        , 'ErrorMessage' )
    Builder      = safeImport( 'builder.Builder', 'Builder' )
    try:
        builder = Builder()
        builder.loadConfiguration( options.conf )
        if options.showConf:
            builder.showConfiguration ()
            sys.exit(0)
        if options.quiet:                        builder.quiet             = True
        if options.release:                      builder.buildMode         = "Release"
        if options.debug:                        builder.buildMode         = "Debug"
        if options.static:                       builder.enableShared      = "OFF"
        if options.doc:                          builder.enableDoc         = "ON"
        if options.checkDb:                      builder.checkDatabase     = "ON"
        if options.checkDeterminism:             builder.checkDeterminism  = "ON"
        if options.verboseMakefile:              builder.verboseMakefile   = "ON"
        if options.rootDir:                      builder.rootDir           = options.rootDir
        if options.noBuild:                      builder.doBuild           = False
        if options.noCache:                      builder.noCache           = True
        if options.rmBuild:                      builder.rmBuild           = True
        if options.ninja:                        builder.ninja             = True
        if options.clang or options.llvmtoolset: builder.clang             = True
        if options.macports:                     builder.macports          = True
        if options.devtoolset:                   builder.devtoolset        = options.devtoolset
        if options.llvmtoolset:                  builder.llvmtoolset       = options.llvmtoolset
        if options.bfd:                          builder.bfd               = "ON"
        if options.qt5:                          builder.qt5               = True
        if options.openmp:                       builder.openmp            = True
        if options.makeArguments:                builder.makeArguments     = options.makeArguments
       #if options.svnMethod:                    builder.svnMethod         = options.svnMethod
       #if options.svnTag:                       builder.svnTag            = options.svnTag
       #if   options.svnStatus:   builder.svnStatus   ( tools=options.tools, projects=options.projects )
       #elif options.svnUpdate:   builder.svnUpdate   ( tools=options.tools, projects=options.projects )
       #elif options.svnDiff:     builder.svnDiff     ( tools=options.tools, projects=options.projects )
       #elif options.svnCheckout: builder.svnCheckout ( tools=options.tools, projects=options.projects )
        if   options.userTarball: builder.userTarball ( tools=options.tools, projects=options.projects )
        elif options.tarball:     builder.svnTarball  ( tools=options.tools, projects=options.projects )
        elif options.doRpm:       builder.doRpm       ()
        elif options.doDeb:       builder.doDeb       ()
        else:                     builder.build       ( tools=options.tools, projects=options.projects )
    except ErrorMessage as e:
        print( e )
        if showTrace: traceback.print_tb(sys.exc_info()[2])
        sys.exit(e.code)
    except KeyboardInterrupt as e:
        print( '\n[ERROR] Interrupted by user\'s request (CTRL+C)' )
        sys.exit(1)
sys.exit(0)
