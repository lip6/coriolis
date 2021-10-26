
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
# |  Python      :   "./builder/BuilderGui.py"                      |
# +-----------------------------------------------------------------+


from PyQt4.QtGui      import QTabWidget, QApplication, QMainWindow, \
                             QAction , QKeySequence
from .OptionsWidget   import OptionsWidget
from .CompileWidget   import CompileWidget
from .ConfigureWidget import ConfigureWidget
from .AboutWidget     import AboutWidget


class BuilderGui ( QMainWindow ):

    def __init__ ( self, confFile, parent=None ):
        QMainWindow.__init__( self, parent )
        self.setWindowTitle( 'Coriolis Toolchain Builder' )
        self._tabWidget       = QTabWidget()
        self._configureWidget = ConfigureWidget(confFile)
        self._optionsWidget   = OptionsWidget(self._configureWidget.conf)
        self._compileWidget   = CompileWidget()
        self._aboutWidget     = AboutWidget()
    
        self._tabWidget.addTab( self._optionsWidget  , 'Options' )
        self._tabWidget.addTab( self._compileWidget  , 'Compile' )
        self._tabWidget.addTab( self._configureWidget, 'Configure' )
        self.setCentralWidget( self._tabWidget )
    
        self._compileWidget.conf    = self._configureWidget
        self._compileWidget.options = self._optionsWidget
    
        self._exitAction = QAction( '&Exit', self )
        self._exitAction.setStatusTip( 'Exit CCB (settings are saved)' )
        self._exitAction.setShortcut ( QKeySequence('CTRL+Q') )
        self._exitAction.triggered.connect( QApplication.closeAllWindows )
    
        self._saveAction = QAction( '&Save Settings', self )
        self._saveAction.setStatusTip( 'Save Settings' )
        self._saveAction.setShortcut ( QKeySequence('CTRL+S') )
        self._saveAction.triggered.connect( self._configureWidget.saveSettings )
        self._saveAction.triggered.connect( self._optionsWidget.saveSettings )
        self._saveAction.triggered.connect( self._compileWidget.saveSettings )
    
        self._aboutAction = QAction( '&About', self )
        self._aboutAction.setStatusTip( 'A Word About Who\'s Responsible for This Thing' )
        self._aboutAction.setShortcut ( QKeySequence('CTRL+A') )
        self._aboutAction.triggered.connect( self._aboutWidget.show )
    
        fileMenu = self.menuBar().addMenu( 'File' )
        fileMenu.addAction( self._exitAction )
        fileMenu.addAction( self._saveAction )
        fileMenu.addAction( self._aboutAction )
        return
  
    def closeEvent ( self, event ):
         self._configureWidget.saveSettings()
         self._optionsWidget  .saveSettings()
         self._compileWidget  .saveSettings()
         event.accept()
         return 
