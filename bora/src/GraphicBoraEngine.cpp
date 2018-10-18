// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Authors     :          Jean-Paul Chaput, Eric LAO              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./GraphicBoraEngine.cpp"                       |
// +-----------------------------------------------------------------+


#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QLineEdit>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/DebugSession.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/DataBase.h"
#include "hurricane/Library.h"
#include "hurricane/viewer/Graphics.h"
#include "hurricane/viewer/HApplication.h"
#include "hurricane/viewer/CellWidget.h"
#include "hurricane/viewer/ExceptionWidget.h"
#include "crlcore/GdsDriver.h"
#include "hurricane/analog/AnalogCellExtension.h"
#include "hurricane/analog/LayoutGenerator.h"
#include "hurricane/analog/TransistorFamily.h"
#include "bora/SlicingWidget.h"
#include "bora/SlicingDataWidget.h"
#include "bora/SlicingPlotWidget.h"
#include "bora/GraphicBoraEngine.h"
#include "bora/SlicingWidget.h"


namespace Bora {

  using namespace std;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Breakpoint;
  using Hurricane::DebugSession;
  using Hurricane::UpdateSession;
  using Hurricane::Point;
  using Hurricane::Net;
  using Hurricane::Graphics;
  using Hurricane::ColorScale;
  using Hurricane::ControllerWidget;
  using Hurricane::ExceptionWidget;
  using CRL::GdsDriver;


// -------------------------------------------------------------------
// Class  :  "Bora".

  size_t             GraphicBoraEngine::_references = 0;
  GraphicBoraEngine* GraphicBoraEngine::_singleton  = NULL;


  BoraEngine* GraphicBoraEngine::createEngine ()
  {
    cdebug_log(536,0) << "BoraEngine* GraphicBoraEngine::createEngine ()" << endl;
    Cell* cell = getCell ();
    if (_slicingWidget == NULL) _slicingWidget =  new SlicingWidget();

    BoraEngine* bora = BoraEngine::get( cell );
    if (not bora) {
      bora = BoraEngine::create( cell );
      bora->setViewer( _viewer );
      _slicingWidget->setViewer( _viewer );
    } else
      cerr << Warning( "%s already has a Bora engine.", getString(cell).c_str() ) << endl;

    return bora;
  }


  BoraEngine* GraphicBoraEngine::getForFramework ( unsigned int flags )
  {
    cdebug_log(536,0) << "BoraEngine* GraphicBoraEngine::getForFramework(unsigned int flags) flags:" << (flags==CreateEngine) << endl;
  // Currently, only one framework is avalaible: Alliance.

    BoraEngine* bora = BoraEngine::get( getCell() ); 
    if (bora){
      if (not _slicingWidget){
        bora->setViewer( _viewer );
        _slicingWidget =  new SlicingWidget();
        _slicingWidget->setViewer( _viewer );
        connect( _slicingWidget, SIGNAL(updatePlacement  ( BoxSet* ))
               , this          , SLOT  (updatePlacementST( BoxSet* )) );
      }
      return bora;
    }

    if (flags & CreateEngine) {
      bora = createEngine();
      if (not bora) 
        throw Error( "Failed to create Bora engine on %s.", getString( getCell()).c_str() );
      connect( _slicingWidget, SIGNAL(updatePlacement  ( BoxSet* ))
             , this          , SLOT  (updatePlacementST( BoxSet* )) );
    } else
      throw Error( "BoraEngine not created yet, out of sequence action." );

    return bora;
  }


  void  GraphicBoraEngine::updateSlicingTree ()
  {
    _viewer->clearToolInterrupt();

    BoraEngine* bora = getForFramework( CreateEngine );
    bora->updateSlicingTree();
    _slicingWidget->updateContents();
  }


  void  GraphicBoraEngine::updatePlacementST ( BoxSet* boxSet )
  {
    cdebug_log(536,0) << "GraphicBoraEngine::updatePlacementST(BoxSet*)" << endl;
    _viewer->clearToolInterrupt();

    BoraEngine* bora = getForFramework( CreateEngine );
    
    if (bora) bora->updatePlacement( boxSet );
  }


  void  GraphicBoraEngine::updatePlacementST ( size_t index )
  {
    cdebug_log(536,0) << "GraphicBoraEngine::updatePlacementST(int)" << endl;
    _viewer->clearToolInterrupt();

    BoraEngine* bora = getForFramework( CreateEngine );
    if (bora){
      BoxSet* bs = _slicingWidget->getDataWidget()->getBoxSet( index ); 
      bora->updatePlacement( bs );
    }
  }


  void GraphicBoraEngine::setToleranceRatio ()
  {
    _viewer->clearToolInterrupt();
    
    BoraEngine* bora = getForFramework( CreateEngine );
    if (bora) {
      double ratioh = QInputDialog::getDouble( _slicingWidget
                                             , "Set height tolerance ratio"
                                             , "Write the desired height tolerance ratio:"
                                             , DbU::getPhysical(bora->getToleranceRatioH(),DbU::Micro)
                                             , 0, 10000, 2 ); 
      double ratiow = QInputDialog::getDouble( _slicingWidget
                                             , "Set width tolerance ratio"
                                             , "Write the desired width tolerance ratio:"
                                             , DbU::getPhysical(bora->getToleranceRatioW(),DbU::Micro)
                                             , 0, 10000, 2 ); 
      bora->setToleranceRatio( DbU::fromPhysical(ratioh, DbU::Micro) 
                             , DbU::fromPhysical(ratiow, DbU::Micro) );
      _slicingWidget->updateContents();
    }
  }


  void GraphicBoraEngine::setToleranceBand ()
  {
    _viewer->clearToolInterrupt();

    BoraEngine* bora = getForFramework( CreateEngine );
    if (bora) {
      double bandh = QInputDialog::getDouble( _slicingWidget
                                             , "Set height tolerance band"
                                             , "Write the desired height tolerance band:"
                                             , DbU::getPhysical(bora->getToleranceBandH(),DbU::Micro)
                                             , 0, 10000, 2 );
      double bandw = QInputDialog::getDouble( _slicingWidget
                                            , "Set width tolerance band"
                                            , "Write the desired width tolerance band:"
                                            , DbU::getPhysical(bora->getToleranceBandW(),DbU::Micro)
                                            , 0, 10000, 2 );
      bora->setToleranceBand( DbU::fromPhysical(bandh, DbU::Micro)
                            , DbU::fromPhysical(bandw, DbU::Micro) );
      _slicingWidget->updateContents();
    }
  }


  void GraphicBoraEngine::showPareto ()
  {
    cdebug_log(536,0) << "GraphicBoraEngine::showPareto(): "  << _viewer << endl;
    if (_viewer) _viewer->clearToolInterrupt();

    BoraEngine* bora = getForFramework( CreateEngine );
    if (bora) _slicingWidget->show();
    
  }


  void  GraphicBoraEngine::addToMenu ( CellViewer* viewer )
  {
    assert( _viewer == NULL );

    _viewer = viewer;
    _viewer->addMenu ( "analog" , "Analog" , CellViewer::TopMenu );
    if (_slicingWidget) _slicingWidget->setViewer( _viewer ); // ??

    if (_viewer->hasMenuAction("analog.STree.create")) {
      cerr << Warning( "GraphicBoraEngine::addToMenu() - Bora tool already hooked in." ) << endl;
      return;
    }

    _viewer->addMenu( "analog.STree", "Slicing Tree", NoFlags );

    _viewer->addToMenu( "analog.STree.update"
                      , "Update possible dimensions"
                      , "Update the possible dimensions at each node of the SlicingTree."
                      , std::bind( &GraphicBoraEngine::updateSlicingTree, this )
                      );
    _viewer->addToMenu( "analog.STree.setTR"
                      , "Set global tolerance ratio"
                      , "Set the tolerance of the global ratio."
                      , std::bind( &GraphicBoraEngine::setToleranceRatio, this )
                      );
    _viewer->addToMenu( "analog.STree.setBR"
                      , "Set band tolerance"
                      , "Set the tolerance of the bands inside of the slicing tree."
                      , std::bind( &GraphicBoraEngine::setToleranceBand, this )
                      );
    _viewer->addToMenu( "analog.STree.show"
                      , "Slicing tree curve"
                      , "Show the complete slicing tree curve."
                      , std::bind( &GraphicBoraEngine::showPareto, this )
                      );
  }


  const Name& GraphicBoraEngine::getName () const
  { return BoraEngine::staticGetName(); }


  Cell* GraphicBoraEngine::getCell ()
  {
    if (_viewer == NULL) {
      throw Error( "<b>Bora:</b> GraphicBoraEngine not bound to any Viewer." );
      return NULL;
    }
    if (_viewer->getCell() == NULL) {
      throw Error( "<b>Bora:</b> No Cell is loaded into the Viewer." );
      return NULL;
    }

    return _viewer->getCell();
  }


  GraphicBoraEngine* GraphicBoraEngine::grab ()
  {
    cdebug_log(536,0) << "GraphicBoraEngine::grab()" << endl;
    if (not _references) {
      _singleton = new GraphicBoraEngine();
    }
    _references++;

    return _singleton;
  }


  size_t  GraphicBoraEngine::release ()
  {
    --_references;
    if (not _references) {
      delete _singleton;
      _singleton = NULL;
    }
    return _references;
  }


  GraphicBoraEngine::GraphicBoraEngine ()
    : GraphicTool()
    , _viewer       (NULL)
    , _slicingWidget(NULL)
  { }


  GraphicBoraEngine::~GraphicBoraEngine ()
  {
    _slicingWidget->deleteLater();
  }


} // End of Bora namespace.
