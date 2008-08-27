
// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./CellViewer.cpp"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QAction>
#include  <QMenu>
#include  <QMenuBar>
#include  <QStatusBar>
#include  <QDockWidget>

#include  "hurricane/DataBase.h"
#include  "hurricane/Cell.h"

//#include  "MapView.h"
#include  "hurricane/viewer/HPalette.h"
#include  "hurricane/viewer/CellWidget.h"
#include  "hurricane/viewer/CellViewer.h"
#include  "hurricane/viewer/HInspectorWidget.h"
#include  "hurricane/viewer/HNetlist.h"
#include  "hurricane/viewer/HMousePosition.h"


namespace Hurricane {


  CellViewer::CellViewer ( QWidget* parent ) : QMainWindow(parent)
                                             , _applicationName(tr("Viewer"))
                                             , _openAction(NULL)
                                             , _nextAction(NULL)
                                             , _saveAction(NULL)
                                             , _exitAction(NULL)
                                             , _refreshAction(NULL)
                                             , _fitToContentsAction(NULL)
                                             , _showSelectionAction(NULL)
                                             , _showPaletteAction(NULL)
                                             , _runInspectorOnDataBase(NULL)
                                             , _runInspectorOnCell(NULL)
                                             , _browseNetlist(NULL)
                                             , _fileMenu(NULL)
                                             , _viewMenu(NULL)
                                             , _toolsMenu(NULL)
                                             //, _mapView(NULL)
                                             , _palette(NULL)
                                             , _mousePosition(NULL)
                                             , _cellWidget(NULL)
                                             , _cellHistory()
  {
    setObjectName("viewer");

    createMenus  ();
    createLayout ();
  }




  void  CellViewer::createActions ()
  {
    if ( _openAction ) return;

    _openAction = new QAction  ( tr("&Open Cell"), this );
    _openAction->setObjectName ( "viewer.file.openCell" );
    _openAction->setIcon       ( QIcon(":/images/stock_open.png") );
    _openAction->setStatusTip  ( tr("Open (load) a new Cell") );
    cerr << "_openAction: " << _openAction << endl;

    _nextAction = new QAction  ( tr("&Next Breakpoint"), this );
    _nextAction->setObjectName ( "viewer.file.nextBreakpoint" );
    _nextAction->setStatusTip  ( tr("Proceed to the next breakpoint") );

    for ( unsigned i=0 ; i<CellHistorySize ; i++ ) {
      _cellHistoryAction[i] = new QAction ( this );
      _cellHistoryAction[i]->setObjectName ( QString("viewer.file.cellHistory[%1]").arg(i) );
      _cellHistoryAction[i]->setVisible ( false );
      _cellHistoryAction[i]->setData    ( i );
      _cellHistoryAction[i]->setFont    ( Graphics::getFixedFont(QFont::Bold,false,false) );
      connect ( _cellHistoryAction[i], SIGNAL(triggered()), this, SLOT(openHistoryCell()));
    }

    _saveAction = new QAction  ( tr("&Save Cell"), this );
    _saveAction->setObjectName ( "viewer.file.saveCell" );
    _saveAction->setIcon       ( QIcon(":/images/stock_save.png") );
    _saveAction->setStatusTip  ( tr("Save the current Cell") );
    _saveAction->setVisible    ( false );

    _exitAction = new QAction  ( tr("&Exit"), this );
    _exitAction->setObjectName ( "viewer.file.exit" );
    _exitAction->setStatusTip  ( tr("Close Coriolis CellViewer") );
    _exitAction->setShortcut   ( QKeySequence(tr("CTRL+Q")) );
    connect ( _exitAction, SIGNAL(triggered()), this, SLOT(close()) );

    _refreshAction = new QAction  ( tr("&Refresh"), this );
    _refreshAction->setObjectName ( "viewer.view.refresh" );
    _refreshAction->setStatusTip  ( tr("Force full redrawing of the display") );
    _refreshAction->setShortcut   ( QKeySequence(tr("CTRL+L")) );

    _fitToContentsAction = new QAction  ( tr("&Fit to Contents"), this );
    _fitToContentsAction->setObjectName ( "viewer.view.fit" );
    _fitToContentsAction->setStatusTip  ( tr("Adjust zoom to fit the whole cell's contents") );
    _fitToContentsAction->setShortcut   ( Qt::Key_F );

    _showSelectionAction = new QAction  ( tr("&Show Selection"), this );
    _showSelectionAction->setObjectName ( "viewer.view.showSelection" );
    _showSelectionAction->setStatusTip  ( tr("Highlight the selected items (darken others)") );
    _showSelectionAction->setShortcut   ( Qt::Key_S );
    _showSelectionAction->setCheckable  ( true );

    _showPaletteAction = new QAction  ( tr("Show &Palette"), this );
    _showPaletteAction->setObjectName ( "viewer.view.showPalette" );
    _showPaletteAction->setStatusTip  ( tr("Hide/Show the Palette sub-window") );
    _showPaletteAction->setCheckable  ( true );
    _showPaletteAction->setChecked    ( true );

    _runInspectorOnDataBase= new QAction   ( tr("Inspect &DataBase"), this );
    _runInspectorOnDataBase->setObjectName ( "viewer.tool.inspectDb" );
    _runInspectorOnDataBase->setStatusTip  ( tr("Run Inspector on Hurricane DataBase") );

    _runInspectorOnCell= new QAction   ( tr("Inspect &Cell"), this );
    _runInspectorOnCell->setObjectName ( "viewer.tool.inspectCell" );
    _runInspectorOnCell->setStatusTip  ( tr("Run Inspector on the current Cell") );

    _browseNetlist= new QAction   ( tr("Browse &Netlist"), this );
    _browseNetlist->setObjectName ( "viewer.tool.browseNetlist" );
    _browseNetlist->setStatusTip  ( tr("Browse netlist from the current Cell") );
  }



  void  CellViewer::createMenus ()
  {
    if (  _fileMenu   ) return;
    if ( !_openAction ) createActions ();

    _fileMenu = menuBar()->addMenu ( tr("File") );
    _fileMenu->setObjectName ( "viewer.file" );
    _fileMenu->addAction ( _openAction );
    _fileMenu->addAction ( _nextAction );
    _fileMenu->addSeparator ();
    for ( size_t i=0 ; i<CellHistorySize ; i++ ) {
      _fileMenu->addAction ( _cellHistoryAction[i] );
    }
    _fileMenu->addSeparator ();
    _fileMenu->addAction ( _saveAction );
    _fileMenu->addAction ( _exitAction );

    _viewMenu = menuBar()->addMenu ( tr("View") );
    _viewMenu->setObjectName ( "viewer.view" );
    _viewMenu->addAction ( _refreshAction );
    _viewMenu->addAction ( _fitToContentsAction );
    _viewMenu->addAction ( _showSelectionAction );
    _viewMenu->addAction ( _showPaletteAction );

    _toolsMenu = menuBar()->addMenu ( tr("Tool") );
    _toolsMenu->setObjectName ( "viewer.tool" );
    _toolsMenu->addAction ( _runInspectorOnDataBase );
    _toolsMenu->addAction ( _runInspectorOnCell );
    _toolsMenu->addAction ( _browseNetlist );
  }



  void  CellViewer::createLayout ()
  {
    if ( _cellWidget ) return;

    _cellWidget = new CellWidget ();
    _palette    = new HPalette ();
  //_mapView    = _cellWidget->getMapView ();

    _cellWidget->bindToPalette ( _palette );

    HMousePosition* _mousePosition = new HMousePosition ();
    statusBar()->addPermanentWidget ( _mousePosition );

    setCorner ( Qt::TopLeftCorner    , Qt::LeftDockWidgetArea  );
    setCorner ( Qt::BottomLeftCorner , Qt::LeftDockWidgetArea  );
    setCorner ( Qt::TopRightCorner   , Qt::RightDockWidgetArea );
    setCorner ( Qt::BottomRightCorner, Qt::RightDockWidgetArea );

  //   QDockWidget* mapViewDock = new QDockWidget ( tr("Map") );
  //   mapViewDock->setObjectName   ( "MapView" );
  //   mapViewDock->setWidget       ( _mapView );
  //   mapViewDock->setAllowedAreas ( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
  //   addDockWidget ( Qt::RightDockWidgetArea, mapViewDock );

    QDockWidget* layerMapDock = new QDockWidget ( tr("Layers") );
    layerMapDock->setFeatures     ( QDockWidget::DockWidgetVerticalTitleBar
                                  | QDockWidget::DockWidgetMovable
                                  | QDockWidget::DockWidgetFloatable
                                  );
    layerMapDock->setObjectName   ( "viewer.dock.paletteWindow" );
    layerMapDock->setWidget       ( _palette );
    layerMapDock->setAllowedAreas ( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
    addDockWidget ( Qt::RightDockWidgetArea, layerMapDock );

    setCentralWidget ( _cellWidget );

    connect ( _refreshAction         , SIGNAL(triggered())  , _cellWidget, SLOT(redraw                ()));
    connect ( _fitToContentsAction   , SIGNAL(triggered())  , _cellWidget, SLOT(fitToContents         ()));
    connect ( _showSelectionAction   , SIGNAL(toggled(bool)), _cellWidget, SLOT(setShowSelection  (bool)));
    connect ( _showPaletteAction     , SIGNAL(toggled(bool)), this       , SLOT(setShowPalette    (bool)));
    connect ( _runInspectorOnDataBase, SIGNAL(triggered())  , this       , SLOT(runInspectorOnDataBase()));
    connect ( _runInspectorOnCell    , SIGNAL(triggered())  , this       , SLOT(runInspectorOnCell    ()));
    connect ( _browseNetlist         , SIGNAL(triggered())  , this       , SLOT(browseNetlist         ()));
    connect ( _cellWidget            , SIGNAL(mousePositionChanged(const Point&))
            , _mousePosition         , SLOT(setPosition(const Point&)) );

    _cellWidget->redraw ();
  }


  void  CellViewer::refreshHistory ()
  {
    Cell* activeCell = getCell();
    _cellHistory.remove ( activeCell );

    if ( _cellHistory.size() > CellHistorySize-1 )
      _cellHistory.pop_front ();
    _cellHistory.push_back ( activeCell );

    list<Cell*>::iterator iname = _cellHistory.begin();
    for ( size_t i=0 ; i<CellHistorySize ; i++ ) {
      if ( iname != _cellHistory.end() ) {
        QString entry = tr("&%1: %2").arg(i+1).arg( getString((*iname)->getName()).c_str() );
        _cellHistoryAction[i]->setText    ( entry );
        _cellHistoryAction[i]->setVisible ( true );
        iname++;
      } else {
        _cellHistoryAction[i]->setVisible ( false );
      }
    }
  }


  void  CellViewer::setCell ( Cell* cell )
  {
    _cellWidget->setCell ( cell );

    QString  title
      = QString("%1:<%2>").arg(_applicationName).arg(getString(cell->getName()).c_str());
    setWindowTitle ( title );

    refreshHistory ();
  }


  Cell* CellViewer::getCell ()
  { return getCellWidget()->getCell(); }


  Cell* CellViewer::getCellFromDb ( const char* name )
  {
    cerr << "[ERROR] virtual function CellViewer::getCellFromDb() has not been overloaded.\n"
         << "        (this will prevent \"Open Cell\" to work)"
         << endl;

    return NULL;
  }


  void  CellViewer::runInspector ( Record* record )
  {
    static HInspectorWidget* inspector = NULL;

    if ( record ) {
    //if ( !inspector )
      inspector = new HInspectorWidget ();

      inspector->setRootRecord ( record );
      inspector->show ();
    } else
      cerr << "[ERROR] Attempt to run Inspector on NULL record." << endl;
  }


  void  CellViewer::setShowPalette ( bool show )
  {
    QDockWidget* paletteWindow = findChild<QDockWidget*>("viewer.dock.paletteWindow");

    if ( !paletteWindow ) {
      cerr << "paletteWindow not found." << endl;
      return;
    }

    if ( show ) paletteWindow->show ();
    else        paletteWindow->hide ();

//  if ( show ) _palette->show ();
//  else        _palette->hide ();
  }


  void  CellViewer::openHistoryCell ()
  {
    QAction* historyAction = qobject_cast<QAction*> ( sender() );
    if ( historyAction ) {
      list<Cell*>::iterator  icell = _cellHistory.begin();
      size_t                 index = historyAction->data().toUInt();

      for ( ; index>0 ; index--, icell++ );

      cerr << "History: " << *icell << endl;
      setCell ( *icell );
    }
  }


  void  CellViewer::runInspectorOnDataBase ()
  {
    runInspector ( getRecord(DataBase::getDB()) );
  }


  void  CellViewer::runInspectorOnCell ()
  {
    Cell* cell = _cellWidget->getCell();
    if ( cell ) runInspector ( getRecord(cell) );
  }


  void  CellViewer::browseNetlist ()
  {
    HNetlist* netlistBrowser = new HNetlist ();
    netlistBrowser->setCellWidget<SimpleNetInformations> ( _cellWidget );
    netlistBrowser->show ();
  }


  void  CellViewer::select ( Occurrence& occurrence )
  { if ( _cellWidget ) _cellWidget->select ( occurrence ); }


  void  CellViewer::unselect ( Occurrence& occurrence )
  { if ( _cellWidget ) _cellWidget->unselect ( occurrence ); }


  void  CellViewer::unselectAll ()
  { if ( _cellWidget ) _cellWidget->unselectAll(); }


} // End of Hurricane namespace.
