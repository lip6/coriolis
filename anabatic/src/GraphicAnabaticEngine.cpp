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


#include <cmath>
#include <boost/bind.hpp>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <hurricane/Warning.h>
#include <hurricane/Error.h>
#include <hurricane/Breakpoint.h>
#include <hurricane/DebugSession.h>
#include <hurricane/UpdateSession.h>
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
#include "anabatic/Dijkstra.h"


namespace Anabatic {

  using namespace std;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Exception;
  using Hurricane::Breakpoint;
  using Hurricane::DebugSession;
  using Hurricane::UpdateSession;
  using Hurricane::Point;
  using Hurricane::Entity;
  using Hurricane::Net;
  using Hurricane::Graphics;
  using Hurricane::ColorScale;
  using Hurricane::DisplayStyle;
  using Hurricane::DrawingStyle;
  using Hurricane::ControllerWidget;
  using Hurricane::ExceptionWidget;
  using CRL::Catalog;
  using CRL::AllianceFramework;
    

// -------------------------------------------------------------------
// Test functions.


  class DigitalDistance {
    public:
      inline            DigitalDistance ( float h, float k );
             DbU::Unit  operator()      ( const Vertex* source ,const Vertex* target,const Edge* edge ) const;
    private:
    // For an explanation of h & k parameters, see:
    //     "KNIK, routeur global pour la plateforme Coriolis", p. 52.
      float  _h;
      float  _k;
  };


  inline DigitalDistance::DigitalDistance ( float h, float k ) : _h(h), _k(k) { }


  DbU::Unit  DigitalDistance::operator() ( const Vertex* source ,const Vertex* target,const Edge* edge ) const
  {
    if (edge->getCapacity() <= 0) return Vertex::unreached;

    float congestion     = (float)edge->getRealOccupancy() / (float)edge->getCapacity();
    float congestionCost = 1.0 + _h / (1.0 + std::exp(_k * (congestion - 1.0)));

    float distance = (float)source->getDistance() + congestionCost * (float)edge->getDistance();

    // Edge* sourceFrom = source->getFrom();
    // if (sourceFrom) {
    //   distance += ((sourceFrom->isHorizontal() xor edge->isHorizontal()) ? 3.0 : 0.0) * (float)Edge::unity;
    // }
    cdebug_log(112,0) << "cong:"   << congestion
                      << " ccost:" << congestionCost
                      << " digitalDistance:" << DbU::getValueString((DbU::Unit)distance) << endl;

    return (DbU::Unit)distance;
  }


  void  anabaticTest_1 ( AnabaticEngine* engine )
  {
    engine->getSouthWestGCell()->doGrid();

    Point  position ( DbU::fromLambda(100.0), DbU::fromLambda(100.0) );
    GCell* gcell    = engine->getGCellUnder( position );

    cerr << "Gcell under:" << position << " is " << gcell << endl;
  }
    

  void  anabaticTest_2 ( AnabaticEngine* engine )
  {
    UpdateSession::open();

    GCell*    row0    = engine->getSouthWestGCell();
    DbU::Unit xcorner = engine->getCell()->getAbutmentBox().getXMin();
    DbU::Unit ycorner = engine->getCell()->getAbutmentBox().getYMin();

    cdebug_log(119,1) << "row0: " << row0 << endl;

    GCell* row1 = row0->hcut( ycorner+DbU::fromLambda(50.0) );
    cdebug_tabw(119,-1);
    cdebug_log(119,1) << "row1: " << row1 << endl;

    GCell* row2 = row1->hcut( ycorner+DbU::fromLambda(2*50.0) );
    cdebug_tabw(119,-1);
    cdebug_log(119,1) << "row2: " << row2 << endl;

    row0 = row0->vcut( xcorner+DbU::fromLambda(50.0) );
    cdebug_tabw(119,-1);
    cdebug_log(119,1) << "row0+1: " << row0 << endl;

    row0 = row0->vcut( xcorner+DbU::fromLambda(3*50.0) );
    cdebug_tabw(119,-1);
    cdebug_log(119,1) << "row0+2: " << row0 << endl;

    row0 = row0->vcut( xcorner+DbU::fromLambda(5*50.0) );
    cdebug_tabw(119,-1);
    cdebug_log(119,1) << "row0+3: " << row0 << endl;
    
    row1 = row1->vcut( xcorner+DbU::fromLambda(2*50.0) );
    cdebug_tabw(119,-1);
    cdebug_log(119,1) << "row1+1: " << row1 << endl;

    cdebug_tabw(119,-1);

    UpdateSession::close();
  }
    

  void  anabaticTest_3 ( AnabaticEngine* engine )
  {
    for ( int i=0 ; i<4 ; ++i ) {
      cdebug_log(110,1) << "Running test 3, loop:" << i << " ..." << endl;

      if ( i%2) anabaticTest_1( engine );
      else      anabaticTest_2( engine );

      engine->reset();
      cdebug_log(110,1) << "Test 3, loop:" << i << " completed." << endl;
      cdebug_tabw(110,-1);
    }
  }
    

  void  anabaticTest_4 ( AnabaticEngine* engine )
  {
    Cell* cell = engine->getCell();
    cell->flattenNets( Cell::Flags::BuildRings );
    cell->createRoutingPadRings( Cell::Flags::BuildRings );

    engine->getSouthWestGCell()->doGrid();

    vector<Net*> nets;
    nets.push_back( cell->getNet( "ialu.inv_x2_3_sig"  ) ); 
    nets.push_back( cell->getNet( "iram.na3_x1_13_sig" ) ); 
    nets.push_back( cell->getNet( "iram.ram_idx_7(1)"  ) ); 

    Dijkstra* dijkstra = new Dijkstra ( engine );
    for ( Net* net : nets ) {
      dijkstra->load( net );
      dijkstra->run();
    }
    delete dijkstra;
  }
    

  void  anabaticTest_5 ( AnabaticEngine* engine )
  {
    Cell* cell = engine->getCell();


    cell->flattenNets( Cell::Flags::BuildRings );
    cell->createRoutingPadRings( Cell::Flags::BuildRings );

  //DebugSession::addToTrace( cell->getNet("alu_out(3)") );
  //DebugSession::addToTrace( cell->getNet("imuxe.not_i(1)") );
  //DebugSession::addToTrace( cell->getNet("r(0)") );
  //DebugSession::addToTrace( cell->getNet("ialu.not_aux104") );
    DebugSession::addToTrace( cell->getNet("mips_r3000_1m_dp_shift32_rshift_se_muxoutput(159)") );

    engine->startMeasures();

    cmess1 << "  o  Building regular grid..." << endl;

    UpdateSession::open();
    engine->getSouthWestGCell()->doGrid();
    UpdateSession::close();

    engine->stopMeasures();
    engine->printMeasures( "Anbatic Grid" );
    engine->startMeasures();

    cmess1 << "  o  Running global routing..." << endl;

    UpdateSession::open();
    Dijkstra* dijkstra = new Dijkstra ( engine );
    dijkstra->setDistance( DigitalDistance( engine->getConfiguration()->getEdgeCostH()
                                          , engine->getConfiguration()->getEdgeCostK() ) );
    for ( Net* net : cell->getNets() ) {
      if (net->isPower() or net->isClock()) continue;
      dijkstra->load( net );
      dijkstra->run();
    }
    UpdateSession::close();

    engine->stopMeasures();
    engine->printMeasures( "Dijkstra" );

    const vector<Edge*>& ovEdges = engine->getOvEdges();
    if (not ovEdges.empty()) {
      size_t count = 0;

      cmess1 << "     - " << ovEdges.size() << " overloaded edges." << endl;
      cmess1 << "       " << ovEdges[0] << endl;
      NetSet nets;
      engine->getNetsFromEdge( ovEdges[0], nets );
      for ( Net* net : nets ) {
        cmess1 << "       [" << setw(2) << count++ << "] " << net << endl;
      }

      UpdateSession::open();
      Net* net = *nets.begin();
      dijkstra->load( net );
      dijkstra->ripup( ovEdges[0] );
      UpdateSession::close();
    }

    UpdateSession::open();
    delete dijkstra;
    UpdateSession::close();
  }
    

// -------------------------------------------------------------------
// Class  :  "Anabatic::GraphicAnabaticEngine".

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
    QPen      pen     = Graphics::getPen  ("Anabatic::GCell",widget->getDarkening()); 
    Box       bb      = gcell->getBoundingBox();
    QRect     pixelBb = widget->dbuToScreenRect(bb);

    painter.setPen  ( pen );
    painter.setBrush( Graphics::getBrush("Anabatic::GCell",widget->getDarkening()) );
    painter.drawRect( pixelBb );

    if (gcell->isFlat()) return;

    if (pixelBb.width() > 150) {
      QString text  = QString("id:%1").arg(gcell->getId());
      QFont   font  = Graphics::getFixedFont( QFont::Bold );
      painter.setFont(font);

      pen.setWidth( 1 );
      painter.setPen( pen );

      painter.save     ();
      painter.translate( widget->dbuToScreenPoint(bb.getCenter().getX(), bb.getCenter().getY()) );
      painter.drawRect (QRect( -75, -25, 150, 50 ));
      painter.drawText (QRect( -75, -25, 150, 50 )
                       , text
                       , QTextOption(Qt::AlignCenter)
                       );
      painter.restore  ();
    }
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
    static QFont font       = Graphics::getFixedFont( QFont::Bold ); 
    static int   fontHeight = QFontMetrics(font).height();

    const Edge* edge = static_cast<const Edge*>(go);

    if (edge) {
      Box          bb        = edge->getBoundingBox();
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

      QRect pixelBb = widget->dbuToScreenRect( bb, false);
      painter.setPen( Qt::NoPen );
      painter.setBrush( brush );
      painter.drawRect( pixelBb );

      if (fontHeight > ((edge->isHorizontal()) ? pixelBb.height() : pixelBb.width()) + 4) return; 

      QString text  = QString("%1/%2").arg(edge->getRealOccupancy()).arg(edge->getCapacity());
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

  //anabaticTest_1( engine );
  //anabaticTest_2( engine );
  //anabaticTest_3( engine );
  //anabaticTest_4( engine );
    anabaticTest_5( engine );

    if (_viewer) _viewer->emitCellChanged();
  }


  void  GraphicAnabaticEngine::_globalRoute ()
  {
    AnabaticEngine* engine = getForFramework( CreateEngine );
    engine->globalRoute();
  }


  void  GraphicAnabaticEngine::_detailRoute ()
  {
    AnabaticEngine* engine = getForFramework( CreateEngine );
    engine->loadGlobalRouting( EngineLoadGrByNet );
    engine->layerAssign( EngineNoNetLayerAssign );
  }


  void  GraphicAnabaticEngine::addToMenu ( CellViewer* viewer )
  {
    assert( _viewer == NULL );

    _viewer = viewer;

    if (_viewer->hasMenuAction("placeAndRoute.anabatic.globalRoute")) {
      cerr << Warning( "GraphicAnabaticEngine::addToMenu() - Anabatic engine already hooked in." ) << endl;
      return;
    }

    _viewer->addMenu  ( "placeAndRoute.anabatic", "Anabatic" );
    _viewer->addToMenu( "placeAndRoute.anabatic.runTest"
                      , "Anabatic - &Test Run"
                      , "Perform a test run of Anabatic on the design"
                      , std::bind(&GraphicAnabaticEngine::_runTest,this)
                      );
    _viewer->addToMenu( "placeAndRoute.anabatic.globalRoute"
                      , "Anabatic - &Global Route"
                      , "Global Route"
                      , std::bind(&GraphicAnabaticEngine::_globalRoute,this)
                      );
    _viewer->addToMenu( "placeAndRoute.anabatic.detailedRoute"
                      , "Anabatic - &Detailed Route"
                      , "Run the <b>Anabatic</b> detailed router"
                      , std::bind(&GraphicAnabaticEngine::_detailRoute,this)
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
