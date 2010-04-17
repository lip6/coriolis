
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
    if ( depth+2 >= Session::getRoutingGauge()->getDepth() ) {
      cerr << Warning("Katabatic::_desaturate(): %s, no remaining upper layers."
                     ,getString(Session::getRoutingGauge()->getRoutingLayer(depth)->getName()).c_str()
                     ) << endl;
      return;
    }

    cmess1 << "  o  Desaturate layer "
           << Session::getRoutingGauge()->getRoutingLayer(depth)->getName() << endl;

  //vector<GCell*> gcells  = *(_gcellGrid->getGCellVector());
    DyKeyQueue     queue   ( depth, *(_gcellGrid->getGCellVector()) );
    AutoSegment*   segment = NULL;
    vector<GCell*> invalidateds;

    bool optimized = true;
    while ( optimized ) {
      optimized = false;
    //sort ( gcells.begin(), gcells.end(), GCell::CompareByDensity(depth) );
      queue.revalidate ();

      std::set<GCell*,GCell::CompareByKey>::const_iterator igcell = queue.getGCells().begin();
      size_t i = 0;
      for ( ; igcell!=queue.getGCells().end() ; ++igcell, ++i ) {
        if ( not (*igcell)->isSaturated ( depth ) ) {
          ltrace(190) << "STOP desaturated: @" << i << " " << *igcell << endl;
          for ( ; igcell!=queue.getGCells().end() ; ++igcell ) {
            if ( (*igcell)->isSaturated ( depth ) ) {
              cerr << "[ERROR] Still saturated: @" << i << " " << *igcell << endl;
              break;
            }
          }
          break;
        }

        ltrace(190) << "step desaturate @: " << i << " " << *igcell << endl;

        optimized = (*igcell)->stepDesaturate ( depth, globalNets, segment );

        if ( segment ) {
          ++total; ++globals;
          segment->getGCells ( invalidateds );
          for ( size_t j=0 ; j<invalidateds.size() ; j++ ) {
            queue.invalidate ( invalidateds[j] );
          }
        }

        if ( optimized ) break;
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

    for ( size_t i=0 ; i < _routingNets.size() ; i++ )
      _layerAssignByLength ( _routingNets[i], total, global, globalNets );
  }


  void  KatabaticEngine::_layerAssignByTrunk ( Net* net, unsigned long& total, unsigned long& global, set<Net*>& globalNets )
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

    for ( size_t i=0 ; i < _routingNets.size() ; i++ )
      _layerAssignByTrunk ( _routingNets[i], total, global, globalNets );
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

    for ( int i=0 ; i < 3 ; i++ ) {
      _desaturate ( 1, globalNets, total, global );
      _desaturate ( 2, globalNets, total, global );

      globalNets.clear ();

      Session::revalidate ();
      if ( !_gcellGrid->updateDensity () ) break;
    }
  //refresh ( false );

#if defined(CHECK_DATABASE)
    _check ( "after layer assignment" );
#endif
#if defined(CHECK_DETERMINISM)
    for ( size_t i=0 ; i < _routingNets.size() ; i++ )
      _print ( _routingNets[i] );
#endif

  // Look for RoutingPad overload.
    vector<GCell*> gcells = *(_gcellGrid->getGCellVector());
    for ( size_t i=0 ; i<gcells.size() ; ++i ) {
      gcells[i]->rpDesaturate ( globalNets );
      // set<RoutingPad*> rps;
      // gcells[i]->getRoutingPads ( rps );

      // if ( rps.size() > 7 ) {
      //   cerr << "[WARNING] " << gcells[i] << "has " << rps.size() << " terminals." << endl;
      // }
    }

    Session::revalidate ();

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
