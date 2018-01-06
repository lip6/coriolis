// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katana/RoutingEventQueue.h"                  |
// +-----------------------------------------------------------------+


#ifndef  KATANA_ROUTING_EVENT_QUEUE_H
#define  KATANA_ROUTING_EVENT_QUEUE_H

#include <set>
#include <vector>
#include "katana/RoutingEvent.h"


namespace Katana {

  using std::set;
  using std::multiset;
  using std::vector;


// -------------------------------------------------------------------
// Class  :  "RoutingEventQueue".

  class RoutingEventQueue {

    public:
                            RoutingEventQueue  ();
                           ~RoutingEventQueue  ();
      inline  bool          empty              () const;
      inline  size_t        size               () const;
      inline  uint32_t      getTopEventLevel   () const;
              RoutingEvent* pop                ();
              void          load               ( const vector<TrackElement*>& );
              void          add                ( TrackElement*, uint32_t level );
      inline  void          push               ( RoutingEvent* );
              void          repush             ( RoutingEvent* );
              void          repushInvalidateds ();
              void          commit             ();
              void          prepareRepair      ();
              void          clear              ();
              void          dump               () const;
              void          _keyCheck          () const;
              Record*       _getRecord         () const;
              string        _getString         () const;
      inline  string        _getTypeName       () const;

    protected:
    // Attributes.
      uint32_t                                       _topEventLevel;
      RoutingEventSet                                _pushRequests;
      multiset<RoutingEvent*,RoutingEvent::Compare>  _events;

    private:
              RoutingEventQueue& operator=         ( const RoutingEventQueue& );
                                 RoutingEventQueue ( const RoutingEventQueue& );

  };


// Inline Functions.
  inline bool      RoutingEventQueue::empty            () const { return _events.empty(); }
  inline size_t    RoutingEventQueue::size             () const { return _events.size(); }
  inline uint32_t  RoutingEventQueue::getTopEventLevel () const { return _topEventLevel; }
  inline string    RoutingEventQueue::_getTypeName     () const { return "EventQueue"; }
  inline void      RoutingEventQueue::push             ( RoutingEvent* event ) { _pushRequests.insert( event ); }


}  // Katana namespace.


#endif  // KATANA_ROUTING_EVENT_QUEUE_H
