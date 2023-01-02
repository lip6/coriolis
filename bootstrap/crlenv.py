#!/usr/bin/env python3

import sys
import os
import os.path
from   pathlib import Path
import socket
import subprocess
import re
import argparse


useDebug = False


reCoriolisPattern      = re.compile( r".*coriolis.*" )
reReleaseSharedPattern = re.compile( r".*Release\.Shared.*" )
reReleaseStaticPattern = re.compile( r".*Release\.Static.*" )
reDebugSharedPattern   = re.compile( r".*Debug\.Shared.*" )
reDebugStaticPattern   = re.compile( r".*Debug\.Static.*" )


def scrubPath ( pathName ):
    """
    Remove from the PATH like environment variable ``pathName`` any
    previous path item referring to a Coriolis location.
    """
    if not pathName in os.environ: return ''
    value    = os.environ[ pathName ]
    elements = value.split( ':' )
    scrubbed = []
    for element in elements:
        if element == '': continue
        if    reCoriolisPattern     .match(element) \
           or reReleaseSharedPattern.match(element) \
           or reReleaseStaticPattern.match(element) \
           or reDebugSharedPattern  .match(element) \
           or reDebugStaticPattern  .match(element):
            continue
        scrubbed.append( element )
    if len(scrubbed) == 0: return ''
    return ':'.join( scrubbed )


def envWriteBack ( pathName, pathValue ):
    """
    Add to the environment PATH like variable ``pathName`` the components
    given in ``pathValue`` and export it back. To avoid having multiple
    Coriolis in the path, it is scrubbed beforehand.
    """
    if pathName in os.environ:
        scrubbed = scrubPath( pathName )
        if scrubbed != '':
            pathValue += ':' + scrubbed
    os.environ[ pathName ] = pathValue
    return pathValue


def setupPaths ( verbose ):
    """
    Guess and setup the main variables to use Coriolis:

    * ``PATH``, to find the binaries.
    * ``LD_LIBRARY_PATH``, to access the dynamic libraries.
    * ``DYLD_LIBRARY_PATH``, same as above under MacOS.
    * ``PYTHONPATH``, to access the various Python modules
      provided by Coriolis.
    """
    global useDebug
    
    # Setup CORIOLIS_TOP.
    osEL9             = re.compile (".*Linux.*el9.*x86_64.*")
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

    uname = subprocess.Popen( ["uname", "-srm"], stdout=subprocess.PIPE )
    lines = uname.stdout.readlines()
    line  = lines[0].decode( 'ascii' )
    if   osSlsoc7x_64     .match(line): osDir = "Linux.el7_64"
    elif osEL9            .match(line): osDir = "Linux.el9"
    elif osSlsoc6x_64     .match(line): osDir = "Linux.slsoc6x_64"
    elif osSlsoc6x        .match(line): osDir = "Linux.slsoc6x"
    elif osSLSoC5x_64     .match(line): osDir = "Linux.SLSoC5x_64"
    elif osSLSoC5x        .match(line): osDir = "Linux.SLSoC5x"
    elif osFedora_64      .match(line): osDir = "Linux.fc_64"
    elif osFedora         .match(line): osDir = "Linux.fc"
    elif osUbuntu1004     .match(line): osDir = "Linux.Ubuntu1004"
    elif osUbuntu1004_64  .match(line): osDir = "Linux.Ubuntu1004_64"
    elif osLinux_64       .match(line): osDir = "Linux.x86_64"
    elif osLinux          .match(line): osDir = "Linux.i386"
    elif osFreeBSD8x_64   .match(line): osDir = "FreeBSD.8x.x86_64"
    elif osFreeBSD8x_amd64.match(line): osDir = "FreeBSD.8x.amd64"
    elif osFreeBSD8x      .match(line): osDir = "FreeBSD.8x.i386"
    elif osDarwin         .match(line): osDir = "Darwin"
    elif osCygwinW7_64    .match(line): osDir = "Cygwin.W7_64"
    elif osCygwinW7       .match(line): osDir = "Cygwin.W7"
    elif osCygwinW8_64    .match(line): osDir = "Cygwin.W8_64"
    elif osCygwinW8       .match(line): osDir = "Cygwin.W8"
    elif osCygwinW10_64   .match(line): osDir = "Cygwin.W10_64"
    elif osCygwinW10      .match(line): osDir = "Cygwin.W10"
    else:
        uname = subprocess.Popen( ["uname", "-sr"], stdout=subprocess.PIPE )
        osDir = uname.stdout.readlines()[0][:-1]

        print( '[WARNING] environment.setupPaths(): Unrecognized OS: "{}".'.format( line[:-1] ))
        print( '          (using: "{}")'.format( osDir ))
    osDir      = Path( osDir )
    homeDir    = Path( os.environ['HOME'] )
    buildType  = Path( 'Release.Debug' if useDebug else 'Release.Shared' )
    scriptPath = Path( __file__ ).resolve()
    topDirs    = []
    if 'CORIOLIS_TOP' in os.environ:
        topDirs += [ Path( os.environ['CORIOLIS_TOP'] ) ]
    topDirs  += [ homeDir / 'coriolis-2.x' / osDir / buildType / 'install'
                , Path( '/soc/coriolis2' ) 
                , Path( '/usr' ) 
                ]
    print( scriptPath )
    for part in scriptPath.parts:
        if part == 'nightly':
            topDirs.append( homeDir / 'nightly' / 'coriolis-2.x' / osDir / buildType / 'install' )
            break
    if verbose:
        print( '  o  Self locating Coriolis:' )
    coriolisTop = None
    for topDir in topDirs:
        if coriolisTop or not (topDir / 'bin' / 'cgt').is_file():
            if verbose: print( '     - {}'.format(topDir) )
            continue
        if verbose: print( '     - {} *'.format(topDir) )
        coriolisTop = topDir
    if not coriolisTop:
        print( '[ERROR] environment.setupPaths(): Unable to locate Coriolis.' )
        return False

    os.environ[ 'CORIOLIS_TOP' ] = coriolisTop.as_posix()
    if coriolisTop == '/usr': sysconfDir = Path( 'etc', 'coriolis2' )
    else:                     sysconfDir = coriolisTop / 'etc' / 'coriolis2'

    # Setup PATH.
    binPath = envWriteBack( 'PATH', (coriolisTop/'bin').as_posix() )

    # Setup LD_LIBRARY_PATH.
    libDirs = []
    for lib in [ Path('lib'), Path('lib64') ]:
        libDir    = lib
        absLibDir = coriolisTop / lib
        if absLibDir.is_dir():
            libDirs.append( absLibDir )
        libDir = None
    if not len(libDirs):
        print( '[ERROR] environment.setupPaths(): Library directory not found.' )
        return False
    libraryPath = ''
    ldPathName  = 'LD_LIBRARY_PATH'
    if osDir.as_posix().startswith( 'Darwin' ):
        ldPathName  = 'DYLD_LIBRARY_PATH'
    for libDir in libDirs:
        if len(libraryPath): libraryPath = libraryPath + ':'
        libraryPath = libraryPath + libDir.as_posix()
    libraryPath = envWriteBack( ldPathName, libraryPath )

    # Setup PYTHONPATH.
    v = sys.version_info
    sitePackagesDir = None
    for pyPackageDir in [ Path('python{}.{}'.format(v.major,v.minor)) / 'site-packages'
                        , Path('python{}.{}'.format(v.major,v.minor)) / 'dist-packages'
                        , Path('{}.{}'.format(v.major,v.minor)) / 'site-packages'
                        , Path('python{}'.format(v.major)) / 'site-packages'
                        , Path('python{}'.format(v.major)) / 'dist-packages'
                        , Path('{}'.format(v.major)) / 'site-packages'
                        ]:
        sitePackagesDir = libDirs[-1] / pyPackageDir
        if sitePackagesDir.is_dir():
            if verbose:
                print( '     - {} *'.format(sitePackagesDir) )
            break
        if verbose:
            print( '     - {}'.format(sitePackagesDir) )
        sitePackagesDir = None
    if sitePackagesDir is None:
        print( '[ERROR] environment.setupPaths(): Python {site,dist}-packages directory not found.' )
        return False
    pythonPath = ''
    for packageDir in [ sitePackagesDir
                      , sitePackagesDir / 'crlcore'
                      , sitePackagesDir / 'cumulus'
                      , sitePackagesDir / 'cumulus/plugins'
                      , sitePackagesDir / 'status'
                      , sysconfDir
                      ]:
        sys.path.append( str(packageDir) )
        if len(pythonPath): pythonPath += ':'
        pythonPath += str(packageDir)
    pythonPath = envWriteBack( 'PYTHONPATH', pythonPath )
    return True


def printVariable ( name ):
    if not name in os.environ:
        print( '{:<16}:'.format( name ))
        print( '- variable_not_set' )
        return
    values = os.environ[ name ].split( ':' )
    print( '{}:'.format( name ))
    for value in values:
        print( '- {}'.format( value ))


def printEnvironment ():
    """
    Display the environment setup, using YAML formatting.
    """
    print( '# crlenv.py: Alliance/Coriolis finder, guessed values.' )
    print( '---' )
    for name in ('CORIOLIS_TOP', 'PATH', 'DYLD_LIBRARY_PATH'
                , 'LD_LIBRARY_PATH', 'PYTHONPATH'):
        printVariable( name )


if __name__ == '__main__':
    """
    Run any script in a environmnent set for Coriolis.

    Example:

    .. code:: bash

       ego@home:~> crlenv.py -- doit clean_flow
       b2v          Run <blif2vst arlet6502 depends=[Arlet6502.blif]>.
       cgt          Run plain CGT (no loaded design)
       clean_flow   Clean all generated (targets) files.
       gds          Run <Alias "gds" for "pnr">.
       pnr          Run <pnr arlet6502_cts_r.gds depends=[arlet6502.vst,Arlet6502.spi]>.
       yosys        Run <yosys Arlet6502.v top=Arlet6502 blackboxes=[] flattens=[]>.
       ego@home:~> crlenv.py -- bash
       [ego@home]$ echo $CORIOLIS_TOP
       /home/ego/coriolis-2.x/Linux.el9/Release.Shared/install
       [ego@home]$ exit
       ego@home:~>
    """
    parser = argparse.ArgumentParser()  
    parser.add_argument( '-v', '--verbose', action='store_true', dest='verbose' )
    parser.add_argument( '-d', '--debug'  , action='store_true', dest='debug'   )
    parser.add_argument( 'command', nargs='*' )
    args = parser.parse_args()

    setupPaths( args.verbose )
    if not len(args.command):
        printEnvironment()
        sys.exit( 0 )
    state = subprocess.run( args.command )
    sys.exit( state.returncode )
