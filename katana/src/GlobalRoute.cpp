// -*- mode: C++; explicit-buffer-name: "GlobalRoute.cpp<katana>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2016-2022, All Rights Reserved
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
#include "hurricane/utilities/Dots.h"
#include "hurricane/Warning.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Cell.h"
#include "hurricane/viewer/CellViewer.h"
#include "crlcore/Utilities.h"
#include "crlcore/Histogram.h"
#include "anabatic/Dijkstra.h"
#include "etesian/BloatProperty.h"
#include "katana/Block.h"
#include "katana/RoutingPlane.h"
#include "katana/KatanaEngine.h"


namespace {

  using std::cout;
  using std::cerr;
  using std::endl;
  using std::dec;
  using std::setw;
  using std::setfill;
  using std::left;
  using std::right;
  using std::set;
  using Hurricane::DbU;
  using Hurricane::Interval;
  using Hurricane::DBo;
  using Hurricane::Net;
  using Hurricane::Segment;
  using Utilities::Dots;
  using Anabatic::Flags;
  using Anabatic::Edge;
  using Anabatic::GCell;
  using Anabatic::Vertex;
  using Anabatic::AnabaticEngine;
  using Etesian::BloatExtension;
  using namespace Katana;


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


  DbU::Unit  DigitalDistance::operator() ( const Vertex* source, const Vertex* target, const Edge* edge ) const
  {
    if (source->getGCell()->isStdCellRow() and target->getGCell()->isStdCellRow())
      return Vertex::unreachable;

    cdebug_log(112,0) << "DigitalDistance::operator(): " << edge
                      << " isGostraight():" << source->getGCell()->isGoStraight() << endl;
    if (    source->getGCell()->isGoStraight()
       and  source->getFrom()
       and (source->getFrom()->isHorizontal() xor edge->isHorizontal())) 
      return Vertex::unreachable;
    cdebug_log(112,0) << "Not a go straight" << endl;

    if (edge->getCapacity() <= 0) {
      if (target->getGCell()->isStdCellRow()
         and target->hasValidStamp() and (target->getConnexId() >= 0) )
        return 0;

      if (source->getGCell()->isStdCellRow()
         and source->hasValidStamp() and (source->getConnexId() >= 0) )
        return 0;
      
      cdebug_log(112,0) << "Negative or null edge capacity: " << edge->getCapacity() << endl;
      return Vertex::unreachable;
    }

    cdebug_log(112,0) << "Computing distance" << endl;
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
                    //<< edge->getHistoricCost() << ") * "
                      << historicCost << ") * "
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
  

  void  selectNets ( KatanaEngine* katana, set<const Net*,Net::CompareByName>& nets )
  {
    if (katana->getViewer()) {
      cmess2 << "  o  Selecting failed nets (slow)." << endl;

      Dots  dots ( cmess2, "     ", 80, 100 );
      if (not cmess2.enabled()) dots.disable();

      katana->getViewer()->setShowSelection( false );
      katana->getViewer()->setCumulativeSelection( true );

      for ( const Net* net : nets ) {
        Occurrence netOcc ( net );
        katana->getViewer()->select( netOcc );
        dots.dot();
      }

      dots.finish( Dots::Reset );
      katana->getViewer()->setShowSelection( true );
    }
  }


  void  selectSegments ( KatanaEngine* katana, set<Segment*,DBo::CompareById>& segments )
  {
    if (katana->getViewer()) {
      cmess2 << "  o  Selecting overflowed edges (slow)." << endl;

      Dots  dots ( cmess2, "     ", 80, 100 );
      if (not cmess2.enabled()) dots.disable();

      katana->getViewer()->setShowSelection( false );
      katana->getViewer()->setCumulativeSelection( true );

    // for ( const Net* net : nets ) {
    //   Occurrence netOcc ( net );
    //   getViewer()->select( netOcc );
    // }

      for ( const Segment* segment : segments ) {
        Occurrence occurrence ( segment );
        katana->getViewer()->select( occurrence );

        dots.dot();
      }

      dots.finish( Dots::Reset );
      katana->getViewer()->setShowSelection( true );
    }
  }
  

  void  selectOverloadedGCells ( KatanaEngine* katana )
  {
    CellViewer* viewer = katana->getViewer();
    
    if (viewer) {
      cmess2 << "  o  Selecting overloaded GCells (slow)." << endl;

      Dots  dots ( cmess2, "     ", 80, 100 );
      if (not cmess2.enabled()) dots.disable();

      viewer->setShowSelection( false );
      viewer->setCumulativeSelection( true );

      for ( GCell* gcell : katana->getGCells() ) {
        Edge* eastEdge   = gcell->getEastEdge ();
        Edge* northEdge  = gcell->getNorthEdge();
        bool  overloaded = false;

        if (eastEdge) {
          if (eastEdge->getRealOccupancy() > eastEdge->getCapacity()) {
            overloaded = true;
          }
          // else if (eastEdge->getHistoricCost() > 3.0) {
          //   overloaded = true;
          // }
        }

        if (northEdge) {
          if (northEdge->getRealOccupancy() > northEdge->getCapacity()) {
            overloaded = true;
          }
          // else if (northEdge->getHistoricCost() > 3.0) {
          //   overloaded = true;
          // }
        }

        if (overloaded) {
          Occurrence gcellOcc ( gcell );
          viewer->select( gcellOcc );
          dots.dot();
        }
      }

      dots.finish( Dots::Reset );
      viewer->setShowSelection( true );
    }
  }
  

  void  selectOverloadedEdges ( KatanaEngine* katana )
  {
    CellViewer* viewer = katana->getViewer();
    
    if (viewer) {
      cmess2 << "  o  Selecting overloaded Edges (slow)." << endl;

      Dots  dots ( cmess2, "     ", 80, 100 );
      if (not cmess2.enabled()) dots.disable();

      viewer->setShowSelection( false );
      viewer->setCumulativeSelection( true );

      for ( GCell* gcell : katana->getGCells() ) {
        for ( Edge* edge : gcell->getEdges( Flags::NorthSide|Flags::EastSide ) ) {
          if (edge->getRealOccupancy() > edge->getCapacity()) {
            Occurrence edgeOcc ( edge );
            viewer->select( edgeOcc );
            dots.dot();
          }
        }
      }

      dots.finish( Dots::Reset );
      viewer->setShowSelection( true );
    }
  }
  

  void  selectBloatedInstances ( KatanaEngine* katana )
  {
    CellViewer* viewer = katana->getViewer();
    
    if (viewer) {
      cmess2 << "  o  Selecting bloated instances (slow)." << endl;

      Dots  dots ( cmess2, "     ", 80, 100 );
      if (not cmess2.enabled()) dots.disable();

      viewer->setShowSelection( false );
      viewer->setCumulativeSelection( true );

      for( Occurrence occurrence : katana->getCell()->getTerminalNetlistInstanceOccurrences() ) {
        if (BloatExtension::get(occurrence)) {
          viewer->select( occurrence );

          dots.dot();
        }
      }

      dots.finish( Dots::Reset );
      viewer->setShowSelection( true );
    }
  }
  

}  // Anonymous namespace.


namespace Katana {

  using Utilities::Dots;
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
  using CRL::Histogram;
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

    if (isChannelStyle()) createChannels();

    if (getGCells().size() == 1) {
      cmess1 << "  o  Building regular grid..." << endl;
      getSouthWestGCell()->doGrid();
    } else {
      cmess1 << "  o  Reusing existing grid." << endl;
    }
    cmess1 << ::Dots::asInt("     - GCells"               ,getGCells().size()) << endl;

    stopMeasures();
    printMeasures( "Anabatic Grid" );

    setupNetDatas();
    computeEdgeCapacities( getHTracksReservedLocal()
                         , getVTracksReservedLocal()
                         , getTermSatThreshold()
                         , getTermSatReservedLocal() 
                         );

#if UNIFORM_EDGE_CAPACITY
    for ( GCell* gcell : getGCells() ) {
      if (not gcell->isMatrix()) continue;

      for ( Edge* edge : gcell->getEdges(Flags::EastSide|Flags::NorthSide) ) {
        if (edge->isHorizontal()) edge->reserveCapacity( getHTracksReservedLocal() );
        else                      edge->reserveCapacity( getVTracksReservedLocal() );
      }
    }
#endif
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
        { int      accuracy = 3;
          int64_t* xs       = new int64_t [targets.size()];
          int64_t* ys       = new int64_t [targets.size()];

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


  void  KatanaEngine::runGlobalRouter ( Flags flags )
  {
    if (getState() >= EngineState::EngineGlobalLoaded)
      throw Error ("KatanaEngine::runGlobalRouter(): Global routing already done or loaded.");

    if (flags & Flags::ShowBloatedInstances) selectBloatedInstances( this );
    Breakpoint::stop( 100, "Bloated cells from previous placement iteration." );

    startMeasures();
    cmess1 << "  o  Running global routing." << endl;

    openSession();

    annotateGlobalGraph();

    float   edgeHInc         = getConfiguration()->getEdgeHInc();
    size_t  globalIterations = getConfiguration()->getGlobalIterations();;

    openSession();
    Dijkstra*           dijkstra = new Dijkstra ( this );
    DigitalDistance*    distance =
      dijkstra->setDistance( DigitalDistance( getConfiguration()->getEdgeCostH()
                                            , getConfiguration()->getEdgeCostK()
                                            , getConfiguration()->getEdgeHScaling() ));
    const vector<Edge*>& ovEdges = getOvEdges();

    if (isChannelStyle())
      dijkstra->setSearchAreaHalo( Session::getSliceHeight()*10 );
    else
      dijkstra->setSearchAreaHalo( Session::getSliceHeight()*getSearchHalo() );

    bool     globalEstimated = false;
    size_t   iteration       = 0;
    size_t   netCount        = 0;
    uint64_t edgeOverflowWL  = 0;
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
        netData->setGlobalRouted( true );
        ++netCount;

        // if (netData->getNet()->getName() == Name("mips_r3000_1m_dp_shift32_rshift_se_msb")) {
        //   Session::close();
        //   Breakpoint::stop( 1, "After global routing of \"mips_r3000_1m_dp_shift32_rshift_se_msb\"." );
        //   openSession();
        // }

        if (useGlobalEstimate()) {
        // Triggers the global routing when we reach nets of less than 11 terminals.
        // High degree nets are routed straight (without taking account the smalls).
        // See the SparsityOrder comparison function.
          if ( (netData->getRpCount() < 11) and not globalEstimated ) {
            for ( NetData* netData2 : getNetOrdering() ) {
              if (netData2->isGlobalRouted() or netData2->isExcluded()) continue;

              updateEstimateDensity( netData2, 1.0 );
              netData2->setGlobalEstimated( true );
            }
            globalEstimated = true;
          }
        }
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

      edgeOverflowWL = 0;
      netCount       = 0;
      if (iteration < globalIterations - 1) {
        for ( Edge* edge : ovEdges ) {
          edgeOverflowWL += edge->getRealOccupancy() - edge->getCapacity();
        }
        
        size_t iEdge = 0;
        while ( iEdge < ovEdges.size() ) {
          Edge* edge  = ovEdges[iEdge];
          netCount   += edge->ripup();

          if (iEdge >= ovEdges.size()) break;
          if (ovEdges[iEdge] == edge) {
            cerr << Error( "AnabaticEngine::globalRoute(): Unable to ripup enough segments of edge:\n"
                         "        %s"
                         , getString(edge).c_str()
                         ) << endl;
            ++iEdge;
          }
        }

        dijkstra->setSearchAreaHalo( (getSearchHalo() + 3*(iteration/3)) * Session::getSliceHeight() );
      }

      cmess2 << " ovE:" << setw(4) << overflow << " ovWL:" << setw(5) << edgeOverflowWL;

      cmess2 << " ripup:" << setw(4) << netCount << right;
      suspendMeasures();
      cmess2 << " " << setw(7) << Timer::getStringMemory(getTimer().getIncrease())
             << " " << setw(6) << Timer::getStringTime  (getTimer().getCombTime()) << endl;
      resumeMeasures();

      ++iteration;
    } while ( (netCount > 0) and (iteration < globalIterations) );

    stopMeasures();
    printMeasures( "Dijkstra" );

    uint32_t hoverflow = 0;
    uint32_t voverflow = 0;
    if (not ovEdges.empty()) {
      Histogram  ovHistogram ( 0.0, 1.0, 1 );
      ovHistogram.setTitle ( "Overflowed", 0 );
      ovHistogram.setColor ( "green"     , 0 );
      ovHistogram.setIndent( "       "   , 0 );

      set<const Net*,Net::CompareByName>  nets;
      set<Segment*  ,DBo::CompareById  >  segments;

      cerr << "  o  Global routing did not complete." << endl;
      for ( size_t iEdge = 0 ; iEdge<ovEdges.size() ; ++iEdge ) {
        unsigned int edgeOverflow = ovEdges[iEdge]->getRealOccupancy() - ovEdges[iEdge]->getCapacity();
        ovHistogram.addSample( (float)edgeOverflow, 0 );
        if (ovEdges[iEdge]->isHorizontal()) hoverflow += edgeOverflow;
        else                                voverflow += edgeOverflow;

        if (edgeOverflow > 0) {
          const vector<Segment*> ovSegs = ovEdges[iEdge]->getSegments();
          for ( size_t iSeg=ovEdges[iEdge]->getCapacity() ; iSeg<ovSegs.size() ; ++iSeg ) {
            Net* net     = ovSegs[iSeg]->getNet();
            auto netData = getNetDatas().find( net->getId() );

            if (netData == getNetDatas().end()) continue;
            if ((*netData).second->getRpCount() > 20) {
              cmess2 << "     - Not showing " << net << " too much terminals ("
                     << (*netData).second->getRpCount() << ")." << endl;
              continue;
            }

            nets.insert( net );
          }
        }
      }

    //cerr << "  o  Conflicting nets:" << endl;
    //size_t count = 0;
    //for ( const Net* net : nets )
    //  cerr << "    " << dec << setw(4) << (++count) << "| " << net->getName() << endl;

      cmess2 << ::Dots::asUInt  ("     - Unsatisfied nets"          ,nets    .size()) << endl;
      cmess2 << ::Dots::asUInt  ("     - Unsatisfied segments"      ,segments.size()) << endl;
      cmess2 << ::Dots::asUInt  ("     - Overflowed edges"          ,ovEdges .size()) << endl;

      ostringstream result;
      float ratio = ((float)hoverflow / (float)(hoverflow+voverflow)) * 100.0;
      result.str( "" );
      result << setprecision(4) << ratio << "% [" << hoverflow << "]";
      cmess2 << ::Dots::asString( "     - H-overflow length", result.str() ) << endl;

      ratio = ((float)voverflow / (float)(hoverflow+voverflow)) * 100.0;
      result.str( "" );
      result << setprecision(4) << ratio << "% [" << voverflow << "]";
      cmess2 << ::Dots::asString( "     - V-overflow length", result.str() ) << endl;

      cmess2 << "  o  Overflowed edges Histogram." << endl;
      cmess2 << ovHistogram.toString(0) << endl;

      addMeasure<uint32_t>( "H-ovE", hoverflow, 12 );
      addMeasure<uint32_t>( "V-ovE", voverflow, 12 );

      if (not Session::isChannelStyle())
        _buildBloatProfile();

      if (flags & Flags::ShowFailedNets      ) selectNets            ( this, nets );
      if (flags & Flags::ShowFailedGSegments ) selectSegments        ( this, segments );
      if (flags & Flags::ShowOverloadedEdges ) selectOverloadedEdges ( this );
      if (flags & Flags::ShowOverloadedGCells) selectOverloadedGCells( this );
      if (flags & Flags::ShowBloatedInstances) selectBloatedInstances( this );
    }

    addMeasure<uint32_t>( "H-ovE", hoverflow, 12 );
    addMeasure<uint32_t>( "V-ovE", voverflow, 12 );

    if (getBlock(0)) {
      getBlock(0)->resizeChannels();
      _resizeMatrix();
    }

    delete dijkstra;

      for ( NetData* netData : getNetOrdering() ) {
        netData->setGlobalRouted(false);
      }

    Session::close();
    if (isChannelStyle()) {
      setupRoutingPlanes();
      setupPowerRails();
      protectRoutingPads();
    }

    setState( EngineState::EngineGlobalLoaded );
    setGlobalRoutingSuccess( ovEdges.empty() );

    // for( Occurrence occurrence : getCell()->getTerminalNetlistInstanceOccurrences() ) {
    //   if (occurrence.getEntity()->getId() == 25202) {
    //     cerr << "REFERENCE INSTANCE" << endl;
    //     cerr << occurrence << " " << occurrence.getPath().getTransformation() << endl;
    //   }
    // }
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
            //size_t   gslaves = 0;

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
