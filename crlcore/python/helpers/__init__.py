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

quiet          = False
sysConfDir     = None
ndaConfDir     = None
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


def truncPath ( path, maxlength=70 ):
    if len(path) < maxlength: return path
    return '.../'+os.sep.join(path.split(os.sep)[-4:])


def showPythonTrace ( scriptPath=None, e=None, tryContinue=True ):
    if scriptPath:
      print '[ERROR] An exception occured while loading the Python script module:'
      print '        \"%s\"\n' % (scriptPath)
      print '        You should check for simple python errors in this module.\n'

    print '        Python stack trace:'
    trace    = traceback.extract_tb( sys.exc_info()[2] )
    maxdepth = len( trace )
    for depth in range( maxdepth ):
      filename, line, function, code = trace[ maxdepth-depth-1 ]
      if len(filename) > 38:
        filename = filename[-38:]
        filename = '.../' + filename[ filename.find('/')+1 : ]
     #print '        [%02d] %45s:%-5d in \"%s()\"' % ( maxdepth-depth-1, filename, line, function )
      print '        #%d in %25s() at %s:%d' % ( depth, function, filename, line )

    if e:
      print '        Error was:'
      print '          %s\n' % e

    if tryContinue:
      print '        Trying to continue anyway...'
    return


class ErrorMessage ( Exception ):

    def __init__ ( self, code, *arguments ):
        self._code   = code
        self._errors = [ 'Malformed call to ErrorMessage()'
                       , '%s' % str(arguments) ]

        text = None
        if len(arguments) == 1:
            if isinstance(arguments[0],Exception): text = str(arguments[0]).split('\n')
            else:
                self._errors = arguments[0]
        elif len(arguments) > 1:
            text = list(arguments)

        if text:
            self._errors = []
            while len(text[0]) == 0: del text[0]

            lstrip = 0
            if text[0].startswith('[ERROR]'): lstrip = 8 

            for line in text:
                if line[0:lstrip  ] == ' '*lstrip or \
                   line[0:lstrip-1] == '[ERROR]':
                    self._errors += [ line[lstrip:] ]
                else:
                    self._errors += [ line.lstrip() ]
        return

    def __str__ ( self ):
        if not isinstance(self._errors,list):
            return "[ERROR] %s" % self._errors

        formatted = "\n"
        for i in range(len(self._errors)):
            if i == 0: formatted += "[ERROR] %s" % self._errors[i]
            else:      formatted += "        %s" % self._errors[i]
            if i+1 < len(self._errors): formatted += "\n"
        return formatted

    def addMessage ( self, message ):
        if not isinstance(self._errors,list):
            self._errors = [ self._errors ]
        if isinstance(message,list):
            for line in message:
                self._errors += [ line ]
        else:
            self._errors += [ message ]
        return

    def terminate ( self ):
        print self
        sys.exit(self._code)

    def _getCode ( self ): return self._code

    code = property(_getCode)

    @staticmethod
    def wrapPrint ( e, footer=None ):
        showTrace = False
        if not isinstance(e,ErrorMessage):
            if isinstance(e,Hurricane.ConstructorError) or \
               isinstance(e,Hurricane.HurricaneError):
                ewrap = ErrorMessage(1,e)
            else:
                ewrap = ErrorMessage(3,'An unmanaged Python exception occurred:')
                ewrap.addMessage(str(e))
                showTrace = True
        else:
            ewrap = e
        if footer: ewrap.addMessage(footer)
        print ewrap
        if showTrace: showPythonTrace()
        return


class WarningMessage ( Exception ):

    def __init__ ( self, message ):
        self._warnings = message
        return

    def __str__ ( self ):
        if not isinstance(self._warnings,list):
            return "[WARNING] %s" % self._warnings

        formatted = "\n"
        for i in range(len(self._warnings)):
            if i == 0: formatted += "[WARNING] %s" % self._warnings[i]
            else:      formatted += "          %s" % self._warnings[i]
            if i+1 < len(self._warnings): formatted += "\n"
        return formatted


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
      if not quiet: print '          - Loading \"%s\".' % truncPath(technoFile)
      execfile(technoFile,moduleGlobals)
      break
  if moduleGlobals.has_key('technology'):
    techno = technology
  else:
    print '[WARNING] The technology is not set. Using <%s>.' % techno

  if moduleGlobals.has_key('NdaDirectory'):
    ndaConfDir = os.path.join( NdaDirectory, 'etc/coriolis2' )
  else:
    ndaConfDir = sysConfDir

  technoDir = os.path.join( ndaConfDir, techno )
  if not quiet: print '          - Technology: %s.' % techno


def staticInitialization ( quiet=False ):
  global sysConfDir
  global technoDir
  global tab
  global _trace
  global unitsLambda

  if sysConfDir != None: return

  unitsLamba = True
  tab        = Tab()
  _trace     = Trace()
  
  reSysConfDir = re.compile(r'.*etc\/coriolis2')
  if not quiet: print '  o  Locating configuration directory:'
  
  for path in sys.path:
    if reSysConfDir.match(path):
      sysConfDir = path
     #if not quiet: print '     - <%s>' % sysConfDir
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
  
  if not quiet: print '     - <%s>' % sysConfDir
  initTechno( quiet )
  return
