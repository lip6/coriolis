
# -*- mode:Python -*-
#
# This file is part of the Coriolis Software.
# Copyright (c) UPMC/LIP6 2012-2018, All Rights Reserved
#
# +-----------------------------------------------------------------+ 
# |                   C O R I O L I S                               |
# |    C o r i o l i s  /  C h a m s   B u i l d e r                |
# |                                                                 |
# |  Author      :                    Jean-Paul Chaput              |
# |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
# | =============================================================== |
# |  Python      :   "./builder/ProjectWidget.py"                   |
# +-----------------------------------------------------------------+


import string
from PyQt4.QtCore import Qt
from PyQt4.QtCore import QObject
from PyQt4.QtCore import QSettings
from PyQt4.QtGui  import QSizePolicy
from PyQt4.QtGui  import QFrame
from PyQt4.QtGui  import QPushButton
from PyQt4.QtGui  import QCheckBox
from PyQt4.QtGui  import QLabel


class ProjectWidgets ( QObject ):

  def __init__ ( self, project ):
    self._project       = project
    self._projectButton = QLabel( string.capwords(self._project.getName()) )
    self._projectButton.setStyleSheet( 'font-weight: bold;' )
    self._projectButton.setFrameShape( QFrame.Box )
    self._projectButton.setSizePolicy( QSizePolicy.Expanding, QSizePolicy.Preferred )

    self._toolsCheckBoxes = []
    for tool in self._project.getTools():
        self._toolsCheckBoxes += [ QCheckBox( tool.name ) ]

   #self._projectButton.clicked.connect( self.toggleToolsVisibility )
    return

  def _getProjectButton   ( self ): return self._projectButton
  def _getToolsCheckBoxes ( self ): return self._toolsCheckBoxes

  def _getActives ( self ):
    actives = []
    for toolCb in self._toolsCheckBoxes:
        if toolCb.isChecked(): actives += [ str(toolCb.text()) ]
    return actives

  projectButton   = property( _getProjectButton )
  toolsCheckBoxes = property( _getToolsCheckBoxes )
  actives         = property( _getActives )

  def addToLayout( self, column, layout ):
    toolsNb = len(self._toolsCheckBoxes)
    if toolsNb <= 10:
      layout.addWidget( self._projectButton, 0, column, Qt.AlignLeft )
      for row in range(toolsNb):
        layout.addWidget( self._toolsCheckBoxes[row], row+1, column, Qt.AlignTop )
      return 1

    columnSpan = toolsNb / 10
    if toolsNb % 10: columnSpan += 1

    layout.addWidget( self._projectButton, 0, column, 1, columnSpan, Qt.AlignJustify )
    for row in range(toolsNb):
      if row and row % 10 == 0: column += 1
      layout.addWidget( self._toolsCheckBoxes[row], row%10+1, column, Qt.AlignTop )

    return columnSpan

 #def toggleToolsVisibility ( self ):
 #  self._visibleTools = not self._visibleTools
 #  for toolCb in self._toolsCheckBoxes:
 #      toolCb.setVisible( self._visibleTools )
 #  return

  def readFromSettings ( self ):
    settings = QSettings()
    for toolCb in self._toolsCheckBoxes:
        toolId = 'tools/'+self._project.getName()+'/'+toolCb.text()
        toolCb.setChecked( settings.value(toolId).toBool() )
    return

  def saveToSettings ( self ):
    settings = QSettings()
    for toolCb in self._toolsCheckBoxes:
        toolId = 'tools/'+self._project.getName()+'/'+toolCb.text()
        settings.setValue( toolId, toolCb.isChecked() )
    return
