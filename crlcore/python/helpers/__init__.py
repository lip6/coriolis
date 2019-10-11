# -*- mode:Python; explicit-buffer-name: "__init__.py<crlcore/helpers>" -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2012-2018, All Rights Reserved
#
# +-----------------------------------------------------------------+ 
# |                   C O R I O L I S                               |
# |    C o r i o l i s  /  C h a m s   B u i l d e r                |
# |                                                                 |
# |  Author      :                    Jean-Paul Chaput              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :   "./crlcore/__init__.py"                        |
# +-----------------------------------------------------------------+
#
# This is file is mandatory to tell python that 'helpers' is a module
# rather than an ordinary directory, thus enabling the uses of the
# 'dot' notation in import.


import sys
import os
import os.path
import re
import traceback
import Hurricane
import helpers.io

quiet          = False
sysConfDir     = None
ndaConfDir     = None
ndaDir         = None
techno         = 'symbolic/cmos'
technoDir      = None
unitsLambda    = True
tab            = None
_trace         = None
moduleGlobals  = globals()


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


def textStackTrace ( trace, showIndent=True, scriptPath=None ):
    indent = ''
    if showIndent: indent = '        '

    s = ''
    if scriptPath:
      if len(scriptPath) > 100:
        filename = scriptPath[-100:]
        filename = '.../' + filename[ filename.find('/')+1 : ]

      if showIndent: s += '[ERROR] '
      s += 'An exception occured while loading the Python script module:\n'
      s += indent + '\"%s\"\n' % (filename)
      s += indent + 'You should check for simple python errors in this module.\n\n'

    s += indent + 'Python stack trace:\n'
    maxdepth = len( trace )
    for depth in range( maxdepth ):
      filename, line, function, code = trace[ maxdepth-depth-1 ]
      if len(filename) > 58:
        filename = filename[-58:]
        filename = '.../' + filename[ filename.find('/')+1 : ]
     #s += indent + '[%02d] %45s:%-5d in \"%s()\"' % ( maxdepth-depth-1, filename, line, function )
      s += indent + '#%d in %25s() at %s:%d\n' % ( depth, function, filename, line )
    return s


def showStackTrace ( trace ):
    print textStackTrace( trace, True )
    return


def textPythonTrace ( scriptPath=None, e=None, tryContinue=True ):
    s = ''
    if scriptPath:
      if len(scriptPath) > 100:
        filename = scriptPath[-100:]
        filename = '.../' + filename[ filename.find('/')+1 : ]
      else:
        filename = scriptPath
      s += '[ERROR] An exception occured while loading the Python script module:\n'
      s += '        \"%s\"\n' % (filename)
      s += '        You should check for simple python errors in this module.\n'

    if isinstance(e,helpers.io.ErrorMessage): trace = e.trace()
    else:                                     trace = sys.exc_info()[2]
    s += textStackTrace( traceback.extract_tb( trace ) )

    if e:
      s += '        Error was:\n'
      s += '          %s\n' % e

    if tryContinue:
      s += '        Trying to continue anyway...'
    return s


def showPythonTrace ( scriptPath=None, e=None, tryContinue=True ):
    print textPythonTrace( scriptPath, e, tryContinue ) 
    return


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
                sys.stdout.write("\n%s"%self._header)

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
      sys.stderr.flush()

    for f in sflags[1]:
      if f == '+': self._tab.inc()
      if f == '-': self._tab.dec()
    return


def trace ( *args ):
  global _trace
  _trace.write( *args )
  return


def setTraceLevel ( level ):
  global _trace
  _trace.level = level
  return


def overload ( defaultParameters, parameters ):
  overloads          = {}
  overloadParameters = []
  
  for parameter in parameters:
    overloads[ parameter[0] ] = parameter
  
  for parameter in defaultParameters:
    if overloads.has_key(parameter[0]):
      overloadParameters.append( overloads[parameter[0]] )
    else:
      overloadParameters.append( parameter )
        
  return tuple(overloadParameters)


def l ( value ): return Hurricane.DbU.fromLambda( value )
def u ( value ): return Hurricane.DbU.fromPhysical( value, Hurricane.DbU.UnitPowerMicro )
def n ( value ): return Hurricane.DbU.fromPhysical( value, Hurricane.DbU.UnitPowerNano  )


def initTechno ( argQuiet ):
  global quiet
  global ndaDir
  global ndaConfDir
  global technoDir
  global techno

  quiet = argQuiet

  technoFiles  = [ sysConfDir+'/techno.conf' ]
  if os.getenv('HOME'):
    technoFiles += [ os.getenv('HOME')+'/.coriolis2/techno.py' ]
  technoFiles += [ os.getcwd()+'/.coriolis2/techno.py' ]

  technoFiles.reverse()
  for technoFile in technoFiles:
    if os.path.isfile(technoFile):
      if not quiet: print '     - Loading \"%s\".' % truncPath(technoFile)
      execfile(technoFile,moduleGlobals)
      break
  if moduleGlobals.has_key('technology'):
    techno = technology
  else:
    print '[WARNING] The technology is not set. Using <%s>.' % techno

  if moduleGlobals.has_key('NdaDirectory'):
    ndaDir     = NdaDirectory
    ndaConfDir = os.path.join( NdaDirectory, 'etc/coriolis2' )
  else:
    ndaConfDir = sysConfDir

  technoDir = os.path.join( ndaConfDir, techno )
  if not quiet: print '     - Technology: %s.' % techno


def staticInitialization ( quiet=False ):
  global sysConfDir
  global technoDir
  global tab
  global _trace
  global unitsLambda

  if sysConfDir != None:
   #if not quiet: print '  o  helpers.staticInitialization() Already run, exit.'
    return

  unitsLamba = True
  tab        = Tab()
  _trace     = Trace()
  
  reSysConfDir = re.compile(r'.*etc\/coriolis2')
  if not quiet: print '  o  Locating configuration directory:'
  
  for path in sys.path:
    if reSysConfDir.match(path):
      sysConfDir = path
      if not quiet: print '     - "%s"' % sysConfDir
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
  
  if not quiet: print '     - "%s"' % sysConfDir
  initTechno( quiet )
  return


def netDirectionToStr ( netDir ):
   flags = [ '-', '-', '-', '-', '-' ]
   if netDir & Hurricane.Net.Direction.DirIn:        flags[0] = 'i'
   if netDir & Hurricane.Net.Direction.DirOut:       flags[1] = 'o'
   if netDir & Hurricane.Net.Direction.ConnTristate: flags[2] = 't'
   if netDir & Hurricane.Net.Direction.ConnWiredOr:  flags[3] = 'w'

   s = flags[0]+flags[1]+flags[2]+flags[3]+' '
   if   netDir == Hurricane.Net.Direction.UNDEFINED: s += '(UNDEFINED)'
   elif netDir == Hurricane.Net.Direction.IN:        s += '(IN)'
   elif netDir == Hurricane.Net.Direction.OUT:       s += '(OUT)'
   elif netDir == Hurricane.Net.Direction.INOUT:     s += '(INOUT)'
   elif netDir == Hurricane.Net.Direction.TRISTATE:  s += '(TRISTATE)'
   elif netDir == Hurricane.Net.Direction.TRANSCV:   s += '(TRANSCV)'
   elif netDir == Hurricane.Net.Direction.WOR_OUT:   s += '(WOR_OUT)'
   elif netDir == Hurricane.Net.Direction.WOR_INOUT: s += '(WOR_INOUT)'
   else: s += '(UNKNOWN)'
   return s
