
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
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./GraphicKiteEngine.cpp"                  |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <functional>
#include  <boost/bind.hpp>

#include  <QAction>
#include  <QMenu>
#include  <QMenuBar>
#include  <QApplication>
#include  <hurricane/Warning.h>
#include  <hurricane/Error.h>
#include  <hurricane/Breakpoint.h>
#include  <hurricane/DebugSession.h>
#include  <hurricane/Go.h>
#include  <hurricane/Net.h>
#include  <hurricane/Cell.h>
#include  <hurricane/UpdateSession.h>
#include  <hurricane/viewer/Graphics.h>
#include  <hurricane/viewer/CellWidget.h>
#include  <hurricane/viewer/CellViewer.h>
#include  <hurricane/viewer/ControllerWidget.h>
#include  <crlcore/Utilities.h>
#include  <crlcore/AllianceFramework.h>
#include  <katabatic/GCell.h>
#include  <knik/KnikEngine.h>
#include  <kite/GraphicKiteEngine.h>
#include  <kite/ConfigurationWidget.h>


namespace Kite {


  using namespace std;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Breakpoint;
  using Hurricane::DebugSession;
  using Hurricane::UpdateSession;
  using Hurricane::Net;
  using Hurricane::Graphics;
  using Hurricane::ColorScale;
  using Hurricane::ControllerWidget;
  using CRL::Catalog;
  using CRL::AllianceFramework;
  using Knik::KnikEngine;


  size_t             GraphicKiteEngine::_references = 0;
  GraphicKiteEngine* GraphicKiteEngine::_singleton  = NULL;


  void  GraphicKiteEngine::initKatabaticAc ( CellWidget* widget )
  {
  //cerr << "GraphicKatabaticEngine::initKatabaticGo()" << endl;
  }


  void  GraphicKiteEngine::drawKatabaticAc ( CellWidget*           widget
                                           , const Go*             go
                                           , const BasicLayer*     basicLayer
                                           , const Box&            box
                                           , const Transformation& transformation
                                           )
  { }


  void  GraphicKiteEngine::initKatabaticGCell ( CellWidget* widget )
  {
    widget->getDrawingPlanes().setPen ( Qt::NoPen );
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
    size_t    density = (size_t)( gcell->getMaxHVDensity() * 255.0 );
    if ( density > 255 ) density = 255;

    painter.setBrush
      ( Graphics::getColorScale(ColorScale::Fire).getBrush(density,widget->getDarkening()) );
    painter.drawRect
      ( widget->dbuToDisplayRect(gcell->getBoundingBox().inflate(0
                                                                ,0
                                                                ,gcell->getTopRightShrink()
                                                                ,gcell->getTopRightShrink())) );
  }


  KiteEngine* GraphicKiteEngine::createEngine ()
  {
    Cell* cell = getCell ();

    KiteEngine* kite = KiteEngine::get ( cell );
    if ( not kite ) {
      kite = KiteEngine::create ( cell );
      kite->setPostEventCb ( boost::bind(&GraphicKiteEngine::postEvent,this) );
    } else
      cerr << Warning("%s already has a Kite engine.",getString(cell).c_str()) << endl;

    return kite;
  }


  KiteEngine* GraphicKiteEngine::getForFramework ()
  {
  // Currently, only one framework is avalaible: Alliance.

    KiteEngine* kite = KiteEngine::get ( getCell() );
    if ( kite ) return kite;

    kite = createEngine ();
    
    if ( not kite ) 
      throw Error("Failed to create Kite engine on %s.",getString(getCell()).c_str());

    return kite;
  }


  void  GraphicKiteEngine::saveGlobalSolution ()
  {
    KiteEngine* kite = KiteEngine::get ( getCell() );
    if ( kite ) kite->saveGlobalSolution ();
  }


  void  GraphicKiteEngine::loadGlobalSolution ()
  {

    KiteEngine* kite = getForFramework ();

    emit cellPreModificated ();
    kite->runGlobalRouter ( LoadGlobalSolution );
    emit cellPostModificated ();
  }


  void  GraphicKiteEngine::runGlobal ()
  {

    KiteEngine* kite = getForFramework ();

    emit cellPreModificated ();
    kite->runGlobalRouter ( BuildGlobalSolution );
    emit cellPostModificated ();
  }


  void  GraphicKiteEngine::runDetailed ()
  {
    static vector<Net*> routingNets;

    KiteEngine* kite = KiteEngine::get ( getCell() );
    if ( not kite ) {
      throw Error("KiteEngine not created yet, run the global router first.");
    } 
    if ( cmess1.enabled() )
      kite->printConfiguration ();

    emit cellPreModificated ();

    _viewer->clearToolInterrupt ();

    // kite->setSaturateRatio  ( 0.85 );
    // kite->setExpandStep     ( 0.20 );
    // kite->setRipupLimit     ( 25, Configuration::BorderRipupLimit );
    // kite->setRipupLimit     ( 15, Configuration::StrapRipupLimit );
    // kite->setRipupLimit     (  5, Configuration::LocalRipupLimit );
    // kite->setRipupLimit     (  5, Configuration::GlobalRipupLimit );
    // kite->setRipupLimit     (  5, Configuration::LongGlobalRipupLimit );
    // kite->setRipupCost      (  3 );
    kite->loadGlobalRouting ( Katabatic::LoadGrByNet, routingNets );
    emit cellPostModificated ();

  //Breakpoint::stop ( 0, "Point d'arret:<br>&nbsp;&nbsp;<b>LayerAssingByTrunk()</b><br>"
  //                      "Assignment des layers, methode globale." );
    emit cellPreModificated ();
    kite->layerAssign ( Katabatic::NoNetLayerAssign );
    emit cellPostModificated ();

  //Breakpoint::stop ( 0, "Point d'arret:<br>&nbsp;&nbsp;<b>runNegociate()</b><br>"
  //                      "Routage par Negociation." );
    emit cellPreModificated ();
    kite->runNegociate ();
    emit cellPostModificated ();
  }


  void  GraphicKiteEngine::finalize ()
  {
    emit cellPreModificated ();
    KiteEngine* kite = KiteEngine::get ( getCell() );
    if ( kite ) {
      kite->finalizeLayout ();
      kite->dumpMeasures ();
      kite->destroy ();
    }
    emit cellPostModificated ();
  }


  void  GraphicKiteEngine::save ()
  {
  //KiteEngine* kite = KiteEngine::get ( getCell() );
  //if ( kite ) {
      Cell*              cell = getCell();
      AllianceFramework* af   = AllianceFramework::get ();

      string name = getString(cell->getName()) + "_kite";
      cell->setName ( name );
      af->saveCell ( cell, Catalog::State::Physical );
  //}
  }


  void  GraphicKiteEngine::postEvent ()
  {
    static unsigned int count = 0;

    if ( not (count++ % 500) ) {
    //UpdateSession::close ();

    //_viewer->getCellWidget()->refresh ();
      QApplication::processEvents ();

    //UpdateSession::open ();

      if ( _viewer->isToolInterrupted() ) {
        KiteEngine* kite = KiteEngine::get ( getCell() );
        if ( kite ) kite->setInterrupt ( true );
        
        _viewer->clearToolInterrupt ();
      }
    }
  }


  void  GraphicKiteEngine::addToMenu ( CellViewer* viewer )
  {
    assert ( _viewer == NULL );

    _viewer = viewer;

    QMenu* prMenu   = _viewer->findChild<QMenu*>("viewer.menuBar.placeAndRoute");
    QMenu* stepMenu = _viewer->findChild<QMenu*>("viewer.menuBar.placeAndRoute.stepByStep");
    if ( !prMenu ) {
      QMenuBar* menuBar = _viewer->findChild<QMenuBar*>("viewer.menuBar");
      if ( !menuBar ) {
        cerr << Warning("GraphicKiteEngine::addToMenu() - No MenuBar in parent widget.") << endl;
        return;
      }
      prMenu = menuBar->addMenu ( tr("P&&R") );
      prMenu->setObjectName ( "viewer.menuBar.placeAndRoute" );

      stepMenu = prMenu->addMenu ( tr("&Step by Step") );
      stepMenu->setObjectName ( "viewer.menuBar.placeAndRoute.stepByStep" );

      prMenu->addSeparator ();
    }

    QAction* dRouteAction = _viewer->findChild<QAction*>("viewer.menuBar.placeAndRoute.detailedRoute");
    if ( dRouteAction )
      cerr << Warning("GraphicKiteEngine::addToMenu() - Kite detailed router already hooked in.") << endl;
    else {
      QAction* gLoadSolutionAction = new QAction  ( tr("Kite - &Load Global Routing"), _viewer );
      gLoadSolutionAction->setObjectName ( "viewer.menuBar.placeAndRoute.stepByStep.loadGlobalRouting" );
      gLoadSolutionAction->setStatusTip  ( tr("Load a solution for the global routing (.kgr)") );
      gLoadSolutionAction->setVisible    ( true );
      stepMenu->addAction ( gLoadSolutionAction );

      QAction* gSaveSolutionAction = new QAction  ( tr("Kite - &Save Global Routing"), _viewer );
      gSaveSolutionAction->setObjectName ( "viewer.menuBar.placeAndRoute.stepByStep.saveGlobalRouting" );
      gSaveSolutionAction->setStatusTip  ( tr("Save a global router solution (.kgr)") );
      gSaveSolutionAction->setVisible    ( true );
      stepMenu->addAction ( gSaveSolutionAction );

      QAction* gRouteAction = new QAction  ( tr("Kite - &Global Route"), _viewer );
      gRouteAction->setObjectName ( "viewer.menuBar.placeAndRoute.globalRoute" );
      gRouteAction->setStatusTip  ( tr("Run the <b>Knik</b> global router") );
      gRouteAction->setVisible    ( true );
      prMenu->addAction ( gRouteAction );

      dRouteAction = new QAction  ( tr("Kite - &Detailed Route"), _viewer );
      dRouteAction->setObjectName ( "viewer.menuBar.placeAndRoute.detailedRoute" );
      dRouteAction->setStatusTip  ( tr("Run the <b>Kite</b> detailed router") );
      dRouteAction->setVisible    ( true );
      prMenu->addAction ( dRouteAction );

      QAction* dFinalizeAction = new QAction ( tr("Kite - &Finalize Routing"), _viewer );
      dFinalizeAction->setObjectName ( "viewer.menuBar.placeAndRoute.finalize" );
      dFinalizeAction->setStatusTip  ( tr("Closing Routing") );
      dFinalizeAction->setVisible    ( true );
      prMenu->addAction ( dFinalizeAction );

      QAction* dSaveAction = new QAction ( tr("Kite - &Save Design"), _viewer );
      dSaveAction->setObjectName ( "viewer.menuBar.placeAndRoute.save" );
      dSaveAction->setStatusTip  ( tr("Save routed design (temporary hack)") );
      dSaveAction->setVisible    ( true );
      prMenu->addAction ( dSaveAction );

      connect ( gLoadSolutionAction, SIGNAL(triggered()), this, SLOT(loadGlobalSolution()) );
      connect ( gSaveSolutionAction, SIGNAL(triggered()), this, SLOT(saveGlobalSolution()) );
      connect ( gRouteAction       , SIGNAL(triggered()), this, SLOT(runGlobal         ()) );
      connect ( dRouteAction       , SIGNAL(triggered()), this, SLOT(runDetailed       ()) );
      connect ( dFinalizeAction    , SIGNAL(triggered()), this, SLOT(finalize          ()) );
      connect ( dSaveAction        , SIGNAL(triggered()), this, SLOT(save              ()) );
    }

    connect ( this, SIGNAL(cellPreModificated ()), _viewer->getCellWidget(), SLOT(cellPreModificate ()) );
    connect ( this, SIGNAL(cellPostModificated()), _viewer->getCellWidget(), SLOT(cellPostModificate()) );

    ControllerWidget* controller = _viewer->getControllerWidget();
    ConfigurationWidget* setting = controller->getSettings()
      ->findChild<ConfigurationWidget*>("controller.tabSettings.setting.kite");

    if ( setting == NULL ) {
      setting = new ConfigurationWidget ();
      setting->setObjectName    ( "controller.tabSettings.setting.kite" );
      setting->setConfiguration ( Configuration::getDefault() );
      controller->addSetting ( setting, "Kite" );
    }
  }


  const Name& GraphicKiteEngine::getName () const
  {
    return KiteEngine::staticGetName ();
  }


  Cell* GraphicKiteEngine::getCell ()
  {
    if ( !_viewer ) {
      throw Error ( "<b>Kite:</b> GraphicKiteEngine not bound to any Viewer." );
      return NULL;
    }

    if ( !_viewer->getCell() ) {
      throw Error ( "<b>Kite:</b> No Cell is loaded into the Viewer." );
      return NULL;
    }

    return _viewer->getCell();
  }


  GraphicKiteEngine* GraphicKiteEngine::grab ()
  {
    if ( !_references ) {
      _singleton = new GraphicKiteEngine ();
    }
    _references++;

    return _singleton;
  }


  size_t  GraphicKiteEngine::release ()
  {
    _references--;
    if ( !_references ) {
      delete _singleton;
      _singleton = NULL;
    }
    return _references;
  }


  GraphicKiteEngine::GraphicKiteEngine ()
    : GraphicTool()
    , _viewer(NULL)
  {
    addDrawGo ( "Katabatic::Ac"   , initKatabaticAc   , drawKatabaticAc    );
    addDrawGo ( "Katabatic::GCell", initKatabaticGCell, drawKatabaticGCell );
  }


  GraphicKiteEngine::~GraphicKiteEngine ()
  { }


}  // End of Kite namespace.
