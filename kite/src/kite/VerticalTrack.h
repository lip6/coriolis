
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
// |  C++ Header  :       "./VerticalTrack.h"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __KITE_VERTICAL_TRACK__
#define  __KITE_VERTICAL_TRACK__

#include  "kite/Track.h"


namespace Kite {


// -------------------------------------------------------------------
// Class  :  "VerticalTrack".
 

  class VerticalTrack : public Track {

    public:
      static  VerticalTrack* create        ( RoutingPlane*, unsigned int index );
      virtual bool           isHorizontal  () const;
      virtual bool           isVertical    () const;
      virtual unsigned int   getDirection  () const;
      virtual Point          getPosition   ( DbU::Unit coordinate ) const;
      virtual string         _getTypeName  () const;
      virtual Record*        _getRecord    () const;

    protected:
    // Constructors & Destructors.
                             VerticalTrack ( RoutingPlane*, unsigned int index );
      virtual               ~VerticalTrack ();
      virtual void           _postCreate   ();
      virtual void           _preDestroy   ();
    private:
                             VerticalTrack ( const VerticalTrack& );
              VerticalTrack& operator=     ( const VerticalTrack& );
  };


} // End of Kite namespace.


#endif  // __KITE_VERTICAL_TRACK__
