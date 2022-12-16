
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
# |  Python      :   "./crlcore/helpers/io.py"                      |
# +-----------------------------------------------------------------+

import sys
import os
import os.path
import re
import traceback
import Cfg
import helpers
from   Hurricane import UpdateSession
import Viewer


# -------------------------------------------------------------------
# Class  :  "ErrorMessage".

class ErrorMessage ( Exception ):

    def __init__ ( self, code, *arguments ):
        self.scriptPath = None
        self.trace      = traceback.extract_stack()
        self.code       = code
        self.errors     = [ 'Malformed call to ErrorMessage().'
                          , 'args:"{}"'.format(arguments) ]

        if not isinstance(self.code,int):
            self.errors = [ 'Malformed call to ErrorMessage(), first argument (code) must be an integer.'
                          , 'code:"{}"'.format(code)
                          , 'args:"{}"'.format(arguments) ]
            return
          
        text = None
        if len(arguments) == 1:
            if isinstance(arguments[0],Exception): text = str(arguments[0]).split('\n')
            else:
                self.errors = arguments[0]
        elif len(arguments) > 1:
            sys.stdout.flush()
            text = list(arguments)
        if text:
            self.errors = []
            while len(text) == 0: del text[0]
            
            lstrip = 0
            if text[0].startswith('[ERROR]'): lstrip = 8 
            
            for line in text:
                if line[0:lstrip] == ' '*lstrip or \
                    line[0:lstrip-1] == '[ERROR]':
                    self.errors += [ line[lstrip:] ]
                else:
                    self.errors += [ line.lstrip() ]
            sys.stdout.flush()
        return

    def __str__ ( self ):
        if not isinstance(self.errors,list):
            return "[ERROR] %s" % self.errors
        formatted = "\n"
        for i in range(len(self.errors)):
            if i == 0: formatted += "[ERROR] %s" % self.errors[i]
            else:      formatted += "        %s" % self.errors[i]
            if i+1 < len(self.errors): formatted += "\n"
        return formatted

    def getLinesAsString ( self ):
        if not isinstance(self.errors,list): return self.errors
        lines = ''
        for line in self.errors: lines += line + '\n'
        return lines
    
    def addMessage ( self, message ):
        if not isinstance(self.errors,list):
            self.errors = [ self.errors ]
        if isinstance(message,list):
            for line in message:
                self.errors += [ line ]
        else:
            self.errors += [ message ]
        return

    def terminate ( self ):
        print( self )
        sys.exit( self.code )

    @staticmethod
    def show ( code, *arguments ):
        e = ErrorMessage( code, *arguments )
        if not Viewer.Graphics.get().isEnabled():
            raise e
        tryCont = Viewer.ErrorWidget.run( e.getLinesAsString()
                                        , helpers.textStackTrace( e.trace, False, e.scriptPath ))
        if not tryCont: raise e 
        return


# -------------------------------------------------------------------
# Function  :  "catch()".
#
# Try to smartly display any exception on the TTY and the graphic
# display, if available.

def catch ( errorObject ):
    if isinstance(errorObject,ErrorMessage):
        em = errorObject
    else:
        em            = ErrorMessage( 2, errorObject )
        em.trace      = traceback.extract_tb( sys.exc_info()[2] )
       #em.scriptPath = __file__
    print( em )
    print( helpers.textStackTrace( em.trace, True, em.scriptPath ))
    if Viewer.Graphics.get().isEnabled():
        Viewer.ErrorWidget.run( em.getLinesAsString()
                              , helpers.textStackTrace( em.trace, False, em.scriptPath ))
    if UpdateSession.getStackSize() > 0: UpdateSession.close()
    return


# -------------------------------------------------------------------
# Class  :  "WarningMessage".

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


# -------------------------------------------------------------------
# Function  :  "vprint()".
#
# Small wrap around print to make use of the verbosity levels.

def isVL ( level ):
    confLevel = 0
    if Cfg.getParamBool('misc.verboseLevel1').asBool(): confLevel = 1
    if Cfg.getParamBool('misc.verboseLevel2').asBool(): confLevel = 2
   #print( 'level {} <= confLevel {}'.format(level,confLevel))
    return level <= confLevel


def vprint ( level, message ):
    if isVL(level): print( message )
    return
