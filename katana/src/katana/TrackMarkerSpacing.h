
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2025-2025.
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katana/TrackMarkerSpacing.h"                 |
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
  using Hurricane::Segment;
  using Hurricane::ParallelSpacings;

  class Track;

  
// -------------------------------------------------------------------
// Class  :  "TrackMarkerSpacing".

  class TrackMarkerSpacing : public TrackMarkerBase {
    public:
      typedef TrackMarkerBase  Super;
    public:
      static  TrackMarkerSpacing* create ( Segment* );
    public:
      virtual Net*         getNet               () const;
              DbU::Unit    getMaxParallelLength ( DbU::Unit trackAxis ) const;
              bool         isValidSpacing       ( const Interval& span, DbU::Unit trackAxis ) const;
              Record*      _getRecord           () const;
              std::string  _getString           () const;
              std::string  _getTypeName         () const;
    private:
              DbU::Unit    _getDistance         ( DbU::Unit trackAxis, ParallelSpacings& ) const;
    private:
      Segment* _wideSegment;
    protected:
                          TrackMarkerSpacing ( Segment* );
                         ~TrackMarkerSpacing () {};
    private:
                          TrackMarkerSpacing ( const TrackMarkerSpacing& );
      TrackMarkerSpacing& operator=          ( const TrackMarkerSpacing& );
  };


// Inline Functions.


}  // Katana namespace.


INSPECTOR_P_SUPPORT(Katana::TrackMarkerSpacing);
