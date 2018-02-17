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
// |  C++ Header  :  "./katana/RoutingEventLoop.h"                   |
// +-----------------------------------------------------------------+


#ifndef  KATANA_ROUTING_EVENT_LOOP_H
#define  KATANA_ROUTING_EVENT_LOOP_H

#include <vector>


namespace Katana {

  class TrackElement;
  class RoutingEvent;


// -------------------------------------------------------------------
// Class  :  "RoutingEventLoop".


  class RoutingEventLoop {
    public:
      class Element {
        public:
          inline         Element ( TrackElement* segment, size_t timestamp=0 );
                 size_t  getId   () const;
        public:
          TrackElement* _segment;
          size_t        _timestamp;
          int           _count;
      };
      class CompareByTimestamp {
        public:
          inline bool  operator() ( const Element& lhs, const Element& rhs );
      };
    public:
                                         RoutingEventLoop ( size_t depth=10, int limit=20 );
      inline bool                        isLooping        () const;
      inline int                         getMaxCount      () const;
      inline const std::vector<Element>& getElements      () const;
             void                        update           ( TrackElement* );
             void                        erase            ( TrackElement* );
    private:
      std::vector<Element>  _elements;
      size_t                _depth;
      int                   _maxCount;
      int                   _countLimit;
      bool                  _isLooping;
  };


  inline RoutingEventLoop::Element::Element ( TrackElement* segment, size_t timestamp )
    : _segment(segment), _timestamp(timestamp), _count(1)
  { }


  inline bool RoutingEventLoop::CompareByTimestamp::operator() ( const RoutingEventLoop::Element& lhs, const RoutingEventLoop::Element& rhs )
  { return lhs._timestamp > rhs._timestamp; }


  inline bool  RoutingEventLoop::isLooping   () const { return _isLooping; }
  inline int   RoutingEventLoop::getMaxCount () const { return _maxCount; }
  inline const std::vector<RoutingEventLoop::Element>&
               RoutingEventLoop::getElements () const { return _elements; }



}  // Katana namespace.


//INSPECTOR_P_SUPPORT(Katana::RoutingEvent);


#endif  // KATANA_ROUTING_EVENT_LOOP_H
