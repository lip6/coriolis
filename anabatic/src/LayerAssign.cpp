// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |         A n a b a t i c  -  Routing Toolbox                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./LayerAssign.cpp"                        |
// +-----------------------------------------------------------------+


#include <cstdlib>
#include <sstream>
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
#include "anabatic/AutoContactTerminal.h"
#include "anabatic/AutoSegment.h"
#include "anabatic/AnabaticEngine.h"


namespace {


  using namespace std;
  using namespace CRL;
  using namespace Hurricane;
  using namespace Anabatic;


  class SortRpByX {
    public:
      inline       SortRpByX  ();
      inline bool  operator() ( RoutingPad* rp1, RoutingPad* rp2 );
  };


  inline  SortRpByX::SortRpByX ()
  { }


  inline bool  SortRpByX::operator() ( RoutingPad* rp1, RoutingPad* rp2 )
  {
    DbU::Unit x1 = rp1->getCenter().getX();
    DbU::Unit x2 = rp2->getCenter().getX();

    if (x1 == x2) return false;
    return (x1 < x2);
  }


// -----------------------------------------------------------------
// Class : "RpsInRow".

  class RpsInRow {
    public:
      class Compare {
        public:
          bool  operator() ( const RpsInRow* lhs, const RpsInRow* rhs ) const;
      };
    public:
      inline                            RpsInRow      ( RoutingPad*, AnabaticEngine* );
      inline const vector<RoutingPad*>& getRps        () const;
      inline       size_t               getSouth      () const;
      inline       size_t               getNorth      () const;
      inline const Interval&            getRpsHSpan   () const;
      inline const Interval&            getRpsVSpan   () const;
                   void                 slacken       ();
    private:
                   void                 _findTopology ();
      inline       void                 _merge        ( RoutingPad* );
    private:
      AnabaticEngine*      _anabatic;
      vector<RoutingPad*>  _rps;
      size_t               _north;
      size_t               _south;
      Interval             _hSpan;
      Interval             _vSpan;
  };


  inline RpsInRow::RpsInRow ( RoutingPad* seed, AnabaticEngine* anabatic )
    : _anabatic(anabatic)
    , _rps     ()
    , _north   (0)
    , _south   (0)
    , _hSpan   ()
    , _vSpan   ( false )
  {
    _rps.push_back( seed );
    _findTopology();
  }

  
  inline const vector<RoutingPad*>& RpsInRow::getRps      () const { return _rps; }
  inline       size_t               RpsInRow::getSouth    () const { return _south; }
  inline       size_t               RpsInRow::getNorth    () const { return _north; }
  inline const Interval&            RpsInRow::getRpsHSpan () const { return _hSpan; }
  inline const Interval&            RpsInRow::getRpsVSpan () const { return _vSpan; }

  
  bool  RpsInRow::Compare::operator() ( const RpsInRow* lhs, const RpsInRow* rhs ) const
  {
    if ( (lhs->_rps.size() == 2) and (rhs->_rps.size() != 2) ) return true;
    if ( (lhs->_rps.size() != 2) and (rhs->_rps.size() == 2) ) return false;
    if ( lhs->_rps.size() != rhs->_rps.size() ) return lhs->_rps.size() < rhs->_rps.size();

    size_t lhsNs = lhs->_south + lhs->_north;
    size_t rhsNs = rhs->_south + rhs->_north;
    if (lhsNs != rhsNs) return lhsNs < rhsNs;

    if (lhs->_vSpan != rhs->_vSpan) return lhs->_vSpan.getSize() < rhs->_vSpan.getSize();
    if (lhs->_hSpan != rhs->_hSpan) return lhs->_hSpan.getSize() < rhs->_hSpan.getSize();
    
    return lhs->_rps[0]->getId() < rhs->_rps[0]->getId();
  }


  inline void  RpsInRow::_merge ( RoutingPad* rp )
  {
    if (rp != _rps[0]) _rps.push_back( rp );

    Box bb ( _rps.back()->getBoundingBox() );
    _hSpan.merge( bb.getCenter().getX() );
    _vSpan.intersection( bb.getYMin(), bb.getYMax() );
  }


  void  RpsInRow::_findTopology ()
  {
    cdebug_log(146,1) << "RpsInRow::findTopology() - " << _rps[0] << endl;

    _merge( _rps[0] );

    AutoSegmentStack stack;

    for ( Component* component : _rps[0]->getSlaveComponents() ) {
      cdebug_log(146,0) << "slave component: " << component << endl;
      AutoContact* rpContact = Session::lookup( dynamic_cast<Contact*>(component) );
      if (rpContact) {
        cdebug_log(146,0) << "Start rp: " << rpContact << endl;

        for ( AutoSegment* segment : rpContact->getAutoSegments() ) {
          cdebug_log(146,0) << "Examining: " << segment << endl;
          AutoContact* target = segment->getOppositeAnchor(rpContact);

          if (target) {
            if (segment->isHorizontal()) {
              stack.push( target, segment );
            } else {
              if (segment->isLocal()) {
                stack.push( target, segment );
              } else {
                if (segment->getAutoSource() == rpContact) ++_north;
                else                                       ++_south;
              }
            }
          }
        }

        // Find Rps in same horizontal GCell range.
        cdebug_log(146,0) << "Find Rps in same horizontal GCell range" << endl;

        while ( not stack.isEmpty() ) {
          AutoSegment* from    = stack.getAutoSegment();
          AutoContact* contact = stack.getAutoContact();
          stack.pop();

          for ( AutoSegment* segment : contact->getAutoSegments() ) {
            if (segment == from) continue;
            if (segment->isVertical() and not segment->isLocal()) {
              if (segment->getAutoSource() == contact) ++_north;
              else                                     ++_south;
              continue;
            }

            AutoContact*         target   = segment->getOppositeAnchor( contact );
            AutoContactTerminal* terminal = dynamic_cast<AutoContactTerminal*>( target );
            if (terminal) {
              _merge( terminal->getRoutingPad() );
            }

            stack.push( target, segment );
          }
        }
      }
    }

    sort( _rps.begin(), _rps.end(), SortRpByX() );

    cdebug_log(146,0) << "findHAlignedsRps() - Exit" << endl;
    cdebug_tabw(146,-1);
  }


  void  RpsInRow::slacken ()
  {
    cdebug_log(149,1) << "RpsInRow::slacken()" << endl;

    DbU::Unit hpitch = 0;
    for ( RoutingPad* rp : _rps ) {
      cdebug_log(149,0) << "Slacken from: " << rp << endl;

      if (rp->getLayer()) {
        size_t depth = _anabatic->getConfiguration()->getLayerDepth( rp->getLayer() );
        if (depth == 1) {
          cdebug_log(149,0) << "In METAL2, skiping" << endl;
          continue;
        }
        hpitch = _anabatic->getConfiguration()->getPitch( depth, Flags::AboveLayer );
      }
         
      for ( Component* component : rp->getSlaveComponents() ) {
        AutoContact* rpContact = Session::lookup( dynamic_cast<Contact*>(component) );
        if (rpContact) {
          cdebug_log(149,0) << "+ " << rpContact << endl;
          if (hpitch) {
            Box bb = rp->getBoundingBox();
            if (bb.getHeight() / hpitch > 6) {
              cdebug_log(149,0) << "| Terminal is tall enough (> 6 pitch), skipping." << endl;
              continue;
            }
          }
          for ( AutoSegment* segment : rpContact->getAutoSegments() ) {
            cdebug_log(149,0) << "| " << segment << endl;

            if (Session::getConfiguration()->isHV()) {
              if (not segment->isNonPref()) {
                if (segment->isVertical()) {
                  if (segment->getDepth() == 1) {
                    cdebug_log(149,0) << "| Slacken: " << segment << endl;
                    segment->changeDepth( 2, Flags::NoFlags );
                    cdebug_log(149,0) << "| After Slacken: " << segment << endl;
                  }
                } else {
                  segment->makeDogleg( rpContact->getGCell() );
                  cdebug_log(149,0) << "| Make dogleg: " << segment << endl;
                }
              }
            } else {
              segment->makeDogleg( rpContact->getGCell() );
              cdebug_log(149,0) << "| Make dogleg: " << segment << endl;
            }
          }
        }
      }
    }
    cdebug_tabw(149,-1);
  }


// -----------------------------------------------------------------
// Class : "GCellRps".

  class GCellRps {
    public:
      class Compare {
        public:
          bool  operator() ( const GCellRps* lhs, const GCellRps* rhs ) const;
      };
    public:
                                      GCellRps     ( GCell*, AnabaticEngine* );
                                     ~GCellRps     ();
      inline GCell*                   getGCell     () const;
      inline size_t                   add          ( RoutingPad* );
      inline void                     consolidate  ();
      inline RpsInRow*                getRpsInRow  ( size_t i );
      inline const vector<RpsInRow*>& getRpsInRows () const;
    private:
      AnabaticEngine*    _anabatic;
      GCell*             _gcell;
      vector<RpsInRow*>  _rpsInRows;
  };


  GCellRps::GCellRps ( GCell* gcell, AnabaticEngine* anabatic )
    : _anabatic (anabatic)
    , _gcell    (gcell)
    , _rpsInRows()
  { }


  GCellRps::~GCellRps ()
  {
    for ( RpsInRow* elem : _rpsInRows ) delete elem;
  }


  inline GCell* GCellRps::getGCell () const { return _gcell; }


  inline size_t  GCellRps::add ( RoutingPad* rp )
  {
    _rpsInRows.push_back( new RpsInRow(rp,_anabatic) );
    return _rpsInRows.size() - 1;
  }


  inline void  GCellRps::consolidate ()
  {
    sort( _rpsInRows.begin(), _rpsInRows.end(), RpsInRow::Compare() );
  }


  inline       RpsInRow*          GCellRps::getRpsInRow  ( size_t i ) { return _rpsInRows[i]; }
  inline const vector<RpsInRow*>& GCellRps::getRpsInRows () const     { return _rpsInRows; }

  
  bool  GCellRps::Compare::operator() ( const GCellRps* lhs, const GCellRps* rhs ) const
  { return lhs->getGCell()->getId() < rhs->getGCell()->getId(); }


}  // Anonymous namespace.


namespace Anabatic {

  using Hurricane::DebugSession;
  using Hurricane::ForEachIterator;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::NetRoutingExtension;


  void  AnabaticEngine::_desaturate ( unsigned int    depth
                                     , set<Net*>&     globalNets
                                     , unsigned long& total
                                     , unsigned long& globals )
  {
    if (depth+2 >  Session::getConfiguration()->getAllowedDepth()) {
      cerr << Warning("Anabatic::_desaturate(): %s, no remaining upper layers."
                     ,getString(Session::getRoutingGauge()->getRoutingLayer(depth)->getName()).c_str()
                     ) << endl;
      return;
    }

    cmess1 << "  o  Desaturate layer "
           << Session::getRoutingGauge()->getRoutingLayer(depth)->getName() << endl;
    cdebug_log(149,0) << "Session::getSaturateRatio()=" << Session::getSaturateRatio() << endl;

    GCellKeyQueue  queue;
    GCell::Set     invalidateds;

    for ( GCell* gcell : getGCells() ) queue.push( gcell->cloneKey(depth) );

    bool optimized = true;
    bool finished  = false;
    while ( optimized ) {
      Session::revalidate ();
      optimized = false;

      while ( not queue.empty() ) {
        GCell::Key* topKey = queue.top();
        GCell*      gcell  = const_cast<GCell*>( topKey->getGCell() );
        queue.pop();

        // if (gcell->getId() == 3534128)
        //   DebugSession::open( 145, 150 );

        cdebug_log(149,0) << "Before, queue.size()=" << queue.size() << endl;
        if (topKey->isActive()) {
          cdebug_log(149,0) << "_desaturate: finish=" << finished << " [" << depth << "]:"
                            << gcell->getDensity(depth) << " " << gcell << endl;
          
          if (not gcell->isSaturated(depth)) {
            cdebug_log(149,0) << "STOP desaturated: " << gcell << endl;
            finished = true;
          } else {
            if (finished) {
              cparanoid << "[ERROR] Still saturated: " << gcell << endl;
            }
          }
          
          if (not finished) {
            optimized = gcell->stepNetDesaturate( depth, globalNets, invalidateds );
            gcell->setSatProcessed( depth );
            if (optimized) {
              for ( GCell* invalidGCell : invalidateds ) {
              //if ((invalidGCell != gcell) or (gcell->isSaturated(depth)))
                if (not invalidGCell->isSatProcessed(depth))
                  queue.push( invalidGCell->cloneKey(depth) );
              }
              invalidateds.clear();
            }
          }
        }
        cdebug_log(149,0) << "After, queue.size()=" << queue.size() << endl;

        delete topKey;

        // if (gcell->getId() == 3534128)
        //   DebugSession::close();
      }
    }
    

#if OLD_QUEUE_DISABLED
    GCellDensitySet queue   ( depth, getGCells() );
    GCell::Set     invalidateds;

    bool optimized = true;
    while ( optimized ) {
      Session::revalidate ();
      optimized = false;
      queue.requeue ();

      std::set<GCell*,GCell::CompareByKey>::const_iterator igcell = queue.getGCells().begin();
      size_t i = 0;
      for ( ; igcell!=queue.getGCells().end() ; ++igcell, ++i ) {
        cdebug_log(149,0) << "_desaturate: [" << depth << "]:"
                          << (*igcell)->getDensity(depth) << " " << *igcell << endl;

        if (not (*igcell)->isSaturated(depth)) {
          cdebug_log(149,0) << "STOP desaturated: @" << i << " " << *igcell << endl;
          for ( ; igcell!=queue.getGCells().end() ; ++igcell ) {
            if ( (*igcell)->isSaturated( depth ) ) {
              cparanoid << "[ERROR] Still saturated: @" << i << " " << *igcell << endl;
              break;
            }
          }
          break;
        }

        optimized = (*igcell)->stepNetDesaturate( depth, globalNets, invalidateds );
        if ( optimized ) {
          for ( GCell::Set::iterator igcell=invalidateds.begin() ; igcell!=invalidateds.end() ; ++igcell ) {
            queue.unqueue( *igcell );
          }
          break;
        }
      }
    }
#endif
  }


  void  AnabaticEngine::_layerAssignByLength ( Net* net, unsigned long& total, unsigned long& global, set<Net*>& globalNets )
  {
    DebugSession::open ( net, 140, 150 );

    cdebug_log(149,0) << "Anabatic::_layerAssignByLength( " << net << " )" << endl;
    cdebug_tabw(145,1);

    bool               isGlobal = false;
    set<AutoContact*>  globalContacts;

    forEach ( Segment*, isegment, net->getSegments() ) {
      total++;
      if ( (*isegment)->getLength() > getGlobalThreshold() ) {
        if (not isGlobal) {
          isGlobal = true;
          globalNets.insert( net );
        }

        global++;
        if ((*isegment)->getLayer() == Session::getRoutingLayer(1)) {
          (*isegment)->setLayer( Session::getRoutingLayer(3) );
          (*isegment)->setWidth( Session::getWireWidth   (3) );
        }
        if ((*isegment)->getLayer() == Session::getRoutingLayer(2)) {
          (*isegment)->setLayer( Session::getRoutingLayer(4) );
          (*isegment)->setWidth( Session::getWireWidth   (4) );
        }
      }
    }

    cdebug_tabw(145,-1);

    DebugSession::close();
  }


  void  AnabaticEngine::_layerAssignByLength ( unsigned long& total, unsigned long& global, set<Net*>& globalNets )
  {
    cmess1 << "  o  Assign Layer (simple wirelength)." << endl;

    forEach ( Net* , inet , getCell()->getNets() ) {
      if (NetRoutingExtension::get(*inet)->isAutomaticGlobalRoute()) {
        _layerAssignByLength ( *inet, total, global, globalNets );
      }
    } // forEach(Net*)
  }


  void  AnabaticEngine::_layerAssignByTrunk ( Net* net, set<Net*>& globalNets, unsigned long& total, unsigned long& global )
  {
    DebugSession::open( net, 145, 150 );

    cdebug_log(149,0) << "Anabatic::_layerAssignByTrunk ( " << net << " )" << endl;
    cdebug_tabw(145,1);

    bool               isGlobal  = false;
    unsigned long      netGlobal = 0;
    unsigned long      netTotal  = 0;
    set<AutoContact*>  globalContacts;

    forEach ( Segment*, isegment, net->getSegments() ) {
      netTotal++;

      if ((*isegment)->getLength() > getGlobalThreshold()) {
        isGlobal = true;
        netTotal = 0;
        globalNets.insert( net );
        break;
      }
    }

    if (isGlobal) {
      forEach ( Segment*, isegment, net->getSegments() ) {
        netTotal++;

        AutoSegment* autoSegment = Session::lookup( *isegment );
        if ( autoSegment and not autoSegment->isStrongTerminal() ) {
          netGlobal++;

          cdebug_log(145,0) << "Migrate to M4/M5: " << autoSegment << endl;
          if (autoSegment->isHorizontal()) {
            autoSegment->setLayer( Session::getRoutingLayer(3) );
            autoSegment->setWidth( Session::getWireWidth   (3) );
          }
          if (autoSegment->isVertical()) {
            autoSegment->setLayer( Session::getRoutingLayer(4) );
            autoSegment->setWidth( Session::getWireWidth   (4) );
          }
        }
      }
    }

    total  += netTotal;
    global += netGlobal;

    cdebug_tabw(145,-1);

    DebugSession::close();
  }


  void  AnabaticEngine::_layerAssignByTrunk ( unsigned long& total, unsigned long& global, set<Net*>& globalNets )
  {
    cmess1 << "  o  Assign Layer (whole net trunk)." << endl;

    forEach ( Net* , inet , getCell()->getNets() ) {
      if (NetRoutingExtension::get(*inet)->isAutomaticGlobalRoute()) {
        _layerAssignByTrunk( *inet, globalNets, total, global );
      }
    }
  }


  void  AnabaticEngine::_layerAssignNoGlobalM2V ( Net* net, set<Net*>& globalNets, unsigned long& total, unsigned long& global )
  {
    cdebug_log(149,0) << "Anabatic::_layerAssignNoGlobalM2V ( " << net << " )" << endl;
    cdebug_tabw(145,1);

    bool               isGlobalNet = false;
    unsigned long      netGlobal   = 0;
    unsigned long      netTotal    = 0;
    set<AutoContact*>  globalContacts;

    for ( Segment* baseSegment : net->getSegments() ) {
      ++netTotal;

      AutoSegment* segment = Session::lookup( baseSegment );

      if (not segment or segment->isLocal()) continue;

      isGlobalNet = true;
      netTotal    = 0;
      globalNets.insert( net );
      break;
    }

    if (isGlobalNet) {
      vector<AutoSegment*> horizontals;

      for ( Segment* baseSegment : net->getSegments() ) {
        AutoSegment* segment = Session::lookup( baseSegment );
        if (not segment or not segment->isCanonical()) continue;

        if (segment->isHorizontal()) horizontals.push_back( segment );
      }

      for ( AutoSegment* horizontal : horizontals ) {
        vector<AutoSegment*> collapseds;
        vector< tuple<AutoSegment*,Flags> > perpandicularsDatas;
      //vector<AutoSegment*> northBounds;
      //vector<AutoSegment*> southBounds;
        DbU::Unit            leftBound;
        DbU::Unit            rightBound;
      //bool                 hasNorth = false;
      //bool                 hasSouth = false;

        AutoSegment::getTopologicalInfos( horizontal
                                        , collapseds
                                        , perpandicularsDatas
                                        , leftBound
                                        , rightBound
                                        );

        for ( auto perpandicularDatas : perpandicularsDatas ) {
          AutoSegment* perpandicular = std::get<0>( perpandicularDatas );
          if (Session::getLayerDepth(perpandicular->getLayer()) > 2) continue;

          bool hasGlobal = false;
          for ( AutoSegment* aligned : perpandicular->getAligneds(Flags::NoCheckLayer|Flags::WithSelf) ) {
            if (aligned->isGlobal()) { hasGlobal = true; break; }
          }
          if (not hasGlobal) continue;

          // if (perpandicular->getSourceY() == horizontal->getAxis()) {
          //   hasNorth = true;
          //   if (hasGlobal) northBounds.push_back( perpandicular );
          // } else {
          //   hasSouth = true;
          //   if (hasGlobal) southBounds.push_back( perpandicular );
          // }
          
          if (  perpandicular->getAutoSource()->getGCell()->getNorth()
             != perpandicular->getAutoTarget()->getGCell()) {
            perpandicular->changeDepth( 3, Flags::Propagate );
            ++netGlobal;
            continue;
          }
        }

        // if (hasSouth and hasNorth) {
        //   if (not northBounds.empty()) {
        //     for ( AutoSegment* perpandicular : northBounds )
        //       perpandicular->changeDepth( 3, Flags::Propagate );
        //   } else {
        //     for ( AutoSegment* perpandicular : southBounds )
        //       perpandicular->changeDepth( 3, Flags::Propagate );
        //   }
        // }
      }
    }

    total  += netTotal;
    global += netGlobal;

    cdebug_tabw(145,-1);
  }


  void  AnabaticEngine::_layerAssignNoGlobalM2V ( unsigned long& total, unsigned long& global, set<Net*>& globalNets )
  {
    cmess1 << "  o  Assign Layer (no global vertical metal2)." << endl;

    for ( Net* net : getCell()->getNets() ) {
      DebugSession::open( net, 145, 150 );

      NetRoutingState* state = NetRoutingExtension::get( net );
      if (not state or state->isAutomaticGlobalRoute()) {
        _layerAssignNoGlobalM2V( net, globalNets, total, global );
      } else {
        cdebug_log(145,0) << net << " is not automatic routed, skipped." << endl;
      }

      DebugSession::close();
    }
  }


#if THIS_IS_DISABLED
  void  AnabaticEngine::moveULeft ( AutoSegment* seed, set<Net*>& globalNets, GCell::Set& invalidateds )
  {
    Net* net = seed->getNet();
    DebugSession::open( net, 145, 150 );

    seed->moveULeft();
    globalNets.insert( net );

    GCell* begin = seed->getAutoSource()->getGCell();
    GCell* end   = seed->getAutoTarget()->getGCell();

    if (seed->isHorizontal()) {
      for ( GCell* gcell=begin ; gcell and gcell!=end ; gcell=gcell->getRight() )
        invalidateds.insert( gcell );
            
      begin = begin->getDown();
      end   = end  ->getDown();

      for ( GCell* gcell=begin ; gcell and gcell!=end ; gcell=gcell->getRight() )
        invalidateds.insert( gcell );
    } else {
      for ( GCell* gcell=begin ; gcell and gcell!=end ; gcell=gcell->getUp() )
        invalidateds.insert( gcell );
            
      begin = begin->getLeft();
      end   = end  ->getLeft();

      for ( GCell* gcell=begin ; gcell and gcell!=end ; gcell=gcell->getUp() )
        invalidateds.insert( gcell );
    }

    DebugSession::close();
  }


  void  AnabaticEngine::moveURight ( AutoSegment* seed, set<Net*>& globalNets, GCell::Set& invalidateds )
  {
    Net* net = seed->getNet();
    DebugSession::open( net, 145, 150 );

    seed->moveURight();
    globalNets.insert( net );

    GCell* begin = seed->getAutoSource()->getGCell();
    GCell* end   = seed->getAutoTarget()->getGCell();

    if (seed->isHorizontal()) {
      for ( GCell* gcell=begin ; gcell and gcell!=end ; gcell=gcell->getRight() )
        invalidateds.insert( gcell );
            
      begin = begin->getUp();
      end   = end  ->getUp();

      for ( GCell* gcell=begin ; gcell and gcell!=end ; gcell=gcell->getRight() )
        invalidateds.insert( gcell );
    } else {
      for ( GCell* gcell=begin ; gcell and gcell!=end ; gcell=gcell->getUp() )
        invalidateds.insert( gcell );
            
      begin = begin->getRight();
      end   = end  ->getRight();

      for ( GCell* gcell=begin ; gcell and gcell!=end ; gcell=gcell->getUp() )
        invalidateds.insert( gcell );
    }

    DebugSession::close();
  }
#endif


  bool  AnabaticEngine::moveUpNetTrunk ( AutoSegment* seed, set<Net*>& globalNets, GCell::Set& invalidateds )
  {
    Net*         net       = seed->getNet();
    unsigned int seedDepth = Session::getRoutingGauge()->getLayerDepth(seed->getLayer());

    DebugSession::open( net, 145, 150 );
    cdebug_log(149,0) << "moveUpNetTrunk() depth:" << seedDepth << " " << seed << endl;

    Flags flags = Flags::Propagate|Flags::AllowTerminal|Flags::NoCheckLayer;
    if (seedDepth > 2) flags |= Flags::IgnoreContacts;
    if (not seed->canMoveUp( 2.0, flags) ) {
      cdebug_log(149,0) << "Reject seed move up, cannot move up." << endl;
      DebugSession::close();
      return false;
    }
    cdebug_tabw(149,1);

    globalNets.insert( net );

    vector< pair<AutoContact*,AutoSegment*> > stack;
    vector<AutoSegment*> globals;
    vector<AutoSegment*> locals;

    stack.push_back( pair<AutoContact*,AutoSegment*>(NULL,seed) );
    while ( not stack.empty() ) {
      AutoContact* from    = stack.back().first;
      AutoSegment* segment = stack.back().second;
      stack.pop_back();
      cdebug_log(149,0) << "> Looking at " << segment << endl;
      cdebug_log(149,0) << "    from " << from << endl;

      if (segment->isLocal()) {
        if (not segment->isStrongTerminal()) {
          cdebug_log(149,0) << "| Push non-strong local " << segment << endl;
          locals.push_back( segment );
        }
        continue;
      }
      if ( (segment->getAnchoredLength() < 3*Session::getSliceHeight()) and (segment != seed) ) {
        cdebug_log(149,0) << "| Push small anchored local " << segment << endl;
        locals.push_back( segment );
        continue;
      }

    // Do something here.
      if (not segment->canMoveUp( 2.0, flags|Flags::CheckLowDensity )) {
        cdebug_log(149,0) << "| Reject global " << segment << endl;
        continue;
      }

      cdebug_log(149,0) << "| Push global " << segment << endl;
      globals.push_back( segment );

      AutoContact* source = segment->getAutoSource();
      if (source != from) {
        for ( AutoSegment* connected : source->getAutoSegments() ) {
          if (connected != segment) { stack.push_back( make_pair(source,connected) ); }
        }
      }
      AutoContact* target = segment->getAutoTarget();
      if (target != from) {
        for ( AutoSegment* connected : target->getAutoSegments() ) {
          if (connected != segment) { stack.push_back( make_pair(target,connected) ); }
        }
      }
    }

    for ( size_t i=0 ; i<globals.size() ; ++i ) {
      cdebug_log(149,0) << "Looking up G:" << globals[i] << endl;
      unsigned int depth = Session::getRoutingGauge()->getLayerDepth( globals[i]->getLayer() );
      globals[i]->changeDepth( depth+2, Flags::WithNeighbors );

      vector<GCell*> gcells;
      globals[i]->getGCells( gcells );
      for ( size_t j=0 ; j<gcells.size() ; j++ ) {
        invalidateds.insert( gcells[j] );
      }
    }

    for ( size_t i=0 ; i<locals.size() ; ++i ) {
      cdebug_log(149,0) << "Looking up L:" << locals[i] << endl;

      unsigned int depth = Session::getRoutingGauge()->getLayerDepth(locals[i]->getLayer());
      if (depth > seedDepth+1) continue;

      bool moved = false;
      if (locals[i]->isNonPref()) {
        locals[i]->changeDepth( depth+1, Flags::Propagate|Flags::WithNeighbors );
        moved = true;

        cdebug_log(149,0) << "Trunk non-pref move up L:" << locals[i] << endl;
      } else {
        if (locals[i]->canPivotUp(2.0,Flags::Propagate|Flags::NoCheckLayer)) {
          locals[i]->changeDepth( depth+2, Flags::WithNeighbors );
          moved = true;

          cdebug_log(149,0) << "Trunk move up L:" << locals[i] << endl;
        }
      }

      if (moved) {
        vector<GCell*> gcells;
        locals[i]->getGCells( gcells );
        for ( size_t j=0 ; j<gcells.size() ; j++ ) {
          invalidateds.insert( gcells[j] );
        }
      }
    }

    cdebug_tabw(149,0) << "Successful moveUpNetTrunk() for " << net << " done" << endl;
    cdebug_tabw(149,-1);
    DebugSession::close();

    return true;
  }


#if THIS_IS_DISABLED
  void  AnabaticEngine::_balanceGlobalDensity ( unsigned int depth )
  {
    startMeasures();
    openSession();

    cmess1 << "  o  Balance Global Density "
           << Session::getRoutingGauge()->getRoutingLayer(depth)->getName() << endl;

    GCellDensitySet queue ( depth, getGCells() );
    GCell::Set      invalidateds;

    bool optimized = true;
    while ( optimized ) {
      Session::revalidate();
      optimized = false;
      queue.requeue();

      std::set<GCell*,GCell::CompareByKey>::const_iterator igcell = queue.getGCells().begin();
      size_t i = 0;
      for ( ; igcell!=queue.getGCells().end() ; ++igcell, ++i ) {
        cdebug_log(149,0) << "_balance: [" << depth << "]:"
                    << (*igcell)->getDensity(depth) << " " << *igcell << endl;

        if (not (*igcell)->isSaturated(depth)) {
          cdebug_log(149,0) << "STOP desaturated: @" << i << " " << *igcell << endl;
          for ( ; igcell!=queue.getGCells().end() ; ++igcell ) {
            if ((*igcell)->isSaturated(depth)) {
              cparanoid << Error( "Still saturated: @%d %s", i, getString(*igcell).c_str() ) << endl;
              break;
            }
          }
          break;
        }

        optimized = (*igcell)->stepBalance( depth, invalidateds );
        if (optimized) {
          for ( GCell::Set::iterator igcell=invalidateds.begin() ; igcell!=invalidateds.end() ; ++igcell ) {
            queue.unqueue( *igcell );
          }
          break;
        }
      }
    }


    Session::close();
    stopMeasures();
    printMeasures( "balance" );
  }


  void  AnabaticEngine::balanceGlobalDensity ()
  {
  //_balanceGlobalDensity( 1 ); // metal2
  //_balanceGlobalDensity( 2 ); // metal3

    set<Net*>   globalNets;
    GCell::Set  invalidateds;

    openSession();

    vector<AutoSegment*>  segments;

    AutoSegmentLut::iterator ilut = _autoSegmentLut.begin();
    for ( ; ilut!=_autoSegmentLut.end() ; ++ilut ) {
      AutoSegment* segment = (*ilut).second;

      if (segment->isLocal() or segment->isFixed()) continue;
      if (not segment->isCanonical()) continue;

      segments.push_back( segment );
    }

  // Sort on id before moving to ensure determinism.
    sort( segments.begin(), segments.end(), AutoSegment::CompareId() );

    for ( size_t i=0 ; i<segments.size() ; ++i ) {
    // Hard-coded: reserve 3 tracks (1/20 * 3).
      if (segments[i]->canMoveULeft(0.10)) {
        moveULeft(segments[i],globalNets,invalidateds);
      } else if (segments[i]->canMoveURight(0.10)) {
        moveURight(segments[i],globalNets,invalidateds);
      }

      for ( GCell::Set::iterator igcell=invalidateds.begin() ; igcell!=invalidateds.end() ; ++igcell ) {
        (*igcell)->updateDensity();
      }
      invalidateds.clear();
      Session::revalidate();
    }

    Session::close();
  }
#endif


  void  AnabaticEngine::layerAssign ( uint32_t method )
  {
  //DebugSession::open( 145, 150 );

    cdebug_log(149,1) << "Layer Assignment" << endl;
    cdebug_log(149,0) << "Desaturate by later density" << endl;

    set<Net*> globalNets;

    unsigned long  total  = 0;
    unsigned long  global = 0;

    startMeasures();
    openSession();

    if (Session::getAllowedDepth() >= 3) {
      switch ( method ) {
        case EngineLayerAssignByLength:    _layerAssignByLength    ( total, global, globalNets ); break;
        case EngineLayerAssignByTrunk:     _layerAssignByTrunk     ( total, global, globalNets ); break;
        case EngineLayerAssignNoGlobalM2V: _layerAssignNoGlobalM2V ( total, global, globalNets ); break;
        case EngineNoNetLayerAssign:    break;
        default:
          stopMeasures();
          Session::close();
          throw Error( badMethod
                     , "Anabatic::layerAssign()"
                     , method
                     , getString(_cell).c_str()
                     );
      }
  
      globalNets.clear();
      Session::revalidate();
  
      if (   (method != EngineLayerAssignNoGlobalM2V) 
         and (getConfiguration()->getAllowedDepth() > 2) ) {
        for ( size_t depth=1 ; depth <= getConfiguration()->getAllowedDepth()-2; ++depth ) {
          _desaturate( depth, globalNets, total, global );
          if ( (depth > 1) and (depth%2 == 0) ) Session::revalidate();
        }
        
        globalNets.clear ();
        Session::revalidate();
      }
  
#if defined(CHECK_DATABASE)
      _check( "after layer assignment" );
#endif
  
      Session::setAnabaticFlags( Flags::WarnOnGCellOverload );
    }
    
    if (Session::getConfiguration()->isHV()) {
      cdebug_log(149,0) << "Desaturate by GCell terminal saturation" << endl;
      set<GCellRps*,GCellRps::Compare> gcellRpss;

      for ( GCell* gcell : getGCells() ) {
        set<RoutingPad*,Entity::CompareById> rps;
        
        const vector<AutoContact*> contacts = gcell->getContacts();
        for ( AutoContact* contact : contacts ) {
          AutoContactTerminal* terminal = dynamic_cast<AutoContactTerminal*>( contact );
          if (terminal) {
            rps.insert( terminal->getRoutingPad() );
          }
        }
        if (rps.size() > getConfiguration()->getSaturateRp()) {
          GCellRps* gcellRps = new GCellRps ( gcell, this );
          gcellRpss.insert( gcellRps );
      
          for ( RoutingPad* rp : rps ) gcellRps->add( rp );
        }
      } 

      for ( GCellRps* gcellRps : gcellRpss ) {
        gcellRps->consolidate();
        
        const vector<RpsInRow*>& rpsInRows = gcellRps->getRpsInRows();
        cdebug_log(149,0) << gcellRps->getGCell() << " has " << rpsInRows.size() << " terminals." << endl;
      
        size_t count = 0;
        for ( RpsInRow* rpsInRow : rpsInRows ) {
          cdebug_log(149,0) << "North:" << rpsInRow->getNorth() << " South:"
               << rpsInRow->getSouth() << " net:"
               << rpsInRow->getRps()[0]->getNet()->getName() << endl;
          cdebug_log(149,0) << "H-Span:" << rpsInRow->getRpsHSpan() << " V-Span:" << rpsInRow->getRpsVSpan() << endl;
          for ( RoutingPad* arp : rpsInRow->getRps() ) {
            cdebug_log(149,0) << "| " << arp << endl;
          }
          if (++count < 2) rpsInRow->slacken();
        }
      
        for ( AutoSegment* segment : gcellRps->getGCell()->getHSegments() ) {
          if (segment->canPivotUp()) {
            cdebug_log(149,0) << "Move up horizontal: " << segment << endl;
            segment->moveUp( Flags::Propagate );
          }
        }
        
        delete gcellRps;
      }
    }
  
    checkGCellDensities();
    Session::close();

    stopMeasures();
    printMeasures( "assign" );

    // cmess2 << "     - Total segments  : " << total  << endl;
    // cmess2 << "     - Global segments : " << global << endl;
    // cmess2 << "     - Ratio : "
    //        << ((float)global/(float)total)*100.0 << "%." << endl;

    Breakpoint::stop( 100, "After layer assignment." );

    cdebug_tabw(149,-1);
  //DebugSession::close();
  }


}  // Anabatic namespace.
