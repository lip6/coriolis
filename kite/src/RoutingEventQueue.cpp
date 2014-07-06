// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./RoutingEventQueue.cpp"                  |
// +-----------------------------------------------------------------+


#include <iostream>
#include <iomanip>
#include <functional>
#include <algorithm>
#include "hurricane/Bug.h"
#include "kite/DataNegociate.h"
#include "kite/TrackSegment.h"
#include "kite/RoutingEventQueue.h"
#include "kite/Session.h"


namespace Kite {

  using std::cerr;
  using std::endl;
  using std::setw;
  using std::max;
  using std::make_heap;
  using std::push_heap;
  using std::pop_heap;

  using Hurricane::tab;
  using Hurricane::inltrace;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::Bug;



// -------------------------------------------------------------------
// Class  :  "RoutingEventQueue".


  RoutingEventQueue::RoutingEventQueue ()
    : _topEventLevel (0)
    , _pushRequests  ()
    , _events        ()
  { }


  RoutingEventQueue::~RoutingEventQueue ()
  { clear (); }


  void  RoutingEventQueue::load ( const vector<TrackElement*>& segments )
  {
    for ( size_t i=0 ; i<segments.size() ; i++ ) {
      if (segments[i]->getDataNegociate()->getRoutingEvent()) {
        cinfo << "[INFO] Already have a RoutingEvent - " << segments[i] << endl;
        continue;
      }
      if (segments[i]->getTrack()) {
        cinfo << "[INFO] Already in Track - " << segments[i] << endl;
        continue;
      }
      RoutingEvent* event = RoutingEvent::create(segments[i]);
      event->updateKey();
      _events.insert( event );
    }
  }


  void  RoutingEventQueue::add ( TrackElement* segment, unsigned int level )
  {
    if (segment->getTrack()) {
      cinfo << "[INFO] Already in Track " << (void*)segment->base()->base()
            << ":" << segment << endl;
      return;
    }

    RoutingEvent* event = RoutingEvent::create(segment);
    event->setEventLevel( level );
    push( event );
  }


  void  RoutingEventQueue::commit ()
  {
    ltrace(200) << "RoutingEventQueue::commit()" << endl;
    ltracein(200);

    size_t addeds = _pushRequests.size();
    size_t before = _events.size();

    RoutingEventSet::iterator ipushEvent = _pushRequests.begin();
    for ( ; ipushEvent != _pushRequests.end() ; ipushEvent++ ) {
      (*ipushEvent)->updateKey();

      _topEventLevel = max( _topEventLevel, (*ipushEvent)->getEventLevel() );
      _events.insert( (*ipushEvent) );

      ltrace(200) << "| " << (*ipushEvent) << endl;
    }
    _pushRequests.clear();
#if defined(CHECK_ROUTINGEVENT_QUEUE)
    _keyCheck();
#endif
    size_t after = _events.size();
    if (after-before != addeds) {
      cerr << Bug( "RoutingEventQueue::commit(): less than %d events pusheds (%d)."
                 , addeds,(after-before) ) << endl;
    }

    ltraceout(200);
  }


  RoutingEvent* RoutingEventQueue::pop ()
  {
    multiset<RoutingEvent*,RoutingEvent::Compare>::iterator ievent;
    RoutingEvent* event = NULL;

#if defined(CHECK_ROUTINGEVENT_QUEUE)
    _keyCheck ();
#endif

    if (not _events.empty()) {
      size_t beforeSize = _events.size();

      ievent = _events.end();
      ievent--;
      event  = (*ievent);
      _events.erase( ievent );

      size_t afterSize = _events.size();

      if (afterSize+1 != beforeSize) {
        cerr << Bug( "RoutingEventQueue::pop(): more than one event popped: %d."
                   , (beforeSize-afterSize) ) << endl;
      }
    }

    return event;
  }


  void  RoutingEventQueue::repush ( RoutingEvent* event )
  {
#if defined(CHECK_ROUTINGEVENT_QUEUE)
    _keyCheck ();
#endif

    multiset<RoutingEvent*,RoutingEvent::Compare>::iterator ievent = _events.find(event);
    size_t count = _events.count(event);
    if (count > 1) {
      cerr << Bug("RoutingEventQueue::repush(): %d events matches key %p.",count,event) << endl;
#if defined(CHECK_ROUTINGEVENT_QUEUE)
      _keyCheck ();
#endif
    }

    if (ievent != _events.end()) {
      _events.erase( ievent );
    }
    push ( event );
  }


  void  RoutingEventQueue::repushInvalidateds ()
  {
    const vector<AutoSegment*>& invalidateds0 = Session::getInvalidateds();
    TrackSegmentSet             invalidateds1;
    for ( size_t i=0 ; i<invalidateds0.size() ; i++ ) {
      TrackSegment* segment = dynamic_cast<TrackSegment*>( Session::lookup(invalidateds0[i]) );
      if (segment)
        invalidateds1.insert( segment );
    }

    TrackSegmentSet::iterator isegment = invalidateds1.begin();
    for ( ; isegment != invalidateds1.end() ; isegment++ ) {
      RoutingEvent* event = (*isegment)->getDataNegociate()->getRoutingEvent();
      if ( event
         and not event->isUnimplemented()
         and not event->isDisabled     ()
         and not event->isProcessed    () ) {
        repush( event );
      }
    }
  }


  void  RoutingEventQueue::prepareRepair ()
  {
    multiset<RoutingEvent*,RoutingEvent::Compare>::const_iterator ievent = _events.begin ();
    for ( ; ievent != _events.end(); ++ievent ) {
      (*ievent)->getSegment()->base()->toOptimalAxis();
    }
  }


  void  RoutingEventQueue::clear ()
  {
    if (not _events.empty()) {
      cerr << Bug("RoutingEvent queue is not empty, %d events remains."
                 ,_events.size()) << endl;
    }
    _events.clear();
  }


  void  RoutingEventQueue::dump () const
  {
    multiset<RoutingEvent*,RoutingEvent::Compare>::const_iterator ievent = _events.begin ();
    for ( ; ievent != _events.end(); ievent++ ) {
      cerr << "Deter| Queue:"
           <<         (*ievent)->getEventLevel()
           << ","  << setw(6) << (*ievent)->getPriority()
           << " "  << setw(6) << DbU::getValueString((*ievent)->getSegment()->getLength())
           << " "             << (*ievent)->getSegment()->isHorizontal()
           << " "  << setw(6) << DbU::getValueString((*ievent)->getSegment()->getAxis())
           << " "  << setw(6) << DbU::getValueString((*ievent)->getSegment()->getSourceU())
           << ": " << (*ievent)->getSegment() << endl;
    }
  }


  void  RoutingEventQueue::_keyCheck () const
  {
    multiset<RoutingEvent*,RoutingEvent::Compare>::const_iterator ievent = _events.begin ();
    for ( ; ievent != _events.end(); ievent++ ) {
      multiset<RoutingEvent*,RoutingEvent::Compare>::const_iterator ieventByKey
        = _events.find ( *ievent );
      
      if ( ieventByKey != ievent ) {
        if ( ieventByKey == _events.end() ) {
          cerr << Bug("Key mismatch in RoutingEvent Queue:\n"
                      "      %p:%s wasn't found by key."
                     ,*ievent,getString(*ievent).c_str()
                     ) << endl;
        } else {
          cerr << Bug("Key mismatch in RoutingEvent Queue:\n"
                      "      %p:%s has same key of\n"
                      "      %p:%s"
                     ,*ievent,getString(*ievent).c_str()
                     ,*ieventByKey,getString(*ieventByKey).c_str()
                     ) << endl;
        }
      }
    }
  }


  string  RoutingEventQueue::_getString () const
  {
    string s = "<" + _getTypeName();

    s += ":" + getString(size());
    s += ">";

    return s;
  }


  Record* RoutingEventQueue::_getRecord () const
  {
    Record* record = new Record ( getString(this) );
    record->add ( getSlot ( "_events", &_events ) );
                                     
    return record;
  }


} // Kite namespace.
