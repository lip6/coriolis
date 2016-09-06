// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./GraphicKatanaEngine.cpp"                |
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
#include <anabatic/GCell.h>
#include <katana/GraphicKatanaEngine.h>


namespace Katana {

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


  size_t               GraphicKatanaEngine::_references = 0;
  GraphicKatanaEngine* GraphicKatanaEngine::_singleton  = NULL;


  KatanaEngine* GraphicKatanaEngine::createEngine ()
  {
    Cell* cell = getCell ();

    KatanaEngine* katana = KatanaEngine::get( cell );
    if (not katana) {
      katana = KatanaEngine::create( cell );
      katana->setPostEventCb( boost::bind(&GraphicKatanaEngine::postEvent,this) );
      katana->setViewer( _viewer );
      katana->digitalInit();
      if (cmess1.enabled()) katana->printConfiguration();
    } else
      cerr << Warning( "%s already has a Katana engine.", getString(cell).c_str() ) << endl;

    return katana;
  }


  KatanaEngine* GraphicKatanaEngine::getForFramework ( unsigned int flags )
  {
  // Currently, only one framework is avalaible: Alliance.

    KatanaEngine* katana = KatanaEngine::get( getCell() );
    if (katana) return katana;

    if (flags & CreateEngine) {
      katana = createEngine();
      if (not katana) 
        throw Error( "Failed to create Katana engine on %s.", getString(getCell()).c_str() );
    } else {
      throw Error( "KatanaEngine not created yet, run the global router first." );
    }

    return katana;
  }


  void  GraphicKatanaEngine::_globalRoute ()
  {
    KatanaEngine* katana = getForFramework( CreateEngine );
    katana->runGlobalRouter();
  }


  void  GraphicKatanaEngine::_loadGlobalRouting ()
  {
    KatanaEngine* anabatic = getForFramework( NoFlags );

    _viewer->clearToolInterrupt();
    anabatic->loadGlobalRouting( Anabatic::EngineLoadGrByNet );
  }


  void  GraphicKatanaEngine::_balanceGlobalDensity ()
  {
    KatanaEngine* katana = getForFramework( NoFlags );
  //katana->balanceGlobalDensity();
    katana->layerAssign( Anabatic::EngineNoNetLayerAssign );
  }


  void  GraphicKatanaEngine::_runNegociatePreRouted ()
  {
    KatanaEngine* katana = getForFramework( CreateEngine );
    katana->runNegociate( Flags::PreRoutedStage );
  }


  void  GraphicKatanaEngine::_runNegociate ()
  {
    KatanaEngine* katana = getForFramework( NoFlags );
    katana->runNegociate();
  }


  void  GraphicKatanaEngine::_finalize ()
  {
    KatanaEngine* katana = getForFramework( NoFlags );
    if (katana) {
      katana->finalizeLayout();
      katana->destroy();
    }
  }

  
  void  GraphicKatanaEngine::_dumpMeasures ()
  {
    KatanaEngine* katana = getForFramework( NoFlags );
    if (katana) katana->dumpMeasures();
  }


  void  GraphicKatanaEngine::_save ()
  {
    Cell*              cell = getCell();
    AllianceFramework* af   = AllianceFramework::get();

    string name = getString(cell->getName()) + "_katana";
    cell->setName( name );
    af->saveCell( cell, Catalog::State::Physical );
  }


  void  GraphicKatanaEngine::_detailRoute ()
  {
    _loadGlobalRouting   ();
  //Breakpoint::stop( 0, "Global routing loaded." );
    _balanceGlobalDensity();
  //Breakpoint::stop( 0, "Global density balanced." );
    _runNegociate        ();
  }


  void  GraphicKatanaEngine::_route ()
  {
    _runNegociatePreRouted();
    _globalRoute          ();
    _detailRoute          ();
    _finalize             ();
  }


  void  GraphicKatanaEngine::postEvent ()
  {
    static unsigned int count = 0;

    if (not (count++ % 500)) {
      QApplication::processEvents();

      if (_viewer->isToolInterrupted()) {
        KatanaEngine* katana = KatanaEngine::get( getCell() );
        if (katana) katana->setInterrupt( true );
        _viewer->clearToolInterrupt();
      }
    }
  }


  void  GraphicKatanaEngine::addToMenu ( CellViewer* viewer )
  {
    assert( _viewer == NULL );

    _viewer = viewer;

    if (_viewer->hasMenuAction("placeAndRoute.katana.route")) {
      cerr << Warning( "GraphicKatanaEngine::addToMenu() - Katana detailed router already hooked in." ) << endl;
      return;
    }

    _viewer->addMenu  ( "placeAndRoute.katana"           , "Katana" );
    _viewer->addMenu  ( "placeAndRoute.katana.stepByStep", "&Step by step" );

    _viewer->addToMenu( "placeAndRoute.katana.route"
                      , "Katana - &Route"
                      , "Route the design (global & detailed)"
                      , std::bind(&GraphicKatanaEngine::_route,this)
                      );

    _viewer->addToMenu( "placeAndRoute.katana.stepByStep.========" );
    _viewer->addToMenu( "placeAndRoute.katana.stepByStep.detailedPreRoute"
                      , "Katana - Detailed Pre-Route"
                      , "Run the <b>Katana</b> detailed router on pre-routed nets"
                      , std::bind(&GraphicKatanaEngine::_runNegociatePreRouted,this)
                      );
    _viewer->addToMenu( "placeAndRoute.katana.stepByStep.globalRoute"
                      , "Katana - &Global Route"
                      , "Run the <b>Knik</b> global router"
                      , std::bind(&GraphicKatanaEngine::_globalRoute,this)
                      );
    _viewer->addToMenu( "placeAndRoute.katana.stepByStep.detailedRoute"
                      , "Katana - &Detailed Route"
                      , "Run the <b>Katana</b> detailed router"
                      , std::bind(&GraphicKatanaEngine::_detailRoute,this)
                      );
    _viewer->addToMenu( "placeAndRoute.katana.stepByStep.finalize"
                      , "Katana - &Finalize Routing"
                      , "Closing Routing"
                      , std::bind(&GraphicKatanaEngine::_finalize,this)
                      );
    _viewer->addToMenu( "placeAndRoute.katana.stepByStep.dumpMeasures"
                      , "Katana - Dump &Measures"
                      , "Dumping Measurements on the disk"
                      , std::bind(&GraphicKatanaEngine::_dumpMeasures,this)
                      );
    _viewer->addToMenu( "placeAndRoute.katana.stepByStep.save"
                      , "Katana - &Save Design"
                      , "Save routed design (temporary hack)"
                      , std::bind(&GraphicKatanaEngine::_save,this)
                      );
  }


  const Name& GraphicKatanaEngine::getName () const
  { return KatanaEngine::staticGetName(); }


  Cell* GraphicKatanaEngine::getCell ()
  {
    if (_viewer == NULL) {
      throw Error( "<b>Katana:</b> GraphicKatanaEngine not bound to any Viewer." );
      return NULL;
    }

    if (_viewer->getCell() == NULL) {
      throw Error( "<b>Katana:</b> No Cell is loaded into the Viewer." );
      return NULL;
    }

    return _viewer->getCell();
  }


  GraphicKatanaEngine* GraphicKatanaEngine::grab ()
  {
    if (not _references) {
      _singleton = new GraphicKatanaEngine ();
    }
    _references++;

    return _singleton;
  }


  size_t  GraphicKatanaEngine::release ()
  {
    --_references;
    if (not _references) {
      delete _singleton;
      _singleton = NULL;
    }
    return _references;
  }


  GraphicKatanaEngine::GraphicKatanaEngine ()
    : GraphicTool()
    , _viewer    (NULL)
  { }


  GraphicKatanaEngine::~GraphicKatanaEngine ()
  { }


}  // Katana namespace.
