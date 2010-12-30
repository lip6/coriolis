
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
// |  C++ Header  :       "./kite/RoutingEventLoop.h"                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __KITE_ROUTING_EVENT_LOOP__
#define  __KITE_ROUTING_EVENT_LOOP__

#include  <vector>


namespace Kite {

  class RoutingEvent;


// -------------------------------------------------------------------
// Class  :  "RoutingEventLoop".


  class RoutingEventLoop {
    public:
      class Element {
        public:
          inline Element ( size_t id=0, size_t timestamp=0 );
        public:
          size_t  _id;
          size_t  _timestamp;
          int     _count;
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
             void                        update           ( size_t id );
             void                        erase            ( size_t id );
    private:
      std::vector<Element>  _elements;
      size_t                _depth;
      int                   _maxCount;
      int                   _countLimit;
      bool                  _isLooping;
  };


  inline RoutingEventLoop::Element::Element ( size_t id, size_t timestamp )
    : _id(id), _timestamp(timestamp), _count(1)
  { }


  inline bool RoutingEventLoop::CompareByTimestamp::operator() ( const RoutingEventLoop::Element& lhs, const RoutingEventLoop::Element& rhs )
  { return lhs._timestamp > rhs._timestamp; }


  inline bool  RoutingEventLoop::isLooping   () const { return _isLooping; }
  inline int   RoutingEventLoop::getMaxCount () const { return _maxCount; }
  inline const std::vector<RoutingEventLoop::Element>&
               RoutingEventLoop::getElements () const { return _elements; }



} // End of Kite namespace.


//INSPECTOR_P_SUPPORT(Kite::RoutingEvent);


#endif  // __KITE_ROUTING_EVENT_LOOP__
