
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
// |  C++ Header  :       "./RoutingEventQueue.h"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __KITE_ROUTING_EVENT_QUEUE__
#define  __KITE_ROUTING_EVENT_QUEUE__


#include  <set>
#include  <vector>
#include  "kite/RoutingEvent.h"


namespace Kite {


  using std::set;
  using std::multiset;
  using std::vector;


// -------------------------------------------------------------------
// Class  :  "RoutingEventQueue".
 

  class RoutingEventQueue {

    public:
                            RoutingEventQueue  ();
                           ~RoutingEventQueue  ();
              void          load               ( const vector<TrackElement*>& );
              void          add                ( TrackElement*, unsigned int level );
      inline  bool          empty              () const;
      inline  size_t        size               () const;
      inline  unsigned int  getTopEventLevel   () const;
              void          push               ( RoutingEvent* );
              RoutingEvent* pop                ();
              void          repush             ( RoutingEvent* );
              void          repushInvalidateds ();
              void          commit             ();
              void          clear              ();
              void          dump               () const;
              void          _keyCheck          () const;
              Record*       _getRecord         () const;
              string        _getString         () const;
      inline  string        _getTypeName       () const;

    protected:
    // Attributes.
      unsigned int                                   _topEventLevel;
      set<RoutingEvent*>                             _pushRequests;
      multiset<RoutingEvent*,RoutingEvent::Compare>  _events;

    private:
              RoutingEventQueue& operator=         ( const RoutingEventQueue& );
                                 RoutingEventQueue ( const RoutingEventQueue& );

  };


// Inline Functions.
  inline bool          RoutingEventQueue::empty            () const { return _events.empty(); }
  inline size_t        RoutingEventQueue::size             () const { return _events.size(); }
  inline unsigned int  RoutingEventQueue::getTopEventLevel () const { return _topEventLevel; }
  inline string        RoutingEventQueue::_getTypeName     () const { return "EventQueue"; }


} // End of Kite namespace.


#endif  // __KITE_ROUTING_EVENT_QUEUE__
