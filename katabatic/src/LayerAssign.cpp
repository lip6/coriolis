// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
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
#include "hurricane/Layer.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Pad.h"
#include "hurricane/Plug.h"
#include "hurricane/Instance.h"
#include "hurricane/Vertical.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Cell.h"
#include "crlcore/RoutingGauge.h"
#include "katabatic/AutoContact.h"
#include "katabatic/AutoSegment.h"
#include "katabatic/GCellGrid.h"
#include "katabatic/KatabaticEngine.h"


namespace Katabatic {

  using Hurricane::DebugSession;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::ForEachIterator;
  using Hurricane::Warning;


  void  KatabaticEngine::_desaturate ( unsigned int   depth
                                     , set<Net*>&     globalNets
                                     , unsigned long& total
                                     , unsigned long& globals )
  {
    if (depth+2 >  Session::getConfiguration()->getAllowedDepth()) {
      cerr << Warning("Katabatic::_desaturate(): %s, no remaining upper layers."
                     ,getString(Session::getRoutingGauge()->getRoutingLayer(depth)->getName()).c_str()
                     ) << endl;
      return;
    }

    cmess1 << "  o  Desaturate layer "
           << Session::getRoutingGauge()->getRoutingLayer(depth)->getName() << endl;

    GCellDensitySet queue   ( depth, *(_gcellGrid->getGCellVector()) );
    GCell::SetIndex invalidateds;

    bool optimized = true;
    while ( optimized ) {
      Session::revalidate ();
      optimized = false;
      queue.requeue ();

      std::set<GCell*,GCell::CompareByKey>::const_iterator igcell = queue.getGCells().begin();
      size_t i = 0;
      for ( ; igcell!=queue.getGCells().end() ; ++igcell, ++i ) {
        ltrace(400) << "_desaturate: [" << depth << "]:"
                    << (*igcell)->getDensity(depth) << " " << *igcell << endl;

        if (not (*igcell)->isSaturated(depth)) {
          ltrace(400) << "STOP desaturated: @" << i << " " << *igcell << endl;
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
          for ( GCell::SetIndex::iterator igcell=invalidateds.begin() ; igcell!=invalidateds.end() ; ++igcell ) {
            queue.unqueue( *igcell );
          }
          break;
        }
      }
    }
  }


  void  KatabaticEngine::_layerAssignByLength ( Net* net, unsigned long& total, unsigned long& global, set<Net*>& globalNets )
  {
    DebugSession::open ( net, 90 );

    ltrace(100) << "Katabatic::_layerAssignByLength( " << net << " )" << endl;
    ltracein(99);

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

    ltraceout(99);

    DebugSession::close();
  }


  void  KatabaticEngine::_layerAssignByLength ( unsigned long& total, unsigned long& global, set<Net*>& globalNets )
  {
    cmess1 << "  o  Assign Layer (simple wirelength)." << endl;

    forEach ( Net* , inet , getCell()->getNets() ) {
      if (NetRoutingExtension::get(*inet)->isAutomaticGlobalRoute()) {
        _layerAssignByLength ( *inet, total, global, globalNets );
      }
    } // forEach(Net*)
  }


  void  KatabaticEngine::_layerAssignByTrunk ( Net* net, set<Net*>& globalNets, unsigned long& total, unsigned long& global )
  {
    DebugSession::open( net, 90 );

    ltrace(100) << "Katabatic::_layerAssignByTrunk ( " << net << " )" << endl;
    ltracein(99);

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

          ltrace(99) << "Migrate to M4/M5: " << autoSegment << endl;
          if (autoSegment->isHorizontal()) autoSegment->setLayer( Session::getRoutingLayer(3) );
          if (autoSegment->isVertical  ()) autoSegment->setLayer( Session::getRoutingLayer(4) );
        }
      }
    }

    total  += netTotal;
    global += netGlobal;

    ltraceout(99);

    DebugSession::close();
  }


  void  KatabaticEngine::_layerAssignByTrunk ( unsigned long& total, unsigned long& global, set<Net*>& globalNets )
  {
    cmess1 << "  o  Assign Layer (whole net trunk)." << endl;

    forEach ( Net* , inet , getCell()->getNets() ) {
      if (NetRoutingExtension::get(*inet)->isAutomaticGlobalRoute()) {
        _layerAssignByTrunk( *inet, globalNets, total, global );
      }
    }
  }


  void  KatabaticEngine::moveULeft ( AutoSegment* seed, set<Net*>& globalNets, GCell::SetIndex& invalidateds )
  {
    Net* net = seed->getNet();
    DebugSession::open( net, 90 );

    ltrace(500) << "Deter| Move left: " << seed << endl;

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


  void  KatabaticEngine::moveURight ( AutoSegment* seed, set<Net*>& globalNets, GCell::SetIndex& invalidateds )
  {
    Net* net = seed->getNet();
    DebugSession::open( net, 90 );

    ltrace(500) << "Deter| Move right: " << seed << endl;

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


  bool  KatabaticEngine::moveUpNetTrunk ( AutoSegment* seed, set<Net*>& globalNets, GCell::SetIndex& invalidateds )
  {
    Net*         net       = seed->getNet();
    unsigned int seedDepth = Session::getRoutingGauge()->getLayerDepth(seed->getLayer());

    DebugSession::open( net, 90 );
    ltrace(500) << "Deter| moveUpNetTrunk() depth:" << seedDepth << " " << seed << endl;

    if (not seed->canMoveUp( 1.0, KbPropagate|KbAllowTerminal|KbNoCheckLayer) ) {
      ltrace(500) << "Deter| Reject seed move up, cannot move up." << endl;
      DebugSession::close();
      return false;
    }
    ltracein(400);

    globalNets.insert( net );

    vector<AutoSegment*> globals;
    vector<AutoSegment*> locals;

    forEach ( Segment*, isegment, net->getSegments() ) {
      AutoSegment* autoSegment = Session::lookup( *isegment );

    //ltrace(500) << "Deter| Loop " << autoSegment << endl;

      if (not autoSegment) continue;
      if (autoSegment->isLocal()) {
        if (autoSegment->isStrongTerminal()) continue;
        locals.push_back( autoSegment );
      } else {
      // Ugly: Hard-coded GCell side.
        if ( (autoSegment->getLength() < 3*Session::getSliceHeight()) and (autoSegment != seed) )
          locals.push_back( autoSegment );
        else
          globals.push_back( autoSegment );
      }
    }

    sort( globals.begin(), globals.end(), AutoSegment::CompareId() );
    sort(  locals.begin(),  locals.end(), AutoSegment::CompareId() );

    for ( size_t i=0 ; i<globals.size() ; ++i ) {
    //ltrace(500) << "Deter| Looking up G:" << globals[i] << endl;

      unsigned int depth = Session::getRoutingGauge()->getLayerDepth( globals[i]->getLayer() );
      if (depth > seedDepth+1) continue;

      if ( globals[i]->isVertical() and (globals[i] != seed) ) continue;

      if ( globals[i]->canMoveUp(1.0,KbPropagate|KbAllowTerminal|KbNoCheckLayer) ) {
        globals[i]->changeDepth( depth+2, KbWithNeighbors );

      //ltrace(500) << "Deter| Trunk move up G:" << globals[i] << endl;

        vector<GCell*> gcells;
        globals[i]->getGCells( gcells );
        for ( size_t j=0 ; j<gcells.size() ; j++ ) {
          invalidateds.insert( gcells[j] );
        }
      } else {
      //ltrace(500) << "Deter| Reject Trunk move up G:" << globals[i] << endl;
      }
    }

    for ( size_t i=0 ; i<locals.size() ; ++i ) {
    //ltrace(500) << "Deter| Looking up L:" << locals[i] << endl;

      unsigned int depth = Session::getRoutingGauge()->getLayerDepth(locals[i]->getLayer());
      if (depth > seedDepth+1) continue;

      if (locals[i]->canPivotUp(2.0,KbPropagate|KbNoCheckLayer)) {
        locals[i]->changeDepth( depth+2, KbWithNeighbors );

      //ltrace(500) << "Deter| Trunk move up L:" << locals[i] << endl;

        vector<GCell*> gcells;
        locals[i]->getGCells( gcells );
        for ( size_t j=0 ; j<gcells.size() ; j++ ) {
          invalidateds.insert( gcells[j] );
        }
      }
    }

    ltraceout(400);
    DebugSession::close();

    return true;
  }


  void  KatabaticEngine::_balanceGlobalDensity ( unsigned int depth )
  {
    startMeasures();
    Session::open( this );

    cmess1 << "  o  Balance Global Density "
           << Session::getRoutingGauge()->getRoutingLayer(depth)->getName() << endl;

    GCellDensitySet queue ( depth, *(_gcellGrid->getGCellVector()) );
    GCell::SetIndex invalidateds;

    bool optimized = true;
    while ( optimized ) {
      Session::revalidate();
      optimized = false;
      queue.requeue();

      std::set<GCell*,GCell::CompareByKey>::const_iterator igcell = queue.getGCells().begin();
      size_t i = 0;
      for ( ; igcell!=queue.getGCells().end() ; ++igcell, ++i ) {
        ltrace(400) << "_balance: [" << depth << "]:"
                    << (*igcell)->getDensity(depth) << " " << *igcell << endl;

        if (not (*igcell)->isSaturated(depth)) {
          ltrace(400) << "STOP desaturated: @" << i << " " << *igcell << endl;
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
          for ( GCell::SetIndex::iterator igcell=invalidateds.begin() ; igcell!=invalidateds.end() ; ++igcell ) {
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


  void  KatabaticEngine::balanceGlobalDensity ()
  {
    ltrace(500) << "Deter| Balance Global Density" << endl;

  //_balanceGlobalDensity( 1 ); // metal2
  //_balanceGlobalDensity( 2 ); // metal3

    set<Net*>        globalNets;
    GCell::SetIndex  invalidateds;

    Session::open( this );

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
        getGCellGrid()->getKatabatic()->moveULeft(segments[i],globalNets,invalidateds);
      } else if (segments[i]->canMoveURight(0.10)) {
        getGCellGrid()->getKatabatic()->moveURight(segments[i],globalNets,invalidateds);
      }

      for ( GCell::SetIndex::iterator igcell=invalidateds.begin() ; igcell!=invalidateds.end() ; ++igcell ) {
        (*igcell)->updateDensity();
      }
      invalidateds.clear();
      Session::revalidate();
    }

    Session::close();
  }


  void  KatabaticEngine::layerAssign ( unsigned int method )
  {
    ltrace(500) << "Deter| Layer Assignment" << endl;

    set<Net*> globalNets;

    unsigned long  total  = 0;
    unsigned long  global = 0;

    startMeasures();
    Session::open( this );

    if (Session::getAllowedDepth() >= 3) {
      switch ( method ) {
        case EngineLayerAssignByLength: _layerAssignByLength( total, global, globalNets ); break;
        case EngineLayerAssignByTrunk:  _layerAssignByTrunk ( total, global, globalNets ); break;
        case EngineNoNetLayerAssign:    break;
        default:
          stopMeasures();
          Session::close();
          throw Error( badMethod
                     , "Katabatic::layerAssign()"
                     , method
                     , getString(_cell).c_str()
                     );
      }
  
      globalNets.clear();
      Session::revalidate();
  
      if (getConfiguration()->getAllowedDepth() > 2) {
      //for ( int i=0 ; i < 3 ; i++ ) {
          for ( size_t depth=1 ; depth <= getConfiguration()->getAllowedDepth()-2; ++depth ) {
            _desaturate( depth, globalNets, total, global );
            if ( (depth > 1) and ((depth-1)%2 == 1) ) Session::revalidate();
          }
  
          globalNets.clear ();
  
      //  if (not _gcellGrid->updateDensity()) break;
      //}
        Session::revalidate();
      }
  
#if defined(CHECK_DATABASE)
      _check( "after layer assignment" );
#endif
  
      Session::setKatabaticFlags( EngineWarnOnGCellOverload );
    }
  
    _gcellGrid->checkDensity();
    Session::close();

    stopMeasures();
    printMeasures( "assign" );

    // cmess2 << "     - Total segments  : " << total  << endl;
    // cmess2 << "     - Global segments : " << global << endl;
    // cmess2 << "     - Ratio : "
    //        << ((float)global/(float)total)*100.0 << "%." << endl;
  }


}  // Katabatic namespace.
