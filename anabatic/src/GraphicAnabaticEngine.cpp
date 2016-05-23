// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "GraphicAnabaticEngine.cpp"                     |
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
#include "anabatic/GCell.h"
#include "anabatic/GraphicAnabaticEngine.h"


namespace Anabatic {

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


  size_t                 GraphicAnabaticEngine::_references = 0;
  GraphicAnabaticEngine* GraphicAnabaticEngine::_singleton  = NULL;


  void  GraphicAnabaticEngine::initGCell ( CellWidget* widget )
  {
    widget->getDrawingPlanes().setPen( Qt::NoPen );
  }


  void  GraphicAnabaticEngine::drawGCell ( CellWidget*           widget
                                         , const Go*             go
                                         , const BasicLayer*     basicLayer
                                         , const Box&            box
                                         , const Transformation& transformation
                                         )
  {
    const GCell* gcell = static_cast<const GCell*>(go);

    QPainter& painter = widget->getPainter();

    painter.setPen  ( Graphics::getPen  ("gcell",widget->getDarkening()) );
    painter.setBrush( Graphics::getBrush("gcell",widget->getDarkening()) );
    painter.drawRect( widget->dbuToScreenRect(gcell->getBoundingBox()) );
  }


  void  GraphicAnabaticEngine::initEdge ( CellWidget* widget )
  {
    widget->getDrawingPlanes().setPen( Qt::NoPen );
  }


  void  GraphicAnabaticEngine::drawEdge ( CellWidget*           widget
                                        , const Go*             go
                                        , const BasicLayer*     basicLayer
                                        , const Box&            box
                                        , const Transformation& transformation
                                        )
  {
    const Edge* edge = static_cast<const Edge*>(go);

    if (edge) {
      unsigned int occupancy = 255;
      if (edge->getRealOccupancy() < edge->getCapacity())
        occupancy = (unsigned int)( 255.0 * ( (float)edge->getRealOccupancy() / (float)edge->getCapacity() ) );

      QPainter& painter = widget->getPainter();
      if (edge->getRealOccupancy() > edge->getCapacity()) {
        QColor color ( Qt::cyan );
        painter.setPen( DisplayStyle::darken(color,widget->getDarkening()) );
      }

      QBrush brush = QBrush( Qt::white, Qt::DiagCrossPattern );
      if (edge->getCapacity() > 0.0)
        brush = Graphics::getColorScale( ColorScale::Fire ).getBrush( occupancy, widget->getDarkening() );

      painter.setPen( Qt::NoPen );
      painter.setBrush( brush );
      painter.drawRect( widget->dbuToScreenRect(edge->getBoundingBox(), false) );
    }
  }


  AnabaticEngine* GraphicAnabaticEngine::createEngine ()
  {
    Cell* cell = getCell();

    AnabaticEngine* engine = AnabaticEngine::get( cell );
    if (not engine) {
      engine = AnabaticEngine::create( cell );
    } else
      cerr << Warning( "%s already has a Anabatic engine.", getString(cell).c_str() ) << endl;

    return engine;
  }


  AnabaticEngine* GraphicAnabaticEngine::getForFramework ( unsigned int flags )
  {
  // Currently, only one framework is avalaible: Alliance.

    AnabaticEngine* engine = AnabaticEngine::get( getCell() );
    if (engine) return engine;

    if (flags & CreateEngine) {
      engine = createEngine();
      if (not engine) 
        throw Error( "Failed to create Anabatic engine on %s.", getString(getCell()).c_str() );
    } else {
      throw Error( "AnabaticEngine not created yet, run the global router first." );
    }

    return engine;
  }


  void  GraphicAnabaticEngine::_runTest ()
  {
    if (_viewer) _viewer->emitCellAboutToChange();
    AnabaticEngine* engine = getForFramework( CreateEngine );

    engine->getSouthWestGCell()->doGrid();

  // GCell*    row0    = getSouthWestGCell();
  // DbU::Unit xcorner = getCell()->getAbutmentBox().getXMin();
  // DbU::Unit ycorner = getCell()->getAbutmentBox().getYMin();

  // cdebug.log(119,1) << "row0: " << row0 << endl;

  // GCell* row1 = row0->hcut( ycorner+DbU::fromLambda(50.0) );
  // cdebug.tabw(119,-1);
  // cdebug.log(119,1) << "row1: " << row1 << endl;

  // GCell* row2 = row1->hcut( ycorner+DbU::fromLambda(2*50.0) );
  // cdebug.tabw(119,-1);
  // cdebug.log(119,1) << "row2: " << row2 << endl;

  // row0 = row0->vcut( xcorner+DbU::fromLambda(50.0) );
  // cdebug.tabw(119,-1);
  // cdebug.log(119,1) << "row0+1: " << row0 << endl;

  // row0 = row0->vcut( xcorner+DbU::fromLambda(3*50.0) );
  // cdebug.tabw(119,-1);
  // cdebug.log(119,1) << "row0+2: " << row0 << endl;

  // row0 = row0->vcut( xcorner+DbU::fromLambda(5*50.0) );
  // cdebug.tabw(119,-1);
  // cdebug.log(119,1) << "row0+3: " << row0 << endl;
    
  // row1 = row1->vcut( xcorner+DbU::fromLambda(2*50.0) );
  // cdebug.tabw(119,-1);
  // cdebug.log(119,1) << "row1+1: " << row1 << endl;


  // cdebug.tabw(119,-1);

  // gcell = gcell->hcut( ycut+DbU::fromLambda(50.0) );
  // cerr << "New GCell: " << gcell << endl;
    
  // DbU::Unit ycut  = getCell()->getAbutmentBox().getYMin() + DbU::fromLambda(50.0);
  // for ( ; ycut < getCell()->getAbutmentBox().getYMax() ; ycut += DbU::fromLambda(50.0) ) {
  //   cdebug.log(119,2) << "H cut line (y coordinate): " << DbU::getValueString(ycut) << endl;
  //   gcell = gcell->hcut( ycut );
  //   cerr << gcell << endl;
  //   cdebug.tabw(119,-2);
  // }

  // if (_viewer) _viewer->emitCellChanged();
  }


  void  GraphicAnabaticEngine::addToMenu ( CellViewer* viewer )
  {
    assert( _viewer == NULL );

    _viewer = viewer;

    if (_viewer->hasMenuAction("placeAndRoute.anabatic")) {
      cerr << Warning( "GraphicAnabaticEngine::addToMenu() - Anabatic engine already hooked in." ) << endl;
      return;
    }

    _viewer->addToMenu( "placeAndRoute.anabatic"
                      , "Anabatic - &Test Run"
                      , "Perform a test run of Anabatic on the design"
                      , std::bind(&GraphicAnabaticEngine::_runTest,this)
                      );
  }


  const Name& GraphicAnabaticEngine::getName () const
  { return AnabaticEngine::staticGetName(); }


  Cell* GraphicAnabaticEngine::getCell ()
  {
    if (_viewer == NULL) {
      throw Error( "<b>Anabatic:</b> GraphicAnabaticEngine not bound to any Viewer." );
      return NULL;
    }

    if (_viewer->getCell() == NULL) {
      throw Error( "<b>Anabatic:</b> No Cell is loaded into the Viewer." );
      return NULL;
    }

    return _viewer->getCell();
  }


  GraphicAnabaticEngine* GraphicAnabaticEngine::grab ()
  {
    if (not _references)
      _singleton = new GraphicAnabaticEngine ();
    _references++;

    return _singleton;
  }


  size_t  GraphicAnabaticEngine::release ()
  {
    --_references;
    if (not _references) {
      delete _singleton;
      _singleton = NULL;
    }
    return _references;
  }


  GraphicAnabaticEngine::GraphicAnabaticEngine ()
    : GraphicTool()
    , _viewer    (NULL)
  {
    addDrawGo( "Anabatic::GCell", initGCell, drawGCell );
    addDrawGo( "Anabatic::Edge" , initEdge , drawEdge  );
  }


  GraphicAnabaticEngine::~GraphicAnabaticEngine ()
  { }



}  // Anabatic namespace.
