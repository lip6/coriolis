
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
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
#include "katabatic/Session.h"
#include "katabatic/AutoSegment.h"
#include "katabatic/KatabaticEngine.h"


namespace Katabatic {

  using namespace std;
  using Hurricane::tab;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::inltrace;
  using Hurricane::ForEachIterator;
  using Hurricane::Net;
  using Hurricane::Segment;
  using Hurricane::DebugSession;


  void  KatabaticEngine::_computeNetOptimals ( Net* net )
  {
    DebugSession::open( net, 88 );
    ltrace(100) << "Katabatic::_computeNetOptimals( " << net << " )" << endl;
    ltracein(99);

    vector<AutoSegment*> segments;
    forEach ( Segment*, segment, net->getSegments() ) {
      AutoSegment* autoSegment = Session::lookup( *segment );
      if (autoSegment) segments.push_back( autoSegment );
    }
    sort( segments.begin(), segments.end(), AutoSegment::CompareId() );

    set<AutoSegment*> processeds;
    for ( size_t i=0 ; i<segments.size() ; i++ )
      segments[i]->computeOptimal( processeds );

    ltraceout(99);
    DebugSession::close();
  }


  void  KatabaticEngine::toOptimals ( Net* net )
  {
    DebugSession::open( net, 88 );
    ltrace(100) << "Katabatic::_toOptimals( " << net << " )" << endl;
    ltracein(99);

    vector<AutoSegment*> segments;
    forEach ( Segment*, segment, net->getSegments() ) {
      AutoSegment* autoSegment = Session::lookup( *segment );
      if (autoSegment) segments.push_back( autoSegment );
    }
    sort( segments.begin(), segments.end(), AutoSegment::CompareId() );

    for ( size_t i=0 ; i<segments.size() ; i++ ) {
      if (segments[i]->isCanonical()) segments[i]->toOptimalAxis();
    }

    ltraceout(99);
    DebugSession::close();
  }


}  // Katabatic namespace.
