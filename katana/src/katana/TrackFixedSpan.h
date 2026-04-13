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
// |  C++ Header  :  "./katana/TrackFixedSpan.h"                     |
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
// Class  :  "TrackFixedSpan".

  class TrackFixedSpan : public TrackBaseFixedSpan {
    public:
      typedef TrackBaseFixedSpan  Super;
    public:
      static  TrackFixedSpan* create       ( Net*, const Box&, Track* );
    public:                                
      virtual Net*            getNet       () const;
      virtual void            setNet       ( Net* );
      virtual Record*         _getRecord   () const;
      virtual string          _getTypeName () const;
    private:
              Net* _net;
    protected:
    // Constructors & Destructors.
                                 TrackFixedSpan ( Net*, const Box& ) ;
      virtual                   ~TrackFixedSpan ();
      virtual void               _preDestroy    ();
    private:
                                 TrackFixedSpan ( const TrackFixedSpan& ) = delete;
              TrackFixedSpan& operator=         ( const TrackFixedSpan& ) = delete;
  };


}  // Katana namespace.


INSPECTOR_P_SUPPORT(Katana::TrackFixedSpan);
