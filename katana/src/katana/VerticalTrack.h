
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
// |  C++ Header  :  "./katana/VerticalTrack.h"                      |
// +-----------------------------------------------------------------+


#ifndef  KATANA_VERTICAL_TRACK_H
#define  KATANA_VERTICAL_TRACK_H

#include  "katana/Track.h"


namespace Katana {


// -------------------------------------------------------------------
// Class  :  "VerticalTrack".

  class VerticalTrack : public Track {

    public:
      static  VerticalTrack* create        ( RoutingPlane*, uint32_t index );
      virtual bool           isHorizontal  () const;
      virtual bool           isVertical    () const;
      virtual Flags          getDirection  () const;
      virtual Point          getPosition   ( DbU::Unit coordinate ) const;
      virtual string         _getTypeName  () const;
      virtual Record*        _getRecord    () const;

    protected:
    // Constructors & Destructors.
                             VerticalTrack ( RoutingPlane*, uint32_t index );
      virtual               ~VerticalTrack ();
      virtual void           _postCreate   ();
      virtual void           _preDestroy   ();
    private:
                             VerticalTrack ( const VerticalTrack& );
              VerticalTrack& operator=     ( const VerticalTrack& );
  };


}  // Katana namespace.


#endif  // KATANA_VERTICAL_TRACK_H
