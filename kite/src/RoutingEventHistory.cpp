
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./RoutingEventHistory.cpp"                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <iomanip>

#include  "hurricane/Error.h"
#include  "kite/RoutingEvent.h"
#include  "kite/RoutingEventHistory.h"


namespace Kite {


  using std::cerr;
  using std::setw;
  using std::setfill;
  using std::endl;
  using Hurricane::Error;


// -------------------------------------------------------------------
// Class  :  "RoutingEventHistory".


  RoutingEventHistory::RoutingEventHistory ()
    : _events    ()
    , _identicals(0)
  { }


  RoutingEventHistory::~RoutingEventHistory ()
  { clear (); }


  RoutingEvent* RoutingEventHistory::getNth ( size_t index ) const
  {
    if ( index < size() ) return _events[index];
    return NULL;
  }


  RoutingEvent* RoutingEventHistory::getRNth ( size_t index ) const
  {
    if ( index < size() ) return _events[size()-index-1];
    return NULL;
  }


  void  RoutingEventHistory::dump ( ostream& o, size_t depth ) const
  {
    o << "  o  Event History top stack:" << endl;
    if ( _events.empty() ) return;

    size_t stop = (_events.size() > depth) ? (_events.size()-depth-1) : 0;
    size_t i    =  _events.size()-1;
    do {
      o << "     - [" << setfill('0') << setw(3) << i << "]: " << _events[i] << endl;
      o << setfill(' ');
    } while ( i && (i-- >= stop) );
  }


  void  RoutingEventHistory::push ( RoutingEvent* event )
  {
    if ( _events.size() && (event->getSegment() == _events.back()->getSegment()) )
      _identicals++;
    else
      _identicals = 0;
         
    _events.push_back(event);

//     if ( _identicals > 30 ) {
//       dump ( cerr, 40 );
//       throw Error("RoutingEventHistory::push(): More than 30 identicals events, we are looping.");
//     }
  }


  void  RoutingEventHistory::clear ()
  {
    for ( size_t i=0 ; i < _events.size() ; i++ )
      _events[i]->destroy();
    _events.clear ();
    _identicals = 0;
  }


  string  RoutingEventHistory::_getString () const
  {
    string s = "<" + _getTypeName();

    s += ":" + getString(size());
    s += ">";

    return s;
  }


  Record* RoutingEventHistory::_getRecord () const
  {
    Record* record = new Record ( getString(this) );
    record->add ( getSlot ( "_events", &_events ) );
                                     
    return record;
  }


} // End of Kite namespace.
