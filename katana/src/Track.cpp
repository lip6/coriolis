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
// |  C++ Module  :       "./Track.cpp"                              |
// +-----------------------------------------------------------------+


#include <cstdlib>
#include <sstream>
#include <memory>
#include <algorithm>
#include "hurricane/DebugSession.h"
#include "hurricane/Warning.h"
#include "hurricane/Bug.h"
#include "hurricane/Layer.h"
#include "hurricane/Net.h"
#include "anabatic/AutoContact.h"
#include "katana/RoutingPlane.h"
#include "katana/Track.h"
#include "katana/TrackMarker.h"
#include "katana/DataNegociate.h"
#include "katana/KatanaEngine.h"


namespace {

  using namespace std;
  using Hurricane::DebugSession;
  using namespace CRL;
  using namespace Katana;


  class GapSet {
    public:
                              GapSet       ( const Track* );
      inline       size_t     size         () const;
      inline const vector< pair<size_t,size_t> >&
                              spans        () const;
      inline const pair<size_t,size_t>&
                              span         ( size_t ) const;
      inline       DbU::Unit  sourceU      ( size_t ) const;
      inline       DbU::Unit  targetU      ( size_t ) const;
      inline       DbU::Unit  spansSourceU () const;
      inline       DbU::Unit  spansTargetU () const;
      inline       size_t     spansSourceI () const;
      inline       size_t     spansTargetI () const;
                   void       merge        ( size_t );
                   void       swap         ( GapSet& );
      inline       void       clear        ();
    private:
      const Track*                   _track;                 
      DbU::Unit                      _halfSpacing;
      vector< pair<size_t,size_t> >  _spans;
  };


  GapSet::GapSet ( const Track* track )
    : _track(track)
    , _halfSpacing(_track->getLayer()->getMinimalSpacing()/2)
    , _spans()
  { }

  inline       size_t                         GapSet::size  () const { return _spans.size(); }
  inline const vector< pair<size_t,size_t> >& GapSet::spans () const { return _spans; }
  inline       void                           GapSet::clear ()
  {
    // cerr << "GapSet::clear()" << endl;
    // for ( size_t i=0 ; i < _spans.size() ; ++i ) {
    //   cerr << "  " << i << "=[" << DbU::getValueString(sourceU(i))
    //        <<              " " <<  DbU::getValueString(targetU(i)) << "] " ;
    // }
    // cerr << endl;
    _spans.clear();
  }

  inline size_t     GapSet::spansSourceI () const { return _spans.empty() ? 0 : _spans.front().first; }
  inline size_t     GapSet::spansTargetI () const { return _spans.empty() ? 0 : _spans.back().second; }
  inline DbU::Unit  GapSet::spansSourceU () const { return _spans.empty() ? 0 : sourceU(0); }
  inline DbU::Unit  GapSet::spansTargetU () const { return _spans.empty() ? 0 : targetU(_spans.size()-1); }

  void  GapSet::swap ( GapSet& other )
  {
    std::swap( _track      , other._track );
    std::swap( _halfSpacing, other._halfSpacing );
    std::swap( _spans      , other._spans );
  }
  
  inline const pair<size_t,size_t>& GapSet::span ( size_t i ) const
  {
    static pair<size_t,size_t> nospan = make_pair(0,0);
    return (i < _spans.size()) ? _spans[i] : nospan;
  }

  inline DbU::Unit  GapSet::sourceU  ( size_t i ) const
  {
    // if (_track->getSegment( _spans[i].first)->isNonPref()) {
    //   cerr << "  Non-pref sourceU: " << DbU::getValueString(_track->getSegment( _spans[i].first )->getSourceU()) << endl;
    //   cerr << "  " << _track->getSegment( _spans[i].first ) << endl;
    // }
    return (i<_spans.size()) ? _track->getSegment( _spans[i].first )->getSourceU()+_halfSpacing : 0;
  }

  inline DbU::Unit  GapSet::targetU  ( size_t i ) const
  {
    // if (_track->getSegment( _spans[i].second)->isNonPref()) {
    //   cerr << "  Non-pref targetU: " << DbU::getValueString(_track->getSegment( _spans[i].second )->getTargetU()) << endl;
    //   cerr << "  " << _track->getSegment( _spans[i].second ) << endl;
    // }
    return (i<_spans.size()) ? _track->getSegment( _spans[i].second )->getTargetU()-_halfSpacing : 0;
  }


  void  GapSet::merge ( size_t i )
  {
    TrackElement* element    = _track->getSegment( i );
    DbU::Unit     segSourceU = element->getSourceU()+_halfSpacing;
    DbU::Unit     segTargetU = element->getTargetU()-_halfSpacing;

    if (_spans.empty()) {
      cdebug_log(159,0) << "GapSet::merge() new range ["
                        << DbU::getValueString(segSourceU) << " "
                        << DbU::getValueString(segTargetU) << "] "
                        << i
                        << " " << _track->getSegment(i) << endl;
      _spans.push_back( make_pair(i,i) );
      return;
    }

    size_t     ispan      = 0;
    cdebug_log(159,0) << "GapSet::merge() ["
                      << DbU::getValueString(segSourceU) << " "
                      << DbU::getValueString(segTargetU) << "] "
                      << element << endl;
    for ( ; ispan<_spans.size() ; ++ispan ) {
      if (targetU(ispan) >= segSourceU) {
        if (targetU(ispan) >= segTargetU) {
          cdebug_log(159,0) << "GapSet::merge() already inside range" << endl;
          return;
        }
        _spans[ispan].second = i;
        cdebug_log(159,0) << "GapSet::merge() becomes range end" << endl;
        break;
      }
    }
    if (ispan == _spans.size()) {
      _spans.push_back( make_pair(i,i) );
      cdebug_log(159,0) << "GapSet::merge() new range" << endl;
      return;
    }
    while ( ispan+1 < _spans.size() ) {
      if (targetU(ispan) >= sourceU(ispan+1)) {
        _spans[ispan].second = std::max( targetU(ispan), targetU(ispan+1) );
        _spans.erase( _spans.begin()+ispan+1 );
        cdebug_log(159,0) << "GapSet::merge() fuse with next range" << endl;
      }
    }
  }

  

  struct isDetachedSegment {
      bool operator() ( const TrackElement* s ) { return not s->getTrack(); };
  };


  // DbU::Unit  getPositionByIterator ( const vector<TrackElement*>& v, size_t i )
  // { return (*(v.begin()+i))->getSourceU(); }


  bool  hasSameLayerTurn ( const Layer* layer, TrackElement* segment, Flags flags )
  {
    if (not segment or not segment->base()) return false;
    AutoContact* contact = (flags & Flags::Source) ? segment->base()->getAutoSource()
                                                   : segment->base()->getAutoTarget() ;
    if (contact->getLayer() != layer) return false;
    if (not contact->isTurn()) return false;
  //AutoSegment* pp = contact->getPerpandicular( segment->base() );
    return contact->getPerpandicular(segment->base())->getLength();
  }


  DbU::Unit  toFoundryGrid ( DbU::Unit u, DbU::SnapMode mode )
  {
    DbU::Unit oneGrid = DbU::fromGrid( 1.0 );
    return DbU::getOnCustomGrid( u, oneGrid, mode );
  }


  void  expandToMinArea ( TrackElement*   minSegment
                        , TrackElement*   maxSegment
                        , const Interval& segSpan
                        , DbU::Unit       minFree
                        , DbU::Unit       maxFree )
  {
    DebugSession::open( minSegment->getNet(), 150, 160 );
    cdebug_log(155,1) << "Track::expandMinArea() for:" << endl;
    cdebug_log(155,0) << "minSegment:" << minSegment << endl;
    cdebug_log(155,0) << "maxSegment:" << maxSegment << endl;

    const Layer* layer      = minSegment->getLayer();
    double       minArea    = layer->getMinimalArea();
    DbU::Unit    minSpacing = layer->getMinimalSpacing();
    DbU::Unit    minLength
      = DbU::fromPhysical( minArea / DbU::toPhysical( minSegment->getWidth(), DbU::UnitPower::Micro )
                         , DbU::UnitPower::Micro );
    minLength = toFoundryGrid( minLength, DbU::Superior );
    DbU::Unit segLength = segSpan.getSize() - minSpacing;
    cdebug_log(155,0) << "minSpacing:" << DbU::getValueString(minSpacing) << endl;
    cdebug_log(155,0) << " minLength:" << DbU::getValueString(minLength)  << endl;
    cdebug_log(155,0) << " segLenght:" << DbU::getValueString(segLength)  << endl;

    DbU::Unit sourceECap = minSegment->base()->getExtensionCap( Flags::Source|Flags::LayerCapOnly );
    DbU::Unit targetECap = maxSegment->base()->getExtensionCap( Flags::Target|Flags::LayerCapOnly );
    cdebug_log(155,0) << "sourceECap:" << DbU::getValueString(sourceECap) << endl;
    cdebug_log(155,0) << "targetECap:" << DbU::getValueString(targetECap) << endl;

    if (segLength < minLength) {
      DbU::Unit marginLeft  = segSpan.getVMin() - minFree;
      DbU::Unit marginRight = maxFree - segSpan.getVMax();
      DbU::Unit expandLeft  = toFoundryGrid( (minLength - segLength)/2 + sourceECap, DbU::Inferior );
      DbU::Unit expandRight = toFoundryGrid( (minLength - segLength)/2 + targetECap, DbU::Superior );
      if ((marginLeft >= expandLeft) and (marginRight >= expandRight)) {
        minSegment->base()->setDuSource( minSegment->base()->getDuSource() - expandLeft  );
        maxSegment->base()->setDuTarget( maxSegment->base()->getDuTarget() + expandRight );
      } else {
        if (marginLeft + marginRight >= expandLeft + expandRight) {
          DbU::Unit shiftLeft = 0;
          if (marginLeft >= expandLeft + expandRight)
            shiftLeft = - expandRight;
          else
            shiftLeft = - marginLeft + expandLeft;
          minSegment->base()->setDuSource( minSegment->base()->getDuSource() - expandLeft  + shiftLeft );
          maxSegment->base()->setDuTarget( maxSegment->base()->getDuTarget() + expandRight + shiftLeft );
        } else {
          cerr << Error( "::expandToMinArea(): Cannot expand %s."
                       , getString(minSegment).c_str() ) << endl;
        }
      }
    }
    cdebug_tabw(155,-1);
    DebugSession::close();
  }


} // Anonymous namespace.


namespace Katana {

  using std::lower_bound;
  using std::remove_if;
  using std::sort;
  using Hurricane::dbo_ptr;
  using Hurricane::tab;
  using Hurricane::DebugSession;
  using Hurricane::Warning;
  using Hurricane::Bug;
  using Hurricane::Layer;
  using Hurricane::Net;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;


// -------------------------------------------------------------------
// Class  :  "Track".


  const size_t  Track::npos = (size_t)-1;


  Track::Track ( RoutingPlane* routingPlane, unsigned int index )
    : _routingPlane (routingPlane)
    , _index        (index)
    , _axis         (routingPlane->getTrackPosition(index))
    , _min          (routingPlane->getTrackMin())
    , _max          (routingPlane->getTrackMax())
    , _segments     ()
    , _markers      ()
    , _localAssigned(false)
    , _segmentsValid(false)
    , _markersValid (false)
  { }


  void  Track::_postCreate ()
  { }


  Track::~Track ()
  { cdebug_log(155,0) << "Track::~Track() - " << (void*)this << endl; }


  void  Track::_preDestroy ()
  {
    cdebug_log(155,1) << "Track::_preDestroy() - " << (void*)this << " " << this << endl;

    TrackSet dummy;
    for ( size_t i=0 ; i<_segments.size() ; i++ )
      if (_segments[i]) {
        _segments[i]->detach( dummy );
        if (not _segments[i]->getTrackCount()) {
        //cerr << "destroy " << _segments[i] << endl;
          _segments[i]->destroy();
        }
      }

    for ( size_t i=0 ; i<_markers.size() ; i++ )
      if (_markers[i]) _markers[i]->destroy();

    cdebug_tabw(155,-1);
  }


  void  Track::destroy ()
  {
    cdebug_log(155,1) << "Track::destroy() - " << (void*)this << " " << this << endl;

    Track::_preDestroy();
    delete this;

    cdebug_tabw(155,-1);
  }


  KatanaEngine* Track::getKatanaEngine () const
  { return _routingPlane->getKatanaEngine(); }


  RoutingLayerGauge* Track::getLayerGauge () const
  { return _routingPlane->getLayerGauge(); }


  uint32_t  Track::getDepth () const
  { return _routingPlane->getDepth(); }


  const Layer* Track::getLayer () const
  { return _routingPlane->getLayer(); }


  const Layer* Track::getBlockageLayer () const
  { return _routingPlane->getBlockageLayer(); }


  Track* Track::getNextTrack () const
  { return getRoutingPlane()->getTrackByIndex( getIndex()+1 ); }


  Track* Track::getPreviousTrack () const
  {
    if (not getIndex()) return NULL;
    return getRoutingPlane()->getTrackByIndex( getIndex()-1 );
  }


  TrackElement* Track::getSegment ( size_t index ) const
  {
    if ( (index == npos) or (index >= getSize()) ) return NULL;
    return _segments[index];
  }


  TrackElement* Track::getSegment ( DbU::Unit position ) const
  {
    uint32_t  state;
    size_t    begin;

    getBeginIndex( position, begin, state );

    if (state & (BeginIsTrackMin|EndIsTrackMax)) return NULL;
    return getSegment(begin);
  }


  TrackElement* Track::getNext ( size_t& index, Net* net ) const
  {
    for ( index++ ; index < _segments.size() ; index++ ) {
      if (_segments[index]->getNet() == net) continue;
      return _segments[index];
    }
    index = npos;

    return NULL;
  }


  TrackElement* Track::getPrevious ( size_t& index, Net* net ) const
  {
    cdebug_log(155,0) << "Track::getPrevious() " << index << " for " << net << endl;
    if ((index == npos) or (index == 0)) return NULL;
    do {
      --index;
      cdebug_log(155,0) << "| " << index << ":" << _segments[index] << endl;
      if (_segments[index]->getNet() != net) return _segments[index];
    } while ( index != 0 );
    index = npos;
    return NULL;
  }


  TrackElement* Track::getNextFixed ( size_t& index ) const
  {
    TrackElement* nextFixed = getNext ( index, NULL );
    for ( ; nextFixed ; nextFixed = getNext(index,NULL) ) {
      if ( nextFixed->base()->isFixed() ) return nextFixed;
    }

    return nextFixed;
  }


  void  Track::getBeginIndex ( DbU::Unit position, size_t& begin, uint32_t& state ) const
  {
    cdebug_log(155,0) << "Track::getBeginIndex(): @" << DbU::getValueString(position)
                      << " begin=" << begin << endl;
    if (_segments.empty()) {
      state = EmptyTrack;
      begin = 0;
      return;
    }

    if (position < _min) {
      cerr << Warning( " Position %s inferior to the lower bound of %s. Returning npos."
                     , DbU::getValueString(position).c_str()
                     , getString(this).c_str() ) << endl;
      state = BeforeFirstElement;
      begin = 0;
      return;
    }

    if (position > _max) {
      cerr << Warning( " Position %s superior to the upper bound of %s. Returning npos."
                     , DbU::getValueString(position).c_str()
                     , getString(this).c_str() ) << endl;
      state = AfterLastElement;
      begin = _segments.size()-1;
      return;
    }

    vector<TrackElement*>::const_iterator  lowerBound
      = lower_bound( _segments.begin(), _segments.end(), position, SourceCompare() );
    begin = lowerBound - _segments.begin();

  // This is suspicious.
  // I guess this has been written for the case of overlapping segments from the same
  // net, we find the first one of the overlapped sets. But what if they are not overlapping
  // but still from the same net?
    size_t sameNetDelta = 0;
    if (begin < _segments.size()) {
      for ( ; (begin > 0) and (_segments[begin-1]->getNet() == _segments[begin]->getNet())
            ; --begin, ++sameNetDelta );
    }

    state = 0;
    if ( (begin == 0) and (position < _segments[0]->getSourceU()) ) {
      state = BeforeFirstElement;
    } else {
      cdebug_log(155,0) << "  Expanding from begin=" << begin << endl;
      if (begin and not sameNetDelta) begin -= 1;

      size_t   usedBegin    = begin;
      Interval usedInterval = getOccupiedInterval( usedBegin );
      cdebug_log(155,0) << "  Used interval " << usedInterval << endl;

      if (usedInterval.isEmpty()) {
        for ( size_t j=0 ; j<_segments.size() ; ++j ) {
          cerr << j << ":" << _segments[j] << endl;
        }
      }

      if (position < usedInterval.getVMax())
        state = InsideElement;
      else
        if (begin+1 == _segments.size())
          state = AfterLastElement;
        else
          state = OutsideElement;
    }
  }


  void  Track::getOverlapBounds ( Interval interval, size_t& begin, size_t& end ) const
  {
    uint32_t iState;

    if (  _segments.empty()
       or (interval.getVMax() <= _min)
       or (interval.getVMin() >= _max)) {
      begin = end = npos;
      return;
    }

    getBeginIndex      ( interval.getVMin(), begin, iState );
    getOccupiedInterval( begin );

    getBeginIndex( interval.getVMax(), end, iState );
    for ( ; end < _segments.size() ; ++end ) {
      if (_segments[end]->getSourceU() >= interval.getVMax()) break;
    }

    cdebug_log(155,0) << "Track::getOverlapBounds(): begin:" << begin << " end:" << end << " AfterLastElement:" << (iState == AfterLastElement) << endl;
  }


  TrackCost& Track::addOverlapCost ( TrackCost& cost ) const
  {
          size_t    begin        = Track::npos;
          size_t    end          = Track::npos;
    const Interval& interval     = cost.getInterval();
          Interval  freeInterval = getFreeInterval( interval.getCenter(), cost.getNet() );

    if (not freeInterval.contains(interval)) {
      cost.setFreeLength( 0 );
    } else {
      cost.setFreeLength( freeInterval.getSize() );
    }
    getOverlapBounds( cost.getInterval(), begin, end );
    cost.setTrack( const_cast<Track*>(this), begin, end );

    cdebug_log(155,1) << "addOverlapCost() @" << DbU::getValueString(_axis)
                      << " [" << DbU::getValueString(interval.getVMin())
                      << ":"  << DbU::getValueString(interval.getVMax())
                      << "] <-> [" << begin << ":"  << end << "]"
                      << endl;
    cdebug_log(155,0) << "initial:" << &cost << endl;
    cdebug_log(155,0) << "freeInterval [" << DbU::getValueString(freeInterval.getVMin())
                      << ":"  << DbU::getValueString(freeInterval.getVMax()) << "]"
                      << endl;

    vector<TrackMarker*>::const_iterator lowerBound
      = lower_bound( _markers.begin(), _markers.end(), interval.getVMin(), TrackMarker::Compare() );
    size_t mbegin = lowerBound - _markers.begin();

    for ( ;     (mbegin < _markers.size())
            and (_markers[mbegin]->getSourceU() <= interval.getVMax()) ; mbegin++ ) {
      cdebug_log(155,0) << "| @" << DbU::getValueString(_axis) << " " << _markers[mbegin] << endl;
      if (_markers[mbegin]->getNet() != cost.getNet()) {
        cdebug_log(155,0) << "* Mark: @" << DbU::getValueString(_axis) << " " << _markers[mbegin] << endl;
        cost.incTerminals( _markers[mbegin]->getWeight(this) );

        if ( (_markers[mbegin]->getRefCount() == 1) and (interval.contains(_markers[mbegin]->getSpan())) ) {
          cdebug_log(155,0) << "  Total overlap of a one track terminal: infinite cost." << endl;
          cost.setInfinite();
        }
      }
    }

    if (begin == npos) {
      if (not _segments.empty()
         and (_segments.back()->getNet() == cost.getNet())
         and (cost.getRefElement()->getAxis() != getAxis())
         ) {
        Interval overlap = interval.getIntersection( _segments.back()->getCanonicalInterval() );
        cdebug_log(155,0) << "overlap:" << overlap
                          << " size:" << DbU::getValueString(overlap.getSize()) << endl;
        if (overlap.getSize() > 0) {
          cdebug_log(155,0) << "Same net overlap, increase delta shared." << endl;
          cost.incDeltaShared ( overlap.getSize() );
        }
      }
      cdebug_log(155,0) << "  begin == npos (after last TrackElement)." << endl;
      cdebug_log(155,0) << "  current:" << &cost << endl;
      cdebug_tabw(155,-1);
      return cost;
    }

    for ( ; begin < end ; begin++ ) {
      Interval overlap = interval.getIntersection( _segments[begin]->getCanonicalInterval() );
      cdebug_log(155,0) << "overlap:" << overlap
                        << " size:" << DbU::getValueString(overlap.getSize()) << endl;
      if (overlap.getSize() == 0) continue;
      
      if (    (_segments[begin]->getNet() == cost.getNet())
         and ((cost.getRefElement()->getAxis() != getAxis())
             or not _segments[begin]->isNonPref() ) ) {
        if (  (_segments[begin] == cost.getRefElement())
           or (_segments[begin] == cost.getSymElement())) {
          cdebug_log(155,0) << "Segment istself in track, skip." << endl;
          continue;
        }
        if (     cost.doIgnoreShort()
           and (_segments[begin]->getLength() < 2*_segments[begin]->getPPitch())) {
          cdebug_log(155,0) << "Overlap with same net and less than one p-pitch, skip." << endl;
          continue;
        }
        cdebug_log(155,0) << "Same net overlap, increase delta shared ("
                          <<            DbU::getValueString(_segments[begin]->getLength())
                          << " > 2*" << DbU::getValueString(_segments[begin]->getPPitch())
                          << ")" << endl;
        cost.incDeltaShared ( overlap.getSize() );
      }
      _segments[begin]->incOverlapCost( cost );
      cdebug_log(155,0) << "| overlap: " << _segments[begin] << endl;
      cdebug_log(155,0) << "| current cost:" << &cost << endl;

      if (cost.isInfinite()) break;
    }

    cdebug_tabw(155,-1);

    return cost;
  }


  void  Track::getTerminalWeight ( Interval interval, Net* net, size_t& count, uint32_t& weight ) const
  {
    cdebug_log(155,1) << "getTerminalWeight() @" << DbU::getValueString(_axis)
                      << " [" << interval.getVMin() << " " << interval.getVMax() << "]" << endl;

  //count  = 0;
  //weight = 0;

    vector<TrackMarker*>::const_iterator lowerBound
      = lower_bound ( _markers.begin(), _markers.end(), interval.getVMin(), TrackMarker::Compare() );
    size_t mbegin = lowerBound - _markers.begin();

    for ( ;    (mbegin < _markers.size())
            && (_markers[mbegin]->getSourceU() <= interval.getVMax()) ; mbegin++ ) {
      cdebug_log(155,0) << "| @" << DbU::getValueString(_axis) << _markers[mbegin] << endl;
      if ( _markers[mbegin]->getNet() == net ) {
        cdebug_log(155,0) << "* Mark: @" << DbU::getValueString(_axis) << " " << _markers[mbegin] << endl;
        weight += _markers[mbegin]->getWeight(this);
        ++count;
      }
    }
    cdebug_tabw(155,-1);
  }


  size_t  Track::find ( const TrackElement* segment ) const
  {
    if (_segments.empty()) return npos;

    vector<TrackElement*>::const_iterator  lowerBound
      = lower_bound( _segments.begin()
                   , _segments.end()
                   , segment
                   , SegmentCompare()
                   );

    if (lowerBound != _segments.end()) {
      while ( segment->getSourceU() == (*lowerBound)->getSourceU() ) {
        if (*lowerBound == segment) return (size_t)(lowerBound-_segments.begin());
        lowerBound++;
      }
    }

    return npos;
  }


  Interval  Track::getFreeInterval ( DbU::Unit position, Net* net ) const
  {
    uint32_t  state = 0;
    size_t    begin = 0;
    size_t    end   = 0;

    if (_segments.empty()) return Interval(_min,_max);

    getBeginIndex( position, begin, state );
    if ( (state == InsideElement) and (_segments[begin]->getNet() != net) ) {
      cdebug_log(155,0) << "Track::getFreeInterval(): Inside other element @" << begin
                        << " - " << _segments[begin] << endl;
      return Interval();
    }

    end = begin;
    return expandFreeInterval( begin, end, state, net );
  }


  Interval  Track::expandFreeInterval ( size_t& begin, size_t& end, uint32_t state, Net* net ) const
  {
    cdebug_log(155,1) << "Track::expandFreeInterval() begin:" << begin << " end:" << end
                      << " state:" << state << " " << net << endl;
    if (begin != Track::npos)
      cdebug_log(155,0) << _segments[begin] << endl;

    DbU::Unit minFree = _min;
    cdebug_log(155,0) << "minFree:" << DbU::getValueString(minFree) << " (track min)" << endl;

    if (not (state & BeginIsTrackMin) and (begin > 0)) {
      if (_segments[begin]->getNet() == net)
        getPrevious( begin, net );

      if (begin != npos) {
        minFree = getOccupiedInterval(begin).getVMax();
        cdebug_log(155,0) << "minFree:" << DbU::getValueString(minFree) << " begin:" << begin << endl;
      }
    }

    if (not (state & EndIsTrackMax) ) {
      if (state & EndIsNextSegmentMin) ++end;
 
      if (_segments[end]->getNet() == net) {
        getNext( end, net );
        if (end != npos) {
          cdebug_log(155,0) << "| same net, end:" << end << " " << _segments[end] << endl;
        } else {
          cdebug_log(155,0) << "| same net, end:" << end << " (after last)" << endl;
        }
      }
      if (end == npos) {
        end  = _segments.size() - 1;
        setMaximalFlags( state, EndIsTrackMax );
      } else {
        setMaximalFlags( state, EndIsSegmentMin );
      }
      cdebug_log(155,0) << "end:" << end << " state:" << state << endl;
      cdebug_log(155,0) << "end:" << _segments[end] << endl;
    }

    cdebug_tabw(155,-1);

    return Interval( minFree, getMaximalPosition(end,state) );
  }


  void  Track::invalidate ()
  { _segmentsValid = false; }


  void  Track::insert ( TrackMarker* marker )
  {
    _markers.push_back ( marker );
    _markersValid = false;
  }


  void  Track::insert ( TrackElement* segment )
  {
    // cdebug_log(9000,0) << "Deter| Track::insert() " << getLayer()->getName()
    //             << " @" << DbU::getValueString(getAxis()) << " " << segment << endl;
    cdebug_log(155,1) << "Track::insert() " << getLayer()->getName()
                << " @" << DbU::getValueString(getAxis()) << " " << segment << endl;

    if (   (getLayer()->getMask()         != segment->getLayer()->getMask())
       and (getBlockageLayer()->getMask() != segment->getLayer()->getMask()) ) {
      cerr << Bug("Track::insert(), segment %s has not the right layer."
                 ,getString(segment).c_str()) << endl;
    }

    cdebug_log(159,0) << "Insert in [" << 0 << "] " << this << segment << endl;
    _segments.push_back( segment );
    _segmentsValid = false;

    if (segment->isWide() or segment->isNonPref()) {
      cdebug_log(155,0) << "Segment is wide or non-pref, trackSpan:" << segment->getTrackSpan() << endl;
      Track* wtrack = getNextTrack();
      for ( size_t i=1 ; wtrack and (i<segment->getTrackSpan()) ; ++i ) {
        cdebug_log(159,0) << "Insert in [" << i << "] " << wtrack << segment << endl;
        wtrack->_segments.push_back ( segment );
        wtrack->_segmentsValid = false;
        wtrack = wtrack->getNextTrack();
      }
    }

    segment->setTrack ( this );
    cdebug_tabw(155,-1);
  }


  void  Track::setSegment ( TrackElement* segment, size_t index )
  {
    if ( index >= _segments.size() ) return;
    _segments[index] = segment;
  }


  bool  Track::check ( uint32_t& overlaps, const char* message ) const
  {
  //if ((getIndex() != 3556) or not isHorizontal()) return true;

    bool coherency = true;
    bool holes     = false;

    if (message) cerr << "     o  Checking Track - " << message << endl;
    cdebug_log(155,0) << /*(void*)this << ":" <<*/ this << endl;

    for ( size_t i=0 ; i<_segments.size() ; i++ ) {
      if (_segments[i]) {
        bool inTrackRange = false;

        if (_segments[i]->isNonPref()) {
          DbU::Unit min = 0;
          DbU::Unit max = 0;
          _segments[i]->base()->getCanonical( min, max );
          inTrackRange = Interval(min,max).contains( _axis );
        } else {
          Interval trackRange ( _segments[i]->getAxis() - (_segments[i]->getTrackSpan()*_segments[i]->getPitch())/2
                              , _segments[i]->getAxis() + (_segments[i]->getTrackSpan()*_segments[i]->getPitch())/2 );
          inTrackRange = trackRange.contains( _axis );
        }
        
        if (i) {
          if (_segments[i-1] == _segments[i]) {
            cerr << "[CHECK] incoherency at " << i << " "
                 << _segments[i] << " is duplicated. " << endl;
            coherency = false;
          }
        }
        if (not _segments[i]->getTrack()) {
          cerr << "[CHECK] incoherency at " << i << " "
               << _segments[i] << " is detached." << endl;
          coherency = false;
        } else { 
          if ( (_segments[i]->getTrack() != this) and not inTrackRange ) {
            cerr << "[CHECK] incoherency at " << i << " "
                 << _segments[i] << " is in track "
                 << _segments[i]->getTrack() << endl;
            coherency = false;
            cerr << _segments[i]->getTrackSpan() << endl;
          }
        }
        if ( (_segments[i]->getAxis() != getAxis()) and not inTrackRange ) {
          cerr << "[CHECK] incoherency at " << i << " "
               << _segments[i] << " is not on Track axis "
               << DbU::getValueString(getAxis()) << "." << endl;
          coherency = false;
        } 
        
        coherency = _segments[i]->_check() and coherency;
      } else {
        cerr << "[CHECK] Hole at position " << i << "." << endl;
        holes     = true;
        coherency = false;
      }
    }

    if (not holes)
      coherency = (checkOverlap(overlaps) == 0) and coherency;

    coherency = (checkMinArea() == 0) and coherency;

    return coherency;
  }


  DbU::Unit  Track::getSourcePosition ( size_t  i ) const
  {
    if ( i == npos) return 0;

    return _segments[i]->getSourceU();
  }


  DbU::Unit  Track::getSourcePosition ( vector<TrackElement*>::iterator isegment ) const
  {
    if ( isegment == _segments.end() ) return 0;

    return (*isegment)->getSourceU();
  }


  DbU::Unit  Track::getMinimalPosition ( size_t index, uint32_t state ) const
  {
    Interval  canonical;

    switch ( state & BeginMask ) {
      case BeginIsTrackMin:   return _min;
      case BeginIsSegmentMin: return _segments[index]->getSourceU ();
      case BeginIsSegmentMax: return _segments[index]->getTargetU ();
    }

    cerr << Bug( " Track::getMinimalPosition(size_t,uint32_t) :"
                 " invalid state value %ud.", state ) << endl;

    return _min;
  }


  DbU::Unit  Track::getMaximalPosition ( size_t index, uint32_t state ) const
  {
    Interval  canonical;

    switch ( state & EndMask ) {
      case EndIsTrackMax:       return _max;
      case EndIsSegmentMin:     return _segments[index  ]->getSourceU ();
      case EndIsNextSegmentMin: if (index+1 >= getSize()) return _max;
                                return _segments[index+1]->getSourceU ();
      case EndIsSegmentMax:     return _segments[index  ]->getTargetU ();
    }

    cerr << Bug( " Track::getMaximalPosition(size_t,uint32_t) :"
                 " invalid state value %ud.", state ) << endl;

    return _min;
  }


  Interval  Track::getNextFree ( size_t index, Net* net )
  {
    if (_segments.empty()) return Interval( _min, _max );

    if (index >= _segments.size()) {
      size_t last = _segments.size()-1;
      return Interval( getOccupiedInterval(last).getVMax(), _max );
    }

    Interval occupied = getOccupiedInterval( index );
    return getFreeInterval( occupied.getVMax()+1, net );
  }


  Interval  Track::getPreviousFree ( size_t index, Net* net )
  {
    cdebug_log(155,1) << "Track::getPreviousFree() index:" << index << " " << net << endl;

    if (_segments.empty()) { cdebug_tabw(155,-1); return Interval( _min, _max ); }

    if (index == 0) {
      size_t   first = 0;
      Interval free  = Interval( _min, getOccupiedInterval(first).getVMin() );
      cdebug_tabw(155,-1);
      return free;
    }

    Interval occupied = getOccupiedInterval( index );
    cdebug_log(155,0) << "Previous occupied:" << occupied << endl;
    Interval free = getFreeInterval( occupied.getVMin()-1, net );
    cdebug_tabw(155,-1);
    return free;
  }


  Interval  Track::getOccupiedInterval ( size_t& begin ) const
  {
    cdebug_log(155,0) << "Track::getOccupiedInterval() begin=" << begin << endl;
    if (begin == npos) return Interval();

    size_t  seed  = begin;
    Net*    owner = _segments[seed]->getNet();

    Interval  segmentInterval;
    Interval  mergedInterval;
    _segments[seed]->getCanonical( mergedInterval );

    cdebug_log(155,0) << "| seed:" << mergedInterval << " " << _segments[seed] << endl;

    size_t i = seed;
    while ( --i != npos ) {
      if (_segments[i]->getNet() != owner) break;

      _segments[i]->getCanonical ( segmentInterval );
      if (segmentInterval.getVMax() >= mergedInterval.getVMin()) {
        cdebug_log(155,0) << "| merge (prev):" << segmentInterval << " " << _segments[i] << endl;

        mergedInterval.merge( segmentInterval );
        begin = i;
      }
    }

    i = seed;
    while ( ++i < _segments.size() ) {
      if (_segments[i]->getNet() != owner) break;

      _segments[i]->getCanonical( segmentInterval );
      if (segmentInterval.getVMin() > mergedInterval.getVMax()) break;

      cdebug_log(155,0) << "| merge (next):" << _segments[i] << endl;

      mergedInterval.merge( segmentInterval );
    }

    return mergedInterval;
  }


  size_t  Track::doRemoval ()
  {
    cdebug_log(155,1) << "Track::doRemoval() - " << this << endl;

    size_t  size = _segments.size();

    vector<TrackElement*>::iterator  beginRemove
      = remove_if( _segments.begin(), _segments.end(), isDetachedSegment() );

    _segments.erase( beginRemove, _segments.end() );

    cdebug_log(155,0) << "After doRemoval " << this << endl;
    cdebug_tabw(155,-1);

    return size - _segments.size();
  }


  void  Track::doReorder ()
  {
    cdebug_log(155,0) << "Track::doReorder() " << this << endl;

    if (not _segmentsValid) {
      std::sort( _segments.begin(), _segments.end(), SegmentCompare() );
      _segmentsValid = true;
    }

    if (not _markersValid) {
      std::sort( _markers.begin(), _markers.end(), TrackMarker::Compare() );
      _markersValid = true;
    }
  }


  uint32_t  Track::repair () const
  {
  //if ((getIndex() == 3473) and isHorizontal()) DebugSession::open( 150, 160 );
    cdebug_log(159,0) << "Track::repair() " << this << endl;
    
    if (_segments.empty()) {
      fillHole( getMin(), getMax() );
    //if ((getIndex() == 3473) and isHorizontal()) DebugSession::close();
      return 0;
    }
    DbU::Unit minSpacing = getLayer()->getMinimalSpacing();

    DbU::Unit spacing = _segments[0]->getSourceU() - getMin();
    if (spacing > 10*getLayerGauge()->getPitch())
      fillHole( getMin(), _segments[0]->getSourceU() );

    bool     netChange  = false;
    uint32_t gaps       = 0;
    GapSet   gapsetPrev ( this );
    GapSet   gapsetCurr ( this );
    for ( size_t i=0 ; i<_segments.size()-1 ; i++ ) {
      cdebug_log(159,0) << "[" << i << "] " << _segments[i] << endl;
      if (_segments[i]->isNonPref()) continue;
      netChange = false;
      gapsetCurr.merge( i );
      if (  (_segments[i]->getNet() != _segments[i+1]->getNet())
         /*or  _segments[i]->getLayer()->isBlockage()*/ ) {
        netChange = true;
        if (gapsetPrev.size() and gapsetCurr.size()) {
          spacing = gapsetCurr.spansSourceU() - gapsetPrev.spansTargetU();
          if (spacing < minSpacing) {
            spacing = minSpacing - spacing;
            TrackElement* element = _segments[ gapsetPrev.span(gapsetPrev.size()-1).second ];
            AutoSegment*  prev    = element->base();
            if (prev and (prev->getDuTarget() >= spacing)) {
              prev->setDuSource( prev->getDuSource() - spacing );
              prev->setDuTarget( prev->getDuTarget() - spacing );
              element->invalidate();
              cerr << Warning( "Track::repair(): Enlarging narrow gap in %s near (shift left):\n"
                               "          %s"
                             , getString(this).c_str()
                             , getString(prev).c_str() ) << endl;
            } else {
              TrackElement* element = _segments[ gapsetCurr.span(0).first ];
              AutoSegment*  curr    = element->base();
              if (curr and (-curr->getDuSource() >= spacing)) {
                curr->setDuSource( curr->getDuSource() + spacing );
                curr->setDuTarget( curr->getDuTarget() + spacing );
                element->invalidate();
                cerr << Warning( "Track::repair(): Enlarging narrow gap in %s near (shift right):\n"
                                 "          %s"
                               , getString(this).c_str()
                               , getString(curr).c_str() ) << endl;
              }
            }
          } else if (spacing > 10*getLayerGauge()->getPitch())
            fillHole( gapsetPrev.spansTargetU(), gapsetCurr.spansSourceU() );
        }
      }

      if ((i+2 == _segments.size()) and (_segments[i]->getNet() == _segments[i+1]->getNet()))
        gapsetCurr.merge( i+1 );

      if (netChange or (i+2 == _segments.size())) {
        if (gapsetCurr.size() > 1) {
          cdebug_log(159,0) << "potential gap around " << _segments[i] << endl;
          for ( size_t j=0 ; j+1 < gapsetCurr.size() ; ++j ) {
            cdebug_log(159,0) << j << "=[" << DbU::getValueString(gapsetCurr.sourceU(j))
                              <<       " " << DbU::getValueString(gapsetCurr.targetU(j)) << "], "
                              << j+1 << "=[" << DbU::getValueString(gapsetCurr.sourceU(j+1))
                              <<         " " << DbU::getValueString(gapsetCurr.targetU(j+1)) << "]" << endl;
            spacing = gapsetCurr.sourceU(j+1) - gapsetCurr.targetU(j);
            cdebug_log(159,0) << "| spacing=" << DbU::getValueString(spacing) << endl;
            if (spacing < minSpacing) {
              cdebug_log(159,0) << j << "=[" << DbU::getValueString(gapsetCurr.sourceU(j))
                                <<       " " << DbU::getValueString(gapsetCurr.targetU(j)) << "], "
                                << j+1 << "=[" << DbU::getValueString(gapsetCurr.sourceU(j+1))
                                <<         " " << DbU::getValueString(gapsetCurr.targetU(j+1)) << "]" << endl;
              if (gapsetCurr.span(j+1).first >= _segments.size()) {
                cerr << Bug("Track::repair(): Assersion gapsetCurr.span(j+1).first < _segments.size() is false.") << endl;
              } else {
                TrackElement* element = _segments[gapsetCurr.span(j+1).first];
                AutoSegment*  first   = element->base();
                
                cdebug_log(159,0) << "spacing:" << DbU::getValueString(spacing) << " " << first << endl;
	            if (first == NULL) {
		          cerr << Bug("Track::repair(): Base of first element is NULL, *unable* to correct gap.") << endl;
                } else if (not first->isNonPref()) {
                  for ( AutoSegment* segment : first->getAligneds() ) {
                    if (segment->getSourcePosition() < first->getSourcePosition())
                      first = segment;
                  }
                  spacing += first->getSourceU() - gapsetCurr.sourceU(j+1);
                  cdebug_log(159,0) << "spacing:" << DbU::getValueString(spacing) << " " << first << endl;
                  cdebug_log(159,0) << "duSource:" << DbU::getValueString(first->getDuSource()) << endl;
                //first->setDuSource( first->getDuSource() - spacing - minSpacing/2 );
                  first->setDuSource( first->getDuSource() - spacing );
                  element->invalidate();
	            }
                ++gaps;
                cerr << Warning( "Track::repair(): Closing same net gap in %s near:\n          %s"
                               , getString(this).c_str()
                               , getString(_segments[(i) ? i-1 : 0]).c_str() ) << endl;
                cdebug_log(159,0) << first << endl;
              }
            } else if (spacing > 10*getLayerGauge()->getPitch())
              fillHole( gapsetCurr.targetU(j), gapsetCurr.sourceU(j+1) );
          }
        }

        gapsetPrev.swap( gapsetCurr );
        gapsetCurr.clear();
      }
    }

    Net*      lastNet     = NULL;
    DbU::Unit lastTargetU = getMax();
    for ( auto isegment = _segments.rbegin() ; isegment != _segments.rend() ; ++isegment ) {
      if (not lastNet) {
        lastNet     = (*isegment)->getNet();
        lastTargetU = (*isegment)->getTargetU();
      } else {
        if (lastNet != (*isegment)->getNet()) break;
        lastTargetU = max( lastTargetU, (*isegment)->getTargetU() );
      }
    }

    spacing = getMax() - lastTargetU;
    if (spacing > 10*getLayerGauge()->getPitch())
      fillHole( lastTargetU, getMax() );

  //if ((getIndex() == 3473) and isHorizontal()) DebugSession::close();
    return gaps;
  }


  uint32_t  Track::checkOverlap ( uint32_t& overlaps ) const
  {
    if ( !_segments.size() ) return 0;

    size_t j = 0;
    for ( size_t i=0 ; i<_segments.size()-1 ; i++ ) {
      if (not _segments[i]->isBlockage() and (_segments[i]->getLayer() != getLayer()) ) {
        cerr << Error( " Track vs. Segment Layer discrepency:\n  %s\n  %s  "
                     , getString(this).c_str()
                     , getString(_segments[i]).c_str() ) << endl;
      }

      if ( _segments[i]->getNet() == _segments[i+1]->getNet() ) {
        if ( _segments[i]->getSourceU() == _segments[i+1]->getSourceU() ) {
          if ( _segments[i]->getTargetU() < _segments[i+1]->getTargetU() ) {
            cerr << Error(" Invalid sorting length order in %s:\n  %s\n  %s  "
                         ,getString(this).c_str()
                         ,getString(_segments[i  ]).c_str()
                         ,getString(_segments[i+1]).c_str()) << endl;
          }
        }
        for ( j=i+1 ; (j<_segments.size()) && (_segments[i]->getNet() == _segments[j]->getNet()) ; j++ );
      } else {
        j = i+1;
      }

      if (    (j<_segments.size())
          and (_segments[i]->getTargetU() > _segments[j]->getSourceU()) ) {
        cerr << Error("Overlap in %s between:\n  %s\n  %s\n  TargetU:%s SourceU:%s"
                     ,getString(this).c_str()
                     ,getString(_segments[i]).c_str()
                     ,getString(_segments[j]).c_str()
                     ,DbU::getValueString(_segments[i]->getTargetU()).c_str()
                     ,DbU::getValueString(_segments[j]->getSourceU()).c_str() ) << endl;
        overlaps++;
      }
    }

    return overlaps;
  }


  void  Track::fillHole ( DbU::Unit umin, DbU::Unit umax ) const
  {
    if (not getKatanaEngine()->getTrackFill()) return;
    if (getIndex() % getKatanaEngine()->getTrackFill() != 0) return;
    if (getLayerGauge()->getType() == Constant::PinOnly) return;
    if (getLayerGauge()->getDepth() > getKatanaEngine()->getConfiguration()->getAllowedDepth()) return;

    Net* fillNet = getKatanaEngine()->getCell()->getNet( "dummy_filler_net" );
    if (not fillNet)
      fillNet = Net::create( getKatanaEngine()->getCell(), "dummy_filler_net" );

    DbU::Unit gapLength = umax - umin;
    if (gapLength > 10*getLayerGauge()->getPitch()) {
      if (isHorizontal())
        Horizontal::create( fillNet
                          , getLayer()
                          , getAxis()
                          , getLayerGauge()->getWireWidth()
                          , umin +  2*getLayerGauge()->getPitch()
                          , umax -  2*getLayerGauge()->getPitch()
                          );
      else
        Vertical::create( fillNet
                        , getLayer()
                        , getAxis()
                        , getLayerGauge()->getWireWidth()
                        , umin +  2*getLayerGauge()->getPitch()
                        , umax -  2*getLayerGauge()->getPitch()
                        );
    }
  }


  uint32_t  Track::checkMinArea () const
  {
    if (_segments.empty()) return 0;

    uint32_t  nonMinArea    = 0;
    DbU::Unit techMinLength = AutoSegment::getMinimalLength( Session::getLayerDepth( getLayer() ));
    for ( size_t j=0 ; j<_segments.size() ; ++j ) {
      if (not _segments[j]->base() or not (_segments[j]->getDirection() & getDirection())) {
        ++j;
        continue;
      }
      if ((j   > 0)                and (_segments[j-1]->getNet() == _segments[j]->getNet())) continue;
      if ((j+1 < _segments.size()) and (_segments[j+1]->getNet() == _segments[j]->getNet())) continue;
      if (not _segments[j]->base()->isNearMinArea()) continue;
      if (_segments[j]->base()->getSpanLength() < techMinLength) {
        cerr << Error( "Below minimal length/area for %s:\n  length:%s, minimal length:%s"
                     , getString(_segments[j]).c_str()
                     , DbU::getValueString(_segments[j]->base()->getSpanLength()).c_str()
                     , DbU::getValueString(techMinLength).c_str() ) << endl;
        ++nonMinArea;
      }
    }
    return nonMinArea;
  }


  void  Track::expandMinArea ()
  {
    if (_segments.empty()) return;

    double minArea = getLayer()->getMinimalArea();
    if (minArea == 0.0) return;
    
    DbU::Unit     prevSpanMin = getMin();
    TrackElement* minSegment  = NULL;
    TrackElement* maxSegment  = NULL;
    Interval      span;
    bool          hasTurn     = false;
    for ( size_t j=0 ; j<_segments.size() ; ) {
      if (not _segments[j]->base() or not (_segments[j]->getDirection() & getDirection())) {
        ++j;
        continue;
      }

      if (not minSegment) {
        minSegment  = _segments[j];
        maxSegment  = _segments[j];
        if (not span.isEmpty()) prevSpanMin = span.getVMax();
        span.makeEmpty();
        span = _segments[j]->getCanonicalInterval();
        hasTurn =    hasSameLayerTurn( getLayer(), _segments[j], Flags::Source )
                  or hasSameLayerTurn( getLayer(), _segments[j], Flags::Target );
        ++j;
        continue;
      }
      
      if (   (_segments[j]->getNet() == minSegment->getNet())
         and (span.getVMax() >= _segments[j]->getSourceU()) ) {
        if (_segments[j]->getTargetU() > span.getVMax()) {
          maxSegment = _segments[j];
          span.merge( _segments[j]->getTargetU() );
          hasTurn = hasTurn or hasSameLayerTurn( getLayer(), _segments[j], Flags::Target );
        }
        ++j;
        continue;
      }
    
      if (not hasTurn and minSegment->base())
        expandToMinArea( minSegment, maxSegment, span, prevSpanMin, _segments[j]->getSourceU() );

      minSegment = NULL;
    }

    if (not hasTurn and minSegment and minSegment->base())
      expandToMinArea( minSegment, maxSegment, span, prevSpanMin, getMax() );
  }


  string  Track::_getString () const
  {
    return "<" + _getTypeName() + " "
               + "[" + getString(_index) + "] "
               + getString(getLayer()->getName()) + " @"
               + DbU::getValueString(_axis) + " ["
               + DbU::getValueString(_min) + ":"
               + DbU::getValueString(_max) + "] ["
               + getString(_segments.size()) + "/"
               + getString(_segments.capacity())
               + "]>";
  }


  Record* Track::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add ( getSlot           ( "_routingPlane",  _routingPlane ) );
    record->add ( getSlot           ( "_index"       , &_index        ) );
    record->add ( DbU::getValueSlot ( "_axis"        , &_axis         ) );
    record->add ( getSlot           ( "_segments"    , &_segments     ) );
                                     
    return record;
  }


} // Katana namespace.
