# -*- mode:Python; explicit-buffer-name: "__init__.py<crlcore/helpers>" -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2012-2015, All Rights Reserved
#
# +-----------------------------------------------------------------+ 
# |                   C O R I O L I S                               |
# |    C o r i o l i s  /  C h a m s   B u i l d e r                |
# |                                                                 |
# |  Author      :                    Jean-Paul Chaput              |
# |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
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

sysConfDir     = None
symbolicTechno = 'cmos'
symbolicDir    = None
realTechno     = 'hcmos9gp'
realDir        = None
tab            = None
_trace         = None
moduleGlobals  = globals()


def stype ( o ): return str(type(o)).split("'")[1]


def truncPath ( path ):
  if len(path) < 70: return path
  return '.../'+os.sep.join(path.split(os.sep)[-4:])


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
        if showTrace: traceback.print_tb(sys.exc_info()[2])
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


def initTechno ( quiet ):
  global realDir
  global realTechno
  global symbolicDir
  global symbolicTechno

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
  if moduleGlobals.has_key('symbolicTechnology'):
    symbolicTechno = symbolicTechnology
  else:
    print '[WARNING] The symbolic technology name is not set. Using <cmos>.' 
  if moduleGlobals.has_key('realTechnology'):
    realTechno = realTechnology
  else:
    print '[WARNING] The real technology name is not set. Using <hcmos9gp>.' 

  symbolicDir = os.path.join( sysConfDir, symbolicTechno )
  realDir     = os.path.join( sysConfDir, realTechno )
  if not quiet: print '          - Technologies: %s+%s.' % (symbolicTechno,realTechno)


def staticInitialization ( quiet=False ):
  global sysConfDir
  global symbolicDir
  global realDir
  global tab
  global _trace

  if sysConfDir != None: return

  tab    = Tab()
  _trace = Trace()
  
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
