// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2021-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |         A n a b a t i c  -  Routing Toolbox                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./AntennaProtect.cpp"                     |
// +-----------------------------------------------------------------+


#include <cstdlib>
#include <sstream>
#include <tuple>
#include "hurricane/Bug.h"
#include "hurricane/Warning.h"
#include "hurricane/DebugSession.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/Net.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/NetRoutingProperty.h"
#include "hurricane/Layer.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Pad.h"
#include "hurricane/Plug.h"
#include "hurricane/Instance.h"
#include "hurricane/Vertical.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Cell.h"
#include "crlcore/RoutingGauge.h"
#include "etesian/EtesianEngine.h"
#include "anabatic/AutoContactTerminal.h"
#include "anabatic/AutoSegment.h"
#include "anabatic/AnabaticEngine.h"


namespace {


  using namespace std;
  using namespace CRL;
  using namespace Hurricane;
  using namespace Anabatic;
  using Etesian::EtesianEngine;


  class CompareGCellByXMin {
    public:
      inline bool operator () ( const GCell* lhs, const GCell* rhs ) const;
  };

  
  inline bool CompareGCellByXMin::operator () ( const GCell* lhs, const GCell* rhs ) const
  { return lhs->getXMin() < rhs->getXMin(); }


// -----------------------------------------------------------------
// Class : "::DiodeCluster".

  class DiodeCluster {
    public:
      static const uint32_t  IsDriver;
      static const uint32_t  IsSink;
      static const uint32_t  HasDiode;
      static const uint32_t  IsSegSource;
      static       string    toStr ( uint32_t );
    public:
      typedef tuple<uint32_t,Segment*>                   RPInfos;
      typedef map<RoutingPad*,RPInfos,DBo::CompareById>  RoutingPadInfos;
      typedef set<Horizontal*,DBo::CompareById>          HorizontalSet;
      typedef set<Vertical*  ,DBo::CompareById>          VerticalSet;
      typedef map< DbU::Unit, vector<GCell*> >           GCellArea;
    public:
                                      DiodeCluster   ( RoutingPad*, AnabaticEngine* );
      inline       bool               hasRp          ( RoutingPad* ) const;
                   bool               hasGCell       ( GCell* ) const;
      inline       Net*               getTopNet      () const;
      inline       RoutingPad*        getRefRp       () const;
      inline const RoutingPadInfos&   getRoutingPads () const;
      inline const vector<Instance*>& getDiodes      () const;
      inline       DbU::Unit          getWL          () const;
                   bool               needsDiode     () const;
                   Box                getBoundingBox () const;
                   void               merge          ( GCell* );
                   void               merge          ( RoutingPad* );
                   void               merge          ( Segment* );
                   void               mergeHalo      ( Segment* );
                   void               forceDiodeOn   ( RoutingPad*, Segment*, Flags );
                   Instance*          createDiode    ( Etesian::Area*, GCell*, Flags side, uint32_t distance=1 );
             const vector<Instance*>& createDiodes   ( Etesian::Area* );
    private:                        
                   void               _consolidate   ();
    private:
      AnabaticEngine*   _anabatic;
      bool              _sortArea;
      DbU::Unit         _WL;
      RoutingPadInfos   _routingPads;
      HorizontalSet     _horizontals;
      VerticalSet       _verticals;
      HorizontalSet     _horizontalHalo;
      VerticalSet       _verticalHalo;
      GCellArea         _area;
      vector<Instance*> _diodes;
  };

  
  const uint32_t  DiodeCluster::IsDriver    = (1 << 0);
  const uint32_t  DiodeCluster::IsSink      = (1 << 1);
  const uint32_t  DiodeCluster::HasDiode    = (1 << 2);
  const uint32_t  DiodeCluster::IsSegSource = (1 << 3);


  string DiodeCluster::toStr ( uint32_t flags )
  {
    string s;
    s += (flags & IsDriver   ) ? 'd' : '-';
    s += (flags & IsSink     ) ? 's' : '-';
    s += (flags & HasDiode   ) ? 'D' : '-';
    s += (flags & IsSegSource) ? 'S' : '-';
    return s;
  }


  DiodeCluster::DiodeCluster ( RoutingPad* rp, AnabaticEngine* anabatic )
    : _anabatic(anabatic)
    , _sortArea(true)
    , _WL(0)
    , _routingPads()
    , _horizontals()
    , _verticals()
    , _horizontalHalo()
    , _verticalHalo()
    , _area()
    , _diodes()
  {
    merge( rp );
  }


  inline       Net*                           DiodeCluster::getTopNet      () const { return getRefRp()->getNet(); }
  inline       DbU::Unit                      DiodeCluster::getWL          () const { return _WL; }
  inline const DiodeCluster::RoutingPadInfos& DiodeCluster::getRoutingPads () const { return _routingPads; }
  inline const vector<Instance*>&             DiodeCluster::getDiodes      () const { return _diodes; }

  bool  DiodeCluster::needsDiode () const
  {
    for ( auto& item : _routingPads ) {
      if (std::get<0>(item.second) & IsSink) return true;
    }
    return false;
  }

  
  inline bool  DiodeCluster::hasRp ( RoutingPad* rp ) const
  { return (_routingPads.find(rp) != _routingPads.end()); }


  inline bool  DiodeCluster::hasGCell ( GCell* gcell ) const
  {
    if (not gcell) return false;
    auto islice = _area.find( gcell->getYMin());
    if (islice == _area.end()) return  false;
    for ( const GCell* igcell : (*islice).second )
      if (igcell == gcell) return true;
    return false;
  }


  inline RoutingPad* DiodeCluster::getRefRp () const
  {
    if (not _routingPads.empty()) return (*_routingPads.begin()).first;
    return NULL;
  }


  void DiodeCluster::merge ( RoutingPad* rp )
  {
    Plug* rpPlug = dynamic_cast<Plug*>( rp->getPlugOccurrence().getEntity() ); 
    if (rpPlug) {
      merge( _anabatic->getGCellUnder( rp->getPosition() ));
      if (rpPlug->getMasterNet()->getDirection() & Net::Direction::DirIn) {
        _routingPads.insert( make_pair(rp,make_tuple(IsSink,(Segment*)NULL)) );
      } else {
        _routingPads.insert( make_pair(rp,make_tuple(IsDriver,(Segment*)NULL)) );
        cdebug_log(147,0) << "| Driver " << rp << endl;
      }
    } else {
      Pin* rpPin = dynamic_cast<Pin*>( rp->getPlugOccurrence().getEntity() ); 
      if (rpPin) {
        _routingPads.insert( make_pair(rp,make_tuple(IsDriver,(Segment*)NULL)) );
        cdebug_log(147,0) << "| Pin (considered driver) " << rp << endl;
      }
    }
  }


  void  DiodeCluster::merge ( Segment* segment )
  {
    _WL += segment->getLength();
    GCellsUnder gcells = _anabatic->getGCellsUnder( segment );
    if (not gcells->empty()) {
      _sortArea = true;
      for ( size_t i=0 ; i<gcells->size() ; ++i ) {
        merge( gcells->gcellAt(i) );
      }
    }
  }


  void  DiodeCluster::merge ( GCell* gcell )
  {
    if (not gcell) return;
    auto islice = _area.find( gcell->getYMin());
    if (islice == _area.end())
      islice = _area.insert( make_pair(gcell->getYMin(),vector<GCell*>()) ).first;
    for ( const GCell* igcell : (*islice).second ) {
      if (igcell == gcell) return;
    }
    (*islice).second.push_back( gcell );
  }


  Box  DiodeCluster::getBoundingBox () const
  {
    Box bb;
    for ( auto& item : _routingPads ) bb.merge( item.first->getPosition() );
    return bb;
  }


  void  DiodeCluster::forceDiodeOn ( RoutingPad* rp, Segment* segment, Flags flags )
  {
    cdebug_log(147,0) << "DiodeCluster::forceDiodeOn(): " << endl;
    cdebug_log(147,0) << "  rp="  << rp << endl;
    cdebug_log(147,0) << "  seg=" << segment << endl;
    if (not rp or not segment) return;
    for ( auto& item : _routingPads ) {
      if (item.first == rp) {
        std::get<0>(item.second) |= HasDiode;
        if (flags & Flags::Source) std::get<0>(item.second) |= IsSegSource;
        std::get<1>(item.second)  = segment;
        return;
      }
    }
    cdebug_log(147,0) << "DiodeCluster::forceDiodeOn(): No RP registered." << endl;
  }

  
  Instance* DiodeCluster::createDiode ( Etesian::Area* area, GCell* gcell, Flags side, uint32_t distance )
  {
    cdebug_log(147,0) << "DiodeCluster::createDiode(): from=" << gcell
                      << " distance=" << distance << endl;

    GCell*   neighbor = gcell;
    GCell*   current  = gcell;
    uint32_t i        = 0;
    while ( i<distance ) {
      if      (side.contains(Flags::WestSide )) current = current->getWest();
      else if (side.contains(Flags::EastSide )) current = current->getEast();
      else if (side.contains(Flags::NorthSide)) current = current->getNorth();
      else if (side.contains(Flags::SouthSide)) current = current->getSouth();
      if (not current) break;
      neighbor = current;
      ++i;
    }

    DbU::Unit xHint = 0;
    if      (side.contains(Flags::WestSide )) xHint = neighbor->getBoundingBox().getXMax();
    else if (side.contains(Flags::EastSide )) xHint = neighbor->getBoundingBox().getXMin();
    else if (side.contains(Flags::NorthSide)) xHint = neighbor->getBoundingBox().getXCenter();
    else if (side.contains(Flags::SouthSide)) xHint = neighbor->getBoundingBox().getXCenter();

    Box bb = neighbor->getBoundingBox(); 
    Instance* diode = area->createDiodeUnder( getRefRp(), bb, xHint );
    if (diode) {
      _diodes.push_back( diode );
      Transformation trans  = getRefRp()->getPlugOccurrence().getPath().getTransformation();
      Point          center = diode->getAbutmentBox().getCenter();
      trans.applyOn( center );
      neighbor = _anabatic->getGCellUnder( center );
      cdebug_log(147,0) << "> GCell area (neighbor): " << bb << endl;
      Contact* sourceContact = gcell->breakGoThrough( getTopNet() );
      Contact* targetContact = neighbor->hasGContact( getTopNet() );

      if (not targetContact) {
        bool hasGoThrough = (neighbor->hasGoThrough(getTopNet()) != NULL);
        targetContact = neighbor->breakGoThrough( getTopNet() );
        cdebug_log(147,0) << "| breakGoThrough(), targetContact= " << targetContact << endl;

        if (not hasGoThrough) {
          cdebug_log(147,0) << "| Target GCell do not have a go-through, connecting. " << endl;
          if (side & Flags::Horizontal) {
            if (sourceContact->getX() > targetContact->getX())
              std::swap( sourceContact, targetContact );

            Horizontal::create( sourceContact
                              , targetContact
                              , _anabatic->getConfiguration()->getGHorizontalLayer()
                              , bb.getCenter().getY()
                              , _anabatic->getConfiguration()->getGHorizontalPitch()
                              );
          } else {
            if (sourceContact->getY() > targetContact->getY())
              std::swap( sourceContact, targetContact );

            Vertical::create( sourceContact
                            , targetContact
                            , _anabatic->getConfiguration()->getGVerticalLayer()
                            , bb.getCenter().getX()
                            , _anabatic->getConfiguration()->getGVerticalPitch()
                            );
          }
        }
      }
    }
    return diode;
  }

  
  const vector<Instance*>& DiodeCluster::createDiodes ( Etesian::Area* area )
  {
    if (not needsDiode()) return _diodes;
    _consolidate();

    Instance* diode = NULL;

    for ( auto& item : _routingPads ) {
      if (not (std::get<0>(item.second) & HasDiode)) continue;

      diode = NULL;
      GCell* gcell = _anabatic->getGCellUnder( item.first->getPosition() );

      Horizontal* h = dynamic_cast<Horizontal*>(std::get<1>(item.second));
      if (h) {
        cdebug_log(147,0) << "> Long segment (forced diode) h=" << h << endl;
        diode = area->createDiodeUnder( getRefRp()
                                      , h->getBoundingBox()
                                      , item.first->getPosition().getX() );
        if (diode) {
          _diodes.push_back( diode );
          continue;
        }
      }
      
      auto islice = _area.find( gcell->getYMin());
      if (islice != _area.end()) {
        Box  gcellsBb;
        bool containsRp = false;
        vector<GCell*>& slice = (*islice).second;
        for ( size_t i=0 ; i < slice.size() ; ++i ) {
          if (slice[i] == gcell) containsRp = true;
          if ((i > 0) and (slice[i-1]->getXMax() < slice[i]->getXMin())) {
            cdebug_log(147,0) << "> GCell area (forced diode): " << gcellsBb << endl;
            if (containsRp) {
              diode = area->createDiodeUnder( getRefRp(), gcellsBb, item.first->getPosition().getX() );
              if (diode) {
                _diodes.push_back( diode );
                containsRp = false;
                break;
              }
            }
            gcellsBb.makeEmpty();
          }
          gcellsBb.merge( slice[i]->getBoundingBox() );
        }
        if (diode) continue;
        if (containsRp and not gcellsBb.isEmpty()) {
          cdebug_log(147,0) << "> GCell area (forced diode, end): " << gcellsBb << endl;
          diode = area->createDiodeUnder( getRefRp(), gcellsBb, item.first->getPosition().getX() );
          if (diode) {
            _diodes.push_back( diode );
            continue;
          }
        }

        cdebug_log(147,0) << "> Try along connecting segment" << endl;
        Horizontal* h = dynamic_cast<Horizontal*>( std::get<1>(item.second) );
        if (h) {
          if (std::get<0>(item.second) & IsSegSource) {
            for ( uint32_t i=0 ; i<3 ; ++i ) {
              if ((diode = createDiode(area,gcell,Flags::EastSide,i))) break;
            }
          } else {
            for ( uint32_t i=0 ; i<3 ; ++i ) {
              if ((diode = createDiode(area,gcell,Flags::WestSide,i))) break;
            }
          }
        } else {
          if (std::get<0>(item.second) & IsSegSource) {
            for ( uint32_t i=0 ; i<3 ; ++i ) {
              if ((diode = createDiode(area,gcell,Flags::NorthSide,i))) break;
            }
          } else {
            for ( uint32_t i=0 ; i<3 ; ++i ) {
              if ((diode = createDiode(area,gcell,Flags::SouthSide,i))) break;
            }
          }
        }
      }
    }
    if (diode) return _diodes;
    
    for ( auto& item : _area ) {
      cdebug_log(147,0) << "+ Slice @" << DbU::getValueString(item.first) << endl;
      Box gcellsBb;
      vector<GCell*>& slice = item.second;
      for ( size_t i=0 ; (i < slice.size()) and not diode ; ++i ) {
        if ((i > 0) and (slice[i-1]->getXMax() < slice[i]->getXMin())) {
          cdebug_log(147,0) << "> GCell area: " << gcellsBb << endl;
          diode = area->createDiodeUnder( getRefRp(), gcellsBb, gcellsBb.getXCenter() );
          if (diode) {
            _diodes.push_back( diode );
            return _diodes;
          }
          gcellsBb.makeEmpty();
        }
        cdebug_log(147,0) << "| Agglomerate [" << i << "]: " << slice[i] << endl;
        gcellsBb.merge( slice[i]->getBoundingBox() );
      }
      if (not gcellsBb.isEmpty()) {
        cdebug_log(147,0) << "> GCell area (end): " << gcellsBb << endl;
        diode = area->createDiodeUnder( getRefRp(), gcellsBb, gcellsBb.getXCenter() );
        if (diode) {
          _diodes.push_back( diode );
          return _diodes;
        }
      }
    }

    for ( auto& item : _area ) {
      vector<GCell*>& slice = item.second;
      if (createDiode(area,slice[0             ],Flags::WestSide)) return _diodes;
      if (createDiode(area,slice[slice.size()-1],Flags::EastSide)) return _diodes;
    }

    for ( auto& item : _routingPads ) {
      GCell* gcell = _anabatic->getGCellUnder( item.first->getPosition() );
      if (createDiode(area,gcell,Flags::NorthSide)) return _diodes;
      if (createDiode(area,gcell,Flags::SouthSide)) return _diodes;
    }
    
    return _diodes;
  }


  void  DiodeCluster::_consolidate ()
  {
    if (not _sortArea) return;
    for ( auto& item : _area ) {
      std::sort( item.second.begin(), item.second.end(), CompareGCellByXMin() );
    }
    _sortArea = false;
  }


}  // Anonymous namespace.


namespace Anabatic {

  using namespace Hurricane;
  using CRL::ToolEngine;
  using Etesian::EtesianEngine;


  void  AnabaticEngine::antennaProtect ( Net* net, uint32_t& failed, uint32_t& total )
  {
    DebugSession::open( net, 145, 150 );
    cdebug_log(147,1) << "Net \"" << net->getName() << endl;

    EtesianEngine* etesian = static_cast<EtesianEngine*>
      ( ToolEngine::get( getCell(), EtesianEngine::staticGetName() ));

    DbU::Unit antennaMaxWL = etesian->getAntennaMaxWL();

    vector<DiodeCluster*>              clusters;
    set<RoutingPad*,DBo::CompareById>  rpsDone;
    set<Segment*   ,DBo::CompareById>  segmentsDone;
    for ( RoutingPad* rp : net->getRoutingPads() ) {
      if (rpsDone.find(rp) != rpsDone.end()) continue;
      
      DiodeCluster* cluster = new DiodeCluster ( rp, this );
      clusters.push_back( cluster );

      vector<Hook*>  hooksStack;
      hooksStack.push_back( rp->getBodyHook() );
      while ( not hooksStack.empty() ) {
        Hook*       topHook  = hooksStack.back();
        RoutingPad* topRp    = dynamic_cast<RoutingPad*>( topHook->getComponent() );
        RoutingPad* connexRp = NULL;
        hooksStack.pop_back();

        for ( Hook* hook : topHook->getHooks() ) {
          RoutingPad* gcellRp = dynamic_cast<RoutingPad*>( hook->getComponent() );
          if (gcellRp) {
            if (not connexRp) connexRp = gcellRp;
            if (rpsDone.find(gcellRp) == rpsDone.end())
              cluster->merge( gcellRp );
            rpsDone.insert( gcellRp );
          }
        }
        
        for ( Hook* hook : topHook->getHooks() ) {
          Segment* segment = dynamic_cast<Segment*>( hook->getComponent() );
          if (segment) {
            cdebug_log(147,0) << "| " << DbU::getValueString(segment->getLength())
                              << " " << segment << endl;
            if (segmentsDone.find(segment) != segmentsDone.end()) continue;
            segmentsDone.insert( segment );
            if (segment->getLength() > antennaMaxWL/2) {
              Flags flags = (segment->getSourceHook() == hook) ? Flags::Source : Flags::Target;
              if (not connexRp) connexRp = topRp;
              cluster->forceDiodeOn( connexRp, segment, flags );
              continue;
            }
            cluster->merge( segment );
            if (dynamic_cast<Segment::SourceHook*>(hook)) {
              hooksStack.push_back( segment->getTargetHook() );
            } else {
              hooksStack.push_back( segment->getSourceHook() );
            }
          }
        }
      }
    }

    Cell* diodeCell   = etesian->getDiodeCell();
    Net*  diodeOutput = NULL;
    for ( Net* net : diodeCell->getNets() ) {
      if (net->isSupply() or not net->isExternal()) continue;
      diodeOutput = net;
      break;
    }

    if (clusters.size() > 1) {
      total += clusters.size();
      cdebug_log(147,1) << "Net \"" << net->getName() << " has " << clusters.size() << " diode clusters." << endl;
      for ( size_t i=0 ; i<clusters.size() ; ++i ) {
        cdebug_log(147,0) << "Cluster [" << i << "] needsDiode=" << clusters[i]->needsDiode()
                          << " bb=" << clusters[i]->getBoundingBox() << endl;
        for ( auto& item : clusters[i]->getRoutingPads() ) {
          cdebug_log(147,0) << "| flags=" << DiodeCluster::toStr(std::get<0>(item.second))
                            << " " << item.first << endl;
        }
        if (not clusters[i]->needsDiode()) continue;
        
        const vector<Instance*> diodes = clusters[i]->createDiodes( etesian->getArea() );
        RoutingPad* rp = clusters[i]->getRefRp();
        if (not diodes.empty()) {
          Net*  topNet   = rp->getNet();
          Plug* sinkPlug = dynamic_cast<Plug*>( rp->getPlugOccurrence().getEntity() );
          Path  path     = rp->getOccurrence().getPath().getHeadPath();
          if (sinkPlug) {
            for ( Instance* diode : diodes ) {
              cdebug_log(147,0) << "  Bind diode input:" << endl;
              cdebug_log(147,0) << "    " << diode    << " @" << diode   ->getTransformation() << endl;
              cdebug_log(147,0) << "    topNet->getCell():" << topNet->getCell() << endl;
              cdebug_log(147,0) << "    " << rp->getOccurrence().getPath() << endl;
            //Path path = rp->getOccurrence().getPath().getHeadPath();
              Plug* diodePlug  = diode->getPlug( diodeOutput );
              diodePlug->setNet( sinkPlug->getNet() );
              RoutingPad* diodeRp = RoutingPad::create( topNet, Occurrence(diodePlug,path), RoutingPad::BiggestArea );

              GCell* gcell = getGCellUnder( diodeRp->getPosition() );
              if (gcell) {
                Contact* contact = gcell->breakGoThrough( topNet );
                contact->getBodyHook()->merge( diodeRp->getBodyHook() );
              }
            }
          } else {
            cerr << Error( "EtesianEngine::antennaProtect(): For %s (rps:%u, clusters:%u)\n"
                         "        Cannot get a Plug from %s (?)."
                         , getString(net).c_str()
                         , rpsDone.size()
                         , clusters.size()
                         , getString(clusters[i]->getRefRp()).c_str()
                         ) << endl;
          }
        } else {
          cerr << Error( "EtesianEngine::antennaProtect(): For %s (rps:%u, clusters:%u)\n"
                         "        Cannot find a diode nearby %s."
                       , getString(net).c_str()
                       , rpsDone.size()
                       , clusters.size()
                       , getString(clusters[i]->getRefRp()).c_str()
                       ) << endl;
          failed += 1;
        }
      }
      cdebug_tabw(147,-1);
    }

    for ( DiodeCluster* cluster : clusters ) delete cluster;

    cdebug_tabw(147,-1);
    DebugSession::close();
  }


  void  AnabaticEngine::antennaProtect ()
  {
  //DebugSession::open( 145, 150 );

    if (not ToolEngine::get( getCell(), EtesianEngine::staticGetName() )) {
      cerr << Warning( "AnabaticEngine::antennaProtect(): No EtesianEngine found, skipped." ) << endl;
      return;
    }
    EtesianEngine* etesian = static_cast<EtesianEngine*>
      ( ToolEngine::get( getCell(), EtesianEngine::staticGetName() ));
    DbU::Unit segmentMaxWL = etesian->getAntennaMaxWL() / 2;

    if (not etesian->getDiodeCell()) {
      cerr << Warning( "AnabaticEngine::antennaProtect(): No diode cell found, skipped." ) << endl;
      return;
    }

    startMeasures();
    openSession();

    uint32_t failed = 0;
    uint32_t total  = 0;
    for ( Net* net : getCell()->getNets() ) {
      if (net->isSupply()) continue;
      if (net->isClock ()) continue;
      antennaProtect( net, failed, total );
    }
    cmess2 << Dots::asString    ( "     - Antenna maximum WL" , DbU::getValueString(etesian->getAntennaMaxWL()) ) << endl;
    cmess2 << Dots::asString    ( "     - Segment maximum WL" , DbU::getValueString(segmentMaxWL) ) << endl;
    cmess2 << Dots::asInt       ( "     - Total needed diodes", total  ) << endl;
    cmess2 << Dots::asInt       ( "     - Failed to allocate" , failed ) << endl;
    cmess2 << Dots::asPercentage( "     - Success ratio"      , (float)(total-failed)/(float)total ) << endl;

    stopMeasures();
    printMeasures( "antennas" );

    // cmess2 << "     - Total segments  : " << total  << endl;
    // cmess2 << "     - Global segments : " << global << endl;
    // cmess2 << "     - Ratio : "
    //        << ((float)global/(float)total)*100.0 << "%." << endl;

    Session::close();
  //DebugSession::close();
    Breakpoint::stop( 99, "After diodes insertions." );
  }


}  // Anabatic namespace.
