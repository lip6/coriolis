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
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katana/RoutingEventHistory.h"                  |
// +-----------------------------------------------------------------+


#ifndef  KATANA_ROUTING_EVENT_HISTORY_H
#define  KATANA_ROUTING_EVENT_HISTORY_H

#include <iostream>
#include <vector>


namespace Katana {

  using std::vector;
  using std::ostream;

  class RoutingEvent;


// -------------------------------------------------------------------
// Class  :  "RoutingEventHistory".
 
  class RoutingEventHistory {

    public:
                            RoutingEventHistory ();
                           ~RoutingEventHistory ();
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

    private:
      RoutingEventHistory& operator=           ( const RoutingEventHistory& );
                           RoutingEventHistory ( const RoutingEventHistory& );

  };


// Inline Functions.
  inline bool    RoutingEventHistory::empty        () const { return _events.empty(); }
  inline size_t  RoutingEventHistory::size         () const { return _events.size(); }
  inline string  RoutingEventHistory::_getTypeName () const { return "RoutingEventHistory"; }


}  // Katana namespace.


#endif  // KATANA_ROUTING_EVENT_HISTORY_H
