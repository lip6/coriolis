#!/usr/bin/env python3
#
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2015-2022, All Rights Reserved
#
# +-----------------------------------------------------------------+ 
# |                   C O R I O L I S                               |
# |     C o r i o l i s  -  Generic Program Launcher                |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :  "./src/coriolis.py"                             |
# +-----------------------------------------------------------------+


import sys
import os.path
import optparse
import subprocess
import distutils.sysconfig


class Location ( object ):
    SetCoriolisTop    = 0x0001
    SetLdLibraryPath  = 0x0002
    SetBasePythonPath = 0x0004
    BaseSysconfDir    = 0x0008
    BaseSystem        = SetCoriolisTop|BaseSysconfDir
    Devtoolset2       = SetCoriolisTop|BaseSysconfDir|SetBasePythonPath
    UserDefined       = SetCoriolisTop|SetLdLibraryPath|SetBasePythonPath


def truncPath ( path, ifirst, ilast ):
    rawDirectories = path.split('/')
    directories    = []
    for directory in rawDirectories:
        if len(directory): directories.append( directory )
    truncated = os.path.join( *directories[ifirst:ilast] )
    if ifirst == 0 and rawDirectories[0] == '':
        truncated = '/'+truncated
    return truncated


def uname ( arguments ):
    return subprocess.Popen( ["uname"] + arguments
                           , stdout=subprocess.PIPE ).stdout.readlines()[0][:-1]


class Pathes ( object ):

    def __init__ ( self, name ):
        self.name       = name
        self.components = []
        if os.environ.has_key(name):
            self.components = os.environ[name].split(':')
        return

    def asString ( self ):
        s = ''
        for i in range(len(self.components)):
            if s: s += ':'
            s += self.components[i]
        return s

    def export ( self ):
        os.environ[self.name] = self.asString()
        return

    def insert ( self, index, path ):
        self.components.insert( index, path )
        return

    def show ( self ):
        print '     %s:' % self.name
        for component in self.components:
          print '       %s' % component
        return


osType = uname( ['-s'] )
arch   = uname( ['-m'] )

libDir = '/lib'
if arch == 'x86_64' and os.path.exists('/usr/lib64'): libDir = '/lib64'

pythonSitePackages = os.path.join( *(distutils.sysconfig.get_python_lib(1).split('/')[-3:]) )

print '     ========================================'
print '     OS:\n       %s' % osType

scriptBinPath = os.path.abspath(os.path.dirname(sys.argv[0]))
print '     Script location:\n       %s' % scriptBinPath
if scriptBinPath == '/usr/bin':
    location    = Location.BaseSystem
    coriolisTop = '/usr'
    print '     Using standard system installation scheme.'
elif scriptBinPath == '/soc/coriolis2/bin':
    location    = Location.Devtoolset2
    coriolisTop = '/soc/coriolis2'
    print '     Using RHEL6 installation scheme.'
    ldLibraryPath = os.getenv('LD_LIBRARY_PATH')
    if not 'devtoolset' in ldLibraryPath:
        print '[ERROR] You must enable the devtoolset-2 before running Coriolis:'
        print '        > scl enable devtoolset-2 bash'
        sys.exit( 1 )
else:
    location    = Location.UserDefined
    coriolisTop = truncPath( scriptBinPath, 0, -1 ) 
    print '     Using User installation scheme.'

if location & Location.SetCoriolisTop:
    os.environ['CORIOLIS_TOP'] = coriolisTop
    print '     CORIOLIS_TOP:\n       %s' % coriolisTop

if location & Location.BaseSysconfDir:
    sysConfDir = truncPath( coriolisTop, 0, -1 ) + '/etc/coriolis2'
else:
    sysConfDir = coriolisTop + '/etc/coriolis2'
print '     Configuration directory:\n       %s' % sysConfDir

os.environ['STRATUS_MAPPING_NAME'] = sysConfDir+'/stratus2sxlib.xml'

binPath = Pathes( 'PATH' )
binPath.insert( 0, scriptBinPath )
binPath.export()
binPath.show()

ldLibraryPathName = 'LD_LIBRARY_PATH'
if osType == 'Darwin':
  ldLibraryPathName = 'DYLD_LIBRARY_PATH'

if location & Location.SetLdLibraryPath:
    ldLibraryPath = Pathes( ldLibraryPathName )
    ldLibraryPath.insert( 0, coriolisTop+libDir )
    ldLibraryPath.export()
    ldLibraryPath.show()

pythonPath = Pathes( 'PYTHONPATH' )
if location & Location.SetBasePythonPath:
    pythonPath.insert( 0, os.path.join(coriolisTop,pythonSitePackages) )
pythonPath.insert( 0, os.path.join(coriolisTop,pythonSitePackages,'crlcore') )
pythonPath.insert( 0, os.path.join(coriolisTop,pythonSitePackages,'stratus') )
pythonPath.insert( 0, os.path.join(coriolisTop,pythonSitePackages,'cumulus') )
pythonPath.insert( 0, os.path.join(coriolisTop,pythonSitePackages,'oroshi' ) )
pythonPath.export()
pythonPath.show()


argvStart   = 0
slaveScript = 'cgt'
if len(sys.argv) > 1 and sys.argv[1].startswith('--run='):
    argvStart   = 1
    slaveScript = sys.argv[1][6:]

print '     Script:\n       %s' % slaveScript
print '     ========================================'

try:
    os.execvp( slaveScript, sys.argv[argvStart:] )
except Exception, e:
    print '[ERROR] An exception occured while lauching <%s>\n' % slaveScript
    print e
    sys.exit( 3 )

sys.exit( 0 )
