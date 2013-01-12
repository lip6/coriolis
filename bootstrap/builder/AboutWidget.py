
# -*- mode:Python -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC/LIP6 2012-2012, All Rights Reserved
#
# +-----------------------------------------------------------------+ 
# |                   C O R I O L I S                               |
# |    C o r i o l i s  /  C h a m s   B u i l d e r                |
# |                                                                 |
# |  Author      :                    Jean-Paul Chaput              |
# |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
# | =============================================================== |
# |  Python      :   "./builder/AboutWidget.py"                     |
# +-----------------------------------------------------------------+


from   PyQt4.QtCore           import Qt
from   PyQt4.QtGui            import QPalette
from   PyQt4.QtGui            import QColor
from   PyQt4.QtGui            import QFont
from   PyQt4.QtGui            import QWidget
from   PyQt4.QtGui            import QFrame
from   PyQt4.QtGui            import QLabel
from   PyQt4.QtGui            import QVBoxLayout
from   PyQt4.QtGui            import QAction
from   PyQt4.QtGui            import QKeySequence
from   PyQt4.QtGui            import QApplication


class AboutWidget ( QWidget ):

  def __init__ ( self, parent=None ):
    QWidget.__init__ ( self, parent )
    self.setFixedSize( 500, 400 )
    self.setStyleSheet( 'background-color: #ffffdd;' )

    topLine = QFrame()
    topLine.setFrameShape( QFrame.HLine )
    topLine.setLineWidth ( 2 )
    botLine = QFrame()
    botLine.setFrameShape( QFrame.HLine )
    botLine.setLineWidth ( 2 )

    title = QLabel( 'CCB' )
    title.setAlignment( Qt.AlignCenter )
    font = title.font()
    font.setPointSize( 72 )
    font.setWeight   ( QFont.Bold )
    title.setFont( font )

    subTitle = QLabel( 'Coriolis & Chams Builder for the Dummies' )
    subTitle.setAlignment( Qt.AlignCenter )
    subTitle.setFont( QFont('Courier',10,QFont.Bold) )

    authors  = QLabel( 'Coriolis CAD System 1.0 . . . . . . . .  ccb 1.0\n'
                       'Copyright (c) 2008-2013 . . . . . . . . . . UPMC\n'
                       'Authors . . . . . . . . . . . . .  Damien Dupuis\n'
                       '        . . . . . . . . . . . . Jean-Paul Chaput\n'
                       'E-Mail  . . . . . . . . Jean-Paul.Chaput@lip6.fr'
                     )
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
    self._exitAction.triggered.connect( QApplication.closeAllWindows )
    self.addAction( self._exitAction )

    self._closeAction = QAction( '&Close', self )
    self._closeAction.setStatusTip( 'Close the About Window' )
    self._closeAction.setShortcut ( QKeySequence('CTRL+A') )
    self._closeAction.triggered.connect( self.close )
    self.addAction( self._closeAction )
    
    return
