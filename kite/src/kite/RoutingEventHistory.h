
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
// |  C++ Header  :       "./RoutingEventHistory.h"                  |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __KITE_ROUTING_EVENT_HISTORY__
#define  __KITE_ROUTING_EVENT_HISTORY__


#include  <iostream>
#include  <vector>


namespace Kite {


  using std::vector;
  using std::ostream;

  class RoutingEvent;


// -------------------------------------------------------------------
// Class  :  "RoutingEventHistory".

 
  class RoutingEventHistory {

    public:
                            RoutingEventHistory ();
                           ~RoutingEventHistory ();
      inline  bool          looping             () const;
      inline  bool          empty               () const;
      inline  size_t        size                () const;
              RoutingEvent* getNth              ( size_t ) const;
              RoutingEvent* getRNth             ( size_t ) const;
              void          push                ( RoutingEvent* );
              void          clear               ();
              void          dump                ( ostream&, size_t depth=10 ) const;
              Record*       _getRecord          () const;
              string        _getString          () const;
      inline  string        _getTypeName        () const;

    protected:
    // Attributes.
      vector<RoutingEvent*>  _events;
      size_t                 _identicals;

    private:
      RoutingEventHistory& operator=           ( const RoutingEventHistory& );
                           RoutingEventHistory ( const RoutingEventHistory& );

  };


// Inline Functions.
  inline bool    RoutingEventHistory::looping      () const { return _identicals > 30; }
  inline bool    RoutingEventHistory::empty        () const { return _events.empty(); }
  inline size_t  RoutingEventHistory::size         () const { return _events.size(); }
  inline string  RoutingEventHistory::_getTypeName () const { return "RoutingEventHistory"; }


} // End of Kite namespace.


#endif  // __KITE_ROUTING_EVENT_HISTORY__
