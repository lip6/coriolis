
# -*- mode:Python -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2012-2021, All Rights Reserved
#
# +-----------------------------------------------------------------+ 
# |                   C O R I O L I S                               |
# |          Alliance / Hurricane  Interface                        |
# |                                                                 |
# |  Author      :                    Jean-Paul Chaput              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :   "./crlcore/helpers/__init__.py"                |
# +-----------------------------------------------------------------+
#
# This is file is mandatory to tell python that 'helpers' is a module
# rather than an ordinary directory, thus enabling the uses of the
# 'dot' notation in import.

import sys
import os
import os.path
import re
import math
import traceback

quiet          = False
sysConfDir     = None
ndaConfDir     = None
ndaDir         = None
techno         = 'symbolic/cmos'
technoDir      = None
moduleGlobals  = globals()
sysModules     = set()
confModules    = set()

if not sysModules:
    for moduleName in sys.modules.keys():
        sysModules.add( moduleName )

from ..Hurricane import DbU, DataBase, Net
from ..Viewer    import Graphics
from ..CRL       import AllianceFramework
from .io         import ErrorMessage, WarningMessage


def irange ( value ):
    if isinstance(value,int): return range(value)
    if isinstance(value,float):
        r,i = math.modf( value )
        if r != 0.0:
            print( '[WARNING] helpers.irange(): value={} is not an integer (truncated).'\
                   .format(value) )
    return range(int( value ))


def stype ( o ): return str(type(o)).split("'")[1]


def isderived ( derived, base ):
    btype = base.mro()[0]
    for dtype in derived.mro():
        if dtype == btype: return True
    return False


def truncPath ( path, maxlength=80 ):
    if len(path) < maxlength: return path
    components = path.split(os.sep)
    trunc      = ''
    for i in range(-1,-len(components),-1):
        if len(trunc)+len(components[i]) >= maxlength: break
        if not len(trunc): trunc = components[i]
        else:              trunc = os.path.join( components[i], trunc ) 
    return '...' + os.sep + trunc


class Dots ( object ):

    def __init__ (self, header="", width=73):
        self._header = header
        self._width  = width - len(self._header)
        self._count  = 0
        return

    def dot (self):
        if self._count == 0:
            sys.stdout.write(self._header)
        else:
            if not (self._count % self._width):
                sys.stdout.write("\n{}".format(self._header))

        sys.stdout.write(".")
        sys.stdout.flush()
        self._count += 1
        return

    def flush (self):
        sys.stdout.write("\n")
        sys.stdout.flush()
        return

    def reset (self):
        self._count = 0
        self.flush()
        return


class Tab ( object ):

    def __init__ ( self, stepSize=2 ):
        self._stepSize = stepSize
        self._tsize    = 0
        return
    
    def inc ( self, step=1 ): self._tsize += step*self._stepSize

    def dec ( self, step=1 ):
        if step*self._stepSize < self._tsize:
            self._tsize -= step*self._stepSize
        else:
            self._tsize  = 0
        return
    
    def __str__ ( self ): return ' '*self._tsize


class Trace ( object ):

    def __init__ ( self ):
        self._tab   = Tab()
        self._level = 10000
        return
  
    @property
    def level ( self ): return self._level
    @level.setter
    def level ( self, level ): self._level = level
  
    def write ( self, level, arg1='', arg2=None ):
        if level < self._level: return
        sys.stdout.flush()
        sflags  = [ '', '' ]
        message = None
        if isinstance(arg1,str) and len(arg1) and arg1[0] in '+-,':
            sflags  = arg1.split( ',' , 1 )
            if len(sflags) < 2: sflags.append('')
            message = arg2
        else:
            message = arg1
        for f in sflags[0]:
            if f == '+': self._tab.inc()
            if f == '-': self._tab.dec()
        if message:
            if not isinstance(message,str):
                message = '\t'+str(message)+'\n'
            if len(message) and message[0] == '\t':
                sys.stderr.write( str(self._tab) )
                sys.stderr.write( message[1:] )
            else:
                sys.stderr.write( message )
        for f in sflags[1]:
            if f == '+': self._tab.inc()
            if f == '-': self._tab.dec()
        sys.stderr.flush()
        return


def trace ( *args ):
    global _trace
    _trace.write( *args )
    return


def setTraceLevel ( level ):
    global _trace
    _trace.level = level
    return


def dots ( ttyWidth, leftText, rightText ):
    dotWidth = ttyWidth - len(leftText) - len(rightText) - 2
    if dotWidth < 0: dotWidth = 0
    print( '{}{}{}'.format(leftText,'.'*dotWidth,rightText) )


def overload ( defaultParameters, parameters ):
    overloads          = {}
    overloadParameters = []
    for parameter in parameters:
        overloads[ parameter[0] ] = parameter
    for parameter in defaultParameters:
        if parameter[0] in overloads:
            overloadParameters.append( overloads[parameter[0]] )
        else:
            overloadParameters.append( parameter )
    return tuple(overloadParameters)


def l ( value ): return DbU.fromLambda( value )
def u ( value ): return DbU.fromPhysical( value, DbU.UnitPowerMicro )
def n ( value ): return DbU.fromPhysical( value, DbU.UnitPowerNano  )


def onFGrid ( u ):
    oneGrid = DbU.fromGrid( 1.0 )
    if u % oneGrid:
        u -= (u % oneGrid)
    return u


def initTechno ( argQuiet ):
    global quiet
    global ndaDir
    global ndaConfDir
    global technoDir
    global techno
    
    quiet        = argQuiet
    technoFiles  = [ sysConfDir+'/techno.conf' ]
    if os.getenv('HOME'):
        technoFiles += [ os.getenv('HOME')+'/.coriolis2/techno.py' ]
    technoFiles += [ os.getcwd()+'/.coriolis2/techno.py' ]
    technoFiles.reverse()
    for technoFile in technoFiles:
        if os.path.isfile(technoFile):
            if not quiet: print( '     - Loading "{}".'.format( truncPath(technoFile) ))
            exec(open( technoFile ).read())  # moduleGlobals
            break
    if 'technology' in moduleGlobals:
        techno = technology
    else:
        print( '[WARNING] The technology is not set. Using "{}".'.format( techno ))
    if 'NdaDirectory' in moduleGlobals:
        ndaDir     = NdaDirectory
        ndaConfDir = os.path.join( NdaDirectory, 'etc/coriolis2' )
    else:
        ndaConfDir = sysConfDir
    technoDir = os.path.join( ndaConfDir, techno )
    if not quiet: print( '     - Technology: {}.'.format( techno ))

  
unitsLambda = True
tab         = Tab()
_trace      = Trace()
ndaTopDir   = None


def setNdaTopDir ( ndaTopDirArg ):
    global ndaTopDir

    if not os.path.isdir(ndaTopDirArg):
        print( WarningMessage( 'helpers.setNdaTopDir(): Directory "{}" does not exists.'.format( ndaTopDirArg )))
    else:
        ndaTopDir = ndaTopDirArg
        sys.path.append( os.path.join(ndaTopDir,'etc/coriolis2') )
        sys.path.append( ndaTopDir )
    return


def staticInitialization ( quiet=True ):
    global sysConfDir
    global technoDir
    global tab
    global _trace
    global unitsLambda
    
    if sysConfDir != None:
        return
    reSysConfDir = re.compile(r'.*etc\/coriolis2')
    if not quiet: print( '  o  Locating configuration directory:' )
    for path in sys.path:
        if reSysConfDir.match(path):
            sysConfDir = path
            if not quiet: print( '     - "{}"'.format( sysConfDir ))
            break
    if not sysConfDir:
        coriolisTop = os.getenv('CORIOLIS_TOP')
        if coriolisTop == '/usr':
            sysConfDir = '/etc/coriolis2'
        elif coriolisTop:
            sysConfDir = os.path.join(coriolisTop,'etc/coriolis2')
        else:
            raise ErrorMessage( 1, [ 'Cannot locate the directoty holding the configuration files.'
                                   , 'The path is something ending by <.../etc/coriolis2>.'] )
    if not quiet: print( '     - "{}"'.format( sysConfDir ))
    initTechno( quiet )
    return


def setSysConfDir ( quiet=False ):
    global sysConfDir
  
    if sysConfDir != None:
        return
    reSysConfDir = re.compile(r'.*etc\/coriolis2')
    if not quiet: print( '  o  Locating configuration directory:' )
    for path in sys.path:
        if reSysConfDir.match(path):
            sysConfDir = path
            if not quiet: print( '     - "{}"'.format( sysConfDir ))
            break
    if not sysConfDir:
        coriolisTop = os.getenv('CORIOLIS_TOP')
        if coriolisTop == '/usr':
            sysConfDir = '/etc/coriolis2'
        elif coriolisTop:
            sysConfDir = os.path.join(coriolisTop,'etc/coriolis2')
        else:
            raise ErrorMessage( 1, [ 'Cannot locate the directoty holding the configuration files.'
                                   , 'The path is something ending by <.../etc/coriolis2>.'] )
    if not quiet: print( '     - "{}"'.format( sysConfDir ))
    sys.path.append( sysConfDir )
    return


def netDirectionToStr ( netDir ):
    flags = [ '-', '-', '-', '-', '-' ]
    if netDir & Net.Direction.DirIn:        flags[0] = 'i'
    if netDir & Net.Direction.DirOut:       flags[1] = 'o'
    if netDir & Net.Direction.ConnTristate: flags[2] = 't'
    if netDir & Net.Direction.ConnWiredOr:  flags[3] = 'w'
    
    s = flags[0]+flags[1]+flags[2]+flags[3]+' '
    if   netDir == Net.Direction.UNDEFINED: s += '(UNDEFINED)'
    elif netDir == Net.Direction.IN:        s += '(IN)'
    elif netDir == Net.Direction.OUT:       s += '(OUT)'
    elif netDir == Net.Direction.INOUT:     s += '(INOUT)'
    elif netDir == Net.Direction.TRISTATE:  s += '(TRISTATE)'
    elif netDir == Net.Direction.TRANSCV:   s += '(TRANSCV)'
    elif netDir == Net.Direction.WOR_OUT:   s += '(WOR_OUT)'
    elif netDir == Net.Direction.WOR_INOUT: s += '(WOR_INOUT)'
    else: s += '(UNKNOWN)'
    return s


setSysConfDir( False )


def unloadUserSettings ():
    global confModules

    print( '  o  Unloading Python user\'s modules.' )

    for moduleName in confModules:
        refcount = sys.getrefcount( sys.modules[moduleName] )
        warning  = ''
        if refcount > 3:
            warning = '(NOTE: More than 3 refcount %d)' % refcount
           #print( WarningMessage( [ 'Configuration module "{}" has more than 3 references ({})".' \
           #                       .format(moduleName,refcount)
           #                       , 'May be unable to unload it from the Python process.'
           #                       ] ))
        print( '     - {:-34} {:-35}'.format( '"{}".'.format(moduleName), warning ))
        del sys.modules[ moduleName ]
    confModules = set()
    return


def loadUserSettings ():
    if DataBase.getDB() is not None:
        print( '  o  DataBase already initialized, skip loading of "./coriolis2/settings.py".' )
        return True
    rvalue        = False
    if os.path.isfile('./coriolis2/settings.py'):
        if os.path.isfile('./coriolis2/__init__.py'):
            sys.path.insert( 0, os.getcwd() )
            import coriolis2.settings
            rvalue = True
        else:
            print( WarningMessage( [ 'User\'s settings directory "{}" exists, but do not contains "__init__.py".'.format( './coriolis2/' )
                                   , '(path:"{}")'.format( os.path.abspath(os.getcwd()) )
                                   ] ))
    else:
      import coriolis.technos.symbolic.cmos
    tagConfModules()
    return rvalue


def tagConfModules ():
    global sysModules
    global confModules

    confModules = set()
    for moduleName in sys.modules.keys():
        if not (moduleName in sysModules):
            confModules.add( moduleName )


def resetCoriolis ():
    print( '  o  Full reset of Coriolis/Hurricane databases.' )
    AllianceFramework.get().destroy()
    Graphics.get().clear()
    DataBase.getDB().destroy()
    unloadUserSettings()
    return
