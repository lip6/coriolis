// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2024-2024.
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katana/TrackFixedSpanRp.h"                   |
// +-----------------------------------------------------------------+


#pragma  once
#include "katana/TrackBaseFixedSpan.h"


namespace Katana {

  using std::string;
  using std::map;
  using Hurricane::Record;
  using Hurricane::Interval;
  using Hurricane::DbU;
  using Hurricane::Net;
  using Hurricane::Layer;

  class Track;


// -------------------------------------------------------------------
// Class  :  "TrackFixedSpanRp".

  class TrackFixedSpanRp : public TrackBaseFixedSpan {
    public:
      typedef TrackBaseFixedSpan  Super;
    public:
      static  TrackFixedSpanRp* create        ( RoutingPad*, const Box&, Track* );
    public:                                
      virtual bool              isFixedSpanRp () const;
      virtual RoutingPad*       getRoutingPad () const;
      virtual Net*              getNet        () const;
      virtual void              setNet        ( Net* );
      virtual Record*           _getRecord    () const;
      virtual string            _getTypeName  () const;
    private:
              RoutingPad* _routingPad;
    protected:
    // Constructors & Destructors.
                                TrackFixedSpanRp ( RoutingPad*, const Box& ) ;
      virtual                  ~TrackFixedSpanRp ();
      virtual void              _preDestroy      ();
    private:
                                TrackFixedSpanRp ( const TrackFixedSpanRp& ) = delete;
              TrackFixedSpanRp& operator=        ( const TrackFixedSpanRp& ) = delete;
  };


}  // Katana namespace.


INSPECTOR_P_SUPPORT(Katana::TrackFixedSpanRp);
