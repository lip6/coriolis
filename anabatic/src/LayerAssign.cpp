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
#include "anabatic/AutoContact.h"
#include "anabatic/AutoSegment.h"
#include "anabatic/AnabaticEngine.h"


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

    GCellDensitySet queue   ( depth, getGCells() );
    GCell::Set      invalidateds;

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
        if ((*isegment)->getLayer() == Session::getRoutingLayer(1)) (*isegment)->setLayer( Session::getRoutingLayer(3) );
        if ((*isegment)->getLayer() == Session::getRoutingLayer(2)) (*isegment)->setLayer( Session::getRoutingLayer(4) );
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
    DebugSession::open( net, 140, 150 );

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
          if (autoSegment->isHorizontal()) autoSegment->setLayer( Session::getRoutingLayer(3) );
          if (autoSegment->isVertical  ()) autoSegment->setLayer( Session::getRoutingLayer(4) );
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


#if THIS_IS_DISABLED
  void  AnabaticEngine::moveULeft ( AutoSegment* seed, set<Net*>& globalNets, GCell::Set& invalidateds )
  {
    Net* net = seed->getNet();
    DebugSession::open( net, 140, 150 );

    cdebug_log(9000,0) << "Deter| Move left: " << seed << endl;

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
    DebugSession::open( net, 140, 150 );

    cdebug_log(9000,0) << "Deter| Move right: " << seed << endl;

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

    DebugSession::open( net, 140, 150 );
    cdebug_log(9000,0) << "Deter| moveUpNetTrunk() depth:" << seedDepth << " " << seed << endl;

    if (not seed->canMoveUp( 1.0, Flags::Propagate|Flags::AllowTerminal|Flags::NoCheckLayer) ) {
      cdebug_log(9000,0) << "Deter| Reject seed move up, cannot move up." << endl;
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

      if (segment->isLocal()) {
        if (not segment->isStrongTerminal()) locals.push_back( segment );
        continue;
      }
      if ( (segment->getLength() < 3*Session::getSliceHeight()) and (segment != seed) ) {
        locals.push_back( segment );
        continue;
      }

    // Do something here.
      if (not segment->canMoveUp( 1.0
                                , Flags::Propagate
                                | Flags::AllowTerminal
                                | Flags::NoCheckLayer
                                | Flags::CheckLowDensity
                                ) ) continue;

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
    //cdebug_log(9000,0) << "Deter| Looking up G:" << globals[i] << endl;
      unsigned int depth = Session::getRoutingGauge()->getLayerDepth( globals[i]->getLayer() );
      globals[i]->changeDepth( depth+2, Flags::WithNeighbors );

      vector<GCell*> gcells;
      globals[i]->getGCells( gcells );
      for ( size_t j=0 ; j<gcells.size() ; j++ ) {
        invalidateds.insert( gcells[j] );
      }
    }

    for ( size_t i=0 ; i<locals.size() ; ++i ) {
    //cdebug_log(9000,0) << "Deter| Looking up L:" << locals[i] << endl;

      unsigned int depth = Session::getRoutingGauge()->getLayerDepth(locals[i]->getLayer());
      if (depth > seedDepth+1) continue;

      if (locals[i]->canPivotUp(2.0,Flags::Propagate|Flags::NoCheckLayer)) {
        locals[i]->changeDepth( depth+2, Flags::WithNeighbors );

      //cdebug_log(9000,0) << "Deter| Trunk move up L:" << locals[i] << endl;

        vector<GCell*> gcells;
        locals[i]->getGCells( gcells );
        for ( size_t j=0 ; j<gcells.size() ; j++ ) {
          invalidateds.insert( gcells[j] );
        }
      }
    }

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

    GCellDensitySet queue ( depth, getGCells()) );
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
    cdebug_log(9000,0) << "Deter| Balance Global Density" << endl;

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
    cdebug_log(9000,0) << "Deter| Layer Assignment" << endl;

    set<Net*> globalNets;

    unsigned long  total  = 0;
    unsigned long  global = 0;

    startMeasures();
    openSession();

    if (Session::getAllowedDepth() >= 3) {
      switch ( method ) {
        case EngineLayerAssignByLength: _layerAssignByLength( total, global, globalNets ); break;
        case EngineLayerAssignByTrunk:  _layerAssignByTrunk ( total, global, globalNets ); break;
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
  
      if (getConfiguration()->getAllowedDepth() > 2) {
        for ( size_t depth=1 ; depth <= getConfiguration()->getAllowedDepth()-2; ++depth ) {
          _desaturate( depth, globalNets, total, global );
          if ( (depth > 1) and ((depth-1)%2 == 1) ) Session::revalidate();
        }
  
        globalNets.clear ();
        Session::revalidate();
      }
  
#if defined(CHECK_DATABASE)
      _check( "after layer assignment" );
#endif
  
      Session::setAnabaticFlags( Flags::WarnOnGCellOverload );
    }
  
    checkGCellDensities();
    Session::close();

    stopMeasures();
    printMeasures( "assign" );

    // cmess2 << "     - Total segments  : " << total  << endl;
    // cmess2 << "     - Global segments : " << global << endl;
    // cmess2 << "     - Ratio : "
    //        << ((float)global/(float)total)*100.0 << "%." << endl;
  }


}  // Anabatic namespace.
