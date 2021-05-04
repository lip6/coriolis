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

  typedef tuple<RoutingPad*,uint32_t>  RPInfosItem;

  class CompareRPInfos {
    public:
      inline bool operator () ( const RPInfosItem& lhs, const RPInfosItem& rhs ) const;
  };

  
  inline bool CompareRPInfos::operator () ( const RPInfosItem& lhs, const RPInfosItem& rhs ) const
  { return std::get<0>(lhs)->getId() < std::get<0>(rhs)->getId(); }

  
  typedef tuple<GCell*,uint32_t,GCell*>  GCellInfosItem;

  inline uint32_t& flags ( GCellInfosItem& item ) { return std::get<1>(item); }

  class CompareGCellInfos {
    public:
      inline bool operator () ( const GCellInfosItem& lhs, const GCellInfosItem& rhs ) const;
  };
  
  inline bool CompareGCellInfos::operator () ( const GCellInfosItem& lhs, const GCellInfosItem& rhs ) const
  {
    if (std::get<1>(lhs) != std::get<1>(rhs)) return std::get<1>(lhs) < std::get<1>(rhs);
    return std::get<0>(lhs)->getId() < std::get<0>(rhs)->getId();
  }

  
  typedef set<RPInfosItem,CompareRPInfos>        RoutingPadInfos;
  typedef set<GCellInfosItem,CompareGCellInfos>  GCellArea;


// -----------------------------------------------------------------
// Class : "::DiodeCluster".

  class DiodeCluster {
    public:
      static const uint32_t  IsDriver;
      static const uint32_t  IsSink;
      static const uint32_t  HasDiode;
      static const uint32_t  IsSegSource;
      static const uint32_t  InCluster;
      static       string    toStr ( uint32_t );
    public:
                                      DiodeCluster   ( RoutingPad*, AnabaticEngine* );
      inline       bool               hasRp          ( RoutingPad* ) const;
                   bool               hasGCell       ( GCell* ) const;
      inline       Net*               getTopNet      () const;
      inline       RoutingPad*        getRefRp       () const;
      inline const RoutingPadInfos&   getRoutingPads () const;
      inline const vector<Instance*>& getDiodes      () const;
      inline       DbU::Unit          getWL          () const;
                   void               showArea       () const;
                   bool               needsDiode     () const;
                   Box                getBoundingBox () const;
                   void               merge          ( GCell*, uint32_t distance, GCell* back=NULL );
                   void               merge          ( RoutingPad* );
                   void               merge          ( Segment* );
                   void               mergeHalo      ( Segment*, uint32_t flags );
                   void               inflateArea    (); 
                   Instance*          createDiode    ( Etesian::Area*, GCell*, GCell* );
             const vector<Instance*>& createDiodes   ( Etesian::Area* );
    private:
      AnabaticEngine*   _anabatic;
      bool              _sortArea;
      DbU::Unit         _WL;
      RoutingPadInfos   _routingPads;
      GCellArea         _area;
      vector<Instance*> _diodes;
  };

  
  const uint32_t  DiodeCluster::IsDriver    = (1 << 0);
  const uint32_t  DiodeCluster::IsSink      = (1 << 1);
  const uint32_t  DiodeCluster::HasDiode    = (1 << 2);
  const uint32_t  DiodeCluster::IsSegSource = (1 << 3);
  const uint32_t  DiodeCluster::InCluster   = (1 << 4);


  string DiodeCluster::toStr ( uint32_t flags )
  {
    string s;
    s += (flags & IsDriver   ) ? 'd' : '-';
    s += (flags & IsSink     ) ? 's' : '-';
    s += (flags & HasDiode   ) ? 'D' : '-';
    s += (flags & IsSegSource) ? 'S' : '-';
    s += (flags & InCluster  ) ? 'C' : '-';
    return s;
  }


  DiodeCluster::DiodeCluster ( RoutingPad* rp, AnabaticEngine* anabatic )
    : _anabatic(anabatic)
    , _sortArea(true)
    , _WL(0)
    , _routingPads()
    , _area()
    , _diodes()
  {
    merge( rp );
  }


  inline       Net*               DiodeCluster::getTopNet      () const { return getRefRp()->getNet(); }
  inline       DbU::Unit          DiodeCluster::getWL          () const { return _WL; }
  inline const RoutingPadInfos&   DiodeCluster::getRoutingPads () const { return _routingPads; }
  inline const vector<Instance*>& DiodeCluster::getDiodes      () const { return _diodes; }


  bool  DiodeCluster::needsDiode () const
  {
    for ( auto& infos : _routingPads ) {
      if (std::get<1>(infos) & IsSink) return true;
    }
    return false;
  }

  
  inline bool  DiodeCluster::hasRp ( RoutingPad* rp ) const
  { return (_routingPads.find(make_tuple(rp,0)) != _routingPads.end()); }


  inline bool  DiodeCluster::hasGCell ( GCell* gcell ) const
  {
    if (not gcell) return false;
    for ( auto& item : _area ) {
      if (std::get<0>(item) == gcell) return true;
    }
    return false;
  }


  inline RoutingPad* DiodeCluster::getRefRp () const
  {
    if (not _routingPads.empty()) return std::get<0>( *_routingPads.begin() );
    return NULL;
  }


  void  DiodeCluster::showArea () const
  {
    cdebug_log(147,1) << "GCell diode area" << endl;
    for ( auto& item : _area ) {
      cdebug_log(147,0) << "| d="   << std::get<1>(item)
                        << " "      << std::get<0>(item)
                        << " back=" << std::get<2>(item) << endl;
    }
    cdebug_tabw(147,-1);
  }

  
  void DiodeCluster::merge ( RoutingPad* rp )
  {
    Plug* rpPlug = dynamic_cast<Plug*>( rp->getPlugOccurrence().getEntity() ); 
    if (rpPlug) {
      merge( _anabatic->getGCellUnder( rp->getPosition() ), 0 );
      if (rpPlug->getMasterNet()->getDirection() & Net::Direction::DirIn) {
        cdebug_log(147,0) << "| Sink " << rp << endl;
        _routingPads.insert( make_tuple(rp,IsSink) );
      } else {
        _routingPads.insert( make_tuple(rp,IsDriver) );
        cdebug_log(147,0) << "| Driver " << rp << endl;
      }
    } else {
      Pin* rpPin = dynamic_cast<Pin*>( rp->getPlugOccurrence().getEntity() ); 
      if (rpPin) {
        _routingPads.insert( make_tuple(rp,IsDriver) );
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
        merge( gcells->gcellAt(i), 0 );
      }
    }
  }


  void  DiodeCluster::merge ( GCell* gcell, uint32_t distance, GCell* back )
  {
    if (not gcell) return;
    if (hasGCell(gcell)) return;
    if (back) distance += 20;
    _area.insert( make_tuple(gcell,distance,back) );
  }


  Box  DiodeCluster::getBoundingBox () const
  {
    Box bb;
    for ( auto& infos : _routingPads ) bb.merge( std::get<0>(infos)->getPosition() );
    return bb;
  }


  void  DiodeCluster::mergeHalo ( Segment* segment, uint32_t flags )
  {
    cdebug_log(147,0) << "DiodeCluster::mergeHalo(): " << segment << endl;
    if (not segment) return;

    GCellsUnder gcells = _anabatic->getGCellsUnder( segment );
    if (not gcells->empty()) {
      size_t count = std::min( gcells->size(), (size_t)10 );
      for ( size_t i=0 ; i<count ; ++i ) {
        size_t igcell = (flags & IsSegSource) ? i : (gcells->size()-1-i);
        merge( gcells->gcellAt(igcell), i+1 );
      }
    }
  }


  void  DiodeCluster::inflateArea ()
  {
    vector< tuple<GCell*,GCell*> > border;
    for ( auto& item : _area ) {
      GCell* current = std::get<0>( item );
      if (std::get<2>(item)) continue;

      GCell* neighbor = current->getEast();
      if (neighbor and not neighbor->hasNet(getTopNet()))
        border.push_back( make_tuple(neighbor,current) );
      neighbor = current->getWest();
      if (neighbor and not neighbor->hasNet(getTopNet()))
        border.push_back( make_tuple(neighbor,current) );
    }

    for ( auto& item : _area ) {
      GCell* current = std::get<0>( item );
      if (std::get<2>(item)) continue;

      GCell* neighbor = current->getNorth();
      if (neighbor and not neighbor->hasNet(getTopNet()))
        border.push_back( make_tuple(neighbor,current) );
      neighbor = current->getSouth();
      if (neighbor and not neighbor->hasNet(getTopNet()))
        border.push_back( make_tuple(neighbor,current) );
    }

    for ( auto& item : border ) {
      merge( std::get<0>(item), 1, std::get<1>(item) );
    }
  }

  
  Instance* DiodeCluster::createDiode ( Etesian::Area* area, GCell* gcell, GCell* backGCell )
  {
    cdebug_log(147,0) << "DiodeCluster::createDiode(): from=" << gcell << endl;

    Box bb = gcell->getBoundingBox(); 
    cdebug_log(147,0) << "> GCell area: " << bb << endl;

    Instance* diode = area->createDiodeUnder( getRefRp(), bb, 0 );
    if (diode) {
      _diodes.push_back( diode );
      Transformation trans  = getRefRp()->getPlugOccurrence().getPath().getTransformation();
      Point          center = diode->getAbutmentBox().getCenter();
      trans.applyOn( center );
      bool     newContact = false;
      Contact* contact    = gcell->hasGContact( getTopNet() );
      if (not contact) {
        newContact = true;
        contact    = gcell->breakGoThrough( getTopNet() );
      }
      cdebug_log(147,0) << "| breakGoThrough(), contact= " << contact << endl;

      if (backGCell and newContact) {
        Contact* backContact = backGCell->breakGoThrough( getTopNet() );
        if (backContact->getY() == contact->getY()) {
          if (contact->getX() > backContact->getX())
              std::swap( backContact, contact );

          Horizontal::create( contact
                            , backContact
                            , _anabatic->getConfiguration()->getGHorizontalLayer()
                            , contact->getY()
                            , _anabatic->getConfiguration()->getGHorizontalPitch()
                            );
        } else {
          if (backContact->getX() == contact->getX()) {
            if (contact->getY() > backContact->getY())
              std::swap( backContact, contact );

            Vertical::create( contact
                            , backContact
                            , _anabatic->getConfiguration()->getGHorizontalLayer()
                            , contact->getX()
                            , _anabatic->getConfiguration()->getGHorizontalPitch()
                            );
          } else {
            cerr << Error( "DiodeCluster::createDiode(): Back GCell not aligned with diode GCell.\n"
                           "        * %s\n"
                           "        * %s"
                         , getString(gcell).c_str()
                         , getString(backGCell).c_str()
                         ) << endl;
          }
        }
      }
    }
    return diode;
  }

  
  const vector<Instance*>& DiodeCluster::createDiodes ( Etesian::Area* area )
  {
    if (not needsDiode()) return _diodes;
    showArea();

    cdebug_log(147,1) << "DiodeCluster::createDiodes()" << endl;
    Instance* diode = NULL;
    for ( auto& item : _area ) {
      GCell* gcell     = std::get<0>( item );
      GCell* backGCell = std::get<2>( item );
      cdebug_log(147,0) << "| d=" << std::get<1>(item) << " " << gcell << endl;
      diode = createDiode( area, gcell, backGCell );
      if (diode) break;
    }
    cdebug_tabw(147,-1);

    return _diodes;
  }


}  // Anonymous namespace.


namespace Anabatic {

  using namespace Hurricane;
  using CRL::ToolEngine;
  using Etesian::EtesianEngine;


//! \function  AnabaticEngine::antennaProtect( Net* net, uint32_t& failed, uint32_t& total );
//! \param     net     The net to protect against antenna effects.
//! \param     failed  A reference to the global counter of diodes that we
//!                    where unsucessful to allocate.
//! \param     total   The total number of diode that where requesteds. 
//!                    counting both successful and unsuccessful allocations.
//!
//! \section   antennaSettings  Configuration Variable  for Antenna Effect
//!
//!            <center>
//!              <table class="UserDefined" width="50%">
//!                <tr><td>\c etesian.antennaMaxWL
//!                    <td>The maximum wirelength whitout a diode effect
//!              </table>
//!            </center>
//!
//!
//! \section   antennaAlgo  A Brief Description of the Antenna Protection Algorithm.
//!
//!            The brute force approach would be to put a diode near all sink
//!            points of the net. To reduce that number, we create clusters whose
//!            total wirelength is less than the one triggering an antenna effect.
//!
//!            The antenna protection stage is called after the global routing
//!            and before the detailed routing. The computed wirelength will be
//!            slightly inaccurate but it allow us to directly amend the global
//!            routing so the detailed router needs no modification.
//!
//!            To build the clusters:
//!
//!            <ol> 
//!               <li>Select an unreached (not part of a cluster) RoutingPad.</li>
//!               <li>Perform a depth-first search (DFS) using the segments as edges
//!                   and the Hook rings as nodes. Use a stack to store the search
//!                   state. An element of the stack is a \c tuple of
//!                   \c(Hook*,Segment*,size_t,uint32_t) :
//!
//!                   <ol>
//!                     <li>\c Hook* :    the hook of the Segment we are coming \e from.</li>
//!                     <li>\c Segment* : the segment we are to process.</li>
//!                     <li>\c size_t :   the index, in the stack, of the predecessor
//!                                       segment.</li>
//!                     <li>\c uint32_t : flags. If this segment is \b already part
//!                                       of the cluster.
//!                   </ol>
//!
//!                   All the elements are kept in the stack until the cluster is
//!                   completed. The current top of the stack is given by the \c stackTop
//!                   index.
//!
//!                   When exploring a new node (ring of Hook), all the adjacent segments
//!                   are put on top of the stack. Their suitablility is assesssed only
//!                   when they are popped up.
//!               </li>
//!               <li>When looking at a new stack element (incrementing \c stackTop, not
//!                   really popping up):
//!
//!                   <ol>
//!                     <li>If the segment length is greater than half the maximum antenna
//!                         wirelength, skip it (assume it connects two clusters).</li>
//!                     <li>If the segment length, added to the cluster total length,
//!                         is greater than the antenna length, skip it.
//!                     <li>If the segment is connected to another RoutingPad, agglomerate
//!                         this one the the cluster and merge the segment and all it's
//!                         predecessors to the cluster. Using the back index and setting
//!                         the DiodeCluster::InCluster flags.
//!                   </ol>
//!               </li>
//!               <li>When we reach the end of the stack, close the cluster and build
//!                   it's halo. Go through each elements of the stack again and look
//!                   for segments not part of it, but directly connected to it
//!                   (that is, they have not the InCluster flags set, but their
//!                   immediate predecessor has).
//!               </li>
//!            </ol> 
//!
//!            Structure of a Cluster:
//!
//!            <ol> 
//!               <li>A vector of RoutingPad.</li>
//!               <li>A set of GCells, ordered by priority (distance).
//!                   <ol> 
//!                     <li>A distance of zero means we are under the Segments belonging
//!                         to the cluster itself (directly connecting the RoutingPad).
//!                     </li>
//!                     <li>A distance between 1 to 9 means we are under the halo, that
//!                         is, Segments that <em>connects to</em> the cluster, with the
//!                         increasing distance.
//!                     </li>
//!                     <li>A distance between 10 to 19 means a GCell which is an
//!                         immediate neighbor of the core or halo segments.
//!                     </li>
//!                   </ol> 
//!               </li>
//!            </ol> 
//!
//!            We try to create the cluster's diode in the GCell of the lowest distance
//!            possible.


  void  AnabaticEngine::antennaProtect ( Net* net, uint32_t& failed, uint32_t& total )
  {
  // tuple is: Hook (S or T), From segment, back segment index, flags.
    typedef tuple<Hook*,Segment*,size_t,uint32_t> StackItem;

    DebugSession::open( net, 145, 150 );
    cdebug_log(147,1) << "Net \"" << net->getName() << endl;

    EtesianEngine* etesian = static_cast<EtesianEngine*>
      ( ToolEngine::get( getCell(), EtesianEngine::staticGetName() ));

    DbU::Unit antennaMaxWL = etesian->getAntennaMaxWL();

    vector<DiodeCluster*>              clusters;
    set<RoutingPad*,DBo::CompareById>  rpsDone;
    for ( RoutingPad* rp : net->getRoutingPads() ) {
      set<Segment*,DBo::CompareById>  segmentsDone;

      if (rpsDone.find(rp) != rpsDone.end()) continue;
      
      cdebug_log(147,0) << "New cluster from " << rp << endl;
      DiodeCluster* cluster = new DiodeCluster ( rp, this );
      clusters.push_back( cluster );
      rpsDone.insert( rp );

      size_t               stackTop = 0;
      vector< StackItem >  hooksStack;
      hooksStack.push_back( make_tuple( rp->getBodyHook()
                                      , (Segment*)NULL
                                      , 0
                                      , DiodeCluster::InCluster ) );
      while ( stackTop < hooksStack.size() ) {
        Hook*       toHook      = std::get<0>( hooksStack[stackTop] );
        Segment*    fromSegment = std::get<1>( hooksStack[stackTop] );
        DbU::Unit   branchWL    = 0;

        cdebug_log(147,0) << "| PROCESS [" << stackTop << "] " << fromSegment << endl;
        if (fromSegment) {
          if (fromSegment->getLength() > antennaMaxWL/2) {
            cdebug_log(147,0) << "| Long connecting wire, skipping" << endl;
            ++stackTop;
            continue;
          }

          size_t backIndex = stackTop;
          while ( backIndex ) {
            Segment* segment = std::get<1>( hooksStack[backIndex] );
            branchWL += segment->getLength();
            backIndex = std::get<2>( hooksStack[backIndex] );
            if (not backIndex) break;
            if (std::get<3>( hooksStack[backIndex] ) & DiodeCluster::InCluster) break;
          }
        }
        cdebug_log(147,0) << "| wl=" << DbU::getValueString(cluster->getWL())
                          << " + " << DbU::getValueString(branchWL) << endl;

        if (cluster->getWL() + branchWL > antennaMaxWL) {
          cdebug_log(147,0) << "| Cluster above maximul WL, skipping" << endl;
          ++stackTop;
          continue;
        }

        rp = NULL;
        for ( Hook* hook : toHook->getHooks() ) {
          RoutingPad* toRp = dynamic_cast<RoutingPad*>( hook->getComponent() );
          if (toRp) {
            if (rpsDone.find(toRp) == rpsDone.end()) {
              cdebug_log(147,0) << "> Agglomerate " << toRp << endl;
              cluster->merge( toRp );
              rpsDone.insert( toRp );
              rp = toRp;
            }
          }
        }

        if (rp) {
          size_t backIndex = stackTop;
          cdebug_log(147,1) << "+ Backtrack" << endl;
          while ( backIndex ) {
            Segment* segment = std::get<1>( hooksStack[backIndex] );
            if (segment->getLength() <= branchWL) branchWL -= segment->getLength();
            else                                  branchWL  = 0;
            cluster->merge( segment );
            std::get<3>( hooksStack[backIndex] ) |= DiodeCluster::InCluster;
            cdebug_log(147,0) << "| back=" << backIndex
                              << " -> " << std::get<2>( hooksStack[backIndex] )
                              << " " << DbU::getValueString(segment->getLength())
                              << " " << segment << endl;

            backIndex = std::get<2>( hooksStack[backIndex] );
            if (std::get<3>( hooksStack[backIndex] ) & DiodeCluster::InCluster) {
              cdebug_log(147,0) << "| stop=" << backIndex << " is part of the cluster" << endl;
              break;
            }
          }
          cdebug_tabw(147,-1);
        }
        
        for ( Hook* hook : toHook->getHooks() ) {
          Segment* segment = dynamic_cast<Segment*>( hook->getComponent() );
          if (segment) {
            if (segment == fromSegment) continue;
            cdebug_log(147,0) << "| progress wl=" << DbU::getValueString(cluster->getWL())
                              << " + " << DbU::getValueString(branchWL)
                              << " / " << DbU::getValueString(segment->getLength())
                              << " " << segment << endl;
            if (segmentsDone.find(segment) != segmentsDone.end()) continue;
            segmentsDone.insert( segment );
            uint32_t flags = (segment->getSourceHook() == hook) ? DiodeCluster::IsSegSource : 0;
            if (dynamic_cast<Segment::SourceHook*>(hook)) {
              hooksStack.push_back( make_tuple( segment->getTargetHook(), segment, stackTop, flags ) );
            } else {
              hooksStack.push_back( make_tuple( segment->getSourceHook(), segment, stackTop, flags ) );
            }
          }
        }

        ++stackTop;
      }

      cdebug_log(147,0) << "Cluster halo" << endl;
      for ( size_t i=1 ; i<hooksStack.size() ; ++i ) {
        Segment* segment = std::get<1>( hooksStack[i] );
        uint32_t flags   = std::get<3>( hooksStack[i] );
        cdebug_log(147,0) << "| [" << i << "] flags=" << flags << " "  << segment << endl;
        if (not (flags & DiodeCluster::InCluster)) {
          size_t j = std::get<2>( hooksStack[i] );
          if (std::get<3>(hooksStack[j]) & DiodeCluster::InCluster) {
            cdebug_log(147,0) << "> Put in halo." << endl;
            cluster->mergeHalo( segment, flags );
          }
        }
      }
      cdebug_log(147,0) << "Cluster border" << endl;
      cluster->inflateArea();
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
        cdebug_log(147,1) << "Cluster [" << i << "] needsDiode=" << clusters[i]->needsDiode()
                          << " bb=" << clusters[i]->getBoundingBox() << endl;
        for ( auto& item : clusters[i]->getRoutingPads() ) {
          cdebug_log(147,0) << "| flags=" << DiodeCluster::toStr(std::get<1>(item))
                            << " " << std::get<0>(item) << endl;
        }
        if (not clusters[i]->needsDiode()) {
          cdebug_tabw(147,-1);
          continue;
        }
        
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
              cdebug_log(147,0) << "    " << rp << endl;

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
        cdebug_tabw(147,-1);
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

    cmess1 << "  o  Antenna effect protection." << endl;
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

    Session::close();
  //DebugSession::close();
    Breakpoint::stop( 99, "After diodes insertions." );
  }


}  // Anabatic namespace.
