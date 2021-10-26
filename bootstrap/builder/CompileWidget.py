
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
# |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
# | =============================================================== |
# |  Python      :   "./builder/CompileWidget.py"                   |
# +-----------------------------------------------------------------+


import re
import subprocess
from   PyQt4.QtCore import Qt, pyqtSignal, QSettings
from   PyQt4.QtGui  import QFont, QColor, QPalette, QTextCharFormat, \
                           QWidget, QLabel, QPushButton, QCheckBox,  \
                           QGroupBox, QButtonGroup, QVBoxLayout,     \
                           QHBoxLayout, QGridLayout, QScrollArea,    \
                           QComboBox, QLineEdit, QTextEdit,          \
                           QFileDialog, QProgressBar, QApplication
from   .Highlighter import Highlighter


class CompileWidget ( QWidget ):
  
    progress = pyqtSignal(int)
  
    def __init__ ( self, parent=None ):
        QWidget.__init__ ( self, parent )
        self._options = None
        self._conf    = None
    
        self._go = QPushButton( 'Go' )
        self._go.setMaximumWidth( 100 )
        font = self._go.font()
        font.setPointSizeF( font.pointSizeF()*2.0 )
        font.setWeight    ( QFont.Bold )
        self._go.setFont( font )
        self._go.clicked.connect( self.go )
    
        self._saveLog = QPushButton( 'Save' )
        saveLogLabel  = QLabel( 'Log File:' )
        saveLogBrowse = QPushButton( '&Browse' )
        saveLogBrowse.clicked.connect( self.browseSaveLog )
        self._saveLogEdit = QLineEdit( '' )
    
        gLayout = QGridLayout()
        gLayout.addWidget( saveLogLabel     , 0, 0, 1, 1, Qt.AlignRight )
        gLayout.addWidget( self._saveLogEdit, 0, 1, 1, 6 )
        gLayout.addWidget( saveLogBrowse    , 0, 7, 1, 1 )
    
        self._console = QTextEdit()
        self._console.setLineWrapMode( QTextEdit.NoWrap )
        self._console.setMinimumSize ( 800, 400 )
        palette = self._console.palette()
        palette.setColor( QPalette.Base, QColor.fromRgb(255,255,221) ) # ffffdd.
        self._console.setPalette( palette )
        font = QFont( 'Bitstream Vera Sans Mono', self._console.font().pointSize() )
        self._console.setFont( font )
        self._highlighter = Highlighter( self._console.document() )
    
        self._progressBar = QProgressBar()
        self._progressBar.setRange      ( 0, 100 )
        self._progressBar.setTextVisible( True )
    
        hLayout = QHBoxLayout()
        hLayout.addStretch()
        hLayout.addWidget( self._go )
        hLayout.addStretch()
        hLayout.addWidget( self._saveLog )
        hLayout.addStretch()
    
        vLayout = QVBoxLayout()
        vLayout.addLayout( hLayout )
        vLayout.addLayout( gLayout )
        vLayout.addWidget( self._progressBar )
        vLayout.addWidget( self._console )
        self.setLayout( vLayout )
    
        self.progress.connect( self._progressBar.setValue )
        self._saveLog.clicked.connect( self.saveLog )
    
        self.readSettings()
        return
    
    def _setOptions ( self, options ): self._options = options
    def _setConf    ( self, conf    ): self._conf    = conf
    def _getOptions ( self ): return self._options
    def _getConf    ( self ): return self._conf
    
    options = property( _getOptions, _setOptions )
    conf    = property( _getConf   , _setConf    )
  
    def browseSaveLog ( self ):
        self._saveLogEdit.setText( QFileDialog.getSaveFileName( self
                                                              , 'Select Log File Report'
                                                              , self._saveLogEdit.text()
                                                                , 'Report Files (*.log *.txt)' ) )
        return
  
    def saveLog ( self ):
        if self._saveLogEdit.text():
            fd = open( self._saveLogEdit.text(), 'w+' )
            fd.write( self._console.toPlainText() )
            fd.close()
        return
  
    def shellCommand ( self ):
        command = [ self.conf.bootstrapDir+'/ccb.py' ]
        for project in self.options.projects:
            for tool in project.actives:
                command += [ '--tool='+tool ]
        toolsCount = len(command) - 1
        if self.conf.rootDir: command += [ '--root=%s'%self.conf.rootDir ]
       #if self.options.svnUpdate:  command += [ '--svn-update' ]
       #if self.options.svnStatus:  command += [ '--svn-update' ]
        if self.options.enableDoc:  command += [ '--doc' ]
        if self.options.devtoolset: command += [ '--devtoolset-8' ]
        if self.options.qt5:        command += [ '--qt5' ]
        if self.options.noCache:    command += [ '--no-cache' ]
        if self.options.rmBuild:    command += [ '--rm-build' ]
        if self.options.verbose:    command += [ '--verbose' ]
        if self.options.make:
            makeArguments='install '+self.options.threads
            command += [ '--make=%s'%makeArguments ]
        if self.options.buildMode == 'Debug':
            command += [ '--debug' ]
        return toolsCount, command
  
    def go ( self ):
        rePercentage  = re.compile(r'^\[\s*(?P<percent>\d+)%\].*')
        reProcessTool = re.compile(r'^Processing tool:\s*"(?P<tool>.+)"')
        if not self.options or not self.conf: return
        toolsCount, command = self.shellCommand()
        if not toolsCount: return
        self._progressBar.reset()
        self._progressBar.setRange( 0, toolsCount*100 )
    
        strCommand = command[0]
        for arg in command[1:]:
            strCommand += ' ' + arg
        strCommand += '\n\n'
        self._console.setFontItalic( True )
        self._console.insertPlainText( strCommand )
        self._console.setFontItalic( False )
    
        toolsDone      = -1
        builderProcess = subprocess.Popen( command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT )
        while True:
            line = builderProcess.stdout.readline()
            if line == '': break
            m = rePercentage.match( line )
            if m:
                self.progress.emit( toolsDone*100+int(m.group('percent')) )
            else:
                m = reProcessTool.match( line )
                if m:
                    toolsDone += 1
            self._console.insertPlainText( line )
            scrollBar = self._console.verticalScrollBar()
            scrollBar.setValue( scrollBar.maximum() )
            QApplication.processEvents()
        builderProcess.wait()
        if builderProcess.returncode == None:
            pass
        return
  
    def readSettings ( self ):
        settings = QSettings()
        self._saveLogEdit.setText( settings.value('compile/saveLog') )
        return
  
    def saveSettings ( self ):
        settings = QSettings()
        settings.setValue( 'compile/saveLog', self._saveLogEdit.text() )
        return
