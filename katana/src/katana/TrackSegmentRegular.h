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
// |  C++ Header  :  "./katana/TrackSegmentRegular.h"                |
// +-----------------------------------------------------------------+


#ifndef  KATANA_TRACK_SEGMENT_REGULAR_H
#define  KATANA_TRACK_SEGMENT_REGULAR_H

#include <set>
#include <functional>
#include "katana/TrackSegment.h"


namespace Katana {

  using std::string;
  using std::map;
  using std::set;
  using std::binary_function;
  using Hurricane::Record;
  using Hurricane::Interval;
  using Hurricane::DbU;
  using Hurricane::Net;
  using Hurricane::Layer;
  using Anabatic::AutoSegment;

  class DataNegociate;
  class Track;
  class TrackCost;


// -------------------------------------------------------------------
// Class  :  "TrackSegmentRegular".

  class TrackSegmentRegular : public TrackSegment {
      friend class TrackSegment;
    public:
      typedef  TrackSegment  Super;
    protected:
                                   TrackSegmentRegular ( AutoSegment*, Track* ) ;
      virtual                     ~TrackSegmentRegular ();
      virtual void                 _postCreate         ();
      virtual void                 _preDestroy         ();
      virtual size_t               getTrackSpan        () const;
      virtual void                 addOverlapCost      ( TrackCost& ) const;
    private:
                                   TrackSegmentRegular ( const TrackSegmentRegular& ) = delete;
              TrackSegmentRegular& operator=           ( const TrackSegmentRegular& ) = delete;
  };


}  // Katana namespace.


INSPECTOR_P_SUPPORT(Katana::TrackSegmentRegular);

#endif  // KATANA_TRACK_SEGMENT_REGULAR_H
