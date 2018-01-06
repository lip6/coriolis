// -*- mode: C++; explicit-buffer-name: "GlobalRoute.cpp<katana>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./GlobalRoute.cpp"                             |
// +-----------------------------------------------------------------+


#include "hurricane/Warning.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/Cell.h"
#include "anabatic/Dijkstra.h"
#include "katana/Block.h"
#include "katana/RoutingPlane.h"
#include "katana/KatanaEngine.h"


namespace {

  using std::cerr;
  using std::endl;
  using std::dec;
  using std::setw;
  using std::setfill;
  using std::left;
  using std::right;
  using Hurricane::DbU;
  using Hurricane::DbU;
  using Hurricane::Net;
  using Anabatic::Edge;
  using Anabatic::Vertex;


  class DigitalDistance {
    public:
      inline            DigitalDistance ( float h, float k );
      inline void       setNet          ( Net* );
             DbU::Unit  operator()      ( const Vertex* source ,const Vertex* target,const Edge* edge ) const;
    private:
    // For an explanation of h & k parameters, see:
    //     "KNIK, routeur global pour la plateforme Coriolis", p. 52.
      float  _h;
      float  _k;
      Net*   _net;
  };


  inline       DigitalDistance::DigitalDistance ( float h, float k ) : _h(h), _k(k), _net(NULL) { }
  inline void  DigitalDistance::setNet          ( Net* net ) { _net = net; }


  DbU::Unit  DigitalDistance::operator() ( const Vertex* source ,const Vertex* target,const Edge* edge ) const
  {
    if (edge->getCapacity() <= 0) return Vertex::unreachable;

    if (source->getGCell()->isStdCellRow() and target->getGCell()->isStdCellRow())
      return Vertex::unreachable;

    float congestionCost = 1.0;
    float congestion     = (float)edge->getRealOccupancy() / (float)edge->getCapacity();

    if (not source->getGCell()->isChannelRow() or not target->getGCell()->isChannelRow())
      congestionCost += _h / (1.0 + std::exp(_k * (congestion - 1.0)));

    float viaCost = 0.0;
    if (    source->getFrom()
       and (source->getFrom()->isHorizontal() xor edge->isHorizontal())
       and not source->hasGContact(_net) ) {
      viaCost += 2.5;
    }

    float hvDistort = (edge->isHorizontal()) ? 1.0 : 1.0 ;
    float distance  = (float)source->getDistance()
                    + (congestionCost + viaCost) * (float)edge->getDistance() * hvDistort
                    + edge->getHistoricCost();

    // Edge* sourceFrom = source->getFrom();
    // if (sourceFrom) {
    //   distance += ((sourceFrom->isHorizontal() xor edge->isHorizontal()) ? 3.0 : 0.0) * (float)Edge::unity;
    // }
    // cdebug_log(112,0) << "cong:"   << congestion
    //                   << " ccost:" << congestionCost
    //                   << " digitalDistance:" << DbU::getValueString((DbU::Unit)distance) << endl;

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
  using Hurricane::Warning;
  using Hurricane::Breakpoint;
  using Hurricane::Timer;
  using Hurricane::Occurrence;
  using Hurricane::Transformation;
  using Hurricane::Instance;
  using Anabatic::EngineState;
  using Anabatic::Dijkstra;
  using Anabatic::NetData;


  void  KatanaEngine::createChannels ()
  {
    Cell* core = getCell();
    if (isChip())
      core = getChipTools().getCore()->getMasterCell();

    Block* block = new Block( this, core );
    block->createChannels();

    _resizeMatrix();
  }


  void  KatanaEngine::setupGlobalGraph ( uint32_t mode )
  {
    Cell* cell = getCell();

    cell->flattenNets( Cell::Flags::BuildRings|Cell::Flags::WarnOnUnplacedInstances );
    cell->createRoutingPadRings( Cell::Flags::BuildRings );

    startMeasures();

    if (isChannelMode()) createChannels();

    if (getGCells().size() == 1) {
      cmess1 << "  o  Building regular grid..." << endl;
      getSouthWestGCell()->doGrid();
    } else {
      cmess1 << "  o  Reusing existing grid." << endl;
    }
    cmess1 << Dots::asInt("     - GCells"               ,getGCells().size()) << endl;

    stopMeasures();
    printMeasures( "Anabatic Grid" );

    setupNetDatas();

    for ( GCell* gcell : getGCells() ) {
      if (not gcell->isMatrix()) continue;

      for ( Edge* edge : gcell->getEdges(Flags::EastSide|Flags::NorthSide) ) {
        if (edge->isHorizontal()) edge->incCapacity( -getHTracksReservedLocal() );
        else                      edge->incCapacity( -getVTracksReservedLocal() );
      }
    }
  }


  void  KatanaEngine::runGlobalRouter ()
  {
    if (getState() >= EngineState::EngineGlobalLoaded)
      throw Error ("KatanaEngine::runGlobalRouter(): Global routing already done or loaded.");

    openSession();

    annotateGlobalGraph();

    startMeasures();
    cmess1 << "  o  Running global routing." << endl;

    float edgeHInc = getConfiguration()->getEdgeHInc();

    openSession();
    Dijkstra*           dijkstra = new Dijkstra ( this );
    DigitalDistance*    distance =
      dijkstra->setDistance( DigitalDistance( getConfiguration()->getEdgeCostH()
                                            , getConfiguration()->getEdgeCostK() ));
    const vector<Edge*>& ovEdges = getOvEdges();

    if (isChannelMode())
      dijkstra->setSearchAreaHalo( Session::getSliceHeight()*2 );

    size_t iteration = 0;
    size_t netCount  = 0;
    do {
      cmess2 << "     [" << setfill(' ') << setw(3) << iteration << "] nets:";

      netCount = 0;
      for ( NetData* netData : getNetOrdering() ) {
        if (netData->isGlobalRouted()) continue;

        distance->setNet( netData->getNet() );
        dijkstra->load( netData->getNet() );
        dijkstra->run();
        ++netCount;
      }
      cmess2 << left << setw(6) << netCount;
      cmess2 << " ovEdges:" << setw(4) << ovEdges.size();

      for ( Edge* edge : ovEdges ) computeNextHCost( edge, edgeHInc );
      // Session::close();
      // Breakpoint::stop( 1, "Before riping up overflowed edges." );
      // openSession();

      netCount = 0;
      if (iteration < 10 - 1) {
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
      }

      cmess2 << " ripup:" << setw(4) << netCount << right;
      suspendMeasures();
      cmess2 << " " << setw(10) << Timer::getStringTime  (getTimer().getCombTime())
             << " " << setw( 6) << Timer::getStringMemory(getTimer().getIncrease()) << endl;
      resumeMeasures();

      ++iteration;
    } while ( (netCount > 0) and (iteration < 10) );

    stopMeasures();
    printMeasures( "Dijkstra" );

    if (not ovEdges.empty()) {
      set< const Net*, Net::CompareByName > nets;

      cerr << "  o  Global routing did not complete, overflowed edges:" << endl;
      for ( size_t iEdge = 0 ; iEdge<ovEdges.size() ; ++iEdge ) {
        cerr << "    " << dec << setw(4) << (iEdge+1) << "| " << ovEdges[iEdge] << endl;
        for ( Segment* segment : ovEdges[iEdge]->getSegments() )
          nets.insert( segment->getNet() );
      }

      cerr << "  o  Conflicting nets:" << endl;
      size_t count = 0;
      for ( const Net* net : nets )
        cerr << "    " << dec << setw(4) << (++count) << "| " << net->getName() << endl;
    }

    if (getBlock(0)) {
      getBlock(0)->resizeChannels();
      _resizeMatrix();
    }

    delete dijkstra;

    Session::close();
    if (isChannelMode()) {
      setupRoutingPlanes();
      setupPowerRails();
      protectRoutingPads();
    }

    setState( EngineState::EngineGlobalLoaded );
  }



}  // Katana namespace.
