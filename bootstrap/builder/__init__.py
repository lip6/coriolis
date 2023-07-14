
# -*- mode:Python -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2012-2021, All Rights Reserved
#
# +-----------------------------------------------------------------+ 
# |                   C O R I O L I S                               |
# |          T o o l c h a i n   B u i l d e r                      |
# |                                                                 |
# |  Author      :                       Damien Dupuis              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :   "./builder/__init__.py"                        |
# +-----------------------------------------------------------------+

import sys


class ErrorMessage ( Exception ):

    def __init__ ( self, code, *arguments ):
        self._code   = code
        self._errors = [ 'Malformed call to ErrorMessage()', '{}'.format(arguments) ]

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
            if i == 0: formatted += "[ERROR] {}".format(self._errors[i] )
            else:      formatted += "        {}".format(self._errors[i] )
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
        print( self )
        sys.exit(self._code)

    def _getCode ( self ): return self._code

    code = property(_getCode)
