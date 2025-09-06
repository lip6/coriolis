
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2008-2025.
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katana/TrackMarker.h"                        |
// +-----------------------------------------------------------------+


#pragma  once
#include "katana/TrackMarkerBase.h"
namespace Hurricane {
  class RoutingPad;
  class Net;
}


namespace Katana {

  using Hurricane::Record;
  using Hurricane::DbU;
  using Hurricane::Interval;
  using Hurricane::RoutingPad;
  using Hurricane::Net;

  class Track;

  
// -------------------------------------------------------------------
// Class  :  "TrackMarker".

  class TrackMarker : public TrackMarkerBase {
    public:
      typedef TrackMarkerBase  Super;
    public:
      static  TrackMarker* create       ( RoutingPad*, size_t depth );
    public:
      virtual Net*         getNet       () const;
      virtual uint32_t     getWeight    ( const Track* ) const;
              Record*      _getRecord   () const;
              std::string  _getString   () const;
              std::string  _getTypeName () const;
    private:
      RoutingPad* _routingPad;
      uint32_t    _weight;
    protected:
                             TrackMarker ( RoutingPad*, size_t depth );
                            ~TrackMarker () {};
    private:
                             TrackMarker ( const TrackMarker& );
      TrackMarker& operator=             ( const TrackMarker& );
  };


// Inline Functions.


}  // Katana namespace.


INSPECTOR_P_SUPPORT(Katana::TrackMarker);
