// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2007-2023, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |       T r a m o n t a n a  -  Extractor & LVX                   |
// |                                                                 |
// |  Algorithm    :                   Christian MASSON              |
// |  First impl.  :                           Yifei WU              |
// |  Second impl. :                   Jean-Paul CHAPUT              |
// |  E-mail       :           Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module   :  "./GraphicTramontanaEngine.cpp"                |
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
#include <tramontana/TabEquipotentials.h>
#include <tramontana/GraphicTramontanaEngine.h>


namespace Tramontana {

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


  size_t                   GraphicTramontanaEngine::_references = 0;
  GraphicTramontanaEngine* GraphicTramontanaEngine::_singleton  = NULL;


#if THIS_IS_DISABLED
  void  GraphicTramontanaEngine::initGCell ( CellWidget* widget )
  {
    widget->getDrawingPlanes().setPen( Qt::NoPen );
    TramontanaEngine* tramontana = TramontanaEngine::get( widget->getCell() );
    if (tramontana) tramontana->setDensityMode( GCell::MaxDensity );
  }


  void  GraphicTramontanaEngine::drawGCell ( CellWidget*           widget
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
#endif


  TramontanaEngine* GraphicTramontanaEngine::createEngine ()
  {
    Cell* cell = getCell ();

    TramontanaEngine* tramontana = TramontanaEngine::get( cell );
    if (not tramontana) {
      tramontana = TramontanaEngine::create( cell );
      tramontana->setViewer( _viewer );
    } else
      cerr << Warning( "%s already has a Tramontana engine.", getString(cell).c_str() ) << endl;

    return tramontana;
  }


  TramontanaEngine* GraphicTramontanaEngine::getForFramework ( uint32_t flags )
  {
  // Currently, only one framework is avalaible: Alliance.

    TramontanaEngine* tramontana = TramontanaEngine::get( getCell() );
    if (tramontana) return tramontana;

    if (flags & CreateEngine) {
      tramontana = createEngine();
      if (not tramontana) 
        throw Error( "Failed to create Tramontana engine on %s.", getString(getCell()).c_str() );
    } else {
      throw Error( "TramontanaEngine not created yet, run the global router first." );
    }

    return tramontana;
  }


  void  GraphicTramontanaEngine::_extract ()
  {
    TramontanaEngine* tramontana = getForFramework( CreateEngine );
    tramontana->extract();

  //Breakpoint::stop( 0, "GraphicTramontanaEngine::_extract() done." );
  }


  void  GraphicTramontanaEngine::addToMenu ( CellViewer* viewer )
  {
    assert(_viewer == NULL);

    _viewer = viewer;

    if (_viewer->hasMenuAction("placeAndRoute.extract")) {
      cerr << Warning( "GraphicTramontanaEngine::addToMenu() - Tramontana extractor already hooked in." ) << endl;
      return;
    }

    _viewer->addToMenu( "placeAndRoute.extract"
                      , "E&xtract  .  .  .  .  . [Tramontana]"
                      , "Run the extractor"
                      , std::bind(&GraphicTramontanaEngine::_extract,this)
                      );

    ControllerWidget* controller = viewer->getControllerWidget();
    if (controller) {
      TabEquipotentials* tabEqui = new TabEquipotentials ();
      tabEqui->setObjectName( "controller.tabEquipotentials" );
      tabEqui->setCellWidget( viewer->getCellWidget() );
      controller->insertTabAfter( "controller.tabNetlist", tabEqui, "Equipotentials" );
    }
  }


  const Name& GraphicTramontanaEngine::getName () const
  { return TramontanaEngine::staticGetName(); }


  Cell* GraphicTramontanaEngine::getCell ()
  {
    if (not _viewer) {
      throw Error( "<b>Tramontana:</b> GraphicTramontanaEngine not bound to any Viewer." );
      return NULL;
    }
    if (not _viewer->getCell()) {
      throw Error( "<b>Tramontana:</b> No Cell is loaded into the Viewer." );
      return NULL;
    }

    return _viewer->getCell();
  }


  GraphicTramontanaEngine* GraphicTramontanaEngine::grab ()
  {
    if (not _references) {
      _singleton = new GraphicTramontanaEngine ();
    }
    _references++;

    return _singleton;
  }


  size_t  GraphicTramontanaEngine::release ()
  {
    --_references;
    if (not _references) {
      delete _singleton;
      _singleton = NULL;
    }
    return _references;
  }


  GraphicTramontanaEngine::GraphicTramontanaEngine ()
    : GraphicTool()
    , _viewer    (NULL)
  {
#if THIS_IS_DISABLED
    addDrawGo( "Anabatic::GCell", initGCell, drawGCell );
    addDrawGo( "Anabatic::Edge" , initEdge , drawEdge  );
#endif
  }


  GraphicTramontanaEngine::~GraphicTramontanaEngine ()
  { }


}  // Tramontana namespace.
