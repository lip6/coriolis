// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
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
// |  C++ Header  :       "./CellViewer.h"                           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __HURRICANE_CELL_VIEWER__
#define  __HURRICANE_CELL_VIEWER__


#include   <list>

using namespace std;

#include  <QMainWindow>

class QEvent;
class QKeyEvent;
class QAction;
class QMenu;

#include  "hurricane/Commons.h"
#include  "hurricane/Name.h"
#include  "hurricane/Occurrence.h"

#include  "hurricane/viewer/MoveCommand.h"
#include  "hurricane/viewer/ZoomCommand.h"
#include  "hurricane/viewer/RulerCommand.h"
#include  "hurricane/viewer/SelectCommand.h"
#include  "hurricane/viewer/HierarchyCommand.h"
#include  "hurricane/viewer/CellWidget.h"


namespace Hurricane {


  class Cell;
//class MapView;
  class GotoWidget;
  class MousePositionWidget;
  class ControllerWidget;
  class ScriptWidget;


  class CellViewer : public QMainWindow {
      Q_OBJECT;
                                   
    public:                        
      enum  { CellHistorySize = 10 };
    public:
                                   CellViewer                ( QWidget* parent=NULL );
      virtual                     ~CellViewer                ();
      inline  bool                 isToolInterrupted         () const;
              QMenu*               createDebugMenu           ();
      inline  void                 setEnableRedrawInterrupt  ( bool );
      inline  void                 setApplicationName        ( const QString& );
              Cell*                getCell                   ();
              void                 setCell                   ( Cell* );
              void                 renameCell                ( const char* );
      virtual Cell*                getCellFromDb             ( const char* );
      inline  CellWidget*          getCellWidget             ();
      inline  ControllerWidget*    getControllerWidget       ();
              void                 select                    ( Occurrence& );
              void                 unselect                  ( Occurrence& );
              void                 unselectAll               ();
    public slots:                  
              void                 doGoto                    ();
              void                 changeSelectionMode       ();
              void                 setShowSelection          ( bool );
              void                 setState                  ( shared_ptr<CellWidget::State>& );
              void                 openHistoryCell           ();
              void                 printDisplay              ();
              void                 imageDisplay              ();
              void                 raiseToolInterrupt        ();
              void                 clearToolInterrupt        ();
              void                 runScript                 ();
    signals:                       
              void                 showSelectionToggled      ( bool );
              void                 stateChanged              ( shared_ptr<CellWidget::State>& );
              void                 redrawCellWidget          ();
    protected:                     
              void                 createActions             ();
              void                 createMenus               ();
              void                 createLayout              ();
              void                 refreshTitle              ();
              void                 refreshHistory            ();

    protected:                     
      QString                  _applicationName;
      QAction*                 _toolInterruptAction;
      QAction*                 _openAction;
      QAction*                 _importAction;
      QAction*                 _nextAction;
      QAction*                 _cellHistoryAction[CellHistorySize];
      QAction*                 _printAction;
      QAction*                 _imageAction;
      QAction*                 _saveAction;
      QAction*                 _exportAction;
      QAction*                 _closeAction;
      QAction*                 _exitAction;
      QAction*                 _refreshAction;
      QAction*                 _fitToContentsAction;
      QAction*                 _gotoAction;
      QAction*                 _showSelectionAction;
      QAction*                 _rubberChangeAction;
      QAction*                 _clearRulersAction;
      QAction*                 _controllerAction;
      QAction*                 _scriptAction;
      QMenu*                   _fileMenu;
      QMenu*                   _viewMenu;
      QMenu*                   _toolsMenu;
      QMenu*                   _debugMenu;
    //MapView*                 _mapView;
      MousePositionWidget*     _mousePosition;
      ControllerWidget*        _controller;
      ScriptWidget*            _script;
      GotoWidget*              _goto;
      CellWidget*              _cellWidget;
      MoveCommand              _moveCommand;
      ZoomCommand              _zoomCommand;
      RulerCommand             _rulerCommand;
      SelectCommand            _selectCommand;
      HierarchyCommand         _hierarchyCommand;
      list< shared_ptr<CellWidget::State> >
                               _cellHistory;
      bool                     _firstShow;
      bool                     _toolInterrupt;
      UpdateState              _updateState;

  };


// Inline Functions.
  inline void        CellViewer::setEnableRedrawInterrupt  ( bool state )
  { _cellWidget->setEnableRedrawInterrupt(state); }

  inline bool              CellViewer::isToolInterrupted   () const { return _toolInterrupt; }
  inline CellWidget*       CellViewer::getCellWidget       () { return _cellWidget; }
  inline ControllerWidget* CellViewer::getControllerWidget () { return _controller; }
  inline void              CellViewer::setApplicationName  ( const QString& name ) { _applicationName = name; }


} // End of Hurricane namespace.


#endif
