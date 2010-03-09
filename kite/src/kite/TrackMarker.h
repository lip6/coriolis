
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
// |  C++ Header  :       "./TrackMarker.h"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#ifndef  __KITE_TRACK_MARKER__
#define  __KITE_TRACK_MARKER__

namespace Hurricane {
  class RoutingPad;
  class Net;
}


namespace Kite {

  using Hurricane::RoutingPad;
  using Hurricane::Net;

  class Track;

  
// -------------------------------------------------------------------
// Class  :  "TrackMarker".
 

  class TrackMarker {

    public:
      static  TrackMarker* create       ( RoutingPad*, size_t depth );
              void         destroy      ();
    public:
              Net*         getNet       () const;
      inline  DbU::Unit    getSourceU   () const;
      inline  DbU::Unit    getTargetU   () const;
      inline  Track*       getTrack     () const;
      inline  unsigned int getWeight    ( const Track* ) const;
      inline  void         setTrack     ( Track* );
              Record*      _getRecord   () const;
              string       _getString   () const;
              string       _getTypeName () const;
    public:
      class Compare {
        public:
          inline bool operator() ( const TrackMarker* lhs , const TrackMarker* rhs  ) const;
          inline bool operator() ( DbU::Unit          lhsU, const TrackMarker* rhs  ) const;
          inline bool operator() ( const TrackMarker* lhs , DbU::Unit          rhsU ) const;
        private:
          inline bool markerLess ( DbU::Unit lhsU, DbU::Unit rhsU ) const; 
      };

    protected:
    // Attributes.
      RoutingPad*   _routingPad;
      DbU::Unit     _sourcePosition;
      DbU::Unit     _targetPosition;
      Track*        _track;
      unsigned int  _weight;
      unsigned int  _refcount;

    protected:
    // Constructors & destructors.
                             TrackMarker ( RoutingPad*, size_t depth );
                            ~TrackMarker () {};
    private:
                             TrackMarker ( const TrackMarker& );
      TrackMarker& operator=             ( const TrackMarker& );
  };


// Inline Functions.
  inline  DbU::Unit     TrackMarker::getSourceU () const { return _sourcePosition; }
  inline  DbU::Unit     TrackMarker::getTargetU () const { return _targetPosition; }
  inline  Track*        TrackMarker::getTrack   () const { return _track; }
  inline  unsigned int  TrackMarker::getWeight  ( const Track* track ) const { return _weight; }
  inline  void          TrackMarker::setTrack   ( Track* track ) { _track = track; }

  inline bool TrackMarker::Compare::operator() ( const TrackMarker* lhs, const TrackMarker* rhs ) const
  { return markerLess ( lhs->getSourceU(), rhs->getSourceU() ); }

  inline bool TrackMarker::Compare::operator() ( DbU::Unit lhsU, const TrackMarker* rhs ) const
  { return markerLess ( lhsU, rhs->getSourceU() ); }

  inline bool TrackMarker::Compare::operator() ( const TrackMarker* lhs, DbU::Unit rhsU ) const
  { return markerLess ( lhs->getSourceU(), rhsU ); }

  inline bool TrackMarker::Compare::markerLess ( DbU::Unit lhsU, DbU::Unit rhsU ) const
  { return ( lhsU < rhsU ); }


}  // End of Kite namespace.


INSPECTOR_P_SUPPORT(Kite::TrackMarker);


#endif  // __KITE_TRACK_MARKER__
