
# This file is part of the Coriolis Software.
# Copyright (c) Sorbonne Université 2014-2021, All Rights Reserved
#
# +-----------------------------------------------------------------+
# |                   C O R I O L I S                               |
# |      C u m u l u s  -  P y t h o n   T o o l s                  |
# |                                                                 |
# |  Author      :                    Jean-Paul CHAPUT              |
# |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
# | =============================================================== |
# |  Python      :       "./plugins/aboutwindow.py"                 |
# +-----------------------------------------------------------------+


import sys
import traceback
pyQtIsEnabled = True
try:
    from PyQt4.QtCore import Qt
    from PyQt4.QtCore import QEventLoop
    from PyQt4.QtGui  import QDialog
    from PyQt4.QtGui  import QPalette
    from PyQt4.QtGui  import QColor
    from PyQt4.QtGui  import QFont
    from PyQt4.QtGui  import QWidget
    from PyQt4.QtGui  import QFrame
    from PyQt4.QtGui  import QLabel
    from PyQt4.QtGui  import QVBoxLayout
    from PyQt4.QtGui  import QAction
    from PyQt4.QtGui  import QKeySequence
    from PyQt4.QtGui  import QApplication
except:
    try:
        from PyQt5.QtCore    import Qt
        from PyQt5.QtCore    import QEventLoop
        from PyQt5.QtWidgets import QDialog
        from PyQt5.QtGui     import QPalette
        from PyQt5.QtGui     import QColor
        from PyQt5.QtGui     import QFont
        from PyQt5.QtWidgets import QWidget
        from PyQt5.QtWidgets import QFrame
        from PyQt5.QtWidgets import QLabel
        from PyQt5.QtWidgets import QVBoxLayout
        from PyQt5.QtWidgets import QAction
        from PyQt5.QtGui     import QKeySequence
        from PyQt5.QtWidgets import QApplication
    except:
        print( '[WARNING] AboutWindow: Neither PyQt4 nor PyQt5 is available, disabling AboutWidget.' )
        pyQtIsEnabled = False
import Viewer
import helpers
from   helpers.io   import ErrorMessage
from   helpers.io   import WarningMessage
import plugins


# --------------------------------------------------------------------
# Class : "AboutWidget".


def getAboutWidget ():
    aboutText  =  'Coriolis CAD System 1.0 . . . . . . . . . . . . . .  ccb 1.0\n' \
                  'Copyright (c) 2008-2022 . . . . . . . .  Sorbonne Université\n' \
                  'Authors . . . . . . . . . . . . . . . . Christophe Alexandre\n' \
                  '        . . . . . . . . . . . . . . . . . .  Sophie Belloeil\n' \
                  '        . . . . . . . . . . . . . . . . . . Jean-Paul Chaput\n' \
                  '        . . . . . . . . . . . . . . . . . . .  Damien Dupuis\n' \
                  '        . . . . . . . . . . . . . . . . . . .  Remy Escassut\n' \
                  '        . . . . . . . . . . . . . . . . . . Christian Masson\n' \
                  'E-Mail  . . . . . . . . . . . . . . Jean-Paul.Chaput@lip6.fr'

    if pyQtIsEnabled:

        class AboutWidget ( QDialog ):
        
            def __init__ ( self, parent=None ):
                QWidget.__init__ ( self, parent )
               #self.setFixedSize( 500, 400 )
               #self.setStyleSheet( 'background-color: #ffffdd;' )
              
                topLine = QFrame()
                topLine.setFrameShape( QFrame.HLine )
                topLine.setLineWidth ( 2 )
                botLine = QFrame()
                botLine.setFrameShape( QFrame.HLine )
                botLine.setLineWidth ( 2 )
              
                title = QLabel( 'CGT' )
                title.setAlignment( Qt.AlignCenter )
                font = title.font()
                font.setPointSize( 72 )
                font.setWeight   ( QFont.Bold )
                title.setFont( font )
              
                subTitle = QLabel( 'Coriolis Graphical Interface' )
                subTitle.setAlignment( Qt.AlignCenter )
                subTitle.setFont( QFont('Courier',10,QFont.Bold) )
              
                authors  = QLabel( aboutText )
                authors.setAlignment( Qt.AlignCenter )
                authors.setFont( QFont('Courier',10,QFont.Bold) )
              
                vLayout = QVBoxLayout()
                vLayout.addStretch(10)
                vLayout.addWidget( topLine )
                vLayout.addWidget( title )
                vLayout.addStretch(1)
                vLayout.addWidget( subTitle )
                vLayout.addWidget( authors )
                vLayout.addStretch(1)
                vLayout.addWidget( botLine )
                vLayout.addStretch(10)
              
                frame = QFrame()
                frame.setFrameShape ( QFrame.Box )
                frame.setFrameShadow( QFrame.Sunken )
                frame.setLayout     ( vLayout )
                frame.setLineWidth  ( 1 )
              
                vLayout = QVBoxLayout()
                vLayout.addWidget( frame )
                
                self.setLayout( vLayout )
              
                self._exitAction = QAction( '&Exit', self )
                self._exitAction.setStatusTip( 'Exit CCB (settings are saved)' )
                self._exitAction.setShortcut ( QKeySequence('CTRL+Q') )
               #self._exitAction.triggered.connect( QApplication.closeAllWindows )
                self._exitAction.triggered.connect( self.reject )
                self.addAction( self._exitAction )
              
                self._closeAction = QAction( '&Close', self )
                self._closeAction.setStatusTip( 'Close the About Window' )
                self._closeAction.setShortcut ( QKeySequence('CTRL+A') )
               #self._closeAction.triggered.connect( self.close )
                self._closeAction.triggered.connect( self.accept )
                self.addAction( self._closeAction )

    else:

        class AboutWidget ( object ):

            def __init__ ( self ):
                self.aboutText = aboutText
                
    return AboutWidget


# --------------------------------------------------------------------
# Plugin hook functions, unicornHook:menus, ScritMain:call

def unicornHook ( **kw ):
    plugins.kwUnicornHook( 'file.About'
                         , 'About Coriolis'
                         , 'Informations about the Coriolis Software'
                         , sys.modules[__name__].__file__
                         , **kw
                         )
    return


def scriptMain ( **kw ):
    try:
       #helpers.setTraceLevel( 550 )
        aboutWidget = getAboutWidget()()
        if not pyQtIsEnabled:
            print( aboutWidget.aboutText )
            return True
        answer = aboutWidget.exec_()
        print( 'answer:', answer )
        if not answer: return True
    except Exception as e:
        helpers.io.catch( e )
    sys.stdout.flush()
    sys.stderr.flush()
    return True
