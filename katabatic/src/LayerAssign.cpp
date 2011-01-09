
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./LayerAssign.cpp"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <cstdlib>
#include  <sstream>

#include  "hurricane/Bug.h"
#include  "hurricane/Warning.h"
#include  "hurricane/DebugSession.h"
#include  "hurricane/Net.h"
#include  "hurricane/NetExternalComponents.h"
#include  "hurricane/Layer.h"
#include  "hurricane/RoutingPad.h"
#include  "hurricane/Pad.h"
#include  "hurricane/Plug.h"
#include  "hurricane/Instance.h"
#include  "hurricane/Vertical.h"
#include  "hurricane/Horizontal.h"

#include  "crlcore/RoutingGauge.h"

#include  "katabatic/AutoContact.h"
#include  "katabatic/AutoSegment.h"
#include  "katabatic/GCellGrid.h"
#include  "katabatic/KatabaticEngine.h"


namespace Katabatic {

  using Hurricane::DebugSession;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::ForEachIterator;
  using Hurricane::Warning;


  void  KatabaticEngine::_splitContactsOfNet ( Net* net )
  {
    DebugSession::open ( net, 90 );

    ltrace(100) << "Katabatic::_splitContactsOfNet ( " << net << " )" << endl;
    ltracein(99);

    Session::invalidate ( net );
    set<AutoContact*>  globalContacts;

    forEach ( Segment*, isegment, net->getSegments() ) {
      if (   ( (*isegment)->getLayer() == Session::getRoutingLayer(3) )
          || ( (*isegment)->getLayer() == Session::getRoutingLayer(4) ) ) {
        AutoContact* contact = Session::lookup ( dynamic_cast<Contact*>((*isegment)->getSource()) );
        if ( contact ) globalContacts.insert ( contact );

        contact = Session::lookup ( dynamic_cast<Contact*>((*isegment)->getTarget()) );
        if ( contact ) globalContacts.insert ( contact );
      }
    }

    set<AutoContact*>::iterator  it = globalContacts.begin();
    for ( ; it != globalContacts.end() ; it++ ) {
      (*it)->split ();
    }

    ltraceout(99);

    DebugSession::close ();
  }


  void  KatabaticEngine::_desaturate ( unsigned int   depth
                                     , set<Net*>&     globalNets
                                     , unsigned long& total
                                     , unsigned long& globals )
  {
    if ( depth+2 >= Session::getConfiguration()->getAllowedDepth() ) {
      cerr << Warning("Katabatic::_desaturate(): %s, no remaining upper layers."
                     ,getString(Session::getRoutingGauge()->getRoutingLayer(depth)->getName()).c_str()
                     ) << endl;
      return;
    }

    cmess1 << "  o  Desaturate layer "
           << Session::getRoutingGauge()->getRoutingLayer(depth)->getName() << endl;

  //vector<GCell*> gcells  = *(_gcellGrid->getGCellVector());
  //vector<GCell*> invalidateds;
    DyKeyQueue    queue   ( depth, *(_gcellGrid->getGCellVector()) );
    GCell::SetId  invalidateds;

    bool optimized = true;
    while ( optimized ) {
      Session::revalidate ();
      optimized = false;
    //sort ( gcells.begin(), gcells.end(), GCell::CompareByDensity(depth) );
      queue.revalidate ();

      std::set<GCell*,GCell::CompareByKey>::const_iterator igcell = queue.getGCells().begin();
      size_t i = 0;
      for ( ; igcell!=queue.getGCells().end() ; ++igcell, ++i ) {
        ltrace(400) << "_desaturate: [" << depth << "]:"
                    << (*igcell)->getDensity(depth) << " " << *igcell << endl;

        if ( not (*igcell)->isSaturated ( depth ) ) {
          ltrace(400) << "STOP desaturated: @" << i << " " << *igcell << endl;
          for ( ; igcell!=queue.getGCells().end() ; ++igcell ) {
            if ( (*igcell)->isSaturated ( depth ) ) {
              cerr << "[ERROR] Still saturated: @" << i << " " << *igcell << endl;
              break;
            }
          }
          break;
        }

      //ltrace(190) << "step desaturate: @ " << i << " " << *igcell << endl;

        // AutoSegment* segment = NULL;
        // optimized = (*igcell)->stepDesaturate ( depth, globalNets, segment );

        // if ( segment ) {
        //   ++total; ++globals;
        //   segment->getGCells ( invalidateds );
        //   for ( size_t j=0 ; j<invalidateds.size() ; j++ ) {
        //     queue.invalidate ( invalidateds[j] );
        //   }
        // }
        // if ( optimized ) break;

        optimized = (*igcell)->stepNetDesaturate ( depth, globalNets, invalidateds );
        if ( optimized ) {
          for ( GCell::SetId::iterator igcell=invalidateds.begin() ; igcell!=invalidateds.end() ; ++igcell ) {
            queue.invalidate ( *igcell );
          }
          break;
        }
      }
    }
  }


  void  KatabaticEngine::_layerAssignByLength ( Net* net, unsigned long& total, unsigned long& global, set<Net*>& globalNets )
  {
    DebugSession::open ( net, 90 );

    ltrace(100) << "Katabatic::_layerAssignByLength ( " << net << " )" << endl;
    ltracein(99);

    bool               isGlobal = false;
    set<AutoContact*>  globalContacts;

    forEach ( Segment*, isegment, net->getSegments() ) {
      total++;
      if ( (*isegment)->getLength() > getGlobalThreshold() ) {
        if ( !isGlobal ) {
          isGlobal = true;
          globalNets.insert ( net );
        //cmess2 << "     - " << getString(net) << endl;
        }

        global++;
        if ( (*isegment)->getLayer() == Session::getRoutingLayer(1) ) (*isegment)->setLayer ( Session::getRoutingLayer(3) );
        if ( (*isegment)->getLayer() == Session::getRoutingLayer(2) ) (*isegment)->setLayer ( Session::getRoutingLayer(4) );
      }
    }

    ltraceout(99);

    DebugSession::close ();
  }


  void  KatabaticEngine::_layerAssignByLength ( unsigned long& total, unsigned long& global, set<Net*>& globalNets )
  {
    cmess1 << "  o  Assign Layer (simple wirelength)." << endl;

    NetSet::iterator  inet = _routingNets.begin();
    for ( ; inet != _routingNets.end() ; ++inet )
      _layerAssignByLength ( *inet, total, global, globalNets );
  }


  void  KatabaticEngine::_layerAssignByTrunk ( Net* net, set<Net*>& globalNets, unsigned long& total, unsigned long& global )
  {
    DebugSession::open ( net, 90 );

    ltrace(100) << "Katabatic::_layerAssignByTrunk ( " << net << " )" << endl;
    ltracein(99);

    bool               isGlobal  = false;
    unsigned long      netGlobal = 0;
    unsigned long      netTotal  = 0;
    set<AutoContact*>  globalContacts;

    forEach ( Segment*, isegment, net->getSegments() ) {
      netTotal++;

      if ( (*isegment)->getLength() > getGlobalThreshold() ) {
        isGlobal = true;
        netTotal = 0;
        globalNets.insert ( net );
        break;
      }
    }

    if ( isGlobal ) {
    //cmess2 << "     - " << getString(net) << endl;

      forEach ( Segment*, isegment, net->getSegments() ) {
        netTotal++;

        AutoSegment* autoSegment = Session::lookup ( *isegment );
        if ( autoSegment && !autoSegment->isTerminal() ) {
          netGlobal++;

          ltrace(99) << "Migrate to M4/M5: " << autoSegment << endl;
          if ( autoSegment->isHorizontal() ) autoSegment->setLayer ( Session::getRoutingLayer(3) );
          if ( autoSegment->isVertical  () ) autoSegment->setLayer ( Session::getRoutingLayer(4) );
        }
      }
    }

    total  += netTotal;
    global += netGlobal;

    ltraceout(99);

    DebugSession::close ();
  }


  void  KatabaticEngine::_layerAssignByTrunk ( unsigned long& total, unsigned long& global, set<Net*>& globalNets )
  {
    cmess1 << "  o  Assign Layer (whole net trunk)." << endl;

    NetSet::iterator inet = _routingNets.begin();
    for ( ; inet != _routingNets.end() ; ++inet )
      _layerAssignByTrunk ( *inet, globalNets, total, global );
  }


  bool  KatabaticEngine::_moveUpNetTrunk ( AutoSegment* seed, set<Net*>& globalNets, GCell::SetId& invalidateds )
  {
    Net* net = seed->getNet();
    DebugSession::open ( net, 90 );

    ltrace(400) << "_moveUpNetTrunk() - " << seed << endl;

    if ( not seed->canMoveUp(1.0,AutoSegment::Propagate|AutoSegment::AllowTerminal) ) {
      DebugSession::close ();
      return false;
    }
    ltracein(400);

    globalNets.insert ( net );

    vector<AutoSegment*> globals;
    vector<AutoSegment*> locals;

    forEach ( Segment*, isegment, net->getSegments() ) {
      AutoSegment* autoSegment = Session::lookup ( *isegment );

      if ( not autoSegment ) continue;
      if ( autoSegment->isLocal() ) {
        if ( autoSegment->isTerminal() ) continue;
        locals.push_back ( autoSegment );
      } else {
      // Ugly: Hard-coded GCell side.
        if ( (autoSegment->getLength() < DbU::lambda(150.0)) and (autoSegment != seed) )
          locals.push_back ( autoSegment );
        else
          globals.push_back ( autoSegment );
      }
    }

    sort ( globals.begin(), globals.end(), AutoSegment::CompareId() );
    sort (  locals.begin(),  locals.end(), AutoSegment::CompareId() );

    for ( size_t i=0 ; i<globals.size() ; ++i ) {
      ltrace(400) << "  | Looking up G:" << globals[i] << endl;

      if ( globals[i]->canMoveUp(1.0,AutoSegment::Propagate|AutoSegment::AllowTerminal) ) {
        unsigned int depth = Session::getRoutingGauge()->getLayerDepth(globals[i]->getLayer());
        globals[i]->changeDepth ( depth+2, true, false );

        ltrace(400) << "  | Trunk move up G:" << globals[i] << endl;

        vector<GCell*> gcells;
        globals[i]->getGCells ( gcells );
        for ( size_t j=0 ; j<gcells.size() ; j++ ) {
          invalidateds.insert ( gcells[j] );
        }
      } else {
        ltrace(400) << "  | Reject Trunk move up G:" << globals[i] << endl;
      }
    }

    for ( size_t i=0 ; i<locals.size() ; ++i ) {
      ltrace(400) << "  | Looking up L:" << locals[i] << endl;

    //if ( locals[i]->canPivotUp(2.0,AutoSegment::Propagate|AutoSegment::IgnoreContact) ) {
      if ( locals[i]->canPivotUp(2.0,AutoSegment::Propagate) ) {
        unsigned int depth = Session::getRoutingGauge()->getLayerDepth(locals[i]->getLayer());
        locals[i]->changeDepth ( depth+2, true, false );

        ltrace(400) << "  | Trunk move up L:" << locals[i] << endl;

        vector<GCell*> gcells;
        locals[i]->getGCells ( gcells );
        for ( size_t j=0 ; j<gcells.size() ; j++ ) {
          invalidateds.insert ( gcells[j] );
        }
      }
    }

    ltraceout(400);
    DebugSession::close ();

    return true;
  }


  void  KatabaticEngine::layerAssign ( unsigned int method )
  {
    set<Net*> globalNets;

    unsigned long  total  = 0;
    unsigned long  global = 0;

    startMeasures ();
    Session::open ( this );

    switch ( method ) {
      case LayerAssignByLength: _layerAssignByLength(total,global,globalNets); break;
      case LayerAssignByTrunk:  _layerAssignByTrunk (total,global,globalNets); break;
      case NoNetLayerAssign:    break;
      default:
        stopMeasures ();
        Session::close ();
        throw Error ( badMethod
                    , "Katabatic::layerAssign()"
                    , method
                    , getString(_cell).c_str()
                    );
    }

    set<Net*>::iterator inet = globalNets.begin();
    for ( ; inet != globalNets.end() ; inet++ )
      _splitContactsOfNet ( *inet );
    globalNets.clear ();

    Session::revalidate ();

  // Look for RoutingPad overload.
    // vector<GCell*> gcells = *(_gcellGrid->getGCellVector());
    // for ( size_t i=0 ; i<gcells.size() ; ++i ) {
    //   gcells[i]->rpDesaturate ( globalNets );
    // }

    if ( getConfiguration()->getAllowedDepth() > 2) {
      for ( int i=0 ; i < 3 ; i++ ) {
        for ( size_t depth=1 ; depth < getConfiguration()->getAllowedDepth()-2; ++depth ) {
          _desaturate ( depth, globalNets, total, global );
          if ( (depth > 1) and ((depth-1)%2 == 1) ) Session::revalidate ();
        }

        globalNets.clear ();

        if ( not _gcellGrid->updateDensity () ) break;
      }
      Session::revalidate ();
    }
  //refresh ( false );

#if defined(CHECK_DATABASE)
    _check ( "after layer assignment" );
#endif
#if defined(CHECK_DETERMINISM)
    NetSet::iterator inet = _routingNets.begin();
    for ( ; inet != _routingNets.end() ; inet++ )
      _print ( *inet );
#endif

    Session::setWarnGCellOverload ( true );
    _gcellGrid->checkDensity ();

    Session::close ();

    stopMeasures ();
    printMeasures ( "assign" );

    cmess2 << "     - Total segments  : " << total  << endl;
    cmess2 << "     - Global segments : " << global << endl;
    cmess2 << "     - Ratio : "
           << ((float)global/(float)total)*100.0 << "%." << endl;
  }


} // End of Katabatic namespace.
