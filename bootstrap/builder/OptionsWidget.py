
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
# |  Python      :   "./builder/OptionsWidget.py"                   |
# +-----------------------------------------------------------------+


import re
import subprocess
from   PyQt4.QtCore             import Qt, pyqtSignal, QSettings
from   PyQt4.QtGui              import QColor, QWidget, QPushButton,          \
                                       QCheckBox, QGroupBox, QButtonGroup,    \
                                       QVBoxLayout, QHBoxLayout, QGridLayout, \
                                       QScrollArea, QComboBox
from   .Project         import Project
from   .ConfigureWidget import ConfigureWidget
from   .ProjectWidgets  import ProjectWidgets


class OptionsWidget ( QWidget ):
  
    progress = pyqtSignal(int)
  
    def __init__ ( self, conf, parent=None ):
        QWidget.__init__ ( self, parent )
        self._conf = conf
        self._projects = []
        for project in self._conf.projects:
            self._projects += [ ProjectWidgets(project) ]
        
        gLayout = QGridLayout()
        column  = 0
        for iproject in range(len(self._projects)):
            column += self._projects[iproject].addToLayout( column, gLayout )
        toolsGroup = QGroupBox( 'Projects && Tools' )
        toolsGroup.setLayout( gLayout )
    
        scrollToolsGroup = QScrollArea()
        scrollToolsGroup.setMinimumHeight( 350 )
       #scrollToolsGroup.setVerticalScrollBarPolicy( Qt.ScrollBarAlwaysOn )
        scrollToolsGroup.setWidget( toolsGroup )
    
        self._buildMode = QComboBox()
        self._buildMode.addItems( ('Release', 'Debug') )
       #self._svnUpdate   = QCheckBox( 'SVN Update' )
       #self._svnStatus   = QCheckBox( 'SVN Status' )
        self._make        = QCheckBox( 'Build' )
        self._enableDoc   = QCheckBox( 'Build Documentation' )
        self._devtoolset  = QCheckBox( 'Build with devtoolset 8' )
        self._qt4         = QCheckBox( 'Build with Qt 4 (Qt 5 default)' )
        self._noCache     = QCheckBox( 'Remove previous CMake cache' )
        self._rmBuild     = QCheckBox( 'Cleanup Build Directory' )
        self._verbose     = QCheckBox( 'Display Compiler Commands' )
        self._threads     = QComboBox()
        for j in range(16):
            self._threads.addItem( '-j%d'%(j+1), j+1 )
    
        self._commandGroup = QButtonGroup()
        self._commandGroup.setExclusive( True )
       #self._commandGroup.addButton( self._svnUpdate )
       #self._commandGroup.addButton( self._svnStatus )
        self._commandGroup.addButton( self._make )
    
        vLayout = QVBoxLayout()
       #vLayout.addWidget( self._svnUpdate )
       #vLayout.addWidget( self._svnStatus )
        vLayout.addWidget( self._make )
        vLayout.addStretch()
        commandGroup = QGroupBox( 'Command' )
        commandGroup.setLayout( vLayout )
    
        vLayout = QVBoxLayout()
        vLayout.addWidget( self._buildMode )
        vLayout.addWidget( self._enableDoc )
        vLayout.addWidget( self._devtoolset )
        vLayout.addWidget( self._qt4 )
        vLayout.addWidget( self._noCache )
        vLayout.addWidget( self._rmBuild )
        vLayout.addStretch()
        optionsGroup = QGroupBox( 'Command Options' )
        optionsGroup.setLayout( vLayout )
    
        vLayout = QVBoxLayout()
        vLayout.addWidget( self._threads )
        vLayout.addWidget( self._verbose )
        vLayout.addStretch()
        miscGroup = QGroupBox( 'Misc. Options' )
        miscGroup.setLayout( vLayout )
    
        hLayout = QHBoxLayout()
        hLayout.addWidget( commandGroup )
        hLayout.addWidget( optionsGroup )
        hLayout.addWidget( miscGroup )
        commands = QWidget()
        commands.setLayout( hLayout )
    
        vLayout = QVBoxLayout()
        vLayout.addWidget( commands )
        vLayout.addWidget( scrollToolsGroup )
        vLayout.addStretch()
        self.setLayout( vLayout )
    
        self.readSettings()
        return
  
    def _getProjects    ( self ): return self._projects
    def _getBuildMode   ( self ): return self._buildMode.currentText()
    def _getThreads     ( self ): return self._threads.currentText()
   #def _getSvnUpdate   ( self ): return self._svnUpdate.isChecked()
   #def _getSvnStatus   ( self ): return self._svnStatus.isChecked()
    def _getMake        ( self ): return self._make.isChecked()
    def _getEnableDoc   ( self ): return self._enableDoc.isChecked()
    def _getDevtoolset  ( self ): return self._devtoolset.isChecked()
    def _getQt4         ( self ): return self._qt4.isChecked()
    def _getNoCache     ( self ): return self._noCache.isChecked()
    def _getRmBuild     ( self ): return self._rmBuild.isChecked()
    def _getVerbose     ( self ): return self._verbose.isChecked()
  
    projects    = property( _getProjects )
    buildMode   = property( _getBuildMode )
    threads     = property( _getThreads )
   #svnUpdate   = property( _getSvnUpdate )
   #svnStatus   = property( _getSvnStatus )
    make        = property( _getMake )
    enableDoc   = property( _getEnableDoc )
    devtoolset  = property( _getDevtoolset )
    qt4         = property( _getQt4 )
    noCache     = property( _getNoCache )
    rmBuild     = property( _getRmBuild )
    verbose     = property( _getVerbose )
  
    def readSettings ( self ):
        settings = QSettings()
       #self._svnUpdate  .setChecked( bool(settings.value('builder/svnUpdate' )) )
       #self._svnStatus  .setChecked( bool(settings.value('builder/svnStatus' )) )
        self._make       .setChecked( bool(settings.value('builder/make'      )) )
        self._enableDoc  .setChecked( bool(settings.value('builder/enableDoc' )) )
        self._devtoolset .setChecked( bool(settings.value('builder/devtoolset')) )
        self._qt4        .setChecked( bool(settings.value('builder/qt4'       )) )
        self._noCache    .setChecked( bool(settings.value('builder/noCache'   )) )
        self._rmBuild    .setChecked( bool(settings.value('builder/rmBuild'   )) )
        self._verbose    .setChecked( bool(settings.value('builder/verbose'   )) )
    
        buildModeName = settings.value('builder/buildMode')
        index = self._buildMode.findText( buildModeName )
        if index >= 0: self._buildMode.setCurrentIndex( index )
    
        threads = settings.value('builder/threads')
        index = self._threads.findText( threads )
        if index >= 0: self._threads.setCurrentIndex( index )
    
        for project in self._projects: project.readFromSettings()
        return
  
    def saveSettings ( self ):
        settings = QSettings()
       #settings.setValue('builder/svnUpdate' , self._svnUpdate .isChecked() )
       #settings.setValue('builder/svnStatus' , self._svnStatus .isChecked() )
        settings.setValue('builder/make'      , self._make      .isChecked() )
        settings.setValue('builder/enableDoc' , self._enableDoc .isChecked() )
        settings.setValue('builder/devtoolset', self._devtoolset.isChecked() )
        settings.setValue('builder/qt4'       , self._qt4       .isChecked() )
        settings.setValue('builder/buildMode' , self._buildMode .currentText() )
        settings.setValue('builder/noCache'   , self._noCache   .isChecked() )
        settings.setValue('builder/rmBuild'   , self._rmBuild   .isChecked() )
        settings.setValue('builder/verbose'   , self._verbose   .isChecked() )
        settings.setValue('builder/threads'   , self._threads   .currentText() )
        for project in self._projects: project.saveToSettings()
        return
