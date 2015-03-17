// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2014-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |   S m u r f  -  A  D e m o   T o o l E n g i n e                |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./GraphicSmurfEngine.cpp"                 |
// +-----------------------------------------------------------------+


#include <functional>
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
#include <hurricane/UpdateSession.h>
#include <hurricane/viewer/Graphics.h>
#include <hurricane/viewer/CellWidget.h>
#include <hurricane/viewer/CellViewer.h>
#include <hurricane/viewer/ControllerWidget.h>
#include <crlcore/Utilities.h>
#include <crlcore/AllianceFramework.h>
#include <smurf/GraphicSmurfEngine.h>


namespace Smurf {

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
  using CRL::Catalog;
  using CRL::AllianceFramework;


  size_t              GraphicSmurfEngine::_references = 0;
  GraphicSmurfEngine* GraphicSmurfEngine::_singleton  = NULL;


  SmurfEngine* GraphicSmurfEngine::createEngine ()
  {
    Cell* cell = getCell ();

    SmurfEngine* smurf = SmurfEngine::get( cell );
    if (not smurf) {
      smurf = SmurfEngine::create( cell );
    } else
      cerr << Warning( "%s already has a Smurf engine.", getString(cell).c_str() ) << endl;

    return smurf;
  }


  SmurfEngine* GraphicSmurfEngine::getForFramework ()
  {
  // Currently, only one framework is avalaible: Alliance.

    SmurfEngine* smurf = SmurfEngine::get( getCell() );
    if (smurf) return smurf;

    smurf = createEngine();
    
    if (not smurf) 
      throw Error( "Failed to create Smurf engine on %s.", getString(getCell()).c_str() );

    return smurf;
  }


  void  GraphicSmurfEngine::place ()
  {
    SmurfEngine* smurf = SmurfEngine::get( getCell() );
    if (not smurf) {
      smurf = createEngine();
    } 
  //if (cmess1.enabled()) smurf->printConfiguration();

    emit cellPreModificated();
    _viewer->clearToolInterrupt();
    smurf->place();
    emit cellPostModificated();
  }


  void  GraphicSmurfEngine::postEvent ()
  {
    static unsigned int count = 0;

    if (not (count++ % 500)) {
      QApplication::processEvents();

      // if (_viewer->isToolInterrupted()) {
      //   SmurfEngine* smurf = SmurfEngine::get( getCell() );
      //   if (smurf) smurf->setInterrupt( true );
      //   _viewer->clearToolInterrupt();
      // }
    }
  }


  void  GraphicSmurfEngine::addToMenu ( CellViewer* viewer )
  {
    assert( _viewer == NULL );

    _viewer = viewer;

    QMenu* prMenu   = _viewer->findChild<QMenu*>("viewer.menuBar.placeAndRoute");
    QMenu* stepMenu = _viewer->findChild<QMenu*>("viewer.menuBar.placeAndRoute.stepByStep");
    if (prMenu == NULL) {
      QMenuBar* menuBar = _viewer->findChild<QMenuBar*>("viewer.menuBar");
      if (menuBar == NULL) {
        cerr << Warning( "GraphicSmurfEngine::addToMenu() - No MenuBar in parent widget." ) << endl;
        return;
      }
      prMenu = menuBar->addMenu( tr("P&&R") );
      prMenu->setObjectName( "viewer.menuBar.placeAndRoute" );

      stepMenu = prMenu->addMenu( tr("&Step by Step") );
      stepMenu->setObjectName( "viewer.menuBar.placeAndRoute.stepByStep" );

      prMenu->addSeparator();
    }

    QAction* placeAction = _viewer->findChild<QAction*>("viewer.menuBar.placeAndRoute.place");
    if (placeAction)
      cerr << Warning( "GraphicSmurfEngine::addToMenu() - Smurf detailed router already hooked in." ) << endl;
    else {
      QAction* placeAction = new QAction ( tr("Smurf - &Place"), _viewer );
      placeAction->setObjectName( "viewer.menuBar.placeAndRoute.place" );
      placeAction->setStatusTip ( tr("Place the design") );
      placeAction->setVisible   ( true );
      prMenu->addAction( placeAction );

      connect( placeAction        , SIGNAL(triggered()), this, SLOT(place             ()) );
    }

    connect( this, SIGNAL(cellPreModificated ()), _viewer->getCellWidget(), SLOT(cellPreModificate ()) );
    connect( this, SIGNAL(cellPostModificated()), _viewer->getCellWidget(), SLOT(cellPostModificate()) );
  }


  const Name& GraphicSmurfEngine::getName () const
  { return SmurfEngine::staticGetName(); }


  Cell* GraphicSmurfEngine::getCell ()
  {
    if (_viewer == NULL) {
      throw Error( "<b>Smurf:</b> GraphicSmurfEngine not bound to any Viewer." );
      return NULL;
    }

    if (_viewer->getCell() == NULL) {
      throw Error( "<b>Smurf:</b> No Cell is loaded into the Viewer." );
      return NULL;
    }

    return _viewer->getCell();
  }


  GraphicSmurfEngine* GraphicSmurfEngine::grab ()
  {
    if (not _references) {
      _singleton = new GraphicSmurfEngine ();
    }
    _references++;

    return _singleton;
  }


  size_t  GraphicSmurfEngine::release ()
  {
    --_references;
    if (not _references) {
      delete _singleton;
      _singleton = NULL;
    }
    return _references;
  }


  GraphicSmurfEngine::GraphicSmurfEngine ()
    : GraphicTool()
    , _viewer    (NULL)
  {
    // addDrawGo( "Knik::Edge"      , GraphicKnikEngine::initKnikEdges , GraphicKnikEngine::drawKnikEdges  );
    // addDrawGo( "Knik::Vertex"    , GraphicKnikEngine::initKnikVertex, GraphicKnikEngine::drawKnikVertex );
    // addDrawGo( "Katabatic::Ac"   , initKatabaticAc   , drawKatabaticAc    );
    // addDrawGo( "Katabatic::GCell", initKatabaticGCell, drawKatabaticGCell );
  }


  GraphicSmurfEngine::~GraphicSmurfEngine ()
  { }


}  // Smurf namespace.
