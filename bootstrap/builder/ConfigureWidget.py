
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
# |  Python      :   "./builder/ConfigureWidget.py"                 |
# +-----------------------------------------------------------------+


from PyQt4.QtCore   import Qt, QVariant, pyqtSignal, QSettings, \
                           QModelIndex, QAbstractTableModel
from PyQt4.QtGui    import QFont, QWidget, QGridLayout, QHBoxLayout,  \
                           QVBoxLayout, QLabel, QPushButton,          \
                           QLineEdit, QAbstractItemView, QHeaderView, \
                           QTableView, QGroupBox, QFileDialog,        \
                           QApplication
from .Configuration import Configuration


class ConfSettingsModel ( QAbstractTableModel ):

    HeaderFont    = QApplication.font()
    PrimaryFont   = QFont('Courier',HeaderFont.pointSize()-2,QFont.Normal)
    SecondaryFont = QFont('Courier',HeaderFont.pointSize()-2,QFont.Normal)
    ValueFont     = QFont('Courier',HeaderFont.pointSize()-2,QFont.Bold)

    def __init__ ( self, conf, parent=None ):
        ConfSettingsModel.HeaderFont.setBold( True )
        ConfSettingsModel.SecondaryFont.setItalic( True )
        QAbstractTableModel.__init__( self, parent )
        self._conf = conf
        self._ids  = self._conf.getAllIds()
        return

    def data ( self, index, role ):
        if role == Qt.SizeHintRole:
            if index.column() == 0: return 300
            else:                   return -1
        elif role == Qt.FontRole:
            if index.column() == 0:
                if index.row() >= len(self._conf.getPrimaryIds()):
                    return ConfSettingsModel.SecondaryFont
                return ConfSettingsModel.PrimaryFont
            else:
                return ConfSettingsModel.ValueFont
        elif role == Qt.DisplayRole:
            row = index.row()
            if row < self.rowCount():
                if   index.column() == 0: return self._ids[row]
                elif index.column() == 1: return getattr( self._conf, self._ids[row] )
        return None

    def headerData ( self, section, orientation, role ):
        if orientation == Qt.Vertical: return None
        if role == Qt.FontRole:        return ConfSettingsModel.HeaderFont
        if role != Qt.DisplayRole:     return None
        if   section == 0: return 'Setting'
        elif section == 1: return 'Value'
        return '?'

    def rowCount    ( self, index=QModelIndex() ): return len(self._ids)
    def columnCount ( self, index=QModelIndex() ): return 2


class ConfSettingsWidget ( QWidget ):

    def __init__ ( self, conf, parent=None ):
        QWidget.__init__( self, parent )
        self._rowHeight = 20

        self._view = QTableView()
        self._view.setShowGrid            ( False )
        self._view.setAlternatingRowColors( True )
        self._view.setSelectionBehavior   ( QAbstractItemView.SelectRows )
       #self._view.setSortingEnabled      ( True )
       #self._view.installEventFilter     ( self )

        horizontalHeader = self._view.horizontalHeader ()
        horizontalHeader.setStretchLastSection ( True )
        horizontalHeader.setMinimumSectionSize ( 150 )
        horizontalHeader.setResizeMode         ( QHeaderView.ResizeToContents )
        horizontalHeader.setDefaultSectionSize ( 150 )

        verticalHeader = self._view.verticalHeader ();
        verticalHeader.setVisible ( False );
        verticalHeader.setDefaultSectionSize ( self._rowHeight );

        self._baseModel = ConfSettingsModel( conf )
        self._view.setModel( self._baseModel );
        self._view.horizontalHeader().setStretchLastSection( True );
        self._view.resizeColumnToContents( 0 );

        peanoDataLayout = QGridLayout();
        peanoDataLayout.addWidget( self._view, 0, 0, 1, 1 );
        self.setLayout ( peanoDataLayout );
        return


class ConfigureWidget ( QWidget ):
  
    def __init__ ( self, confFile, parent=None ):
        QWidget.__init__ ( self, parent )
        self._confFile = confFile
        self._conf     = Configuration()
        self._rootDir  = ''
    
        rootDirLabel  = QLabel( 'Root Directory' )
        rootDirBrowse = QPushButton( '&Browse' )
        rootDirBrowse.clicked.connect( self.browseRootDir )
        self._rootDirEdit = QLineEdit( '' )
       #self._rootDirEdit.setFixedWidth( 600 )
    
        gLayout = QGridLayout()
        gLayout.addWidget( rootDirLabel     , 0, 0, 1, 1 )
        gLayout.addWidget( self._rootDirEdit, 0, 1, 1, 6 )
        gLayout.addWidget( rootDirBrowse    , 0, 7, 1, 1 )
        groupDirs = QGroupBox( 'Directories' )
        groupDirs.setLayout( gLayout )
        gLayout = QGridLayout()
        groupConf = QGroupBox( 'Configuration' )
        groupConf.setLayout( gLayout )
    
        vLayout = QVBoxLayout()
        vLayout.addWidget ( groupDirs )
        vLayout.addWidget ( groupConf )
       #vLayout.addStretch()
    
        self.setLayout( vLayout )
        self._rootDirEdit.textChanged.connect( self.rootDirChanged )
        self.readSettings()
    
        noteLabel = QLabel( 'Those settings can be changed only by editing build.conf' )
        gLayout.addWidget( noteLabel                     , 0, 0, 1, 1 )
        gLayout.addWidget( ConfSettingsWidget(self._conf), 1, 0, 1, 1 )
  
    def _getConf         ( self ): return self._conf
    def _getRootDir      ( self ): return self._rootDir
    def _getBootstrapDir ( self ): return self._getConf().bootstrapDir
  
    conf         = property( _getConf )
    rootDir      = property( _getRootDir )
    bootstrapDir = property( _getBootstrapDir )
  
    def rootDirChanged ( self, rootDir ):
        self._rootDir = rootDir
        return
  
    def browseRootDir ( self ):
        self._rootDirEdit.setText( QFileDialog.getExistingDirectory(self,'Select the Building Root Directory') )
        return
  
    def readSettings ( self ):
        settings = QSettings()
        self._rootDirEdit.setText( settings.value('conf/rootDir') )
        if not self._confFile and settings.value('conf/confFile'):
            self._confFile = str( settings.value('conf/confFile') )
        self._conf.load( self._confFile )
        return
  
    def saveSettings ( self ):
        settings = QSettings()
        settings.setValue( 'conf/rootDir' , self._rootDirEdit.text() )
        settings.setValue( 'conf/confFile', self._confFile )
        return
