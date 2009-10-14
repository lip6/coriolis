// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
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
  class HPalette;
  class HGraphics;
  class HDisplayFilter;
//class MapView;
  class MousePositionWidget;
  class HSelection;
  class ControllerWidget;


  class CellViewer : public QMainWindow {
      Q_OBJECT;

    public:
                                   CellViewer                ( QWidget* parent=NULL );
      virtual                     ~CellViewer                ();
      inline  bool                 isToolInterrupted         () const;
              QMenu*               createDebugMenu           ();
      inline  void                 setEnableRedrawInterrupt  ( bool );
      inline  void                 setApplicationName        ( const QString& );
              void                 setCell                   ( Cell* );
              Cell*                getCell                   ();
      virtual Cell*                getCellFromDb             ( const char* name );
      inline  CellWidget*          getCellWidget             ();
      inline  ControllerWidget*    getControllerWidget       ();
              void                 select                    ( Occurrence& );
              void                 unselect                  ( Occurrence& );
              void                 unselectAll               ();
    public slots:                  
              void                 changeSelectionMode       ();
              void                 setShowSelection          ( bool );
              void                 setState                  ( shared_ptr<CellWidget::State>& );
              void                 openHistoryCell           ();
              void                 printDisplay              ();
              void                 imageDisplay              ();
              void                 raiseToolInterrupt        ();
              void                 clearToolInterrupt        ();
    signals:                       
              void                 showSelectionToggled      ( bool );
              void                 stateChanged              ( shared_ptr<CellWidget::State>& );
              void                 redrawCellWidget          ();
                                   
    public:                        
              enum                 { CellHistorySize = 10 };
    protected:                     
              QString              _applicationName;
              QAction*             _toolInterruptAction;
              QAction*             _openAction;
              QAction*             _nextAction;
              QAction*             _cellHistoryAction[CellHistorySize];
              QAction*             _printAction;
              QAction*             _imageAction;
              QAction*             _saveAction;
              QAction*             _closeAction;
              QAction*             _exitAction;
              QAction*             _refreshAction;
              QAction*             _fitToContentsAction;
              QAction*             _showSelectionAction;
              QAction*             _rubberChangeAction;
              QAction*             _clearRulersAction;
              QAction*             _controllerAction;
              QMenu*               _fileMenu;
              QMenu*               _viewMenu;
              QMenu*               _toolsMenu;
              QMenu*               _debugMenu;
            //MapView*             _mapView;
              HPalette*            _palette;
              MousePositionWidget* _mousePosition;
              ControllerWidget*    _controller;
              CellWidget*          _cellWidget;
              MoveCommand          _moveCommand;
              ZoomCommand          _zoomCommand;
              RulerCommand         _rulerCommand;
              SelectCommand        _selectCommand;
              HierarchyCommand     _hierarchyCommand;
              list< shared_ptr<CellWidget::State> >
                                   _cellHistory;
              bool                 _firstShow;
              bool                 _toolInterrupt;
              UpdateState          _updateState;
                                   
    protected:                     
              void                 createActions             ();
              void                 createMenus               ();
              void                 createLayout              ();
              void                 refreshTitle              ();
              void                 refreshHistory            ();
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
