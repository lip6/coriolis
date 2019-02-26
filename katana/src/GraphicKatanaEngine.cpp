// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
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
  using Hurricane::DisplayStyle;
  using Hurricane::ControllerWidget;
  using Hurricane::ExceptionWidget;
  using CRL::Catalog;
  using CRL::AllianceFramework;
  using Anabatic::Edge;
  using Anabatic::GCell;


  size_t               GraphicKatanaEngine::_references = 0;
  GraphicKatanaEngine* GraphicKatanaEngine::_singleton  = NULL;


  void  GraphicKatanaEngine::initGCell ( CellWidget* widget )
  {
    widget->getDrawingPlanes().setPen( Qt::NoPen );
    KatanaEngine* katana = KatanaEngine::get( widget->getCell() );
    if (katana) katana->setDensityMode( GCell::MaxDensity );
  }


  void  GraphicKatanaEngine::drawGCell ( CellWidget*           widget
                                       , const Go*             go
                                       , const BasicLayer*     basicLayer
                                       , const Box&            box
                                       , const Transformation& transformation
                                       )
  {
    const GCell* gcell = static_cast<const GCell*>(go);

    QPainter& painter = widget->getPainter();
    QPen      pen     = Graphics::getPen  ("Anabatic::GCell",widget->getDarkening()); 
    Box       bb      = gcell->getBoundingBox();
    QRect     pixelBb = widget->dbuToScreenRect(bb);

    if (GCell::getDisplayMode() == GCell::Density) {
      uint32_t density = (unsigned int)( 255.0 * gcell->getDensity() );
      if (density > 255) density = 255;

      painter.setBrush( Graphics::getColorScale( ColorScale::Fire ).getBrush( density, widget->getDarkening() ) );
      painter.drawRect( pixelBb );
    } else {
      int fontScale   =  0;
      int halfHeight  = 20;
      int halfWidth   = 80;
      if (widget->isPrinter()) {
        fontScale   = -5;
        halfHeight  =  9;
        halfWidth   = 39;
        
      }
      
      painter.setPen  ( pen );
      painter.setBrush( Graphics::getBrush("Anabatic::GCell",widget->getDarkening()) );
      painter.drawRect( pixelBb );

      if ( (pixelBb.width() > 2*halfWidth) and (pixelBb.height() > 2*halfHeight) ) {
        QString text  = QString("%1").arg(gcell->getId());
        QFont   font  = Graphics::getFixedFont( QFont::Normal, false, false, fontScale );
        painter.setFont(font);

        pen.setWidth( 1 );
        painter.setPen( pen );

        painter.save     ();
        painter.translate( widget->dbuToScreenPoint(bb.getCenter().getX(), bb.getCenter().getY()) );
        painter.drawRect ( QRect( -halfWidth, -halfHeight, 2*halfWidth, 2*halfHeight ) );
        painter.drawText ( QRect( -halfWidth, -halfHeight, 2*halfWidth, 2*halfHeight )
                         , text
                         , QTextOption(Qt::AlignCenter)
                         );
        painter.restore  ();
      }
    }
  }


  void  GraphicKatanaEngine::initEdge ( CellWidget* widget )
  {
    widget->getDrawingPlanes().setPen( Qt::NoPen );
  }


  void  GraphicKatanaEngine::drawEdge ( CellWidget*           widget
                                      , const Go*             go
                                      , const BasicLayer*     basicLayer
                                      , const Box&            box
                                      , const Transformation& transformation
                                      )
  {
    static QFont font       = Graphics::getFixedFont( QFont::Bold ); 
    static int   fontHeight = QFontMetrics(font).height();

    const Edge* edge = static_cast<const Edge*>(go);

    if (edge) {
      Box      bb        = edge->getBoundingBox();
      uint32_t occupancy = 255;
    //if (edge->getRealOccupancy() < edge->getCapacity())
    //  occupancy = (uint32_t)( 255.0 * ( (float)edge->getRealOccupancy() / (float)edge->getCapacity() ) );

      float edgeOccupancy = edge->getEstimateOccupancy() + (float)edge->getRealOccupancy();

      if ((unsigned int)edgeOccupancy < edge->getCapacity())
        occupancy = (uint32_t)( 255.0 * (edgeOccupancy / (float)edge->getCapacity()) );

      QPainter& painter = widget->getPainter();
      if ((unsigned int)edgeOccupancy > edge->getCapacity()) {
        QColor color ( Qt::cyan );
        painter.setPen( DisplayStyle::darken(color,widget->getDarkening()) );
      }

      QBrush brush = QBrush( Qt::white, Qt::DiagCrossPattern );
      if (edge->getCapacity() > 0.0)
        brush = Graphics::getColorScale( ColorScale::Fire ).getBrush( occupancy, widget->getDarkening() );

      QRect pixelBb = widget->dbuToScreenRect( bb, false);
      painter.setPen( Qt::NoPen );
      painter.setBrush( brush );
      painter.drawRect( pixelBb );

      if (fontHeight > ((edge->isHorizontal()) ? pixelBb.height() : pixelBb.width()) + 4) return; 

    //QString text  = QString("%1/%2").arg(edge->getRealOccupancy()).arg(edge->getCapacity());
      QString text  = QString("%1/%2 %3")
        .arg( edgeOccupancy )
        .arg( edge->getCapacity() )
        .arg( edge->getHistoricCost() );
      QColor  color ( (occupancy > 170) ? Qt::black : Qt::white );
      painter.setPen (DisplayStyle::darken(color,widget->getDarkening()));
      painter.setFont(font);

      if (edge->isVertical()) {
        painter.save     ();
        painter.translate( widget->dbuToScreenPoint(bb.getXMin(), bb.getYMin()) );
        painter.rotate   ( -90 );
        painter.drawText (QRect( 0
                               , 0
                               , widget->dbuToScreenLength(bb.getHeight())
                               , widget->dbuToScreenLength(bb.getWidth ()))
                         , text
                         , QTextOption(Qt::AlignCenter)
                         );
        painter.restore  ();
      } else
        painter.drawText( widget->dbuToScreenRect(bb,false ), text, QTextOption(Qt::AlignCenter) );

      painter.setPen( Qt::NoPen );
    }
  }


  KatanaEngine* GraphicKatanaEngine::createEngine ()
  {
    Cell* cell = getCell ();

    KatanaEngine* katana = KatanaEngine::get( cell );
    if (not katana) {
      katana = KatanaEngine::create( cell );
      katana->setPostEventCb( boost::bind(&GraphicKatanaEngine::postEvent,this) );
      katana->setViewer( _viewer );
      katana->printConfiguration();
      katana->digitalInit();
    } else
      cerr << Warning( "%s already has a Katana engine.", getString(cell).c_str() ) << endl;

    return katana;
  }


  KatanaEngine* GraphicKatanaEngine::getForFramework ( uint32_t flags )
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
    KatanaEngine* katana = getForFramework( NoFlags );

    _viewer->clearToolInterrupt();
    katana->loadGlobalRouting( Anabatic::EngineLoadGrByNet );
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
    katana->runNegociate( Flags::PairSymmetrics );
  }


  void  GraphicKatanaEngine::_finalize ()
  {
    KatanaEngine* katana = getForFramework( NoFlags );
    if (katana) {
      katana->finalizeLayout();
      katana->destroy();
    }
  }


  void  GraphicKatanaEngine::_runTest ()
  {
    KatanaEngine* katana = getForFramework( NoFlags );
    if (katana) {
      katana->loadGlobalRouting( Anabatic::EngineLoadGrByNet );
    // Now done through Horus.
    //katana->runTest();
      katana->runNegociate( Flags::PairSymmetrics );
    //katana->runNegociate();
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
    static uint32_t count = 0;

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
                      , "Run the <b>Katana</b> global router"
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
#if NO_NEED_OF_IT_NOW
    _viewer->addToMenu( "placeAndRoute.katana.stepByStep.runTest"
                      , "Katana - Run &Test"
                      , "Run Test Program (symmetric routing of gmChamla)"
                      , std::bind(&GraphicKatanaEngine::_runTest,this)
                      );
#endif
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
  {
    addDrawGo( "Anabatic::GCell", initGCell, drawGCell );
    addDrawGo( "Anabatic::Edge" , initEdge , drawEdge  );
  }


  GraphicKatanaEngine::~GraphicKatanaEngine ()
  { }


}  // Katana namespace.
