
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |         A n a b a t i c  -  Routing Toolbox                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./NetOptimals.cpp"                        |
// +-----------------------------------------------------------------+


#include <cstdlib>
#include <sstream>
#include "hurricane/DebugSession.h"
#include "hurricane/Net.h"
#include "hurricane/Segment.h"
#include "anabatic/Session.h"
#include "anabatic/AutoSegment.h"
#include "anabatic/AnabaticEngine.h"


namespace Anabatic {

  using namespace std;
  using Hurricane::tab;
  using Hurricane::ForEachIterator;
  using Hurricane::Net;
  using Hurricane::Segment;
  using Hurricane::DebugSession;


  void  AnabaticEngine::_computeNetOptimals ( Net* net )
  {
    DebugSession::open( net, 140, 150 );
    cdebug_log(149,0) << "Anabatic::_computeNetOptimals( " << net << " )" << endl;
    cdebug_tabw(145,1);

    vector<AutoSegment*> segments;
    forEach ( Segment*, segment, net->getSegments() ) {
      AutoSegment* autoSegment = Session::lookup( *segment );
      if (autoSegment) segments.push_back( autoSegment );
    }
    sort( segments.begin(), segments.end(), AutoSegment::CompareId() );

    set<AutoSegment*> processeds;
    for ( size_t i=0 ; i<segments.size() ; i++ )
      segments[i]->computeOptimal( processeds );

    cdebug_tabw(145,-1);
    DebugSession::close();
  }


  void  AnabaticEngine::toOptimals ( Net* net )
  {
    DebugSession::open( net, 140, 150 );
    cdebug_log(149,0) << "Anabatic::_toOptimals( " << net << " )" << endl;
    cdebug_tabw(145,1);

    vector<AutoSegment*> segments;
   forEach ( Segment*, segment, net->getSegments() ) {
      AutoSegment* autoSegment = Session::lookup( *segment );
      if (autoSegment) segments.push_back( autoSegment );
    }
    sort( segments.begin(), segments.end(), AutoSegment::CompareId() );

    for ( size_t i=0 ; i<segments.size() ; i++ ) {
      if (segments[i]->isCanonical()) segments[i]->toOptimalAxis();
    }

    cdebug_tabw(145,-1);
    DebugSession::close();
  }


}  // Anabatic namespace.
