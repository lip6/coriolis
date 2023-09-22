// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2014-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |   E t e s i a n  -  A n a l y t i c   P l a c e r               |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./GraphicEtesianEngine.cpp"               |
// +-----------------------------------------------------------------+


#include <functional>
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
#include <hurricane/UpdateSession.h>
#include <hurricane/viewer/Graphics.h>
#include <hurricane/viewer/CellWidget.h>
#include <hurricane/viewer/CellViewer.h>
#include <hurricane/viewer/ControllerWidget.h>
#include <hurricane/viewer/ExceptionWidget.h>
#include <crlcore/Utilities.h>
#include <crlcore/AllianceFramework.h>
#include <etesian/GraphicEtesianEngine.h>


namespace Etesian {

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
  using CRL::Catalog;
  using CRL::AllianceFramework;


  size_t                GraphicEtesianEngine::_references = 0;
  GraphicEtesianEngine* GraphicEtesianEngine::_singleton  = NULL;


  EtesianEngine* GraphicEtesianEngine::createEngine ()
  {
    Cell* cell = getCell ();

    EtesianEngine* etesian = EtesianEngine::get( cell );
    if (not etesian) {
      etesian = EtesianEngine::create( cell );
      etesian->setViewer( _viewer );
    } else
      cerr << Warning( "%s already has a Etesian engine.", getString(cell).c_str() ) << endl;

    return etesian;
  }


  EtesianEngine* GraphicEtesianEngine::getForFramework ( unsigned int flags )
  {
  // Currently, only one framework is avalaible: Alliance.

    EtesianEngine* etesian = EtesianEngine::get( getCell() );
    if (etesian) return etesian;

    if (flags & CreateEngine) {
      etesian = createEngine();
      if (not etesian) 
        throw Error( "Failed to create Etesian engine on %s.", getString(getCell()).c_str() );
    } else {
      throw Error( "EtesianEngine not created yet, out of sequence action." );
    }

    return etesian;
  }


  void  GraphicEtesianEngine::_place ()
  {
    _viewer->clearToolInterrupt();

    EtesianEngine* etesian = getForFramework( CreateEngine );
    etesian->resetPlacement();
    etesian->place();
    etesian->toHurricane();
  }


  void  GraphicEtesianEngine::postEvent ()
  {
    static unsigned int count = 0;

    if (not (count++ % 500)) {
      QApplication::processEvents();

      // if (_viewer->isToolInterrupted()) {
      //   EtesianEngine* etesian = EtesianEngine::get( getCell() );
      //   if (etesian) etesian->setInterrupt( true );
      //   _viewer->clearToolInterrupt();
      // }
    }
  }


  void  GraphicEtesianEngine::addToMenu ( CellViewer* viewer )
  {
    assert( _viewer == NULL );

    _viewer = viewer;

    if (_viewer->hasMenuAction("placeAndRoute.etesianPlaceBlock")) {
      cerr << Warning( "GraphicEtesianEngine::addToMenu() - Etesian placer already hooked in." ) << endl;
      return;
    }

    _viewer->addToMenu( "placeAndRoute.etesianPlaceBlock"
                      , "Plac&e Block"
                      , "Place a block [<b>Etesian</b>]"
                      , std::bind(&GraphicEtesianEngine::_place,this)
                      , QIcon()
                      , "placeAndRoute.clockTree"
                      );
  }


  const Name& GraphicEtesianEngine::getName () const
  { return EtesianEngine::staticGetName(); }


  Cell* GraphicEtesianEngine::getCell ()
  {
    if (_viewer == NULL) {
      throw Error( "<b>Etesian:</b> GraphicEtesianEngine not bound to any Viewer." );
      return NULL;
    }

    if (_viewer->getCell() == NULL) {
      throw Error( "<b>Etesian:</b> No Cell is loaded into the Viewer." );
      return NULL;
    }

    return _viewer->getCell();
  }


  GraphicEtesianEngine* GraphicEtesianEngine::grab ()
  {
    if (not _references) {
      _singleton = new GraphicEtesianEngine ();
    }
    _references++;

    return _singleton;
  }


  size_t  GraphicEtesianEngine::release ()
  {
    --_references;
    if (not _references) {
      delete _singleton;
      _singleton = NULL;
    }
    return _references;
  }


  GraphicEtesianEngine::GraphicEtesianEngine ()
    : GraphicTool()
    , _viewer    (NULL)
  {
    // addDrawGo( "Knik::Edge"      , GraphicKnikEngine::initKnikEdges , GraphicKnikEngine::drawKnikEdges  );
    // addDrawGo( "Knik::Vertex"    , GraphicKnikEngine::initKnikVertex, GraphicKnikEngine::drawKnikVertex );
    // addDrawGo( "Katabatic::Ac"   , initKatabaticAc   , drawKatabaticAc    );
    // addDrawGo( "Katabatic::GCell", initKatabaticGCell, drawKatabaticGCell );
  }


  GraphicEtesianEngine::~GraphicEtesianEngine ()
  { }


}  // Etesian namespace.
