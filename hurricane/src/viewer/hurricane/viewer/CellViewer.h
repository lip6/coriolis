
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/viewer/CellViewer.h"               |
// +-----------------------------------------------------------------+


#ifndef  __HURRICANE_CELL_VIEWER__
#define  __HURRICANE_CELL_VIEWER__


#include   <list>

using namespace std;

#include  <QMainWindow>

class QEvent;
class QKeyEvent;
class QAction;
class QMenu;
class QPrinter;

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
              Cell*                getCell                   () const;
              void                 setCell                   ( Cell* );
              void                 renameCell                ( const char* );
      virtual Cell*                getCellFromDb             ( const char* );
      inline  CellWidget*          getCellWidget             ();
      inline  const CellWidget*    getCellWidget             () const;
      inline  ControllerWidget*    getControllerWidget       ();
              void                 setAnonNetSelectable      ( bool );
              void                 select                    ( Occurrence& );
              void                 unselect                  ( Occurrence& );
              void                 unselectAll               ();
      inline  void                 setLayerVisible           ( const Name& layer, bool visible );
      virtual std::string          _getString                () const;
    public slots:                  
              void                 doGoto                    ();
              void                 changeSelectionMode       ();
              void                 setShowSelection          ( bool );
              void                 setState                  ( shared_ptr<CellWidget::State>& );
              void                 openHistoryCell           ();
              void                 printDisplay              ();
              void                 print                     ( QPrinter* );
              void                 imageDisplay              ();
              void                 raiseToolInterrupt        ();
              void                 clearToolInterrupt        ();
              void                 runScript                 ();
            //void                 runStratusScript          ();
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
      QAction*                 _stratusAction;
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
  inline bool              CellViewer::isToolInterrupted   () const { return _toolInterrupt; }
  inline CellWidget*       CellViewer::getCellWidget       () { return _cellWidget; }
  inline const CellWidget* CellViewer::getCellWidget       () const { return _cellWidget; }
  inline ControllerWidget* CellViewer::getControllerWidget () { return _controller; }
  inline void              CellViewer::setApplicationName  ( const QString& name ) { _applicationName = name; }
  inline void              CellViewer::setLayerVisible     ( const Name& layer, bool visible ) { _cellWidget->setLayerVisible(layer,visible); }

  inline void  CellViewer::setEnableRedrawInterrupt  ( bool state )
  { _cellWidget->setEnableRedrawInterrupt(state); }


} // Hurricane namespace.


GETSTRING_POINTER_SUPPORT(Hurricane::CellViewer)
IOSTREAM_POINTER_SUPPORT(Hurricane::CellViewer)


#endif
