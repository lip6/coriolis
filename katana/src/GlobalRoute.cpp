// -*- mode: C++; explicit-buffer-name: "GlobalRoute.cpp<katana>" -*-
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


#include "hurricane/Cell.h"
#include "anabatic/Dijkstra.h"
#include "katana/RoutingPlane.h"
#include "katana/KatanaEngine.h"


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

    float distance = (float)source->getDistance()
                   + congestionCost * (float)edge->getDistance();
                   + edge->getHistoricCost();

    // Edge* sourceFrom = source->getFrom();
    // if (sourceFrom) {
    //   distance += ((sourceFrom->isHorizontal() xor edge->isHorizontal()) ? 3.0 : 0.0) * (float)Edge::unity;
    // }
    cdebug_log(112,0) << "cong:"   << congestion
                      << " ccost:" << congestionCost
                      << " digitalDistance:" << DbU::getValueString((DbU::Unit)distance) << endl;

    return (DbU::Unit)distance;
  }


  void  computeNextHCost ( Edge* edge, float edgeHInc )
  {
    float congestion = (float)edge->getRealOccupancy() / (float)edge->getCapacity();
    float hCost      = edge->getHistoricCost();

    float alpha = (congestion < 1.0) ? congestion : std::exp( std::log(8)*( congestion - 1 ) );

    edge->setHistoricCost( alpha * (hCost + ((congestion < 1.0) ? 0.0 : edgeHInc) ));

    cdebug_log(113,0) << edge << endl;
  }


}  // Anonymous namespace.


namespace Katana {

  using Hurricane::Error;
  using Hurricane::Timer;
  using Anabatic::EngineState;
  using Anabatic::Dijkstra;
  using Anabatic::NetData;


  void  KatanaEngine::setupGlobalGraph ( unsigned int mode )
  {
    Cell* cell = getCell();

    cell->flattenNets( Cell::Flags::BuildRings|Cell::Flags::WarnOnUnplacedInstances );
    cell->createRoutingPadRings( Cell::Flags::BuildRings );
  
    Super::chipPrep();

    startMeasures();

    if (getGCells().size() == 1) {
      cmess1 << "  o  Building regular grid..." << endl;
      getSouthWestGCell()->doGrid();
    } else {
      cmess1 << "  o  Reusing existing grid." << endl;
    }
    cmess1 << Dots::asInt("     - GCells"               ,getGCells().size()) << endl;

    stopMeasures();
    printMeasures( "Anabatic Grid" );

  //setupSpecialNets();
  //setupPreRouteds();
    setupNetDatas();

    for ( GCell* gcell : getGCells() ) {
      if (not gcell->isMatrix()) continue;

      for ( Edge* edge : gcell->getEdges(Flags::EastSide|Flags::NorthSide) ) {
        if (edge->isHorizontal()) edge->incCapacity( -getHTracksReservedLocal() );
        else                      edge->incCapacity( -getVTracksReservedLocal() );
      }
    }

    openSession();
    size_t maxDepth = getConfiguration()->getRoutingGauge()->getDepth();
    _routingPlanes.reserve( maxDepth );
    for ( size_t depth=0 ; depth < maxDepth ; depth++ ) {
      _routingPlanes.push_back( RoutingPlane::create( this, depth ) );
    }
    Session::close();
  }


  void  KatanaEngine::runGlobalRouter ()
  {
    if (getState() >= EngineState::EngineGlobalLoaded)
      throw Error ("KatanaEngine::runGlobalRouter(): Global routing already done or loaded.");

    openSession();

    annotateGlobalGraph();

    startMeasures();
    cmess1 << "  o  Running global routing..." << endl;

    float edgeHInc = getConfiguration()->getEdgeHInc();

    openSession();
    Dijkstra* dijkstra = new Dijkstra ( this );
    dijkstra->setDistance( DigitalDistance( getConfiguration()->getEdgeCostH()
                                          , getConfiguration()->getEdgeCostK() ) );

    size_t iteration = 0;
    size_t netCount  = 0;
    do {
      cmess2 << "     [" << setw(3) << iteration << "] nets:";

      netCount = 0;
      for ( NetData* netData : getNetOrdering() ) {
        if (netData->isGlobalRouted()) continue;

        dijkstra->load( netData->getNet() );
        dijkstra->run();
        ++netCount;
      }
      cmess2 << left << setw(6) << netCount;

      const vector<Edge*>& ovEdges = getOvEdges();
      cmess2 << " ovEdges:" << setw(4) << ovEdges.size();

      for ( Edge* edge : ovEdges ) computeNextHCost( edge, edgeHInc );

      netCount = 0;
      size_t iEdge = 0;
      while ( iEdge < ovEdges.size() ) {
        Edge* edge = ovEdges[iEdge];
        netCount += edge->ripup();

        if (ovEdges.empty()) break;
        if (ovEdges[iEdge] == edge) {
          cerr << Error( "AnabaticEngine::globalRoute(): Unable to ripup enough segments of edge:\n"
                         "        %s"
                       , getString(edge).c_str()
                       ) << endl;
          ++iEdge;
        }
      }

      dijkstra->setSearchAreaHalo( Session::getSliceHeight()*3 );

      cmess2 << " ripup:" << setw(4) << netCount << right;
      stopMeasures();
      cmess2 << " " << setw(10) << Timer::getStringTime  (getTimer().getCombTime())
             << " " << setw( 6) << Timer::getStringMemory(getTimer().getIncrease()) << endl;
      startMeasures();

      ++iteration;
    } while ( (netCount > 0) and (iteration < 5) );

    stopMeasures();
    printMeasures( "Dijkstra" );

    delete dijkstra;
    Session::close();

    setState( EngineState::EngineGlobalLoaded );
  }



}  // Katana namespace.
