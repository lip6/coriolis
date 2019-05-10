// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./GraphicKiteEngine.cpp"                  |
// +-----------------------------------------------------------------+


#include <boost/bind.hpp>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <hurricane/Warning.h>
#include <hurricane/Error.h>
#include <hurricane/Breakpoint.h>
#include <hurricane/DebugSession.h>
#include <hurricane/Go.h>
#include <hurricane/Net.h>
#include <hurricane/Cell.h>
#include <hurricane/viewer/Graphics.h>
#include <hurricane/viewer/CellWidget.h>
#include <hurricane/viewer/CellViewer.h>
#include <hurricane/viewer/ControllerWidget.h>
#include <hurricane/viewer/ExceptionWidget.h>
#include <crlcore/Utilities.h>
#include <crlcore/AllianceFramework.h>
#include <katabatic/GCell.h>
#include <katabatic/GCellGrid.h>
#include <knik/Edge.h>
#include <knik/Vertex.h>
#include <knik/KnikEngine.h>
#include <knik/GraphicKnikEngine.h>
#include <kite/GraphicKiteEngine.h>
//#include <kite/ConfigurationWidget.h>


namespace Kite {

  using namespace std;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Exception;
  using Hurricane::Breakpoint;
  using Hurricane::DebugSession;
  using Hurricane::Point;
  using Hurricane::Entity;
  using Hurricane::Net;
  using Hurricane::Graphics;
  using Hurricane::ColorScale;
  using Hurricane::ControllerWidget;
  using Hurricane::ExceptionWidget;
  using CRL::Catalog;
  using CRL::AllianceFramework;
  using Knik::KnikEngine;
  using Knik::GraphicKnikEngine;


  size_t             GraphicKiteEngine::_references = 0;
  GraphicKiteEngine* GraphicKiteEngine::_singleton  = NULL;


  void  GraphicKiteEngine::initKatabaticAc ( CellWidget* widget )
  { }


  void  GraphicKiteEngine::drawKatabaticAc ( CellWidget*           widget
                                           , const Go*             go
                                           , const BasicLayer*     basicLayer
                                           , const Box&            box
                                           , const Transformation& transformation
                                           )
  { }


  void  GraphicKiteEngine::initKatabaticGCell ( CellWidget* widget )
  {
    widget->getDrawingPlanes().setPen( Qt::NoPen );

    KiteEngine* kite = KiteEngine::get( widget->getCell() );
    if ( kite ) {
      kite->getGCellGrid()->setDensityMode( Katabatic::GCellGrid::MaxDensity );
    }
  }


  void  GraphicKiteEngine::drawKatabaticGCell ( CellWidget*           widget
                                              , const Go*             go
                                              , const BasicLayer*     basicLayer
                                              , const Box&            box
                                              , const Transformation& transformation
                                              )
  {
    const Katabatic::GCell* gcell = static_cast<const Katabatic::GCell*>(go);

    QPainter& painter = widget->getPainter();
    size_t    density = (size_t)( gcell->getDensity() * 255.0 );
    if (density > 255) density = 255;

    painter.setBrush
      ( Graphics::getColorScale(ColorScale::Fire).getBrush(density,widget->getDarkening()) );
    painter.drawRect
      ( widget->dbuToScreenRect(gcell->getBoundingBox().inflate(0
                                                               ,0
                                                               ,gcell->getTopRightShrink()
                                                               ,gcell->getTopRightShrink())) );
  }


  KiteEngine* GraphicKiteEngine::createEngine ()
  {
    Cell* cell = getCell ();

    KiteEngine* kite = KiteEngine::get( cell );
    if (not kite) {
      kite = KiteEngine::create( cell );
      kite->setPostEventCb( boost::bind(&GraphicKiteEngine::postEvent,this) );
      kite->setViewer( _viewer );
      if (cmess1.enabled()) kite->printConfiguration();
    } else
      cerr << Warning( "%s already has a Kite engine.", getString(cell).c_str() ) << endl;

    return kite;
  }


  KiteEngine* GraphicKiteEngine::getForFramework ( unsigned int flags )
  {
  // Currently, only one framework is avalaible: Alliance.

    KiteEngine* kite = KiteEngine::get( getCell() );
    if (kite) return kite;

    if (flags & CreateEngine) {
      kite = createEngine();
      if (not kite) 
        throw Error( "Failed to create Kite engine on %s.", getString(getCell()).c_str() );
    } else {
      throw Error( "KiteEngine not created yet, run the global router first." );
    }

    return kite;
  }


  void  GraphicKiteEngine::_wipeoutRouting ()
  {
    if (getCell()) {
      KiteEngine::wipeoutRouting( getCell() );
      _viewer->getCellWidget()->refresh();
    }
  }


  void  GraphicKiteEngine::_loadGlobalSolution ()
  {
    KiteEngine* kite = getForFramework( CreateEngine );
    kite->runGlobalRouter( KtLoadGlobalRouting );
  }


  void  GraphicKiteEngine::_saveGlobalSolution ()
  {
    KiteEngine* kite = getForFramework( NoFlags );
    if (kite) kite->saveGlobalSolution ();
  }


  void  GraphicKiteEngine::_globalRoute ()
  {
    KiteEngine* kite = getForFramework( CreateEngine );
    kite->runGlobalRouter( KtBuildGlobalRouting );
  }


  void  GraphicKiteEngine::_loadGlobalRouting ()
  {
    KiteEngine* kite = getForFramework( NoFlags );

    _viewer->clearToolInterrupt();
    kite->loadGlobalRouting( Katabatic::EngineLoadGrByNet );
  }


  void  GraphicKiteEngine::_balanceGlobalDensity ()
  {
    KiteEngine* kite = getForFramework( NoFlags );
    kite->balanceGlobalDensity ();
    kite->layerAssign ( Katabatic::EngineNoNetLayerAssign );
  }


  void  GraphicKiteEngine::_runNegociatePreRouted ()
  {
    KiteEngine* kite = getForFramework( CreateEngine );
    kite->runNegociate( KtPreRoutedStage );
  }


  void  GraphicKiteEngine::_runNegociate ()
  {
    KiteEngine* kite = getForFramework( NoFlags );
    kite->runNegociate();
  }


  void  GraphicKiteEngine::_finalize ()
  {
    KiteEngine* kite = getForFramework( NoFlags );
    if (kite) {
      kite->finalizeLayout();
      kite->destroy();
    }
  }

  
  void  GraphicKiteEngine::_dumpMeasures ()
  {
    KiteEngine* kite = getForFramework( NoFlags );
    if (kite) kite->dumpMeasures();
  }


  void  GraphicKiteEngine::_save ()
  {
    Cell*              cell = getCell();
    AllianceFramework* af   = AllianceFramework::get();

    string name = getString(cell->getName()) + "_kite";
    cell->setName( name );
    af->saveCell( cell, Catalog::State::Physical );
  }


  void  GraphicKiteEngine::_detailRoute ()
  {
    _loadGlobalRouting   ();
    _balanceGlobalDensity();
    _runNegociate        ();
  }


  void  GraphicKiteEngine::_route ()
  {
    _runNegociatePreRouted();
    _globalRoute          ();
    _detailRoute          ();
    _finalize             ();
  }


  void  GraphicKiteEngine::postEvent ()
  {
    static unsigned int count = 0;

    if (not (count++ % 500)) {
      QApplication::processEvents();

      if (_viewer->isToolInterrupted()) {
        KiteEngine* kite = KiteEngine::get( getCell() );
        if (kite) kite->setInterrupt( true );
        _viewer->clearToolInterrupt();
      }
    }
  }


  void  GraphicKiteEngine::addToMenu ( CellViewer* viewer )
  {
    assert( _viewer == NULL );

    _viewer = viewer;

    if (_viewer->hasMenuAction("misc.obsolete.placeAndRoute.route")) {
      cerr << Warning( "GraphicKiteEngine::addToMenu() - Kite detailed router already hooked in." ) << endl;
      return;
    }

    if (not _viewer->hasMenuAction("misc.obsolete.placeAndRoute"))
      _viewer->addMenu( "misc.obsolete.placeAndRoute", "P&&R" );

    if (not _viewer->hasMenuAction("misc.obsolete.placeAndRoute.stepByStep"))
      _viewer->addMenu( "misc.obsolete.placeAndRoute.stepByStep", "&Step by step" );

    _viewer->addToMenu( "misc.obsolete.placeAndRoute.route"
                      , "&Route"
                      , "Complete routing of the design (global, detailed and finalize) [<b>Kite</b>]"
                      , std::bind(&GraphicKiteEngine::_route,this)
                      , QIcon()
                      , "misc.obsolete.placeAndRoute.stepByStep"
                      );

    _viewer->addToMenu( "misc.obsolete.placeAndRoute.stepByStep.========" );
    _viewer->addToMenu( "misc.obsolete.placeAndRoute.stepByStep.wipeoutRouting"
                      , "Erase Previous Routing"
                      , "Erase any previously routed wires"
                      , std::bind(&GraphicKiteEngine::_wipeoutRouting,this)
                      );
    _viewer->addToMenu( "misc.obsolete.placeAndRoute.stepByStep.detailedPreRoute"
                      , "Detailed Pre-Route"
                      , "Run the detailed router on global pre-routed nets [<b>Kite</b>]"
                      , std::bind(&GraphicKiteEngine::_runNegociatePreRouted,this)
                      );
    _viewer->addToMenu( "misc.obsolete.placeAndRoute.stepByStep.globalRoute"
                      , "&Global Route"
                      , "Run the global router [<b>nik</b>]"
                      , std::bind(&GraphicKiteEngine::_globalRoute,this)
                      );
    _viewer->addToMenu( "misc.obsolete.placeAndRoute.stepByStep.loadGlobalRouting"
                      , "&Load Global Routing"
                      , "Load a solution for the global routing (.kgr) [<b>Knik</b>]"
                      , std::bind(&GraphicKiteEngine::_loadGlobalSolution,this)
                      );
    _viewer->addToMenu( "misc.obsolete.placeAndRoute.stepByStep.saveGlobalRouting"
                      , "&Save Global Routing"
                      , "Save a global router solution (.kgr) [<b>Knik</b>]"
                      , std::bind(&GraphicKiteEngine::_saveGlobalSolution,this)
                      );
    _viewer->addToMenu( "misc.obsolete.placeAndRoute.stepByStep.detailedRoute"
                      , "&Detailed Route"
                      , "Run the detailed router [<b>Kite</b>]"
                      , std::bind(&GraphicKiteEngine::_detailRoute,this)
                      );
    _viewer->addToMenu( "misc.obsolete.placeAndRoute.stepByStep.finalize"
                      , "&Finalize Routing"
                      , "Cleanup all routing related data structures [<b>Kite</b>]"
                      , std::bind(&GraphicKiteEngine::_finalize,this)
                      );
    _viewer->addToMenu( "misc.obsolete.placeAndRoute.stepByStep.dumpMeasures"
                      , "Dump &Measures"
                      , "Dumping Measurements on the disk [<b>Kite</b>]"
                      , std::bind(&GraphicKiteEngine::_dumpMeasures,this)
                      );
    // _viewer->addToMenu( "misc.obsolete.placeAndRoute.stepByStep.save"
    //                   , "&Save Design"
    //                   , "Save routed design (temporary hack)"
    //                   , std::bind(&GraphicKiteEngine::_save,this)
    //                   );
  }


  const Name& GraphicKiteEngine::getName () const
  { return KiteEngine::staticGetName(); }


  Cell* GraphicKiteEngine::getCell ()
  {
    if (_viewer == NULL) {
      throw Error( "<b>Kite:</b> GraphicKiteEngine not bound to any Viewer." );
      return NULL;
    }

    if (_viewer->getCell() == NULL) {
      throw Error( "<b>Kite:</b> No Cell is loaded into the Viewer." );
      return NULL;
    }

    return _viewer->getCell();
  }


  GraphicKiteEngine* GraphicKiteEngine::grab ()
  {
    if (not _references) {
      _singleton = new GraphicKiteEngine ();
    }
    _references++;

    return _singleton;
  }


  size_t  GraphicKiteEngine::release ()
  {
    --_references;
    if (not _references) {
      delete _singleton;
      _singleton = NULL;
    }
    return _references;
  }


  GraphicKiteEngine::GraphicKiteEngine ()
    : GraphicTool()
    , _viewer    (NULL)
  {
    addDrawGo( "Knik::Edge"      , GraphicKnikEngine::initKnikEdges , GraphicKnikEngine::drawKnikEdges  );
    addDrawGo( "Knik::Vertex"    , GraphicKnikEngine::initKnikVertex, GraphicKnikEngine::drawKnikVertex );
    addDrawGo( "Katabatic::Ac"   , initKatabaticAc   , drawKatabaticAc    );
    addDrawGo( "Katabatic::GCell", initKatabaticGCell, drawKatabaticGCell );
  }


  GraphicKiteEngine::~GraphicKiteEngine ()
  { }


}  // Kite namespace.
