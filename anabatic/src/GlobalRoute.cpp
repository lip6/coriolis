// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./GlobalRoute.cpp"                             |
// +-----------------------------------------------------------------+


#include <sstream>
#include <iostream>
#include "hurricane/Error.h"
#include "hurricane/RegularLayer.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/Cell.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/DebugSession.h"
#include "crlcore/RoutingGauge.h"
#include "anabatic/GCell.h"
#include "anabatic/Dijkstra.h"
#include "anabatic/AnabaticEngine.h"


namespace {

  using std::cerr;
  using std::endl;
  using std::setw;
  using std::left;
  using std::right;
  using Hurricane::DbU;
  using Anabatic::Edge;
  using Anabatic::Vertex;


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


}  // Anonymous namespace.


namespace Anabatic {

  using std::cerr;
  using std::cout;
  using std::endl;
  using std::setw;
  using std::ostringstream;
  using Hurricane::Error;
  using Hurricane::RegularLayer;
  using Hurricane::Component;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Hurricane::Cell;
  using Hurricane::UpdateSession;
  using Hurricane::DebugSession;
  using CRL::RoutingGauge;
  using CRL::RoutingLayerGauge;


// -------------------------------------------------------------------
// Class  :  "Anabatic::AnabaticEngine".
//
// Methods dedicateds to complete global routing.


  void  AnabaticEngine::globalRoute ()
  {
    Cell* cell = getCell();

    cell->flattenNets( Cell::Flags::BuildRings );
    cell->createRoutingPadRings( Cell::Flags::BuildRings );

  //DebugSession::addToTrace( cell->getNet("alu_out(3)") );
  //DebugSession::addToTrace( cell->getNet("imuxe.not_i(1)") );
  //DebugSession::addToTrace( cell->getNet("r(0)") );
  //DebugSession::addToTrace( cell->getNet("a_from_pads(0)") );
  //DebugSession::addToTrace( cell->getNet("ialu.not_aux104") );
  //DebugSession::addToTrace( cell->getNet("mips_r3000_1m_dp_shift32_rshift_se_muxoutput(126)") );

    startMeasures();

    UpdateSession::open();
    if (getGCells().size() == 1) {
      cmess1 << "  o  Building regular grid..." << endl;
      getSouthWestGCell()->doGrid();
    } else {
      cmess1 << "  o  Reusing existing grid." << endl;
    }
    cmess1 << Dots::asInt("     - GCells"               ,getGCells().size()) << endl;
    UpdateSession::close();

    stopMeasures();
    printMeasures( "Anabatic Grid" );

    Session::open( this );
    setupSpecialNets();
    setupPreRouteds ();
    Session::close();

    startMeasures();

    cmess1 << "  o  Running global routing..." << endl;

    NetSet  netsToRoute;
    for ( Net* net : cell->getNets() ) {
      NetRoutingState* state = getRoutingState( net );
      if (state) {
        if (state->isMixedPreRoute()) continue;
      }

      netsToRoute.insert( net );
    }

    UpdateSession::open();
    Dijkstra* dijkstra = new Dijkstra ( this );
    dijkstra->setDistance( DigitalDistance( getConfiguration()->getEdgeCostH()
                                          , getConfiguration()->getEdgeCostK() ) );

    size_t iteration = 0;
    while ( not netsToRoute.empty() and (iteration < 5) ) {
      cmess2 << "     [" << setw(3) << iteration << "] nets:"
             << left << setw(6) << netsToRoute.size() << right;

      for ( Net* net : netsToRoute ) {
        dijkstra->load( net );
        dijkstra->run();
      }

      netsToRoute.clear();
      const vector<Edge*>& ovEdges = getOvEdges();
      cmess2 << " ovEdges:" << ovEdges.size();

      while ( not ovEdges.empty() ) {
        Edge*   ovEdge = ovEdges[0];
        NetSet  netsToUnroute;
        getNetsFromEdge( ovEdge, netsToUnroute );

        for ( Net* net : netsToUnroute ) {
          dijkstra->load( net );
          dijkstra->ripup( ovEdge );
          netsToRoute.insert( net );
        }
      }

      cmess2 << " ripup:" << netsToRoute.size();

      stopMeasures();
      cmess2 << " " << setw(10) << Timer::getStringTime  (_timer.getCombTime())
             << " " << setw( 6) << Timer::getStringMemory(_timer.getIncrease()) << endl;
      startMeasures();

      ++iteration;
    }

    stopMeasures();
    printMeasures( "Dijkstra" );

    delete dijkstra;
    UpdateSession::close();

    _state = EngineGlobalLoaded;
  }


}  // Anabatic namespace.
