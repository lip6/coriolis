// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |              M a u k a  -  P l a c e r                          |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./GraphicMaukaEngine.cpp"                 |
// +-----------------------------------------------------------------+


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
#include  <hurricane/viewer/ExceptionWidget.h>
#include  <crlcore/Utilities.h>
#include  <crlcore/ToolBox.h>
#include  <crlcore/AllianceFramework.h>
#include  <nimbus/NimbusEngine.h>
#include  <metis/MetisEngine.h>
#include  <mauka/Container.h>
#include  <mauka/GraphicMaukaEngine.h>
//#include  <mauka/ConfigurationWidget.h>


namespace Mauka {


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
  using Hurricane::ExceptionWidget;
  using CRL::Catalog;
  using CRL::getInstancesCount;
  using CRL::AllianceFramework;
  using Nimbus::NimbusEngine;
  using Metis::MetisEngine;


  size_t              GraphicMaukaEngine::_references = 0;
  GraphicMaukaEngine* GraphicMaukaEngine::_singleton  = NULL;


  void  GraphicMaukaEngine::initMaukaContainer ( CellWidget* widget )
  {
    widget->getDrawingPlanes().setPen   ( Graphics::getPen  ("mauka.container",widget->getDarkening()) );
    widget->getDrawingPlanes().setBrush ( Graphics::getBrush("mauka.container",widget->getDarkening()) );
  }


  void  GraphicMaukaEngine::drawMaukaContainer ( CellWidget*           widget
                                               , const Go*             go
                                               , const BasicLayer*     basicLayer
                                               , const Box&            box
                                               , const Transformation& transformation
                                               )
  {
    const Container* container = static_cast<const Container*>(go);

    QPainter& painter = widget->getPainter();
    painter.drawRect ( widget->dbuToDisplayRect(container->getBoundingBox()) );
  }


  void  GraphicMaukaEngine::refreshViewer ()
  {
  //static unsigned int count = 0;

  //if ( not (count++ % 500) ) {
  //UpdateSession::close ();

    _viewer->getCellWidget()->fitToContents ();
    _viewer->getCellWidget()->refresh ();
    QApplication::processEvents ();

  //UpdateSession::open ();

      // if ( _viewer->isToolInterrupted() ) {
      //   KiteEngine* kite = KiteEngine::get ( getCell() );
      //   if ( kite ) kite->setInterrupt ( true );
        
      //   _viewer->clearToolInterrupt ();
      // }
  //}
  }


  MaukaEngine* GraphicMaukaEngine::createEngine ()
  {
    Cell* cell = getCell ();

    MaukaEngine* mauka = MaukaEngine::get ( cell );
    if ( mauka == NULL ) {
      NimbusEngine* nimbus = NimbusEngine::get ( cell );
      if ( nimbus == NULL )
        NimbusEngine::create ( cell );

      mauka = MaukaEngine::create ( cell );
      if ( cmess1.enabled() )
        mauka->getConfiguration()->print( cell );
    } else
      cerr << Warning("%s already has a Mauka engine.",getString(cell).c_str()) << endl;

    return mauka;
  }


  MaukaEngine* GraphicMaukaEngine::getForFramework ( unsigned int flags )
  {
    MaukaEngine* mauka = MaukaEngine::get( getCell() );
    if (mauka) return mauka;

    if (flags & CreateEngine) {
      mauka = createEngine();
      if (not mauka) 
        throw Error( "Failed to create Mauka engine on %s.", getString(getCell()).c_str() );
    } else {
      throw Error( "MaukaEngine not created yet, out of sequence action." );
    }

    return mauka;
  }


  void  GraphicMaukaEngine::_doQuadriPart ()
  {
    if ( not MetisEngine::isHMetisCapable() ) {
      cerr << Warning("Mauka has not been compiled againts hMETIS.\n"
                      "          Quadri-partition step is disabled, simulated annealing may be *very* long." ) << endl;
      return;
    }
    Cell* cell = getCell();

    NimbusEngine* nimbus = NimbusEngine::get( cell );
    if (nimbus == NULL) {
      nimbus = NimbusEngine::create( cell );
      if (cmess1.enabled())
        nimbus->getConfiguration()->print( cell );
    }

    MetisEngine* metis = MetisEngine::get( cell );
    if (metis == NULL) {
      metis  = MetisEngine ::create( cell );
      if (getInstancesCount(cell) < metis->getNumberOfInstancesStopCriterion()*4)
        return;

      if (cmess1.enabled())
        metis->getConfiguration()->print( cell );
    }
    metis->setRefreshCb( std::bind(&GraphicMaukaEngine::refreshViewer,this) );

    MetisEngine::doQuadriPart( cell );

    MaukaEngine* mauka = MaukaEngine::get( cell );
    if (mauka != NULL)
      throw Warning("GraphicMaukaEngine::doQuadriPart(): Placement already done on <%s>"
                   ,getString(cell->getName()).c_str());

    mauka = createEngine();
    MaukaEngine::regroupOverloadedGCells( cell );

    _viewer->clearToolInterrupt();
    _viewer->getCellWidget()->fitToContents();
    _viewer->redrawCellWidget();
  }


  void  GraphicMaukaEngine::_doSimulatedAnnealing ()
  {
    MaukaEngine* mauka = getForFramework( CreateEngine );
    mauka->setRefreshCb ( std::bind(&GraphicMaukaEngine::refreshViewer,this) );

    _viewer->clearToolInterrupt ();
    _viewer->getCellWidget()->fitToContents ();

    mauka->Run ();
    _viewer->redrawCellWidget();
  }


  void  GraphicMaukaEngine::_place ()
  {
    if (MetisEngine::isHMetisCapable()) {
      doQuadriPart();
    } else {
      cerr << Warning("Mauka has not been compiled againts hMETIS.\n"
                      "          Quadri-partition step is disabled, simulated annealing may be *very* long." ) << endl;
    }

    doSimulatedAnnealing();
    _save();
  }


  void  GraphicMaukaEngine::_save ()
  {
    MaukaEngine* mauka = getForFramework( NoFlags );

    _viewer->clearToolInterrupt ();
    _viewer->redrawCellWidget();
    mauka->Save ();
  }


  void  GraphicMaukaEngine::doQuadriPart ()
  { ExceptionWidget::catchAllWrapper( std::bind(&GraphicMaukaEngine::_doQuadriPart,this) ); }


  void  GraphicMaukaEngine::doSimulatedAnnealing ()
  { ExceptionWidget::catchAllWrapper( std::bind(&GraphicMaukaEngine::_doSimulatedAnnealing,this) ); }


  void  GraphicMaukaEngine::place ()
  { ExceptionWidget::catchAllWrapper( std::bind(&GraphicMaukaEngine::_place,this) ); }


  void  GraphicMaukaEngine::save ()
  { ExceptionWidget::catchAllWrapper( std::bind(&GraphicMaukaEngine::_save,this) ); }


  void  GraphicMaukaEngine::addToMenu ( CellViewer* viewer )
  {
    assert ( _viewer == NULL );

    _viewer = viewer;

    QMenu* prMenu   = _viewer->findChild<QMenu*>("viewer.menuBar.placeAndRoute");
    QMenu* stepMenu = _viewer->findChild<QMenu*>("viewer.menuBar.placeAndRoute.stepByStep");
    if ( prMenu == NULL ) {
      QMenuBar* menuBar = _viewer->findChild<QMenuBar*>("viewer.menuBar");
      if ( menuBar == NULL ) {
        cerr << Warning("GraphicMaukaEngine::addToMenu() - No MenuBar in parent widget.") << endl;
        return;
      }
      prMenu = menuBar->addMenu ( tr("P&&R") );
      prMenu->setObjectName ( "viewer.menuBar.placeAndRoute" );

      stepMenu = prMenu->addMenu ( tr("&Step by Step") );
      stepMenu->setObjectName ( "viewer.menuBar.placeAndRoute.stepByStep" );

      prMenu->addSeparator ();
    }

    QAction* placeAction = _viewer->findChild<QAction*>("viewer.menuBar.placeAndRoute.maukaPlace");
    if ( placeAction != NULL )
      cerr << Warning("GraphicMaukaEngine::addToMenu() - Mauka placer already hooked in.") << endl;
    else {
      QAction* quadriPartAction = new QAction  ( tr("Mauka - &QuadriPartition"), _viewer );
      quadriPartAction->setObjectName ( "viewer.menuBar.placeAndRoute.quadriPartition" );
      quadriPartAction->setStatusTip  ( tr("Run the <b>hMETIS</b> quadri-partitioner") );
      quadriPartAction->setVisible    ( true );
      stepMenu->addAction ( quadriPartAction );

      QAction* annealingAction = new QAction  ( tr("Mauka - &Place"), _viewer );
      annealingAction->setObjectName ( "viewer.menuBar.placeAndRoute.maukaPlace" );
      annealingAction->setStatusTip  ( tr("Run the <b>Mauka</b> placer") );
      annealingAction->setVisible    ( true );
      stepMenu->addAction ( annealingAction );

      QAction* placeAction = new QAction  ( tr("Mauka - &Place"), _viewer );
      placeAction->setObjectName ( "viewer.menuBar.placeAndRoute.place" );
      placeAction->setStatusTip  ( tr("Run the <b>Mauka</b> placer") );
      placeAction->setVisible    ( true );
      prMenu->addAction ( placeAction );

      connect ( quadriPartAction, SIGNAL(triggered()), this, SLOT(doQuadriPart()) );
      connect ( annealingAction , SIGNAL(triggered()), this, SLOT(doSimulatedAnnealing()) );
      connect ( placeAction     , SIGNAL(triggered()), this, SLOT(place()) );
    }

    // ControllerWidget*    controller = _viewer->getControllerWidget();
    // ConfigurationWidget* setting     = controller->getSettings()
    //   ->findChild<ConfigurationWidget*>("controller.tabSettings.setting.mauka");

    // if ( setting == NULL ) {
    //   setting = new ConfigurationWidget ();
    //   setting->setObjectName    ( "controller.tabSettings.setting.mauka" );
    //   setting->setConfiguration ( Nimbus::Configuration::getDefault()
    //                             , Metis::Configuration::getDefault()
    //                             , Configuration::getDefault()
    //                             );
    //   controller->addSetting ( setting, "Mauka" );
    // }
  }


  const Name& GraphicMaukaEngine::getName () const
  {
    return MaukaEngine::staticGetName ();
  }


  Cell* GraphicMaukaEngine::getCell ()
  {
    if ( _viewer == NULL ) {
      throw Error ( "<b>Mauka:</b> GraphicMaukaEngine not bound to any Viewer." );
      return NULL;
    }

    if ( _viewer->getCell() == NULL ) {
      throw Error ( "<b>Mauka:</b> No Cell is loaded into the Viewer." );
      return NULL;
    }

    return _viewer->getCell();
  }


  GraphicMaukaEngine* GraphicMaukaEngine::grab ()
  {
    if ( not _references ) {
      _singleton = new GraphicMaukaEngine ();
    }
    _references++;

    return _singleton;
  }


  size_t  GraphicMaukaEngine::release ()
  {
    _references--;
    if ( not _references ) {
      delete _singleton;
      _singleton = NULL;
    }
    return _references;
  }


  GraphicMaukaEngine::GraphicMaukaEngine ()
    : GraphicTool()
    , _viewer    (NULL)
  {
    addDrawGo ( "Mauka::Container", initMaukaContainer, drawMaukaContainer );
    addDrawGo ( "Mauka::Bin"      , initMaukaContainer, drawMaukaContainer );
  }


  GraphicMaukaEngine::~GraphicMaukaEngine ()
  { }


}  // End of Mauka namespace.
