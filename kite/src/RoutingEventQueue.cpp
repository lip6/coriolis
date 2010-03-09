
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
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./RoutingEvent.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <iostream>
#include  <iomanip>
#include  <functional>
#include  <algorithm>

#include  "hurricane/Bug.h"
#include  "kite/DataNegociate.h"
#include  "kite/TrackElement.h"
#include  "kite/RoutingEventQueue.h"
#include  "kite/Session.h"


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
      if ( segments[i]->getDataNegociate()->getGCellOrder() >= Session::getOrder() ) {
        if ( segments[i]->getDataNegociate()->getRoutingEvent() ) {
          cinfo << "[INFO] Already have a RoutingEvent - " << segments[i] << endl;
          continue;
        }
        if ( segments[i]->getTrack() ) {
          cinfo << "[INFO] Already in Track - " << segments[i] << endl;
          continue;
        }
        RoutingEvent* event = RoutingEvent::create(segments[i]);
        event->updateKey ();
        _events.insert ( event );
      }
    }
  }


  void  RoutingEventQueue::add ( TrackElement* segment, unsigned int level )
  {
    if ( segment->getTrack() ) {
      cinfo << "[INFO] Already in Track " << (void*)segment->base()->base()
            << ":" << segment << endl;
      return;
    }

    RoutingEvent* event = RoutingEvent::create(segment);
    event->setEventLevel ( level );
    push ( event );
  }


  void  RoutingEventQueue::push ( RoutingEvent* event )
  { _pushRequests.insert ( event ); }


  void  RoutingEventQueue::commit ()
  {
    ltrace(200) << "RoutingEventQueue::commit()" << endl;
    ltracein(200);

    size_t addeds = _pushRequests.size();
    size_t before = _events.size();

    set<RoutingEvent*>::iterator ipushEvent = _pushRequests.begin();
    for ( ; ipushEvent != _pushRequests.end() ; ipushEvent++ ) {
      (*ipushEvent)->updateKey ();

      _topEventLevel = max ( _topEventLevel, (*ipushEvent)->getEventLevel() );
      _events.insert ( (*ipushEvent) );

      ltrace(200) << "| " << (*ipushEvent) << endl;
    }
    _pushRequests.clear ();
#if defined(CHECK_ROUTINGEVENT_QUEUE)
    _keyCheck ();
#endif
    size_t after = _events.size();
    if ( after-before != addeds ) {
      cerr << Bug("RoutingEventQueue::commit(): less than %d events pusheds (%d)."
                 ,addeds,(after-before)) << endl;
    }

    // if (   (RoutingEvent::getProcesseds() > 61246)
    //    and (RoutingEvent::getProcesseds() < 61256) ) {
    //   cerr << "RoutingEventQueue::commit()" << endl;
    //   dump ();
    // }

    ltraceout(200);
  }


  RoutingEvent* RoutingEventQueue::pop ()
  {
    multiset<RoutingEvent*,RoutingEvent::Compare>::iterator ievent;
    RoutingEvent* event = NULL;

#if defined(CHECK_ROUTINGEVENT_QUEUE)
    _keyCheck ();
#endif

    if ( !_events.empty() ) {
      size_t beforeSize = _events.size();

      ievent = _events.end();
      ievent--;
      event  = (*ievent);
      _events.erase ( ievent );

      // if (   (RoutingEvent::getProcesseds() > 61246)
      //    and (RoutingEvent::getProcesseds() < 61256) ) {
      //   cerr << "Popped: " << event << endl;
      //   dump ();
      // }

      size_t afterSize = _events.size();

      if ( afterSize+1 != beforeSize ) {
        cerr << Bug("RoutingEventQueue::pop(): more than one event popped: %d."
                   ,(beforeSize-afterSize)) << endl;
      }
//       size_t erased = _events.erase ( event );
//       if ( erased != 1 ) {
//         cerr << Bug("RoutingEventQueue::pop(): %d events matches key %p.",erased,event) << endl;
// #if defined(CHECK_ROUTINGEVENT_QUEUE)
//         _keyCheck ();
// #endif
//       }
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
    if ( count > 1 ) {
      cerr << Bug("RoutingEventQueue::repush(): %d events matches key %p.",count,event) << endl;
#if defined(CHECK_ROUTINGEVENT_QUEUE)
      _keyCheck ();
#endif
    }

    if ( ievent != _events.end() ) {
      // if (   (RoutingEvent::getProcesseds() > 61246)
      //    and (RoutingEvent::getProcesseds() < 61256) ) {
      //   cerr << "Erasing: " << *ievent << endl;
      // }
      _events.erase ( ievent );
    }
    push ( event );

    // if (   (RoutingEvent::getProcesseds() > 61246)
    //    and (RoutingEvent::getProcesseds() < 61256) ) {
    //   cerr << "After repush: " << event << endl;
    //   dump();
    // }
  }


  void  RoutingEventQueue::repushInvalidateds ()
  {
    const vector<AutoSegment*>& invalidateds0 = Session::getInvalidateds();
    set<TrackElement*>          invalidateds1;
    for ( size_t i=0 ; i<invalidateds0.size() ; i++ ) {
      TrackElement* segment = Session::lookup ( invalidateds0[i] );
      if ( segment )
        invalidateds1.insert ( segment );
    }

    set<TrackElement*>::iterator isegment = invalidateds1.begin();
    for ( ; isegment != invalidateds1.end() ; isegment++ ) {
      RoutingEvent* event = (*isegment)->getDataNegociate()->getRoutingEvent();
      if ( event
         and not event->isUnimplemented()
         and not event->isDisabled     ()
         and not event->isProcessed    () ) {
        repush ( event );
      } else {
        // if (   (RoutingEvent::getProcesseds() > 61246)
        //    and (RoutingEvent::getProcesseds() < 61256) ) {
        //   cerr << "NOT repushed: " << event << endl;
        // }
      }
    }
  }


  void  RoutingEventQueue::clear ()
  {
    if ( not _events.empty() ) {
      cerr << Bug("RoutingEvent queue is not empty, %d events remains."
                 ,_events.size()) << endl;
    }
    _events.clear ();
  }


  void  RoutingEventQueue::dump () const
  {
    multiset<RoutingEvent*,RoutingEvent::Compare>::const_iterator ievent = _events.begin ();
    for ( ; ievent != _events.end(); ievent++ ) {
      cerr << "Order: Queue:"
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


} // End of Kite namespace.
