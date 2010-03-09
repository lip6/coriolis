
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
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
// |  C++ Module  :       "./NetOptimals.cpp"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <cstdlib>
#include  <sstream>

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
    DebugSession::open ( net, 88 );

    ltrace(100) << "Katabatic::_computeNetOptimals ( " << net << " )" << endl;
    ltracein(99);

  //cmess2 << "     - " << net << endl;

    vector<AutoSegment*> segments;
    forEach ( Segment*, segment, net->getSegments() ) {
      AutoSegment* autoSegment = Session::lookup ( *segment );
      if ( !autoSegment ) continue;
      segments.push_back ( autoSegment );
    }
    sort ( segments.begin(), segments.end(), AutoSegment::CompareCanonical() );

    set<AutoSegment*> processeds;
    for ( size_t i=0 ; i<segments.size() ; i++ )
      segments[i]->computeOptimal ( &processeds );

    ltraceout(99);

    DebugSession::close ();
  }


  void  KatabaticEngine::_toOptimals ( Net* net, bool onlyNew )
  {
    DebugSession::open ( net, 88 );

    ltrace(100) << "Katabatic::_toOptimals ( " << net << " )" << endl;
    ltracein(99);

  //cmess2 << "     - " << net << endl;

    vector<AutoSegment*> segments;
    forEach ( Segment*, segment, net->getSegments() ) {
      AutoSegment* autoSegment = Session::lookup ( *segment );
      if ( not autoSegment ) continue;
      segments.push_back ( autoSegment );
    }
    sort ( segments.begin(), segments.end(), AutoSegment::CompareCanonical() );

    set<AutoSegment*> processeds;
    for ( size_t i=0 ; i<segments.size() ; i++ )
      segments[i]->toOptimalAxis ( &processeds );

    ltraceout(99);

    DebugSession::close ();
  }


} // End of Katabatic namespace.
