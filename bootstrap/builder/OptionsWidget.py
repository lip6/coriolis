
# -*- mode:Python -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC/LIP6 2012-2018, All Rights Reserved
#
# +-----------------------------------------------------------------+ 
# |                   C O R I O L I S                               |
# |    C o r i o l i s  /  C h a m s   B u i l d e r                |
# |                                                                 |
# |  Author      :                       Damien Dupuis              |
# |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
# | =============================================================== |
# |  Python      :   "./builder/OptionsWidget.py"                   |
# +-----------------------------------------------------------------+


import re
import subprocess
from   PyQt4.QtCore             import Qt
from   PyQt4.QtCore             import pyqtSignal
from   PyQt4.QtCore             import QSettings
from   PyQt4.QtGui              import QColor
from   PyQt4.QtGui              import QWidget
from   PyQt4.QtGui              import QPushButton
from   PyQt4.QtGui              import QCheckBox
from   PyQt4.QtGui              import QGroupBox
from   PyQt4.QtGui              import QButtonGroup
from   PyQt4.QtGui              import QVBoxLayout
from   PyQt4.QtGui              import QHBoxLayout
from   PyQt4.QtGui              import QGridLayout
from   PyQt4.QtGui              import QScrollArea
from   PyQt4.QtGui              import QComboBox
from   builder.Project          import Project
from   builder.ConfigureWidget  import ConfigureWidget
from   builder.ProjectWidgets   import ProjectWidgets


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
    self._devtoolset2 = QCheckBox( 'Build with devtoolset 2' )
    self._qt5         = QCheckBox( 'Build with Qt 5 (Qt 4 default)' )
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
    vLayout.addWidget( self._devtoolset2 )
    vLayout.addWidget( self._qt5 )
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
  def _getDevtoolset2 ( self ): return self._devtoolset2.isChecked()
  def _getQt5         ( self ): return self._qt5.isChecked()
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
  devtoolset2 = property( _getDevtoolset2 )
  qt5         = property( _getQt5 )
  noCache     = property( _getNoCache )
  rmBuild     = property( _getRmBuild )
  verbose     = property( _getVerbose )


  def readSettings ( self ):
    settings = QSettings()
   #self._svnUpdate  .setChecked( settings.value('builder/svnUpdate').toBool() )
   #self._svnStatus  .setChecked( settings.value('builder/svnStatus').toBool() )
    self._make       .setChecked( settings.value('builder/make'     ).toBool() )
    self._enableDoc  .setChecked( settings.value('builder/enableDoc').toBool() )
    self._devtoolset2.setChecked( settings.value('builder/devtoolset2').toBool() )
    self._qt5        .setChecked( settings.value('builder/qt5').toBool() )
    self._noCache    .setChecked( settings.value('builder/noCache'  ).toBool() )
    self._rmBuild    .setChecked( settings.value('builder/rmBuild'  ).toBool() )
    self._verbose    .setChecked( settings.value('builder/verbose'  ).toBool() )

    buildModeName = settings.value('builder/buildMode').toString()
    index = self._buildMode.findText( buildModeName )
    if index >= 0: self._buildMode.setCurrentIndex( index )

    threads = settings.value('builder/threads').toString()
    index = self._threads.findText( threads )
    if index >= 0: self._threads.setCurrentIndex( index )

    for project in self._projects: project.readFromSettings()
    return


  def saveSettings ( self ):
    settings = QSettings()
   #settings.setValue('builder/svnUpdate'  , self._svnUpdate  .isChecked() )
   #settings.setValue('builder/svnStatus'  , self._svnStatus  .isChecked() )
    settings.setValue('builder/make'       , self._make       .isChecked() )
    settings.setValue('builder/enableDoc'  , self._enableDoc  .isChecked() )
    settings.setValue('builder/devtoolset2', self._devtoolset2.isChecked() )
    settings.setValue('builder/qt5'        , self._qt5        .isChecked() )
    settings.setValue('builder/buildMode'  , self._buildMode  .currentText() )
    settings.setValue('builder/noCache'    , self._noCache    .isChecked() )
    settings.setValue('builder/rmBuild'    , self._rmBuild    .isChecked() )
    settings.setValue('builder/verbose'    , self._verbose    .isChecked() )
    settings.setValue('builder/threads'    , self._threads    .currentText() )

    for project in self._projects: project.saveToSettings()
    return
