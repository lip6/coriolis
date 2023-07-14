// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Universite 2019-2019, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katana/TrackSegmentNonPref.h"                |
// +-----------------------------------------------------------------+


#pragma  once
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
// Class  :  "TrackSegmentNonPref".

  class TrackSegmentNonPref : public TrackSegment {
      friend class TrackSegment;
    public:
      typedef  TrackSegment  Super;
    protected:
                                   TrackSegmentNonPref ( AutoSegment* ) ;
      virtual                     ~TrackSegmentNonPref ();
      virtual void                 _postCreate         ();
      virtual void                 _preDestroy         ();
      virtual bool                 isNonPref           () const;
      virtual size_t               getTrackSpan        () const;
      virtual uint32_t             getTrackCount       () const;
      virtual void                 addOverlapCost      ( TrackCost& ) const;
      virtual void                 addTrackCount       ( int32_t );
      virtual void                 updateTrackSpan     ();
      virtual void                 invalidate          ();
    private:                      
                                   TrackSegmentNonPref ( const TrackSegmentNonPref& ) = delete;
              TrackSegmentNonPref& operator=           ( const TrackSegmentNonPref& ) = delete;
    private:
      size_t    _trackSpan;
      uint32_t  _trackCount;
  };


}  // Katana namespace.


INSPECTOR_P_SUPPORT(Katana::TrackSegmentNonPref);
