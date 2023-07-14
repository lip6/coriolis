// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
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


#pragma  once
#include <list>
#include <map>
#include <functional>
#include <boost/any.hpp>
#include <QIcon>
#include <QMainWindow>
class QEvent;
class QKeyEvent;
class QAction;
class QMenu;
class QPrinter;
#include "hurricane/Commons.h"
#include "hurricane/Observer.h"
#include "hurricane/Name.h"
#include "hurricane/Occurrence.h"
#include "hurricane/viewer/MoveCommand.h"
#include "hurricane/viewer/ZoomCommand.h"
#include "hurricane/viewer/RulerCommand.h"
#include "hurricane/viewer/SelectCommand.h"
#include "hurricane/viewer/HierarchyCommand.h"
#include "hurricane/viewer/CellWidget.h"


namespace Hurricane {


  class Cell;
//class MapView;
  class GotoWidget;
  class MousePositionWidget;
  class ControllerWidget;
  class ScriptWidget;
  class CellViewer;


// -------------------------------------------------------------------
// Class  :  "CellObserver".

  // class CellObserver : public Observer<CellViewer> {
  //   public:
  //     inline        CellObserver ( CellViewer* );
  //     virtual void  notify       ( unsigned int flags );
  //   private:
  //                   CellObserver ( const CellObserver& );
  // };


  // inline  CellObserver::CellObserver ( CellViewer* owner )
  //   : Observer<CellViewer>(owner)
  // { }


// -------------------------------------------------------------------
// Class  :  "CellViewer".

  class CellViewer : public QMainWindow {
      Q_OBJECT;
                                   
    public:                        
      enum               { CellHistorySize = 10 };
      enum  Flag         { InCellChange    = 0x0001 };
      enum  FunctionFlag { NoFlags         = 0x0000
                         , TopMenu         = 0x0001 };
    private:
      typedef  std::map< const QString, boost::any >  ActionLut;
      typedef  bool (QWidget::* SlotMethod)();
    public:
      static  void                  notify                    ( CellViewer*, unsigned int flags );
    public:
                                    CellViewer                ( QWidget* parent=NULL );
      virtual                      ~CellViewer                ();
      inline  bool                  isToolInterrupted         () const;
              void                  refreshTitle              ();
              QMenu*                createDebugMenu           ();
              bool                  hasMenu                   ( const QString& path ) const;
              bool                  hasMenuAction             ( const QString& path ) const;
              QAction*              getMenuAction             ( const QString& path ) const;
              QAction*              addMenu                   ( const QString& path
                                                              , std::string text
                                                              , unsigned int flags=NoFlags
                                                              );
              bool                  addToMenu                 ( const QString& path
                                                              ,       QString  beforePath="" );
              QAction*              addToMenu                 ( const QString& path
                                                              , std::string    text
                                                              , std::string    textTip
                                                              , std::function< void() >
                                                              , QIcon          icon=QIcon()
                                                              ,      QString   beforePath="" );
              QAction*              addToMenu                 ( const QString& path
                                                              , std::string    text
                                                              , std::string    textTip
                                                              , std::string    scriptPath
                                                              ,       QString  beforePath="" );
              QAction*              addToMenu                 ( QString             path
                                                              , QString             text
                                                              , QString             textTip
                                                              , const QKeySequence& shortCut 
                                                              , QIcon               icon    =QIcon()
                                                              , QString             beforePath="" );
      inline  void                  setEnableRedrawInterrupt  ( bool );
      inline  void                  setApplicationName        ( const QString& );
      inline  void                  setPixelThreshold         ( int );
      inline  void                  setDbuMode                ( int );
      inline  Observer<CellViewer>* getCellObserver           ();
              Cell*                 getCell                   () const;
      virtual void                  setCell                   ( Cell* );
              void                  renameCell                ( const char* );
      virtual Cell*                 getCellFromDb             ( const char* );
      inline  CellWidget*           getCellWidget             ();
      inline  const CellWidget*     getCellWidget             () const;
      inline  ControllerWidget*     getControllerWidget       ();
              void                  setAnonNetSelectable      ( bool );
              void                  select                    ( Occurrence& );
              void                  unselect                  ( Occurrence& );
              void                  unselectAll               ();
      inline  void                  setLayerVisible           ( const Name& layer, bool visible );
              void                  reframe                   ( const Box& , bool historyEnable=true );
              void                  runScript                 ( QString scriptPath );
      virtual CellViewer*           vcreate                   () const;
      virtual std::string           _getString                () const;
    public slots:                   
              void                  doAction                  ();
              void                  doGoto                    ();
              void                  changeSelectionMode       ();
              void                  setShowSelection          ( bool );
              void                  setCumulativeSelection    ( bool );
              void                  setState                  ( shared_ptr<CellWidget::State>& );
              void                  removeHistory             ( Cell* );
              void                  openHistoryCell           ();
              void                  openDesignBlob            ();
              void                  saveDesignBlob            ();
              void                  printDisplay              ();
              void                  print                     ( QPrinter* );
              void                  imageDisplay              ();
              void                  raiseToolInterrupt        ();
              void                  clearToolInterrupt        ();
              void                  runScriptWidget           ();
              void                  runStressScript           ();
      inline  void                  emitCellAboutToChange     ();
      inline  void                  emitCellChanged           ();
      inline  void                  emitCellPreModificated    ();
      inline  void                  emitCellPostModificated   ();
    signals:                        
              void                  cellLoadedFromDisk        ( Cell* );
              void                  showSelectionToggled      ( bool );
              void                  stateChanged              ( shared_ptr<CellWidget::State>& );
              void                  redrawCellWidget          ();
              void                  cellPreModificated        ();
              void                  cellPostModificated       ();
    protected:                      
              void                  createMenus               ();
              void                  refreshHistory            ();
              void                  rebuildHistory            ();
    private:                        
              QString               _getAbsWidgetPath         ( const QString& relPath ) const;
              QAction*              _getParentMenu            ( const QString& ) const;
              void                  _runScript                ( QString scriptPath );

    protected:                     
      static QString                  _prefixWPath;
             Observer<CellViewer>     _cellObserver;
             QString                  _applicationName;
             QAction*                 _openAction;
             QAction*                 _cellHistoryAction[CellHistorySize];
             QAction*                 _showSelectionAction;
             QMenu*                   _debugMenu;
             ActionLut                _actionCallbacks;
    //       MapView*                 _mapView;
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
             unsigned int             _flags;
             UpdateState              _updateState;
             QString                  _pyScriptName;
  };


// Inline Functions.
  inline bool                  CellViewer::isToolInterrupted       () const { return _toolInterrupt; }
  inline Observer<CellViewer>* CellViewer::getCellObserver         () { return &_cellObserver; }
  inline CellWidget*           CellViewer::getCellWidget           () { return _cellWidget; }
  inline const CellWidget*     CellViewer::getCellWidget           () const { return _cellWidget; }
  inline ControllerWidget*     CellViewer::getControllerWidget     () { return _controller; }
  inline void                  CellViewer::setApplicationName      ( const QString& name ) { _applicationName = name; }
  inline void                  CellViewer::setLayerVisible         ( const Name& layer, bool visible ) { _cellWidget->setLayerVisible(layer,visible); }
  inline void                  CellViewer::emitCellAboutToChange   () { _flags |=  InCellChange; emit cellPreModificated(); }
  inline void                  CellViewer::emitCellChanged         () { _flags &= ~InCellChange; emit cellPostModificated(); }
  inline void                  CellViewer::emitCellPreModificated  () { emit cellPreModificated(); }
  inline void                  CellViewer::emitCellPostModificated () { emit cellPostModificated(); }

  inline  void  CellViewer::setPixelThreshold ( int threshold )
  { _cellWidget->setPixelThreshold( threshold ); }

  inline void  CellViewer::setEnableRedrawInterrupt  ( bool state )
  { _cellWidget->setEnableRedrawInterrupt(state); }

  inline void  CellViewer::setDbuMode ( int mode )
  { _cellWidget->setDbuMode(mode); }


} // Hurricane namespace.


GETSTRING_POINTER_SUPPORT(Hurricane::CellViewer)
IOSTREAM_POINTER_SUPPORT(Hurricane::CellViewer)
