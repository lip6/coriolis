
# -*- mode:Python -*-
#
# This is file is mandatory to tell python that 'helpers' is a module
# rather than an ordinary directory, thus enabling the uses of the
# 'dot' notation in import.

import sys
import os
import os.path
import traceback
import Hurricane


sysConfDir    = os.sep.join(os.path.abspath(__file__).split(os.sep)[:-2])
xmlCompatMode = False


def stype ( o ): return str(type(o)).split("'")[1]


def truncPath ( path ):
  if len(path) < 70: return path
  return '.../'+os.sep.join(path.split(os.sep)[-4:])


class ErrorMessage ( Exception ):

    def __init__ ( self, message ):
        self._errors = []

        if isinstance(message,Exception):
            text = str(message).split('\n')

            while len(text[0]) == 0: del text[0]

            lstrip = 0
            if text[0].startswith('[ERROR]'): lstrip = 8 

            for line in text:
                if line[0:lstrip  ] == ' '*lstrip or \
                   line[0:lstrip-1] == '[ERROR]':
                    self._errors += [ line[lstrip:] ]
                else:
                    self._errors += [ line.lstrip() ]
        else:
            self._errors = message
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

    @staticmethod
    def wrapPrint ( e, footer=None ):
        showTrace = False
        if not isinstance(e,ErrorMessage):
            if isinstance(e,Hurricane.ConstructorError) or \
               isinstance(e,Hurricane.HurricaneError):
                ewrap = ErrorMessage(e)
            else:
                ewrap = ErrorMessage('An unmanaged Python exception occurred:')
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
