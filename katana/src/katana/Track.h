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
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katana/Track.h"                              |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/Point.h"
namespace Hurricane {
  class Layer;
}
#include "crlcore/RoutingLayerGauge.h"
#include "katana/TrackCost.h"
#include "katana/TrackElement.h"


namespace Katana {

  using Hurricane::Point;
  using Hurricane::Layer;
  using Hurricane::DbU;
  using CRL::RoutingLayerGauge;

  class TrackMarker;
  class TrackMarkerSpacing;
  class RoutingPlane;
  class KatanaEngine;


//! -------------------------------------------------------------------
//!
//! \class        Track
//! \brief        Contains all the TrackElement placed inside a routing track.
//!
//! \defgroup     TrackIndexState  Index State Flag
//!
//! This set of constants is used to give meaning to the indexes returned by
//! Track::getBeginIndex(). It tells how the bounds of the returned segment
//! should be interpreted to compute the requested interval or position.
//! Constants are divided in three groups:
//!
//! - How to interpret the `begin` index to compute the *lower bound*.
//! - How to interpret the `end` index to compute the *upper bound*.
//! - Combinations of the two previous ones in order to get the complete
//!   interval specification.
//!
//! Recapitulative table of the combinated states values and the associateds
//! intervals:
//!
//! State Value                |  Interval Computation
//! ---------------------------|--------------------------------------------------
//! Track::BeforeFirstElement  | `[ trackMin, elem[begin].getVMin() ]`
//! Track::InsideElement       | `[ elem[begin].getVMin(), elem[end].getVMax() ]`
//! Track::OutsideElement      | `[ elem[begin].getVMax(), elem[end].getVMin() ]`
//! Track::AfterLastElement    | `[ element[begin].getVMax(), trackMax ]`
//! Track::EmptyTrack          | `[ trackMin, trackMax ]`
//!
//!
//! \{
//!
//! \var  size_t Track::npos;
//!       Special value that indicates that the index has no meaningful value.
//!       (set to the largest size_t number possible). 
//!
//! \var  uint32_t Track::BeginIsTrackMin;
//!       The requested interval starts with the *beginning* of the track,
//!       that is, it is *before* any TrackElement.
//!
//! \var  uint32_t Track::BeginIsSegmentMin;
//!       The requested interval starts with the *lower bound* of the `begin`
//!       TrackElement.
//!
//! \var  uint32_t Track::BeginIsSegmentMax;
//!       The requested interval starts with the *upper bound* of the `begin`
//!       TrackElement.
//!
//! \var  uint32_t Track::EndIsTrackMax;
//!       The requested interval ends with the *end* of the track,
//!       that is, it is *after* all TrackElement.
//!
//! \var  uint32_t Track::EndIsSegmentMin;
//!       The requested interval ends with the *lower bound* of the `end`
//!       TrackElement.
//!
//! \var  uint32_t Track::EndIsSegmentMax;
//!       The requested interval ends with the *upper bound* of the `end`
//!       TrackElement.
//!
//! \var  uint32_t Track::BeforeFirstElement;
//!       Tells that the track index is *before* the first TrackElement, the
//!       associated interval is:
//!
//!       >  `[ trackMin, element[begin].getVMin() ]`
//!
//! \var  uint32_t Track::InsideElement;
//!       Tells that the track index is inside a TrackElement, or a set of
//!       TrackElement, the associated interval is:
//!
//!       >  `[ element[begin].getVMin(), element[end].getVMax() ]`
//!
//! \var  uint32_t Track::OutsideElement;
//!       Tells that the track index is betwwen two TrackElement,
//!       the associated interval is:
//!
//!       >  `[ element[begin].getVMax(), element[end].getVMin() ]`
//!
//! \var  uint32_t Track::AfterLastElement;
//!       Tells that the track index is *after* the last TrackElement, the
//!       associated interval is:
//!
//!       >  `[ element[begin].getVMax(), trackMax ]`
//!
//! \var  uint32_t Track::EmptyTrack;
//!       The whole track is empty (the returned indexes should be Track::npos
//!       and must not be used. The associated interval is:
//!
//!       >  `[ trackMin, trackMax ]`
//! \}

  class Track {

    public:
      static const uint32_t BeginIsTrackMin     = (1 <<  0);
      static const uint32_t BeginIsSegmentMin   = (1 <<  1);
      static const uint32_t BeginIsSegmentMax   = (1 <<  2);
      static const uint32_t EndIsTrackMax       = (1 <<  3);
      static const uint32_t EndIsSegmentMin     = (1 <<  4);
      static const uint32_t EndIsNextSegmentMin = (1 <<  5);
      static const uint32_t EndIsSegmentMax     = (1 <<  6);
      static const uint32_t ForFreeInterval     = (1 <<  7);
      static const uint32_t BeforeFirstElement  = BeginIsTrackMin  |EndIsSegmentMin;
      static const uint32_t InsideElement       = BeginIsSegmentMin|EndIsSegmentMax;
      static const uint32_t OutsideElement      = BeginIsSegmentMax|EndIsNextSegmentMin;
      static const uint32_t AfterLastElement    = BeginIsSegmentMax|EndIsTrackMax;
      static const uint32_t EmptyTrack          = BeginIsTrackMin  |EndIsTrackMax;
      static const uint32_t BeginMask           = BeginIsTrackMin  |BeginIsSegmentMin|BeginIsSegmentMax;
      static const uint32_t EndMask             = EndIsTrackMax    |EndIsSegmentMin  |EndIsNextSegmentMin|EndIsSegmentMax;

    public:
    // Static Attributes.
      static const size_t  npos = (size_t)-1;;

    public:
              void               destroy               ();
      virtual bool               isHorizontal          () const = 0;
      virtual bool               isVertical            () const = 0;
      inline  bool               isLocalAssigned       () const;
              void               show                  () const;
      inline  RoutingPlane*      getRoutingPlane       () const;
              RoutingLayerGauge* getLayerGauge         () const;
              KatanaEngine*      getKatanaEngine       () const;
      virtual Flags              getDirection          () const = 0;
      inline  size_t             getIndex              () const;
              uint32_t           getDepth              () const;
              const Layer*       getLayer              () const;
              const Layer*       getBlockageLayer      () const;
      inline  DbU::Unit          getAxis               () const;
      inline  DbU::Unit          getMin                () const;
      inline  DbU::Unit          getMax                () const;
              Track*             getNextTrack          () const;
              Track*             getPreviousTrack      () const;
      inline  size_t             getSize               () const;
      virtual Point              getPosition           ( DbU::Unit coordinate ) const = 0;
              TrackElement*      getSegment            ( size_t  index ) const;
              TrackElement*      getSegment            ( DbU::Unit position ) const;
              TrackElement*      getNext               ( size_t& index, Net* ) const;
              TrackElement*      getPrevious           ( size_t& index, Net* ) const;
              TrackElement*      getNextFixed          ( size_t& index ) const;
              size_t             getSpanRpUnder        ( const Interval&, std::vector<TrackElement*>& ) const;
              size_t             find                  ( const TrackElement* ) const;
              DbU::Unit          getSourcePosition     ( std::vector<TrackElement*>::iterator ) const;
              DbU::Unit          getMinimalPosition    ( size_t index, uint32_t state ) const;
              DbU::Unit          getMaximalPosition    ( size_t index, uint32_t state ) const;
              Interval           getFreeMarkerInterval ( DbU::Unit, Net*, uint32_t threshold ) const;
              Interval           getFreeInterval       ( DbU::Unit position, Net* net=NULL ) const;
              Interval           getNextFree           ( size_t index, Net* net );
              Interval           getPreviousFree       ( size_t index, Net* net );
              bool               hasViaMarker          ( Net* net, Interval span );
              Interval           getOccupiedInterval   ( size_t& begin ) const;
              Interval           expandFreeInterval    ( DbU::Unit position, size_t& begin, size_t& end, uint32_t state, Net* ) const;
              void               getBeginIndex         ( DbU::Unit position, size_t& begin, uint32_t& state, Net* ) const;
              void               getOverlapBounds      ( Interval, size_t& begin, size_t& end, Net* net ) const;
              TrackCost&         addOverlapCost        ( TrackCost& ) const;
              void               getTerminalWeight     ( Interval, Net*, size_t& count, uint32_t& weight ) const;
              DbU::Unit          getSourcePosition     ( size_t index ) const;
              uint32_t           repair                () const;
              void               fillHole              ( DbU::Unit umin, DbU::Unit umax ) const;
              void               disableNearMinArea    () const;
              bool               check                 ( uint32_t& overlaps, const char* message=NULL ) const;
              uint32_t           checkOverlap          ( uint32_t& overlaps ) const;
              uint32_t           checkMinArea          () const;
              void               expandMinArea         ();
     inline   void               setLocalAssigned      ( bool );
              void               invalidate            ();
              void               insert                ( TrackElement* );
              void               insert                ( TrackMarker* );
              void               insert                ( TrackMarkerSpacing* );
      inline  void               updateInvalidBounds   ( TrackElement* );
              void               setSegment            ( TrackElement*, size_t );
              size_t             doRemoval             ();
              void               doReorder             ();
      virtual Record*            _getRecord            () const;
      virtual string             _getString            () const;
      virtual string             _getTypeName          () const = 0;

    protected:
    // Attributes.
      RoutingPlane*                    _routingPlane;
      size_t                           _index;
      DbU::Unit                        _axis;
      DbU::Unit                        _min;
      DbU::Unit                        _max;
      std::vector<TrackElement*>       _segments;
      std::vector<TrackMarker*>        _markers;
      std::vector<TrackMarkerSpacing*> _markersSpacing;
      bool                             _localAssigned;
      bool                             _segmentsValid;
      bool                             _markersValid;
      bool                             _markersSpacingValid;
      DbU::Unit                        _minInvalid;
      DbU::Unit                        _maxInvalid;

    protected:
    // Constructors & Destructors.
                            Track           ( RoutingPlane*, uint32_t index );
      virtual              ~Track           ();
      virtual void          _postCreate     ();
      virtual void          _preDestroy     ();
    private:                                
              Track                         ( const Track& );
              Track&        operator=       ( const Track& );
    protected:
    // Protected functions.
      inline  uint32_t  setMinimalFlags ( uint32_t& state, uint32_t flags ) const;
      inline  uint32_t  setMaximalFlags ( uint32_t& state, uint32_t flags ) const;

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

    public:
      struct Compare {
          inline bool operator() ( const Track* lhs, const Track* rhs ) const;
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

  inline uint32_t  Track::setMinimalFlags ( uint32_t& state, uint32_t flags ) const
  {
    state &=         ~BeginMask;
    state |= (flags & BeginMask);
    return state;
  }

  inline uint32_t  Track::setMaximalFlags ( uint32_t& state, uint32_t flags ) const
  {
    state &=         ~EndMask;
    state |= (flags & EndMask);
    return state;
  }


  inline  void  Track::updateInvalidBounds ( TrackElement* element )
  {
    _minInvalid = std::min( _minInvalid, element->getSourceU() );
    _maxInvalid = std::max( _maxInvalid, element->getTargetU() );
  }
  

  inline bool Track::Compare::operator() ( const Track* lhs, const Track* rhs ) const
  {
    if (lhs->isHorizontal() xor rhs->isHorizontal()) return lhs->isHorizontal();
    if (lhs->getDepth    () !=  rhs->getDepth    ()) return lhs->getDepth() < rhs->getDepth();

    return (lhs->getAxis() < rhs->getAxis());
  }


  class TrackSet : public std::set<Track*,Track::Compare> { };


}  // Katana namespace.


INSPECTOR_P_SUPPORT(Katana::Track);
