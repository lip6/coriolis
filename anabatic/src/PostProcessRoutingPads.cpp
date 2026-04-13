// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2025-2025.
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :      "./PostProcessRoutingPads.cpp"              |
// +-----------------------------------------------------------------+


#include <map>
#include <list>
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/DebugSession.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/RegularLayer.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/Rectilinear.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Occurrence.h"
#include "anabatic/AutoContactTerminal.h"
#include "anabatic/AutoSegment.h"
#include "anabatic/AnabaticEngine.h"


namespace {

  using namespace std;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::DebugSession;
  using Hurricane::tab;
  using Hurricane::ForEachIterator;
  using Hurricane::DbU;
  using Hurricane::Box;
  using Hurricane::Interval;
  using Hurricane::Go;
  using Hurricane::Layer;
  using Hurricane::BasicLayer;
  using Hurricane::RegularLayer;
  using Hurricane::Pin;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Hurricane::Rectilinear;
  using Hurricane::Plug;
  using Hurricane::Transformation;
  using Hurricane::RoutingPad;
  using Hurricane::Occurrence;
  using Hurricane::Path;
  using Hurricane::NetRoutingExtension;
  using Hurricane::NetRoutingState;
  using namespace Anabatic;


  bool  checkForLoopHV ( const vector<AutoContactTerminal*>& terminals )
  {
    cdebug_log(145,1) << "checkForLoopHV()" << endl;

    vector< pair<AutoContactTerminal*,AutoSegment*> >  prefs;
    vector< pair<AutoContactTerminal*,AutoSegment*> >  nonprefs;

    for ( AutoContactTerminal* terminal : terminals ) {
      if (terminal->getSegment()->isHorizontal()) {
        prefs.push_back( make_pair( terminal, terminal->getSegment() ));
      } else {
        nonprefs.push_back( make_pair( terminal, terminal->getSegment() ));
      }
    }

    cdebug_log(145,0) << "prefs:" << prefs.size() << " nonprefs:" << nonprefs.size() << endl;

    if ((prefs.size() == 1) and (nonprefs.size() == 1)) {
      cdebug_log(145,0) << "Case mixed pref + non-pref" << endl;
      if (prefs[0].second->isGlobal()) {
        cdebug_log(145,0) << "Pref is global -> false" << endl;
        cdebug_tabw(145,-1);
        return false;
      }
      if (prefs[0].second->getAnchoredLength() > prefs[0].second->getPitch()) {
        cdebug_log(145,0) << "Non-pref is longer than 1 pitch -> false" << endl;
        cdebug_tabw(145,-1);
        return false;
      }

      AutoSegment* ppPref    =    prefs[0].second->getOppositeAnchor(    prefs[0].first )->getPerpandicular(    prefs[0].second ); 
      AutoSegment* ppNonpref = nonprefs[0].second->getOppositeAnchor( nonprefs[0].first )->getPerpandicular( nonprefs[0].second ); 
      if (not ppPref or not ppNonpref) {
        cdebug_log(145,0) << "pp-pref or pp-nopref are missing -> false" << endl;
        cdebug_tabw(145,-1);
        return false;
      }
      if (not ppPref->isReduced()) {
        cdebug_log(145,0) << "Non-pref is longer than 1 pitch -> false" << endl;
        cdebug_tabw(145,-1);
        return false;
      }

      cdebug_log(145,0) << "P+NP Loop suppression on " << ppPref << endl;
      ppPref->setAxis( nonprefs[0].second->getAxis() );
      cdebug_tabw(145,-1);
      return true;
    }

    if (nonprefs.size() == 2) {
      cdebug_log(145,0) << "Case 2 non-prefs" << endl;
      bool isSource0 = (nonprefs[0].first == nonprefs[0].second->getAutoSource());
      bool isSource1 = (nonprefs[1].first == nonprefs[1].second->getAutoSource());
      if (isSource0 xor isSource1) {
        cdebug_log(145,0) << "Going opposite direction -> false" << endl;
        cdebug_tabw(145,-1);
        return false;
      }

      bool shorter0 = (nonprefs[0].second->getLength() < nonprefs[1].second->getLength());
      if (shorter0)
        nonprefs[0].second->setAxis( nonprefs[1].second->getAxis() );
      else
        nonprefs[1].second->setAxis( nonprefs[0].second->getAxis() );
      cdebug_log(145,0) << "Same direction, aligning shorter" << endl;
      cdebug_tabw(145,-1);
      return true;
    }

    if (prefs.size() == 2) {
      cdebug_log(145,0) << "Case 2 prefs" << endl;
      bool isSource0 = (prefs[0].first == prefs[0].second->getAutoSource());
      bool isSource1 = (prefs[1].first == prefs[1].second->getAutoSource());
      if (isSource0 xor isSource1) {
        cdebug_log(145,0) << "Going opposite direction -> false" << endl;
        cdebug_tabw(145,-1);
        return false;
      }

      AutoSegment* ppPref0 = prefs[0].second->getOppositeAnchor( prefs[0].first )->getPerpandicular( prefs[0].second ); 
      AutoSegment* ppPref1 = prefs[1].second->getOppositeAnchor( prefs[1].first )->getPerpandicular( prefs[1].second ); 
      if (not ppPref0 or not ppPref1) {
        cdebug_log(145,0) << "No simple perpandicular -> false" << endl;
        cdebug_tabw(145,-1);
        return false;
      }
      cdebug_log(145,0) << "ppPref0 " << ppPref0 << endl;
      cdebug_log(145,0) << "ppPref1 " << ppPref1 << endl;
      if (ppPref0->getAxis() == ppPref1->getAxis()) {
        cdebug_log(145,0) << "Perpandicular axis identicals (already aligned) -> false" << endl;
        cdebug_tabw(145,-1);
        return false;
      }

      if (not ppPref0->isReduced() or not ppPref1->isReduced()) {
        cdebug_log(145,0) << "At least one perdandicular not reduced -> false" << endl;
        cdebug_tabw(145,-1);
        return false;
      }

      if (std::abs(ppPref0->getAxis() - ppPref1->getAxis()) <= ppPref0->getPitch() + ppPref0->getPitch()/2) {
        DbU::Unit xRP  = prefs[0].first->getAnchor()->getBoundingBox().getXCenter();
        DbU::Unit axis = ppPref0->getAxis();
        if (std::abs(ppPref1->getAxis() - xRP) < std::abs(ppPref0->getAxis() - xRP))
          axis = ppPref1->getAxis();
        cdebug_log(145,0) << "Align on the vertical closest to the terminal axis="
                          << DbU::getValueString(axis) << endl;
        ppPref0->setAxis( axis );
        ppPref1->setAxis( axis );
        cdebug_tabw(145,-1);
        return true;
      }
    }
    
    cdebug_log(145,0) << "Not a managed case -> false" << endl;
    cdebug_tabw(145,-1);
    return false;
  }
  

  void  postProcessRoutingPad ( RoutingPad* rp )
  {
    cdebug_log(145,1) << "Post-process " << rp << endl;

    DbU::Unit pitch = Session::getPitch( 1 );

    vector<AutoContactTerminal*> vias;
    for ( Contact* contact : rp->getSlaveComponents().getSubSet<Contact*>() ) {
      AutoContact* via = Session::lookup( contact );
      if (via)
        vias.push_back( dynamic_cast<AutoContactTerminal*>( via ));
    }
    if (vias.size() < 2) {
      cdebug_log(145,0) << "Less than 2 VIAs attached, skipping." << endl;
      cdebug_tabw(145,-1);
      return;
    }
    if (vias.size() > 3) {
      cerr << Warning( "postProcessRoutingPads(): More than three VIAs on %s."
                     , getString(rp).c_str() ) << endl;
      cdebug_tabw(145,-1);
      return;
    }

    vector< vector<AutoContactTerminal*> >  overlappings;
    for ( AutoContactTerminal* via : vias ) {
      bool sorted = false;
      for ( auto& overlapping : overlappings ) {
        for ( AutoContactTerminal* sortedVia : overlapping ) {
          DbU::Unit dx = std::abs( sortedVia->getX() - via->getX() );
          DbU::Unit dy = std::abs( sortedVia->getY() - via->getY() );
          if ((dx < pitch) and (dy < pitch)) {
            cdebug_log(145,0) << "Overlapping: " << via << endl;
            overlapping.push_back( via );
            sorted = true;
            break;
          }
        }
        if (sorted) break;
      }
      if (sorted) continue;

      cdebug_log(145,0) << "Isolated: " << via << endl;
      overlappings.push_back( vector<AutoContactTerminal*>() );
      overlappings.back().push_back( via );
    }

    for ( auto overlapping : overlappings ) {
      if (overlapping.size() < 2) {
        cdebug_log(145,0) << "Less than 2 VIAs in the overlapping set, skipping." << endl;
        continue;
      }

      for ( size_t i=1 ; i<overlapping.size() ; ++i ) {
        BasicLayer* layer = overlapping[i]->getSegment()->getLayer()->getBasicLayers().getFirst();
        Box         viaBb = Box( overlapping[i]->base()->getBoundingBox( layer ));
        overlapping[i]->setLayer( overlapping[i]->getSegment()->getLayer() );
        overlapping[i]->setSizes( viaBb.getWidth(), viaBb.getHeight() );
      }

      bool loopRemoved = checkForLoopHV( overlapping );

      if (not loopRemoved) {
        BasicLayer* layer  = overlapping[0]->getSegment()->getLayer()->getBasicLayers().getFirst();
        Box         viaBb1 = Box( overlapping[0]->base()->getBoundingBox( layer ));
        Horizontal* strap  = Horizontal::create( overlapping[0]->getNet()
                                               , layer
                                               , overlapping[0]->getY()
                                               , viaBb1.getHeight()
                                               , overlapping[0]->getX()
                                               , overlapping[1]->getX()
                                               );
        cdebug_log(145,0) << "Added strap " << strap << endl;
      }
    }

    cdebug_tabw(145,-1);
  }
  

} // End of anonymous namespace.


namespace Anabatic {

  using Hurricane::Breakpoint;
  using Hurricane::DataBase;
  using Hurricane::Technology;
  using Hurricane::BasicLayer;
  using Hurricane::Cell;


  void  AnabaticEngine::_postProcessRoutingPads ()
  {
    // Breakpoint::stop( 0, "AnabaticEngine::_postProcessRoutingPads()" );
    cmess1 << "  o  Post-process RoutingPads." << endl;

  //openSession();

    for ( Net* net : getCell()->getNets() ) {
      if (net->isSupply()) continue;

      DebugSession::open( net, 145, 150 );
      cdebug_log(145,0) << "Post-process RoutingPads of " << net << endl;

      NetData* data = getNetData( net );
      if (data and data->isFixed()) continue;

      vector<RoutingPad*> rps;
      for ( RoutingPad* rp : net->getRoutingPads() )
        rps.push_back( rp );

      for ( size_t i=0 ; i<rps.size() ; ++i )
        postProcessRoutingPad( rps[i] );

      DebugSession::close();
    }

    Session::revalidate();
  //Session::close();
  }


}  // Anabatic namespace.
