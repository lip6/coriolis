
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
                                             , _openAction(NULL)
                                             , _nextCellAction(NULL)
                                             , _previousCellAction(NULL)
                                             , _nextAction(NULL)
                                             , _saveAction(NULL)
                                             , _exitAction(NULL)
                                             , _refreshAction(NULL)
                                             , _fitToContentsAction(NULL)
                                             , _showSelectionAction(NULL)
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
  {
    createMenus  ();
    createLayout ();
  }




  void  CellViewer::createActions ()
  {
    if ( _openAction ) return;

    _openAction = new QAction ( tr("&Open Cell"), this );
    _openAction->setIcon ( QIcon(":/images/stock_open.png") );
    _openAction->setStatusTip ( tr("Open (load) a new Cell") );

    _nextCellAction = new QAction ( tr("Next Cell"), this );
    _nextCellAction->setStatusTip ( tr("Go to the next Cell in history") );

    _previousCellAction = new QAction ( tr("Previous Cell"), this );
    _previousCellAction->setStatusTip ( tr("Go to the previous Cell in history") );

    _nextAction = new QAction ( tr("&Next Breakpoint"), this );
    _nextAction->setStatusTip ( tr("Proceed to the next breakpoint") );

    _saveAction = new QAction ( tr("&Save Cell"), this );
    _saveAction->setIcon ( QIcon(":/images/stock_save.png") );
    _saveAction->setStatusTip ( tr("Save the current Cell") );

    _exitAction = new QAction ( tr("&Exit"), this );
    _exitAction->setStatusTip ( tr("Close Coriolis CellViewer") );
    _exitAction->setShortcut  ( QKeySequence(tr("CTRL+Q")) );
    connect ( _exitAction, SIGNAL(triggered()), this, SLOT(close()) );

    _refreshAction = new QAction ( tr("&Refresh"), this );
    _refreshAction->setStatusTip ( tr("Force full redrawing of the display") );
    _refreshAction->setShortcut  ( QKeySequence(tr("CTRL+L")) );

    _fitToContentsAction = new QAction ( tr("&Fit to Contents"), this );
    _fitToContentsAction->setStatusTip ( tr("Adjust zoom to fit the whole cell's contents") );
    _fitToContentsAction->setShortcut  ( Qt::Key_F );

    _showSelectionAction = new QAction ( tr("&Show Selection"), this );
    _showSelectionAction->setStatusTip ( tr("Highlight the selected items (darken others)") );
    _showSelectionAction->setShortcut  ( Qt::Key_S );
    _showSelectionAction->setCheckable ( true );

    _runInspectorOnDataBase= new QAction ( tr("Inspect &DataBase"), this );
    _runInspectorOnDataBase->setStatusTip ( tr("Run Inspector on Hurricane DataBase") );

    _runInspectorOnCell= new QAction ( tr("Inspect &Cell"), this );
    _runInspectorOnCell->setStatusTip ( tr("Run Inspector on the current Cell") );

    _browseNetlist= new QAction ( tr("Browse &Netlist"), this );
    _browseNetlist->setStatusTip ( tr("Browse netlist from the current Cell") );
  }



  void  CellViewer::createMenus ()
  {
    if (  _fileMenu   ) return;
    if ( !_openAction ) createActions ();

    _fileMenu = menuBar()->addMenu ( tr("File") );
    _fileMenu->addAction ( _openAction );
    _fileMenu->addAction ( _nextCellAction );
    _fileMenu->addAction ( _previousCellAction );
    _fileMenu->addAction ( _nextAction );
    _fileMenu->addAction ( _saveAction );
    _fileMenu->addAction ( _exitAction );

    _viewMenu = menuBar()->addMenu ( tr("View") );
    _viewMenu->addAction ( _refreshAction );
    _viewMenu->addAction ( _fitToContentsAction );
    _viewMenu->addAction ( _showSelectionAction );

    _toolsMenu = menuBar()->addMenu ( tr("Tool") );
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
    layerMapDock->setObjectName   ( "HPalette" );
    layerMapDock->setWidget       ( _palette );
    layerMapDock->setAllowedAreas ( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
    addDockWidget ( Qt::RightDockWidgetArea, layerMapDock );

    setCentralWidget ( _cellWidget );

    connect ( _refreshAction         , SIGNAL(triggered())  , _cellWidget, SLOT(redraw                ()));
    connect ( _fitToContentsAction   , SIGNAL(triggered())  , _cellWidget, SLOT(fitToContents         ()));
    connect ( _showSelectionAction   , SIGNAL(toggled(bool)), _cellWidget, SLOT(setShowSelection  (bool)));
    connect ( _runInspectorOnDataBase, SIGNAL(triggered())  , this       , SLOT(runInspectorOnDataBase()));
    connect ( _runInspectorOnCell    , SIGNAL(triggered())  , this       , SLOT(runInspectorOnCell    ()));
    connect ( _browseNetlist         , SIGNAL(triggered())  , this       , SLOT(browseNetlist         ()));
    connect ( _cellWidget            , SIGNAL(mousePositionChanged(const Point&))
            , _mousePosition         , SLOT(setPosition(const Point&)) );

    _cellWidget->redraw ();
  }


  void  CellViewer::setCell ( Cell* cell )
  {
    _cellWidget->setCell ( cell );
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
