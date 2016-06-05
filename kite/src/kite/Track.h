// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./kite/Track.h"                                |
// +-----------------------------------------------------------------+


#ifndef  KITE_TRACK_H
#define  KITE_TRACK_H

#include  "hurricane/Point.h"
namespace Hurricane {
  class Layer;
}

#include  "kite/TrackCost.h"
#include  "kite/TrackElement.h"


namespace Kite {

  using Hurricane::Point;
  using Hurricane::Layer;

  class TrackMarker;
  class RoutingPlane;
  class KiteEngine;


// -------------------------------------------------------------------
// Class  :  "Track".

  class Track {

    public:
      enum IndexState { BeginIsTrackMin     = 0x00000001
                      , BeginIsSegmentMin   = 0x00000002
                      , BeginIsSegmentMax   = 0x00000004
                      , EndIsTrackMax       = 0x00000008
                      , EndIsSegmentMin     = 0x00000010
                      , EndIsNextSegmentMin = 0x00000020
                      , EndIsSegmentMax     = 0x00000040
                      , BeforeFirstElement  = BeginIsTrackMin  |EndIsSegmentMin
                      , InsideElement       = BeginIsSegmentMin|EndIsSegmentMax
                      , OutsideElement      = BeginIsSegmentMax|EndIsNextSegmentMin
                      , AfterLastElement    = BeginIsSegmentMax|EndIsTrackMax
                      , EmptyTrack          = BeginIsTrackMin  |EndIsTrackMax
                      , BeginMask           = BeginIsTrackMin  |BeginIsSegmentMin|BeginIsSegmentMax
                      , EndMask             = EndIsTrackMax    |EndIsSegmentMin  |EndIsNextSegmentMin|EndIsSegmentMax
                      };

    public:
    // Static Attributes.
      static const size_t  npos;

    public:
              void           destroy             ();
      virtual bool           isHorizontal        () const = 0;
      virtual bool           isVertical          () const = 0;
      inline  bool           isLocalAssigned     () const;
      inline  RoutingPlane*  getRoutingPlane     () const;
              KiteEngine*    getKiteEngine       () const;
      virtual unsigned int   getDirection        () const = 0;
      inline  size_t         getIndex            () const;
              unsigned int   getDepth            () const;
              const Layer*   getLayer            () const;
              const Layer*   getBlockageLayer    () const;
      inline  DbU::Unit      getAxis             () const;
      inline  DbU::Unit      getMin              () const;
      inline  DbU::Unit      getMax              () const;
              Track*         getNextTrack        () const;
              Track*         getPreviousTrack    () const;
      inline  size_t         getSize             () const;
      virtual Point          getPosition         ( DbU::Unit coordinate ) const = 0;
              TrackElement*  getSegment          ( size_t  index ) const;
              TrackElement*  getSegment          ( DbU::Unit position ) const;
              TrackElement*  getNext             ( size_t& index, Net* ) const;
              TrackElement*  getPrevious         ( size_t& index, Net* ) const;
              TrackElement*  getNextFixed        ( size_t& index ) const;
              size_t         find                ( const TrackElement* ) const;
              DbU::Unit      getSourcePosition   ( vector<TrackElement*>::iterator ) const;
              DbU::Unit      getMinimalPosition  ( size_t index, unsigned int state ) const;
              DbU::Unit      getMaximalPosition  ( size_t index, unsigned int state ) const;
              Interval       getFreeInterval     ( DbU::Unit position, Net* net=NULL ) const;
              Interval       getOccupiedInterval ( size_t& begin ) const;
              Interval       expandFreeInterval  ( size_t& begin, size_t& end, unsigned int state, Net* ) const;
              void           getBeginIndex       ( DbU::Unit position, size_t& begin, unsigned int& state ) const;
              void           getOverlapBounds    ( Interval, size_t& begin, size_t& end ) const;
              TrackCost      getOverlapCost      ( Interval, Net*, size_t begin, size_t end, unsigned int flags ) const;
              TrackCost      getOverlapCost      ( Interval, Net*, unsigned int flags ) const;
              TrackCost      getOverlapCost      ( TrackElement*, unsigned int flags ) const;
              void           getTerminalWeight   ( Interval, Net*, size_t& count, unsigned int& weight ) const;
              DbU::Unit      getSourcePosition   ( size_t index ) const;
              bool           check               ( unsigned int& overlaps, const char* message=NULL ) const;
              unsigned int   checkOverlap        ( unsigned int& overlaps ) const;
     inline   void           setLocalAssigned    ( bool );
              void           invalidate          ();
              void           insert              ( TrackElement* );
              void           insert              ( TrackMarker* );
              void           setSegment          ( TrackElement*, size_t );
              size_t         doRemoval           ();
              void           doReorder           ();
      virtual Record*        _getRecord          () const;
      virtual string         _getString          () const;
      virtual string         _getTypeName        () const = 0;

    protected:
    // Attributes.
      RoutingPlane*          _routingPlane;
      size_t                 _index;
      DbU::Unit              _axis;
      DbU::Unit              _min;
      DbU::Unit              _max;
      vector<TrackElement*>  _segments;
      vector<TrackMarker*>   _markers;
      bool                   _localAssigned;
      bool                   _segmentsValid;
      bool                   _markersValid;

    protected:
    // Constructors & Destructors.
                            Track           ( RoutingPlane*, unsigned int index );
      virtual              ~Track           ();
      virtual void          _postCreate     ();
      virtual void          _preDestroy     ();
    private:                                
              Track                         ( const Track& );
              Track&        operator=       ( const Track& );
    protected:
    // Protected functions.
      inline  unsigned int  setMinimalFlags ( unsigned int& state, unsigned int flags ) const;
      inline  unsigned int  setMaximalFlags ( unsigned int& state, unsigned int flags ) const;

    protected:
    // Sub-Classes.
      struct SourceCompare {
        public:
          inline bool operator() ( const TrackElement* lhs      , const TrackElement* rhs );
          inline bool operator() (       DbU::Unit     lhsSource, const TrackElement* rhs );
          inline bool operator() ( const TrackElement* lhs      ,       DbU::Unit     rhsSource );
        private:
          inline bool lessSource (       DbU::Unit     lhs      ,       DbU::Unit     rhsSource );
      };
      struct SegmentCompare {
          inline bool operator() ( const TrackElement* lhs, const TrackElement* rhs );
      };
  };


// Inline Functions.
  inline bool  Track::SourceCompare::operator() ( const TrackElement* lhs, const TrackElement* rhs )
  { return lessSource(lhs->getSourceU(),rhs->getSourceU()); }


  inline bool  Track::SourceCompare::operator() ( DbU::Unit lhsSource, const TrackElement* rhs )
  { return lessSource(lhsSource,rhs->getSourceU()); }


  inline bool  Track::SourceCompare::operator() ( const TrackElement* lhs, DbU::Unit rhsSource )
  { return lessSource(lhs->getSourceU(),rhsSource); }


  inline bool  Track::SourceCompare::lessSource ( DbU::Unit lhsSource, DbU::Unit rhsSource )
  { return lhsSource < rhsSource; }


  inline bool Track::SegmentCompare::operator() ( const TrackElement* lhs, const TrackElement* rhs )
  {
    if (lhs->getSourceU() < rhs->getSourceU())
      return true;
    else {
      if (   (lhs->getSourceU() == rhs->getSourceU())
         and (lhs->getTargetU() >  rhs->getTargetU()) )
        return true;
    }
    return false;
  }


  inline bool          Track::isLocalAssigned  () const { return _localAssigned; }
  inline RoutingPlane* Track::getRoutingPlane  () const { return _routingPlane; }
  inline size_t        Track::getIndex         () const { return _index; }
  inline DbU::Unit     Track::getAxis          () const { return _axis; }
  inline DbU::Unit     Track::getMin           () const { return _min; }
  inline DbU::Unit     Track::getMax           () const { return _max; }
  inline size_t        Track::getSize          () const { return _segments.size(); }
  inline void          Track::setLocalAssigned ( bool state ) { _localAssigned=state; }

  inline unsigned int  Track::setMinimalFlags ( unsigned int& state, unsigned int flags ) const
  {
    state &=         ~BeginMask;
    state |= (flags & BeginMask);
    return state;
  }

  inline unsigned int  Track::setMaximalFlags ( unsigned int& state, unsigned int flags ) const
  {
    state &=         ~EndMask;
    state |= (flags & EndMask);
    return state;
  }


}  // Kite namespace.


INSPECTOR_P_SUPPORT(Kite::Track);

#endif  // KITE_TRACK_H
