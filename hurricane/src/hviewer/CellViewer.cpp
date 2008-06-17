

// -*- C++ -*-


#include  <QAction>
#include  <QMenu>
#include  <QMenuBar>
#include  <QDockWidget>

#include  "hurricane/DataBase.h"
#include  "hurricane/Cell.h"

//#include  "MapView.h"
#include  "hurricane/viewer/Palette.h"
#include  "hurricane/viewer/CellWidget.h"
#include  "hurricane/viewer/CellViewer.h"
#include  "hurricane/inspector/HInspectorWidget.h"


namespace Hurricane {


  CellViewer::CellViewer ( Cell* cell ) : QMainWindow()
                                        , _openAction(NULL)
                                        , _nextCellAction(NULL)
                                        , _previousCellAction(NULL)
                                        , _nextAction(NULL)
                                        , _saveAction(NULL)
                                        , _exitAction(NULL)
                                        , _refreshAction(NULL)
                                        , _fitToContentsAction(NULL)
                                        , _showBoundariesAction(NULL)
                                        , _runInspectorOnDataBase(NULL)
                                        , _runInspectorOnCell(NULL)
                                        , _fileMenu(NULL)
                                        , _viewMenu(NULL)
                                        , _toolsMenu(NULL)
                                      //, _mapView(NULL)
                                        , _palette(NULL)
                                        , _cellWidget(NULL)
  {
    createMenus  ();
    createLayout ( cell );
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

    _showBoundariesAction = new QAction ( tr("&Boundaries"), this );
    _showBoundariesAction->setCheckable ( true );
    _showBoundariesAction->setStatusTip ( tr("Show/hide cell & instances abutment boxes") );

    _runInspectorOnDataBase= new QAction ( tr("Inspect &DataBase"), this );
    _runInspectorOnDataBase->setStatusTip ( tr("Run Inspector on Hurricane DataBase") );

    _runInspectorOnCell= new QAction ( tr("Inspect &Cell"), this );
    _runInspectorOnCell->setStatusTip ( tr("Run Inspector on the current Cell") );
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
  //_viewMenu->addAction ( _showBoundariesAction );

    _toolsMenu = menuBar()->addMenu ( tr("Tool") );
    _toolsMenu->addAction ( _runInspectorOnDataBase );
    _toolsMenu->addAction ( _runInspectorOnCell );
  }



  void  CellViewer::createLayout ( Cell* cell )
  {
    if ( _cellWidget ) return;

    _cellWidget = new CellWidget ( cell );
    _palette    = _cellWidget->getPalette();
  //_mapView    = _cellWidget->getMapView ();

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
    layerMapDock->setObjectName   ( "Palette" );
    layerMapDock->setWidget       ( _palette );
    layerMapDock->setAllowedAreas ( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
    addDockWidget ( Qt::RightDockWidgetArea, layerMapDock );

    setCentralWidget ( _cellWidget );

    connect ( _refreshAction         , SIGNAL(triggered())  , _cellWidget, SLOT(redraw                ()));
    connect ( _fitToContentsAction   , SIGNAL(triggered())  , _cellWidget, SLOT(fitToContents         ()));
    connect ( _runInspectorOnDataBase, SIGNAL(triggered())  , this       , SLOT(runInspectorOnDataBase()));
    connect ( _runInspectorOnCell    , SIGNAL(triggered())  , this       , SLOT(runInspectorOnCell    ()));

  //_showBoundariesAction->setChecked ( _cellWidget->showBoundaries() );
  //connect ( _showBoundariesAction, SIGNAL(toggled(bool)), _cellWidget, SLOT(setShowBoundaries(bool)) );

    _cellWidget->redraw ();
  }


  void  CellViewer::runInspector ( Record* record )
  {
  //cerr << "INITIAL Records: " << Record::getAllocateds()  << endl;
  //cerr << "INITIAL Slots:   " << Slot::getAllocateds()  << endl;
  //cerr << "Inspector created." << endl;

    if ( record ) {
      HInspectorWidget* inspector = new HInspectorWidget ( record );
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
    runInspector ( getRecord(_cellWidget->getCell()) );
  }


} // End of Hurricane namespace.
