
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
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./GraphicKatabaticEngine.cpp"             |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <QAction>
#include  <QMenu>
#include  <QMenuBar>
#include  <hurricane/Warning.h>
#include  <hurricane/Error.h>
#include  <hurricane/Breakpoint.h>
#include  <hurricane/DebugSession.h>
#include  <hurricane/Go.h>
#include  <hurricane/Net.h>
#include  <hurricane/Cell.h>
#include  <hurricane/viewer/Graphics.h>
#include  <hurricane/viewer/CellWidget.h>
#include  <hurricane/viewer/CellViewer.h>
#include  <crlcore/AllianceFramework.h>
#include  <katabatic/GCell.h>
#include  <katabatic/GraphicKatabaticEngine.h>


namespace Katabatic {


  using namespace std;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Breakpoint;
  using Hurricane::DebugSession;
  using Hurricane::Net;
  using Hurricane::Graphics;
  using Hurricane::ColorScale;
  using CRL::AllianceFramework;


  size_t                  GraphicKatabaticEngine::_references = 0;
  GraphicKatabaticEngine* GraphicKatabaticEngine::_singleton  = NULL;


  void  GraphicKatabaticEngine::initKatabaticAc ( CellWidget* widget )
  {
  //cerr << "GraphicKatabaticEngine::initKatabaticGo()" << endl;
  }


  void  GraphicKatabaticEngine::drawKatabaticAc ( CellWidget*           widget
                                                , const Go*             go
                                                , const BasicLayer*     basicLayer
                                                , const Box&            box
                                                , const Transformation& transformation
                                                )
  { }


  void  GraphicKatabaticEngine::initKatabaticGCell ( CellWidget* widget )
  {
    widget->getDrawingPlanes().setPen ( Qt::NoPen );
  }


  void  GraphicKatabaticEngine::drawKatabaticGCell ( CellWidget*           widget
                                                   , const Go*             go
                                                   , const BasicLayer*     basicLayer
                                                   , const Box&            box
                                                   , const Transformation& transformation
                                                   )
  {
    const GCell* gcell = static_cast<const GCell*>(go);

    QPainter& painter = widget->getPainter();
    painter.setBrush 
      ( Graphics::getColorScale(ColorScale::Fire).getBrush((size_t)(gcell->getMaxHVDensity()*255.0)
                                                          ,widget->getDarkening()) );
    painter.drawRect ( widget->dbuToDisplayRect(gcell->getBoundingBox()) );
  }


  KatabaticEngine* GraphicKatabaticEngine::create ( const RoutingGauge* rg, Cell* cell )
  {
    KatabaticEngine* ktbt = KatabaticEngine::get ( cell );
    if ( !ktbt )
      ktbt = KatabaticEngine::create ( rg, cell );
    else
      cerr << Warning("%s already has a Katabatic engine.",getString(cell).c_str()) << endl;

    return ktbt;
  }


  void  GraphicKatabaticEngine::run ()
  {
    static vector<Net*> routingNets;

    emit cellPreModificated ();

    AllianceFramework* af = AllianceFramework::get ();
    KatabaticEngine* ktbt = create ( af->getRoutingGauge(), _viewer->getCell() );

  //DebugSession::addToTrace ( _viewer->getCell(), "sel" );

    ktbt->loadGlobalRouting ( LoadGrByNet, routingNets );
    emit cellPostModificated ();

  //Breakpoint::stop ( 0, "Point d'arret:<br>&nbsp;&nbsp;<b>LayerAssingByTrunk()</b><br>"
  //                      "Assignment des layers, methode globale." );
    emit cellPreModificated ();
    ktbt->layerAssign ( LayerAssignByTrunk );
    emit cellPostModificated ();

  //throw ( "Rend-toi t'est cerné" );
  }


  void  GraphicKatabaticEngine::closeRoute ()
  {
    emit cellPreModificated ();
    KatabaticEngine* ktbt = KatabaticEngine::get ( _viewer->getCell() );

    ktbt->destroy ();
    emit cellPostModificated ();
  }


  void  GraphicKatabaticEngine::addToMenu ( CellViewer* viewer )
  {
    assert ( _viewer == NULL );

    _viewer = viewer;

    QMenu* prMenu = _viewer->findChild<QMenu*>("viewer.menuBar.placeAndRoute");
    if ( !prMenu ) {
      QMenuBar* menuBar = _viewer->findChild<QMenuBar*>("viewer.menuBar");
      if ( !menuBar ) {
        cerr << Warning("GraphicKatabaticEngine::addToMenu() - No MenuBar in parent widget.") << endl;
        return;
      }
      prMenu = menuBar->addMenu ( tr("P&&R") );
      prMenu->setObjectName ( "viewer.menuBar.placeAndRoute" );
    }

    QAction* fRouteAction = _viewer->findChild<QAction*>("viewer.menuBar.placeAndRoute.finalRoute");
    if ( fRouteAction )
      cerr << Warning("GraphicKatabaticEngine::addToMenu() - Katabatic final router already hooked in.") << endl;
    else {
      fRouteAction = new QAction  ( tr("Katabatic - &Final Route"), _viewer );
      fRouteAction->setObjectName ( "viewer.menuBar.placeAndRoute.finalRoute" );
      fRouteAction->setStatusTip  ( tr("Run the <b>Katabatic</b> final router") );
      fRouteAction->setVisible    ( true );
      prMenu->addAction ( fRouteAction );

      QAction* fCloseRouteAction = new QAction ( tr("Close Routing"), _viewer );
      fCloseRouteAction->setObjectName ( "viewer.menuBar.placeAndRoute.closeRoute" );
      fCloseRouteAction->setStatusTip  ( tr("Closing Routing") );
      fCloseRouteAction->setVisible    ( true );
      prMenu->addAction ( fCloseRouteAction );

      connect ( fCloseRouteAction, SIGNAL(triggered()), this, SLOT(closeRoute()) );
      connect ( fRouteAction     , SIGNAL(triggered()), this, SLOT(run()) );
    }

    connect ( this, SIGNAL(cellPreModificated ()), _viewer->getCellWidget(), SLOT(cellPreModificate ()) );
    connect ( this, SIGNAL(cellPostModificated()), _viewer->getCellWidget(), SLOT(cellPostModificate()) );
  }


  const Name& GraphicKatabaticEngine::getName () const
  {
    return KatabaticEngine::staticGetName ();
  }


  GraphicKatabaticEngine* GraphicKatabaticEngine::grab ()
  {
    if ( !_references ) {
      _singleton = new GraphicKatabaticEngine ();
    }
    _references++;

    return _singleton;
  }


  size_t  GraphicKatabaticEngine::release ()
  {
    _references--;
    if ( !_references ) {
      delete _singleton;
      _singleton = NULL;
    }
    return _references;
  }


  GraphicKatabaticEngine::GraphicKatabaticEngine ()
    : GraphicTool()
    , _viewer(NULL)
  {
    addDrawGo ( "Katabatic::Ac"   , initKatabaticAc   , drawKatabaticAc    );
    addDrawGo ( "Katabatic::GCell", initKatabaticGCell, drawKatabaticGCell );
  }


  GraphicKatabaticEngine::~GraphicKatabaticEngine ()
  { }


}  // End of Katabatic namespace.
