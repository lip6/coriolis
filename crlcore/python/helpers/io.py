# -*- mode:Python; explicit-buffer-name: "io.py<crlcore/helpers>" -*-
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
# |  Python      :   "./crlcore/helpers/io.py"                      |
# +-----------------------------------------------------------------+

import sys
import os
import os.path
import re
import traceback
try:
  from PyQt4.QtCore import Qt
  from PyQt4.QtGui  import QSizePolicy
  from PyQt4.QtGui  import QDialog
  from PyQt4.QtGui  import QPalette
  from PyQt4.QtGui  import QColor
  from PyQt4.QtGui  import QFont
  from PyQt4.QtGui  import QFontMetrics
  from PyQt4.QtGui  import QWidget
  from PyQt4.QtGui  import QFrame
  from PyQt4.QtGui  import QLabel
  from PyQt4.QtGui  import QPixmap
  from PyQt4.QtGui  import QPushButton
  from PyQt4.QtGui  import QTextEdit
  from PyQt4.QtGui  import QVBoxLayout
  from PyQt4.QtGui  import QHBoxLayout
  from PyQt4.QtGui  import QAction
  from PyQt4.QtGui  import QKeySequence
except Exception, e:
  try:
    from PyQt5.QtCore    import Qt
    from PyQt5.QtWidgets import QSizePolicy
    from PyQt5.QtWidgets import QDialog
    from PyQt5.QtGui     import QPalette
    from PyQt5.QtGui     import QColor
    from PyQt5.QtGui     import QFont
    from PyQt5.QtGui     import QFontMetrics
    from PyQt5.QtWidgets import QWidget
    from PyQt5.QtWidgets import QFrame
    from PyQt5.QtWidgets import QLabel
    from PyQt5.QtGui     import QPixmap
    from PyQt5.QtWidgets import QPushButton
    from PyQt5.QtWidgets import QTextEdit
    from PyQt5.QtWidgets import QVBoxLayout
    from PyQt5.QtWidgets import QHBoxLayout
    from PyQt5.QtWidgets import QAction
    from PyQt5.QtGui     import QKeySequence
  except e:
    print '[ERROR] helpers.io, neither PyQt4 nor PyQt5 is available.'
    sys.exit( 1 )
import Cfg
import helpers
from   Hurricane    import UpdateSession
import Viewer


# -------------------------------------------------------------------
# Class  :  "ErrorWidget".

class ErrorWidget ( QDialog ):

  def __init__ ( self, e ):
    QWidget.__init__ ( self, None )

    self.setWindowTitle( 'Error' )

    message = QLabel( e.getLinesAsString() )
    message.setAlignment( Qt.AlignLeft )
    message.setFont( QFont('Courier',10,QFont.Bold) )

    error = QLabel( '[ERROR]' )
    error.setAlignment( Qt.AlignLeft )
    font = error.font()
    font.setWeight( QFont.Bold )
    error.setFont( font )

    self._tryCont = QPushButton()
    self._tryCont.setSizePolicy( QSizePolicy.Fixed, QSizePolicy.Fixed )
    self._tryCont.setText      ( 'Try to continue' )
    self._abort = QPushButton()
    self._abort.setSizePolicy( QSizePolicy.Fixed, QSizePolicy.Fixed )
    self._abort.setText      ( 'Abort' )
    self._abort.setDefault   ( True )

    traceFont    = QFont('Courier',10,QFont.Normal)
    lineHeight   = QFontMetrics( traceFont ).height()
    traceText    = helpers.textStackTrace( e.trace, False, e.scriptPath )
    lineCount    = traceText.count( '\n' ) + 2
    minimumWidth = 400
    if Viewer.Graphics.isHighDpi(): minimumWidth = 2100
    self._trace = QTextEdit()
    self._trace.setReadOnly    ( True );
    self._trace.setLineWrapMode( QTextEdit.NoWrap );
    self._trace.setMinimumSize ( minimumWidth, lineCount*lineHeight );
    self._trace.setFont        ( traceFont )
    self._trace.setText        ( traceText )

    buttonLayout = QHBoxLayout()
    buttonLayout.addStretch( 1 )
    buttonLayout.addWidget ( self._tryCont )
    buttonLayout.addStretch( 1 )
    buttonLayout.addWidget ( self._abort )
    buttonLayout.addStretch( 1 )

    vLayout = QVBoxLayout()
    vLayout.addWidget ( error )
    vLayout.addStretch( 1 )
    vLayout.addWidget ( message )
    vLayout.addStretch( 1 )
    vLayout.addWidget ( self._trace )
    vLayout.addStretch( 1 )
    vLayout.addLayout ( buttonLayout )

    pixmapWidth = 150
    if not Viewer.Graphics.isHighDpi(): pixmapWidth = 70
    pixmap = QPixmap( ':/images/angry-birds-red.png' )
    pixmap = pixmap.scaledToWidth( pixmapWidth )
    icon = QLabel()
    icon.setPixmap( pixmap )

    hLayout = QHBoxLayout()
    hLayout.addWidget( icon )
    hLayout.addLayout( vLayout )
    self.setLayout( hLayout )

    self._tryCont.clicked.connect( self.accept )
    self._abort  .clicked.connect( self.reject )

    self._exitAction = QAction( '&Exit', self )
    self._exitAction.setStatusTip( 'Exit Coriolis' )
    self._exitAction.setShortcut ( QKeySequence('CTRL+Q') )
    self._exitAction.triggered.connect( self.reject )
    self.addAction( self._exitAction )

    self._closeAction = QAction( '&Close', self )
    self._closeAction.setStatusTip( 'Try to continue' )
    self._closeAction.setShortcut ( QKeySequence('CTRL+W') )
    self._closeAction.triggered.connect( self.reject )
    self.addAction( self._closeAction )
    
    return  

  def closeEvent ( self, event ):
    self.setResult( QDialog.Rejected )
    event.accept()
    return


# -------------------------------------------------------------------
# Class  :  "ErrorMessage".

class ErrorMessage ( Exception ):

    def __init__ ( self, code, *arguments ):
        self.scriptPath = None
        self.trace      = traceback.extract_stack()
        self._code      = code
        self._errors    = [ 'Malformed call to ErrorMessage()'
                          , '%s' % str(arguments) ]

        text = None
        if len(arguments) == 1:
          if isinstance(arguments[0],Exception): text = str(arguments[0]).split('\n')
          else:
            self._errors = arguments[0]
        elif len(arguments) > 1:
          sys.stdout.flush()
          text = list(arguments)

        if text:
          self._errors = []
          while len(text[0]) == 0: del text[0]

          lstrip = 0
          if text[0].startswith('[ERROR]'): lstrip = 8 

          for line in text:
            if line[0:lstrip] == ' '*lstrip or \
              line[0:lstrip-1] == '[ERROR]':
              self._errors += [ line[lstrip:] ]
            else:
              self._errors += [ line.lstrip() ]
          sys.stdout.flush()
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

    def getLinesAsString ( self ):
        if not isinstance(self._errors,list): return self._errors

        lines = ''
        for line in self._errors: lines += line + '\n'
        return lines
    
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
        sys.exit( self._code )

    def _getCode ( self ): return self._code

    code = property(_getCode)

    @staticmethod
    def show ( code, *arguments ):
        e = ErrorMessage( code, *arguments )
        if Viewer.Graphics.get().isEnabled():
          tryCont = ErrorWidget( e ).exec_()
          if not tryCont: raise e 
        else:
          raise e
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

    print em
    print helpers.textStackTrace( em.trace, True, em.scriptPath )

    if Viewer.Graphics.get().isEnabled():
        tryCont = ErrorWidget( em ).exec_()

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
    return confLevel >= level


def vprint ( level, message ):
    if isVL(level): print message
    return
