// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2008-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./CellViewer.cpp"                              |
// +-----------------------------------------------------------------+


#include <Python.h>
#include <unistd.h>
#include <algorithm>
#include <sstream>
#include <exception>

#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QApplication>
#include <QPrinter>
#include <QPrintDialog>
#include <QFileDialog>

#include "hurricane/utilities/Path.h"
#include "hurricane/configuration/Configuration.h"
#include "hurricane/DebugSession.h"
#include "hurricane/DataBase.h"
#include "hurricane/Library.h"
#include "hurricane/Cell.h"
//#include  "MapView.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/viewer/Script.h"
#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/CellViewer.h"
#include "hurricane/viewer/CellPrinter.h"
#include "hurricane/viewer/CellImage.h"
#include "hurricane/viewer/MousePositionWidget.h"
#include "hurricane/viewer/ControllerWidget.h"
#include "hurricane/viewer/ScriptWidget.h"
#include "hurricane/viewer/ExceptionWidget.h"
#include "hurricane/viewer/GotoWidget.h"
#include "hurricane/viewer/DesignBlob.h"
#include "hurricane/viewer/OpenBlobDialog.h"
#include "hurricane/viewer/SelectCommand.h"
#include "hurricane/viewer/PyCellViewer.h"


namespace Hurricane {

  using Isobar::PyCell_Link;


// -------------------------------------------------------------------
// Class  :  "CellObserver".

  void  CellViewer::notify ( CellViewer* viewer, unsigned int flags )
  {
    cdebug.log(19) << "CellViewer::notify() " << viewer << endl;

    switch ( flags & (Cell::Flags::CellAboutToChange
                     |Cell::Flags::CellChanged
                     |Cell::Flags::CellDestroyed) ) {
      case Cell::Flags::CellAboutToChange:
        viewer->emitCellAboutToChange();
        break;
      case Cell::Flags::CellChanged:
        viewer->emitCellChanged();
        break;
      case Cell::Flags::CellDestroyed:
        viewer->emitCellAboutToChange();
        viewer->removeHistory( viewer->getCell() );
        viewer->setCell( NULL );
        viewer->emitCellChanged();
        break;
    }
  }


// -------------------------------------------------------------------
// Class  :  "CellViewer".

  QString  CellViewer::_prefixWPath ( "viewer.menuBar." );


  CellViewer::CellViewer ( QWidget* parent ) : QMainWindow             (parent)
                                             , _cellObserver           (this)
                                             , _applicationName        (tr("Viewer"))
                                             , _showSelectionAction    (NULL)
                                             , _debugMenu              (NULL)
                                             , _actionCallbacks        ()
                                           //, _mapView                (NULL)
                                             , _mousePosition          (NULL)
                                             , _controller             (NULL)
                                             , _script                 (NULL)
                                             , _goto                   (NULL)
                                             , _cellWidget             (NULL)
                                             , _moveCommand            ()
                                             , _zoomCommand            ()
                                             , _rulerCommand           ()
                                             , _selectCommand          ()
                                             , _hierarchyCommand       ()
                                             , _cellHistory            ()
                                             , _firstShow              (false)
                                             , _toolInterrupt          (false)
                                             , _flags                  (0)
                                             , _updateState            (ExternalEmit)
                                             , _pyScriptName           ()
  {
    setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
    setObjectName( "viewer" );
    menuBar()->setObjectName ( _getAbsWidgetPath("") );

    _cellWidget       = new CellWidget      ();
    _controller       = new ControllerWidget();
    _goto             = new GotoWidget      ();
    _goto->changeDbuMode( _cellWidget->getDbuMode(), _cellWidget->getUnitPower() );

    _cellWidget->bindCommand( &_moveCommand );
    _cellWidget->bindCommand( &_zoomCommand );
    _cellWidget->bindCommand( &_rulerCommand );
    _cellWidget->bindCommand( &_selectCommand );
    _cellWidget->bindCommand( &_hierarchyCommand );
    _controller->setCellWidget( _cellWidget );

    _mousePosition = new MousePositionWidget();
    statusBar()->addPermanentWidget( _mousePosition );

    setCorner( Qt::TopLeftCorner    , Qt::LeftDockWidgetArea  );
    setCorner( Qt::BottomLeftCorner , Qt::LeftDockWidgetArea  );
    setCorner( Qt::TopRightCorner   , Qt::RightDockWidgetArea );
    setCorner( Qt::BottomRightCorner, Qt::RightDockWidgetArea );

  //_mapView = _cellWidget->getMapView ();
  //QDockWidget* mapViewDock = new QDockWidget ( tr("Map") );
  //mapViewDock->setFeatures     ( QDockWidget::DockWidgetVerticalTitleBar
  //                             | QDockWidget::DockWidgetMovable
  //                             | QDockWidget::DockWidgetFloatable
  //                             );
  //mapViewDock->setObjectName   ( "viewer.menuBar.dock.mapView" );
  //mapViewDock->setWidget       ( _mapView );
  //mapViewDock->setAllowedAreas ( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
  //addDockWidget( Qt::RightDockWidgetArea, mapViewDock );

    setCentralWidget( _cellWidget );

    createMenus();

    connect( this           , SIGNAL(cellPreModificated()) , _cellWidget, SLOT(cellPreModificate())  );
    connect( this           , SIGNAL(cellPostModificated()), _cellWidget, SLOT(cellPostModificate()) );
    connect( this           , SIGNAL(redrawCellWidget())   , _cellWidget, SLOT(refresh()) );

    connect( _cellWidget    , SIGNAL(dbuModeChanged(unsigned int,DbU::UnitPower))
           , _goto          , SLOT  (changeDbuMode (unsigned int,DbU::UnitPower)) );

    connect( _cellWidget    , SIGNAL(mousePositionChanged(const Point&))
           , _mousePosition , SLOT  (setPosition(const Point&)) );

    connect( _cellWidget    , SIGNAL(selectionModeChanged())
           , this           , SLOT  (changeSelectionMode ()) );
    connect( &_selectCommand, SIGNAL(selectionToggled (Occurrence))
           ,  _cellWidget   , SLOT  (select           (Occurrence)) );

    connect( _cellWidget    , SIGNAL(stateChanged(shared_ptr<CellWidget::State>&))
           , this           , SLOT  (setState    (shared_ptr<CellWidget::State>&)) );
    connect( this           , SIGNAL(stateChanged(shared_ptr<CellWidget::State>&))
           , _cellWidget    , SLOT  (setState    (shared_ptr<CellWidget::State>&)) );

    _cellWidget->refresh();
  }


  CellViewer::~CellViewer ()
  {
    _controller->deleteLater ();
  //_script->deleteLater ();
    _goto->deleteLater ();
  }


  CellViewer* CellViewer::vcreate () const
  { return new CellViewer(); }


  QString  CellViewer::_getAbsWidgetPath ( const QString& relPath ) const
  {
    if (relPath.startsWith("viewer.")) return relPath;
    return QString(_prefixWPath).append( relPath );
  }


  bool  CellViewer::hasMenu( const QString& relativePath ) const
  { return findChild<QObject*>(_getAbsWidgetPath(relativePath)) != NULL; }


  bool  CellViewer::hasMenuAction( const QString& relativePath ) const
  { return findChild<QAction*>(_getAbsWidgetPath(relativePath)) != NULL; }


  QAction* CellViewer::getMenuAction( const QString& relativePath ) const
  { return findChild<QAction*>(_getAbsWidgetPath(relativePath)); }


  QAction* CellViewer::_getParentMenu( const QString& absolutePath ) const
  {
    QString  parentPath = absolutePath.section('.',0,-2);
    QAction* parentMenu = findChild<QAction*>(parentPath);
    if (parentMenu == NULL) {
      if (parentPath != "viewer") {
        cerr << "parentPath:\"" << parentPath.toStdString() << "\"" << endl;
        cerr << Warning( "CellViewer::_getParentMenu() - Missing parent menu for %s."
                       , absolutePath.toStdString().c_str() ) << endl;
      }
      return NULL;
    }
    return parentMenu;
  }


  QAction* CellViewer::addMenu ( const QString& path, string text, unsigned int flags )
  {
    QString  absolutePath = _getAbsWidgetPath( path );
    QAction* actionMenu   = findChild<QAction*>(absolutePath);

    if (actionMenu != NULL) return actionMenu;

    QMenu* menu = new QMenu ( tr(text.c_str()), this );
    
    if (flags & TopMenu) {
      actionMenu = menuBar()->addMenu( menu );
      actionMenu->setObjectName( absolutePath );
    } else {
      menu->setStyleSheet( "font-family: Bitstream Vera Sans Mono" );

      QAction* parentMenu = _getParentMenu( absolutePath );
      if (parentMenu == NULL) return NULL;

      actionMenu = parentMenu->menu()->addMenu( menu );
      actionMenu->setObjectName( absolutePath );
    }
    return actionMenu;
  }


  bool  CellViewer::addToMenu ( const QString& path, QString beforePath )
  {
    if (not path.endsWith("====")) return false;

    QString  absolutePath = _getAbsWidgetPath( path );
    QAction* menuAction   = _getParentMenu( absolutePath );
    if (menuAction == NULL) return false;

    QAction* before = getMenuAction( beforePath );
    QAction* action = new QAction( this );
    action->setSeparator ( true );
    action->setObjectName( absolutePath );
    menuAction->menu()->insertAction( before, action );

    return true;
  }


  QAction* CellViewer::addToMenu ( const QString&          path
                                 , string                  text
                                 , string                  textTip
                                 , std::function< void() > callback
                                 , QIcon                   icon 
                                 , QString                 beforePath )
  {
    QString  absolutePath = _getAbsWidgetPath( path );
    QAction* action       = findChild<QAction*>(absolutePath);
    if (action == NULL) {
      QAction* parentMenu = _getParentMenu( absolutePath );
      if (parentMenu == NULL) return NULL;

      action = new QAction( tr(text.c_str()), this );
      action->setObjectName( absolutePath );
      action->setStatusTip ( tr(textTip.c_str()) );
      action->setVisible   ( true );
      if (not icon.isNull()) action->setIcon( icon );

      QAction* before = getMenuAction( beforePath );
      parentMenu->menu()->insertAction( before, action );

      _actionCallbacks.insert( make_pair(absolutePath,boost::any(callback)) );
      connect( action, SIGNAL(triggered()), this, SLOT(doAction()) );
    }
    return action;
  }


  QAction* CellViewer::addToMenu ( const QString& path
                                 , string         text
                                 , string         textTip
                                 , string         scriptPath
                                 , QString        beforePath )
  {
    QString  absolutePath = _getAbsWidgetPath( path );
    QAction* action       = findChild<QAction*>(absolutePath);
    if (action == NULL) {
      action = new QAction( tr(text.c_str()), this );
      action->setObjectName( absolutePath );
      action->setStatusTip ( tr(textTip.c_str()) );
      action->setVisible   ( true );

      QAction* parentMenu = _getParentMenu( absolutePath );
      if (parentMenu != NULL) {
        QAction* before = getMenuAction( beforePath );
        parentMenu->menu()->insertAction( before, action );
      } else if (absolutePath == "viewer") {
        addAction( action );
      }

      _actionCallbacks.insert( make_pair(absolutePath,boost::any(QString(scriptPath.c_str()))) );
      connect( action, SIGNAL(triggered()), this, SLOT(doAction()) );
    }
    return action;
  }


  QAction* CellViewer::addToMenu ( QString             path
                                 , QString             text
                                 , QString             textTip
                                 , const QKeySequence& shortCut
                                 , QIcon               icon
                               //, QWidget*            receiver
                               //, SlotMethod          slotMethod
                                 , QString             beforePath
                                 )
  {
    QString  absolutePath = _getAbsWidgetPath( path );
    QAction* action       = findChild<QAction*>(absolutePath);
    if (action == NULL) {
      action = new QAction( text, this );
      action->setObjectName( absolutePath );
      action->setStatusTip ( textTip );
      action->setShortcut  ( shortCut );
      action->setVisible   ( true );
      if (not icon.isNull()) action->setIcon( icon );

      QAction* parentMenu = _getParentMenu( absolutePath );
      if (parentMenu != NULL) {
        QAction* before = getMenuAction( beforePath );
        parentMenu->menu()->insertAction( before, action );
      } else if (absolutePath == "viewer") {
        addAction( action );
      }

    //if ((receiver != NULL) and (slotMethod != NULL))
    //  connect( action, &QAction::triggered, receiver, slotMethod );
    }
    return action;
  }


  void  CellViewer::doAction ()
  {
    QString path = sender()->objectName();
    ActionLut::const_iterator iaction = _actionCallbacks.find( path );
    if (iaction == _actionCallbacks.end()) {
      cerr << Error( "CellViewer::doAction() - Path \"%s\" in not registered."
                   , path.toStdString().c_str() ) << endl;
      return;
    }

    const boost::any& callback = iaction->second;
    if (callback.type() == typeid( std::function<void()> )) {
      ExceptionWidget::catchAllWrapper( boost::any_cast< std::function<void()> >(callback) );
    } else if (callback.type() == typeid( QString )) {
      runScript( boost::any_cast<QString>(callback) );
    } else {
      cerr << Error("CellViewer::doAction(): For action \"%s\",\n"
                    "        cannot cast the callback into QString or std::function<void()>."
                   , path.toStdString().c_str() ) << endl;
    }
  }


  void  CellViewer::createMenus ()
  {
    addMenu( "file"         , "File"      , TopMenu );
    addMenu( "view"         , "View"      , TopMenu );
    addMenu( "tools"        , "Tools"     , TopMenu );
    addMenu( "placeAndRoute", "P&&R"      , TopMenu );
    addMenu( "misc"         , "Misc"      , TopMenu );
    addMenu( "misc.alpha"   , "&alpha"    );
    addMenu( "misc.obsolete", "&Obsolete" );

  // Building the "File" menu.
    QAction* action = addToMenu( "viewer.interrupt"
                               , tr("Interrupt")
                               , tr("Interrupt the running tool")
                               , QKeySequence(tr("CTRL+C"))
                               );
    action->setVisible( false );
    addAction( action );
    connect( action, SIGNAL(triggered()), this, SLOT(raiseToolInterrupt()) );

    action = addToMenu( "file.openCell"
                      , tr("&Open Cell")
                      , tr("Open (load) a new Cell")
                      , QKeySequence(tr("CTRL+O"))
                      , QIcon(":/images/stock_open.png")
                      );
    addToMenu( "file.========" );

    for ( int i=0 ; i<CellHistorySize ; i++ ) {
      _cellHistoryAction[i] = addToMenu( QString("file.cellHistory[%1]").arg(i)
                                       , QString("Slot[%1]").arg(i)
                                       , QString("History empty Slot[%1]").arg(i)
                                       , QKeySequence()
                                       );
      _cellHistoryAction[i]->setVisible( false );
      _cellHistoryAction[i]->setData   ( i );
      _cellHistoryAction[i]->setFont   ( Graphics::getFixedFont(QFont::Bold,false,false) );
      connect( _cellHistoryAction[i], SIGNAL(triggered()), this, SLOT(openHistoryCell()) );
    }
    addToMenu( "file.========" );

    action = addToMenu( "file.saveCell"
                      , tr("&Save Cell")
                      , tr("Save (write) the current Cell")
                      , QKeySequence()
                      , QIcon(":/images/stock_save.png")
                      );
    action->setVisible( false );
    addToMenu( "file.========" );

    action = addToMenu( "file.openDesignBlob"
                      , tr("&Open Design Blob")
                      , tr("Reload (restore) the whole Hurricane DataBase state")
                      , QKeySequence()
                      , QIcon(":/images/stock_open.png")
                      );
    connect( action, SIGNAL(triggered()), this, SLOT(openDesignBlob()) );
    action = addToMenu( "file.saveDesignBlob"
                      , tr("&Save Design Blob")
                      , tr("Save (dump) the whole Hurricane DataBase state")
                      , QKeySequence()
                      , QIcon(":/images/stock_save.png")
                      );
    connect( action, SIGNAL(triggered()), this, SLOT(saveDesignBlob()) );
    addToMenu( "file.========" );

    action = addToMenu( "file.importCell"
                      , tr("&Import Cell")
                      , tr("Import (convert) a new Cell")
                      , QKeySequence()
                      );
    action = addToMenu( "file.exportCell"
                      , tr("&Export Cell")
                      , tr("Export (convert) Cell")
                      , QKeySequence()
                      );
    addToMenu( "file.========" );

    action = addToMenu( "file.print"
                      , tr("&Print")
                      , tr("Print the displayed area")
                      , QKeySequence(tr("CTRL+P"))
                      );
    connect( action, SIGNAL(triggered()), this, SLOT(printDisplay()) );

    action = addToMenu( "file.image"
                      , tr("Save to &Image")
                      , tr("Save the displayed area to image")
                      , QKeySequence()
                      );
    connect( action, SIGNAL(triggered()), this, SLOT(imageDisplay()) );

    action = addToMenu( "file.nextBreakpoint"
                      , tr("&Next Breakpoint")
                      , tr("Proceed to the next breakpoint")
                      , QKeySequence()
                      );
    addToMenu( "file.========" );

    action = addToMenu( "file.close"
                      , tr("&Close")
                      , tr("Close This Coriolis CellViewer")
                      , QKeySequence(tr("CTRL+W"))
                      );
    connect( action, SIGNAL(triggered()), this, SLOT(close()) );

    action = addToMenu( "file.close"
                      , tr("&Close")
                      , tr("Close This Coriolis CellViewer")
                      , QKeySequence(tr("CTRL+W"))
                      );

    action = addToMenu( "file.exit"
                      , tr("&Exit")
                      , tr("Exit All Coriolis CellViewer")
                      , QKeySequence(tr("CTRL+Q"))
                      );
    connect( action, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()) );

  // Building the "View" menu.
    action = addToMenu( "view.refresh"
                      , tr("&Refresh")
                      , tr("Force full redrawing of the display")
                      , QKeySequence(tr("CTRL+L"))
                      );
    connect( action, SIGNAL(triggered()), _cellWidget, SLOT(refresh()) );

    action = addToMenu( "view.fit"
                      , tr("&Fit to Contents")
                      , tr("Adjust zoom to fit the whole cell's contents")
                      , Qt::Key_F
                      );
    connect( action, SIGNAL(triggered()), _cellWidget, SLOT(fitToContents()) );

    action = addToMenu( "view.goto"
                      , tr("&Goto")
                      , tr("Center view on that point, with zoom adjustment")
                      , Qt::Key_G
                      );
    connect( action, SIGNAL(triggered()), this, SLOT(doGoto()) );

    _showSelectionAction = addToMenu( "view.showSelection"
                                    , tr("&Show Selection")
                                    , tr("Highlight the selected items (darken others)")
                                    , Qt::Key_S
                                    );
    _showSelectionAction->setCheckable( true );
    connect( _showSelectionAction, SIGNAL(toggled(bool)), this, SLOT(setShowSelection(bool)) );

    action = addToMenu( "view.changeRubber"
                      , tr("Change Rubber Style")
                      , tr("Cycle through all avalaibles rubber drawing styles")
                      , Qt::Key_Asterisk
                      );
    connect( action, SIGNAL(triggered()), _cellWidget, SLOT(rubberChange()) );

    action = addToMenu( "view.clearRulers"
                      , tr("Clear Rulers")
                      , tr("Remove all rulers")
                      , QKeySequence()
                      );
    connect( action, SIGNAL(triggered()), _cellWidget, SLOT(clearRulers()) );

  // Building the "Tools" menu.
    action = addToMenu( "tools.controller"
                      , tr("Controller")
                      , tr("Fine Tune && Inspect DataBase")
                      , QKeySequence(tr("CTRL+I"))
                      , QIcon(":/images/swiss-knife.png")
                      );
    connect( action, SIGNAL(triggered()), _controller, SLOT(toggleShow()) );

    action = addToMenu( "tools.script"
                      , tr("Python Script")
                      , tr("Run Python Script. Must provide a ScripMain(cell) function")
                      , QKeySequence(tr("SHIFT+P,SHIFT+S"))
                      , QIcon(":/images/python-logo-v3.png")
                      );
    connect( action, SIGNAL(triggered()), this, SLOT(runScriptWidget()) );

    // action = addToMenu( "tools.stressScript"
    //                   , tr("Python Stress Script")
    //                   , tr("Run Python Stress Script (50 times...).")
    //                   , QKeySequence()
    //                   , QIcon(":/images/python-logo-v3.png")
    //                   );
    // connect( action, SIGNAL(triggered()), this, SLOT(runStressScript()) );
  }


  QMenu* CellViewer::createDebugMenu ()
  {
    if (not _debugMenu)
      addMenu ( "debug" , "Debug" , TopMenu );
    return _debugMenu;
  }


  void  CellViewer::refreshTitle ()
  {
    QString  cellName = "None";
    if ( getCell() )
      cellName = getString(getCell()->getName()).c_str();

    QString  title = QString("%1:<%2>").arg(_applicationName).arg(cellName);
    setWindowTitle ( title );
  }


  void  CellViewer::refreshHistory ()
  {
    if ( getCell() == NULL ) return;

    shared_ptr<CellWidget::State> activeState = _cellWidget->getState();
    _cellHistory.remove ( activeState );

    if ( _cellHistory.size() > CellHistorySize-1 )
      _cellHistory.pop_front ();
    _cellHistory.push_back ( activeState );

    rebuildHistory ();
  }


  void  CellViewer::rebuildHistory ()
  {
    list< shared_ptr<CellWidget::State> >::iterator istate = _cellHistory.begin();
    for ( size_t i=0 ; i<CellHistorySize ; i++ ) {
      if ( istate != _cellHistory.end() ) {
        QString entry = tr("&%1: %2 %3")
          .arg(i+1)
          .arg( getString((*istate)->getName()).c_str() )
          .arg( (*istate)->getTopPath().getCompactString().c_str() );
        _cellHistoryAction[i]->setText    ( entry );
        _cellHistoryAction[i]->setVisible ( true );
        istate++;
      } else {
        _cellHistoryAction[i]->setVisible ( false );
      }
    }
  }


  void  CellViewer::removeHistory ( Cell* cell )
  {
    Name cellName = (cell) ? cell->getName() : "empty";

    list< shared_ptr<CellWidget::State> >::iterator istate
      = find_if( _cellHistory.begin(), _cellHistory.end(), CellWidget::FindStateName(cellName) );

    if (istate != _cellHistory.end()) {
      _cellHistory.remove ( *istate );
      rebuildHistory ();
    }
  }


  void  CellViewer::setState ( shared_ptr<CellWidget::State>& state )
  {
    static bool isEmitter = false;

    if ( sender() == this ) {
        isEmitter = true;
        emit stateChanged ( state );
    } else {
      if ( !isEmitter ) {
        blockSignals ( true );

        refreshTitle   ();
        refreshHistory ();
        
        blockSignals ( false );
      } else
        isEmitter = false;
    }
  }


  void  CellViewer::setCell ( Cell* cell )
  {
    if (cell == getCell()) return;
    if (getCell()) getCell()->removeObserver( getCellObserver() );

    Name cellName = (cell) ? cell->getName() : "empty";

    list< shared_ptr<CellWidget::State> >::iterator istate
      = find_if( _cellHistory.begin(), _cellHistory.end(), CellWidget::FindStateName(cellName) );

    if (istate != _cellHistory.end()) {
      cerr << "CellViewer::setCell() " << (*istate)->getCell() << endl;

      (*istate)->getCell()->addObserver( getCellObserver() );
      emit stateChanged ( *istate );
      return;
    }

    if (cell) cell->addObserver( getCellObserver() );
    _cellWidget->setCell( cell );
  }


  Cell* CellViewer::getCell () const
  { return getCellWidget()->getCell(); }


  Cell* CellViewer::getCellFromDb ( const char* name )
  {
    cerr << "[ERROR] virtual function CellViewer::getCellFromDb() has not been overloaded.\n"
         << "        (this will prevent \"Open Cell\" to work)"
         << endl;

    return NULL;
  }


  void  CellViewer::renameCell ( const char* name )
  {
    Cell* cell = getCell();
    if ( cell == NULL ) return;

    cell->setName ( name );

    refreshTitle   ();
    refreshHistory ();
  }


  void  CellViewer::doGoto ()
  {
    if ( _goto->exec() == QDialog::Accepted ) {
      if ( not _goto->hasXy() ) return;

      Box        gotoArea ( _goto->getX(), _goto->getY() );
      DbU::Unit  width;
      DbU::Unit  height;

      if ( _goto->hasAperture() ) {
        width = height = _goto->getAperture() / 2;
      } else {
        Box visibleArea = _cellWidget->getVisibleArea ();
        width  = visibleArea.getWidth ()/2;
        height = visibleArea.getHeight()/2;
      }

      gotoArea.inflate ( width, height );
      _cellWidget->reframe ( gotoArea );
    }
  }


  void  CellViewer::setAnonNetSelectable ( bool state )
  {
    SelectCommand* command = static_cast<SelectCommand*>( getCellWidget()->getCommand(SelectCommand::getStaticName()) );
    unsigned int   mode    = (state) ? SelectCommand::NetMode : SelectCommand::NoAnonNetMode;

    if (command) command->setSelectMode( mode );
  }


  void  CellViewer::changeSelectionMode ()
  {
    if ( _updateState != InternalEmit ) {
      _showSelectionAction->blockSignals ( true );
      _showSelectionAction->setChecked   ( _cellWidget->getState()->cumulativeSelection() );
      _showSelectionAction->blockSignals ( false );
    }
    _updateState = ExternalEmit;
  }


  void  CellViewer::setShowSelection ( bool )
  {
    _updateState = InternalEmit;
    _cellWidget->setShowSelection ( not _cellWidget->showSelection() );
  }


  void  CellViewer::setCumulativeSelection ( bool state )
  {
    _updateState = InternalEmit;
    _cellWidget->setCumulativeSelection ( state );
  }


  void  CellViewer::raiseToolInterrupt ()
  { _toolInterrupt = true; }


  void  CellViewer::clearToolInterrupt ()
  { _toolInterrupt = false; }


  void  CellViewer::openHistoryCell ()
  {
    QAction* historyAction = qobject_cast<QAction*>( sender() );
    if (historyAction) {
      list< shared_ptr<CellWidget::State> >::iterator  istate = _cellHistory.begin();
      size_t index = historyAction->data().toUInt();
      for ( ; index>0 ; index--, ++istate )
	;
      emit stateChanged ( *istate );
    }
  }


  void  CellViewer::openDesignBlob ()
  {
    QString blobName;
    if (OpenBlobDialog::runDialog(this,blobName)) {
      string fileName = blobName.toStdString() + ".blob";
    //DebugSession::open( 0, 500 );

      Cell*       topCell = NULL;
      DesignBlob* blob    = DesignBlob::fromJson( fileName );

      if (blob) topCell = blob->getTopCell();
      delete blob;

    //DebugSession::close();

      setCell ( topCell );
      emit cellLoadedFromDisk( topCell );
    }
  }


  void  CellViewer::saveDesignBlob ()
  {
    Cell* cell = getCell();
    if (not cell) return;

    string     blobName = getString(cell->getName()) + ".blob.json.bz2";
    DesignBlob blob     ( cell );

  //DebugSession::open( 0, 500 );
    JsonWriter writer ( blobName );
    writer.setFlags( JsonWriter::DesignBlobMode );
    jsonWrite( &writer, &blob );
  //DebugSession::close();
  }


  void  CellViewer::select ( Occurrence& occurrence )
  { if ( _cellWidget ) _cellWidget->select ( occurrence ); }


  void  CellViewer::unselect ( Occurrence& occurrence )
  { if ( _cellWidget ) _cellWidget->unselect ( occurrence ); }


  void  CellViewer::unselectAll ()
  { if ( _cellWidget ) _cellWidget->unselectAll(); }


  void  CellViewer::reframe ( const Box& area, bool historyEnable )
  { if ( _cellWidget ) _cellWidget->reframe( area, historyEnable ); }


  void  CellViewer::printDisplay ()
  {
    if ( !_cellWidget ) return;
    if ( !_cellWidget->getCell() ) {
      cerr << Warning("Unable to print, no cell loaded yet.") << endl;
      return;
    }

    QPrinter printer ( QPrinter::ScreenResolution );
    printer.setOutputFileName ( "unicorn-snapshot.pdf" );
    printer.setPageSize       ( (QPrinter::PaperSize  )Cfg::getParamEnumerate("viewer.printer.paper"      ,0)->asInt() );
    printer.setPageOrientation( (QPageLayout::Orientation)Cfg::getParamEnumerate("viewer.printer.orientation",0)->asInt() );

    QPrintDialog  dialog ( &printer );
    if ( dialog.exec() == QDialog::Accepted )
      print ( &printer );
  }
 

  void  CellViewer::print ( QPrinter* printer )
  {
    CellPrinter* cellPrinter = new CellPrinter();

    cellPrinter->setScreenCellWidget( _cellWidget );
    cellPrinter->toPdf              ( printer, false );

    delete cellPrinter;
  }


  void  CellViewer::imageDisplay ()
  {
    if ( !_cellWidget ) return;
    if ( !_cellWidget->getCell() ) {
      cerr << Warning("Unable to save to image, no cell loaded yet.") << endl;
      return;
    }

    CellImage* cellImage = new CellImage();
    cellImage->setScreenCellWidget( _cellWidget );
    QImage* image = cellImage->toImage(0);
    delete cellImage;

    char workingDirectory [1024];
    getcwd ( workingDirectory, 1024 );

    QString filePath = QFileDialog::getSaveFileName ( this
                                                    , tr("Save Image as ...")
                                                    , workingDirectory
                                                    , tr("Image (*.png)")
                                                    );

    image->save ( filePath, "png" );
    delete image;
  }


  void  CellViewer::_runScript ( QString scriptPath )
  {
    if (scriptPath.endsWith(".py",Qt::CaseInsensitive))
      scriptPath.truncate( scriptPath.size()-3 );

    Utilities::Path userScript    ( scriptPath.toStdString() );
    Utilities::Path userDirectory ( userScript.dirname() );

    if (not userDirectory.absolute())
      userDirectory = Utilities::Path::cwd() / userDirectory;

    Isobar::Script::addPath( userDirectory.toString() );

    dbo_ptr<Isobar::Script> script = Isobar::Script::create(userScript.basename().toString());
    script->addKwArgument( "cell"      , (PyObject*)PyCell_Link(getCell()) );
    script->addKwArgument( "editor"    , (PyObject*)PyCellViewer_Link(this) );
    script->runFunction  ( "scriptMain", getCell() );

    Isobar::Script::removePath( userDirectory.toString() );
  }


  void  CellViewer::runScript ( QString scriptPath )
  { ExceptionWidget::catchAllWrapper( std::bind( &CellViewer::_runScript, this, scriptPath ) ); }


  void  CellViewer::runStressScript ()
  {
    for ( size_t i=0 ; i<1000 ; ++i ) {
      cerr << "Calling ./stressScript [" << setw(3) << right << setfill('0') << i << "]" << setfill(' ') << endl;
      ExceptionWidget::catchAllWrapper( std::bind( &CellViewer::_runScript, this, "stressScript.py" ) );
    }
  }


  void  CellViewer::runScriptWidget ()
  { ScriptWidget::runScript( this, _pyScriptName, getCell() ); }


  string  CellViewer::_getString () const
  {
    ostringstream s;
    s << "<CellViewer ";
    Cell* cell = getCell();
    if (cell) s << getString(cell->getName());
    else      s << "No_Cell_Loaded";
    s << ">";
    return s.str();
  }


} // End of Hurricane namespace.
