# -*- mode:Python -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC 2012-2014, All Rights Reserved
#
# +-----------------------------------------------------------------+ 
# |                   C O R I O L I S                               |
# |    C o r i o l i s  /  C h a m s   B u i l d e r                |
# |                                                                 |
# |  Author      :                    Jean-Paul Chaput              |
# |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
# | =============================================================== |
# |  Python      :   "./builder/__init__.py"                        |
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
xmlCompatMode  = False
symbolicTechno = 'cmos'
symbolicDir    = None
realTechno     = 'hcmos9'
realDir        = None


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


def staticInitialization ():
  global sysConfDir
  global symbolicDir
  global realDir
  
  reSysConfDir = re.compile(r'.*etc\/coriolis2')
  print '  o  Locating configuration directory:'
  
  for path in sys.path:
    if reSysConfDir.match(path):
      sysConfDir = path
      print '     - <%s>' % sysConfDir
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
  
  print '     - <%s>' % sysConfDir
  symbolicDir = os.path.join( sysConfDir, symbolicTechno )
  realDir     = os.path.join( sysConfDir, realTechno )
  return
