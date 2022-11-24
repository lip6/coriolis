
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./kite/HorizontalTrack.h"                      |
// +-----------------------------------------------------------------+


#ifndef  KITE_HORIZONTAL_TRACK_H
#define  KITE_HORIZONTAL_TRACK_H

#include "kite/Track.h"


namespace Kite {


// -------------------------------------------------------------------
// Class  :  "HorizontalTrack".

  class HorizontalTrack : public Track {

    public:
      static  HorizontalTrack* create          ( RoutingPlane*, unsigned int index );
      virtual bool             isHorizontal    () const;
      virtual bool             isVertical      () const;
      virtual unsigned int     getDirection    () const;
      virtual Point            getPosition     ( DbU::Unit coordinate ) const;
      virtual string           _getTypeName    () const;
      virtual Record*          _getRecord      () const;

    protected:
    // Constructors & Destructors.
                               HorizontalTrack ( RoutingPlane*, unsigned int index );
      virtual                 ~HorizontalTrack ();
      virtual void             _postCreate     ();
      virtual void             _preDestroy     ();
    private:
              HorizontalTrack                  ( const HorizontalTrack& );
              HorizontalTrack& operator=       ( const HorizontalTrack& );
  };


} // Kite namespace.


#endif  // KITE_HORIZONTAL_TRACK_H
