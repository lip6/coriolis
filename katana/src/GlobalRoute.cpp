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


#include "flute.h"
#include "hurricane/Warning.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/RoutingPad.h"
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
  using Hurricane::Interval;
  using Hurricane::Net;
  using Anabatic::Flags;
  using Anabatic::Edge;
  using Anabatic::GCell;
  using Anabatic::Vertex;
  using Anabatic::AnabaticEngine;


  class DigitalDistance {
    public:
      inline            DigitalDistance ( float h, float k, float hScaling );
      inline void       setNet          ( Net* );
             DbU::Unit  operator()      ( const Vertex* source ,const Vertex* target,const Edge* edge ) const;
    private:
    // For an explanation of h & k parameters, see:
    //     "KNIK, routeur global pour la plateforme Coriolis", p. 52.
      float  _h;
      float  _k;
      float  _hScaling;
      Net*   _net;
  };


  inline       DigitalDistance::DigitalDistance ( float h, float k, float hScaling ) : _h(h), _k(k), _hScaling(hScaling), _net(NULL) { }
  inline void  DigitalDistance::setNet          ( Net* net ) { _net = net; }


  DbU::Unit  DigitalDistance::operator() ( const Vertex* source ,const Vertex* target,const Edge* edge ) const
  {
    if (source->getGCell()->isStdCellRow() and target->getGCell()->isStdCellRow())
      return Vertex::unreachable;

    if (edge->getCapacity() <= 0) {
      if (target->getGCell()->isStdCellRow()
         and target->hasValidStamp() and (target->getConnexId() >= 0) )
        return 0;

      if (source->getGCell()->isStdCellRow()
         and source->hasValidStamp() and (source->getConnexId() >= 0) )
        return 0;
      
      return Vertex::unreachable;
    }

    float congestionCost = 1.0;
    float congestion     = ((float)edge->getRealOccupancy() + edge->getEstimateOccupancy())
                         /  (float)edge->getCapacity();

    if (not source->getGCell()->isChannelRow() or not target->getGCell()->isChannelRow())
      congestionCost += _h / (1.0 + std::exp(_k * (congestion - 1.0)));

    float viaCost = 0.0;
    if (    source->getFrom()
       and (source->getFrom()->isHorizontal() xor edge->isHorizontal())
       /*and not source->hasGContact(_net)*/ ) {
      viaCost += 2.5;
    }

    
    float realCongestion = (float)edge->getRealOccupancy() /  (float)edge->getCapacity();
    float historicCost   = edge->getHistoricCost();
    if (realCongestion <= 1.0)
      historicCost += edge->getEstimateOccupancy() * realCongestion;
    else
      historicCost += edge->getEstimateOccupancy() * exp( log(8) * (realCongestion - 1.0) );
  //const_cast<Edge*>(edge)->setHistoricCost( historicCost );

    float edgeDistance = (float)edge->getDistance();
    if (  (source->getGCell()->isChannelRow() and target->getGCell()->isStdCellRow())
       or (source->getGCell()->isStdCellRow() and target->getGCell()->isChannelRow()) )
      edgeDistance *= 10.0;

    float hvScaling = (edge->isHorizontal()) ? _hScaling : 1.0 ;
    float distance
      = (float)source->getDistance()
      + (congestionCost + viaCost + historicCost) * edgeDistance * hvScaling;

    cdebug_log(112,0) << "distance:"
                      << DbU::getValueString(source->getDistance()) << " + ("
                      << congestionCost << " + "
                      << viaCost << " + "
                      << edge->getHistoricCost() << ") * "
                      << DbU::getValueString(edgeDistance) << " * "
                      << hvScaling
                      << endl;
    // Edge* sourceFrom = source->getFrom();
    // if (sourceFrom) {
    //   distance += ((sourceFrom->isHorizontal() xor edge->isHorizontal()) ? 3.0 : 0.0) * (float)Edge::unity;
    // }
    // cdebug_log(112,0) << "cong:"   << congestion
    //                   << " ccost:" << congestionCost
    //                   << " digitalDistance:" << DbU::getValueString((DbU::Unit)distance) << endl;

    return (distance >= (float)DbU::Max) ? Vertex::unreachable : (DbU::Unit)distance;
  }


  void  computeNextHCost ( Edge* edge, float edgeHInc )
  {
    float congestion = (float)edge->getRealOccupancy() / (float)edge->getCapacity();
    float hCost      = edge->getHistoricCost();
    float alpha      = (congestion < 1.0) ? congestion : std::exp( std::log(8)*( congestion - 1 ) );

    edge->setHistoricCost( alpha * (hCost + ((congestion < 1.0) ? 0.0 : edgeHInc) ));

    cdebug_log(113,0) << edge << endl;
  }


  void  updateEstimateDensityOfPath ( AnabaticEngine* anabatic, GCell* source, GCell* target, double weight )
  {
    Interval hoverlap     = source->getHSide().getIntersection( target->getHSide() );
    Interval voverlap     = source->getVSide().getIntersection( target->getVSide() );
    bool     straightLine = not (hoverlap.isEmpty() and voverlap.isEmpty());
    double   cost         = ((straightLine) ? 1.0 : 0.5) * weight;

    for ( Edge* edge : anabatic->getEdgesUnderPath(source,target,Flags::NorthPath) ) {
      edge->incEstimateOccupancy( cost );
    }

    if (not straightLine) {
      for ( Edge* edge : anabatic->getEdgesUnderPath(source,target,Flags::NoFlags) ) {
        edge->incEstimateOccupancy( cost );
      }
    }
  }
  

}  // Anonymous namespace.


namespace Katana {

  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Breakpoint;
  using Hurricane::Timer;
  using Hurricane::Occurrence;
  using Hurricane::Transformation;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Hurricane::Contact;
  using Hurricane::RoutingPad;
  using Hurricane::RoutingPad;
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
        if (edge->isHorizontal()) edge->reserveCapacity( getHTracksReservedLocal() );
        else                      edge->reserveCapacity( getVTracksReservedLocal() );
      }
    }
  }


  void  KatanaEngine::updateEstimateDensity ( NetData* netData, double weight )
  {
    // if (   (netData->getNet()->getName() != "ialu.inv_x2_sig")
    //    and (netData->getNet()->getName() != "ra(0)")
    //    and (netData->getNet()->getName() != "iram.oa2a22_x2_11_sig")) return;

    vector<GCell*> targets;
    for ( Component* component : netData->getNet()->getComponents() ) {
      RoutingPad* rp = dynamic_cast<RoutingPad*>( component );
      if (rp) {
        if (not getConfiguration()->selectRpComponent(rp))
          cerr << Warning( "KatanaEngine::updateEstimateDensity(): %s has no components on grid.", getString(rp).c_str() ) << endl;

        Point  center = rp->getBoundingBox().getCenter();
        GCell* gcell  = getGCellUnder( center );

        targets.push_back( gcell );
      }
    }

    switch ( targets.size() ) {
      case 0:
      case 1:
        return;
      case 2:
        updateEstimateDensityOfPath( this, targets[0], targets[1], weight );
        return;
      default:
        { int  accuracy = 3;
          int* xs       = new int [targets.size()];
          int* ys       = new int [targets.size()];

          for ( size_t itarget=0 ; itarget<targets.size() ; ++itarget ) {
            Point center =  targets[itarget]->getCenter();
            xs[ itarget ] = center.getX();
            ys[ itarget ] = center.getY();
          }

          Flute::Tree tree = Flute::flute( targets.size(), xs, ys, accuracy );

          for ( size_t i=0 ; (int)i < 2*tree.deg - 2 ; ++i ) {
            size_t j = tree.branch[i].n;
            GCell* source = getGCellUnder( tree.branch[i].x, tree.branch[i].y );
            GCell* target = getGCellUnder( tree.branch[j].x, tree.branch[j].y );

            if (not source) {
              cerr << Error( "KatanaEngine::updateEstimateDensity(): No GCell under (%s,%s) for %s."
                           , DbU::getValueString((DbU::Unit)tree.branch[i].x).c_str()
                           , DbU::getValueString((DbU::Unit)tree.branch[i].y).c_str()
                           , getString(netData->getNet()).c_str()
                           ) << endl;
              continue;
            }
            if (not target) {
              cerr << Error( "KatanaEngine::updateEstimateDensity(): No GCell under (%s,%s) for %s."
                           , DbU::getValueString((DbU::Unit)tree.branch[j].x).c_str()
                           , DbU::getValueString((DbU::Unit)tree.branch[j].y).c_str()
                           , getString(netData->getNet()).c_str()
                           ) << endl;
              continue;
            }

            updateEstimateDensityOfPath( this, source, target, weight );
          }
        }
        return;
    }
  }


  void  KatanaEngine::runGlobalRouter ()
  {
    if (getState() >= EngineState::EngineGlobalLoaded)
      throw Error ("KatanaEngine::runGlobalRouter(): Global routing already done or loaded.");

    openSession();

    annotateGlobalGraph();
    for ( NetData* netData : getNetOrdering() ) {
      if (netData->isGlobalRouted() or netData->isExcluded()) continue;

      updateEstimateDensity( netData, 1.0 );
      netData->setGlobalEstimated( true );
    }

    // Session::close();
    // Breakpoint::stop( 1, "After global routing estimation." );
    // openSession();

    startMeasures();
    cmess1 << "  o  Running global routing." << endl;

    float   edgeHInc         = getConfiguration()->getEdgeHInc();
    size_t  globalIterations = getConfiguration()->getGlobalIterations();;

    openSession();
    Dijkstra*           dijkstra = new Dijkstra ( this );
    DigitalDistance*    distance =
      dijkstra->setDistance( DigitalDistance( getConfiguration()->getEdgeCostH()
                                            , getConfiguration()->getEdgeCostK()
                                            , getConfiguration()->getEdgeHScaling() ));
    const vector<Edge*>& ovEdges = getOvEdges();

    if (isChannelMode())
      dijkstra->setSearchAreaHalo( Session::getSliceHeight()*10 );

    size_t iteration = 0;
    size_t netCount  = 0;
    do {
      cmess2 << "     [" << setfill(' ') << setw(3) << iteration << "] nets:";

      long   wireLength = 0;
      long   viaCount   = 0;

      netCount = 0;
      for ( NetData* netData : getNetOrdering() ) {
        if (netData->isGlobalRouted() or netData->isExcluded()) continue;
        if (netData->isGlobalEstimated()) {
          updateEstimateDensity( netData, -1.0 );
          netData->setGlobalEstimated( false );
        }

        distance->setNet( netData->getNet() );
        dijkstra->load( netData->getNet() );
        dijkstra->run();
        ++netCount;
      }
      cmess2 << left << setw(6) << netCount;

      computeGlobalWireLength( wireLength, viaCount );
      cmess2 <<  " nWL:" << setw(7) << (wireLength /*+ viaCount*3*/);
      cmess2 << " VIAs:" << setw(7) << viaCount;

      size_t overflow = ovEdges.size();
      for ( Edge* edge : ovEdges ) {
        edge->setHistoricCost( edge->getHistoricCost() + edgeHInc );
      //computeNextHCost( edge, edgeHInc );
      }

    //Session::close();
    //Breakpoint::stop( 1, "Before riping up overflowed edges." );
    //openSession();

      netCount = 0;
      if (iteration < globalIterations - 1) {
        size_t iEdge = 0;
        while ( iEdge < ovEdges.size() ) {
          Edge* edge = ovEdges[iEdge];
          netCount += edge->ripup();

          if (iEdge >= ovEdges.size()) break;
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

      cmess2 << " ovE:" << setw(4) << overflow;

      cmess2 << " ripup:" << setw(4) << netCount << right;
      suspendMeasures();
      cmess2 << " " << setw(5) << Timer::getStringTime  (getTimer().getCombTime())
             << " " << setw(6) << Timer::getStringMemory(getTimer().getIncrease()) << endl;
      resumeMeasures();

      ++iteration;
    } while ( (netCount > 0) and (iteration < globalIterations) );

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


  void  KatanaEngine::computeGlobalWireLength ( long& wireLength, long& viaCount )
  {
    const Layer* hLayer = getConfiguration()->getGHorizontalLayer();
    const Layer* vLayer = getConfiguration()->getGVerticalLayer();
    const Layer* cLayer = getConfiguration()->getGContactLayer();

    DbU::Unit hWireLength = 0;
    DbU::Unit vWireLength = 0;

    for ( NetData* netData : getNetOrdering() ) {
      if (not netData->isGlobalRouted()) continue;

      for ( Component* component : netData->getNet()->getComponents() ) {
        if (component->getLayer() == hLayer) {
          hWireLength += static_cast<Horizontal*>( component )->getLength();
        } else {
          if (component->getLayer() == vLayer) {
            vWireLength += static_cast<Vertical*>( component )->getLength();
          } else {
            if (component->getLayer() == cLayer) {
              Contact* contact = static_cast<Contact*>( component );
              size_t   gslaves = 0;

              for ( Component* slave : contact->getSlaveComponents().getSubSet<Segment*>() ) {
                if (slave->getLayer() == vLayer) { ++viaCount; break; }
                // if (slave->getLayer() == hLayer) {
                //   ++gslaves;
                //   if (gslaves >= 2) { ++viaCount; break; }
                // }
              }
            }
          }
        }
      }
    }

    wireLength  = hWireLength / GCell::getMatrixHSide();
    wireLength += vWireLength / GCell::getMatrixVSide();
  }


}  // Katana namespace.
