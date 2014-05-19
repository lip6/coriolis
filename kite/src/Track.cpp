// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2014, All Rights Reserved
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
#include "hurricane/Warning.h"
#include "hurricane/Bug.h"
#include "hurricane/Layer.h"
#include "hurricane/Net.h"
#include "kite/RoutingPlane.h"
#include "kite/Track.h"
#include "kite/TrackMarker.h"
#include "kite/DataNegociate.h"


namespace {

  using namespace std;
  using namespace CRL;
  using namespace Kite;


  struct isDetachedSegment {
      bool operator() ( const TrackElement* s ) { return not s->getTrack(); };
  };


  DbU::Unit  getPositionByIterator ( const vector<TrackElement*>& v, size_t i )
  { return (*(v.begin()+i))->getSourceU(); }


} // Anonymous namespace.


namespace Kite {

  using std::lower_bound;
  using std::remove_if;
  using std::sort;
  using Hurricane::dbo_ptr;
  using Hurricane::tab;
  using Hurricane::inltrace;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::Warning;
  using Hurricane::Bug;
  using Hurricane::Layer;
  using Hurricane::Net;


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
  { ltrace(90) << "Track::~Track() - " << (void*)this << endl; }


  void  Track::_preDestroy ()
  {
    ltrace(90) << "Track::_preDestroy() - " << (void*)this << " " << this << endl;
    ltracein(90);

    for ( size_t i=0 ; i<_segments.size() ; i++ )
      if (_segments[i]) _segments[i]->detach();

    for ( size_t i=0 ; i<_markers.size() ; i++ )
      if (_markers[i]) _markers[i]->destroy();

    ltraceout(90);
  }


  void  Track::destroy ()
  {
    ltrace(90) << "Track::destroy() - " << (void*)this << " " << this << endl;

    Track::_preDestroy();
    delete this;
  }


  KiteEngine* Track::getKiteEngine () const
  { return _routingPlane->getKiteEngine(); }


  unsigned int  Track::getDepth () const
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
    unsigned int  state;
    size_t        begin;

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
    for ( index-- ; index != npos ; index-- ) {
      if (inltrace(148)) {
        cerr << tab << index << ":"; cerr.flush();
        cerr << _segments[index] << endl;
      }
      if (_segments[index]->getNet() == net) continue;
      return _segments[index];
    }
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


  void  Track::getBeginIndex ( DbU::Unit position, size_t& begin, unsigned int& state ) const
  {
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
    if (begin < _segments.size())
      for ( ; (begin > 0) and (_segments[begin-1]->getNet() == _segments[begin]->getNet()) ; --begin );

    state = 0;
    if ( (begin == 0) and (position < _segments[0]->getSourceU()) ) {
      state = BeforeFirstElement;
    } else {
      if (begin) begin -= 1;

      size_t   usedBegin    = begin;
      Interval usedInterval = getOccupiedInterval( usedBegin );

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
    unsigned int  iState;

    if (  _segments.empty()
       or (interval.getVMax() <= _min)
       or (interval.getVMin() >= _max)) {
      begin = end = npos;
      return;
    }

    getBeginIndex      ( interval.getVMin(), begin, iState );
    getOccupiedInterval( begin );

    getBeginIndex( interval.getVMax(), end, iState );
    while ( end < _segments.size() ) {
      if (_segments[end++]->getSourceU() >= interval.getVMax()) break;
    }
  }


  TrackCost  Track::getOverlapCost ( Interval     interval
                                   , Net*         net
                                   , size_t       begin
                                   , size_t       end
                                   , unsigned int flags ) const
  {
    TrackCost  cost ( const_cast<Track*>(this), interval, begin, end, net, flags );

    ltrace(190) << "getOverlapCost() @" << DbU::getValueString(_axis)
                << " [" << DbU::getValueString(interval.getVMin())
                << ":"  << DbU::getValueString(interval.getVMax())
                << "] <-> [" << begin << ":"  << end << "]"
                << endl;

    ltracein(148);

    vector<TrackMarker*>::const_iterator lowerBound
      = lower_bound( _markers.begin(), _markers.end(), interval.getVMin(), TrackMarker::Compare() );
    size_t mbegin = lowerBound - _markers.begin();

    for ( ;     (mbegin < _markers.size())
            and (_markers[mbegin]->getSourceU() <= interval.getVMax()) ; mbegin++ ) {
      ltrace(148) << "| @" << DbU::getValueString(_axis) << _markers[mbegin] << endl;
      if ( _markers[mbegin]->getNet() != net ) {
        ltrace(148) << "* Mark: @" << DbU::getValueString(_axis) << " " << _markers[mbegin] << endl;
        cost.incTerminals( _markers[mbegin]->getWeight(this) );
      }
    }

    if (begin == npos) {
      ltrace(148) << "  begin == npos (after last TrackElement)." << endl;
      ltraceout(148);
      return cost;
    }

    for ( ; begin < end ; begin++ ) {
      Interval overlap = interval.getIntersection( _segments[begin]->getCanonicalInterval() );
      if ( _segments[begin]->getNet() == net ) {
        cost.incDeltaShared ( overlap.getSize() );
      }
      ltrace(190) << "| overlap: " << _segments[begin] << endl;
      _segments[begin]->incOverlapCost( net, cost );
      if (cost.isInfinite()) break;
    }

    ltraceout(148);

    return cost;
  }


  TrackCost  Track::getOverlapCost ( Interval interval, Net* net, unsigned int flags ) const
  {
    size_t begin;
    size_t end;

    getOverlapBounds( interval, begin, end );

    return getOverlapCost( interval, net, begin, end, flags );
  }


  TrackCost  Track::getOverlapCost ( TrackElement* segment, unsigned int flags ) const
  { return getOverlapCost ( segment->getCanonicalInterval(), segment->getNet(), flags ); }


  void  Track::getTerminalWeight ( Interval interval, Net* net, size_t& count, unsigned int& weight ) const
  {
    ltrace(148) << "getTerminalWeight() @" << DbU::getValueString(_axis)
                << " [" << interval.getVMin() << " " << interval.getVMax() << "]" << endl;
    ltracein(148);

  //count  = 0;
  //weight = 0;

    vector<TrackMarker*>::const_iterator lowerBound
      = lower_bound ( _markers.begin(), _markers.end(), interval.getVMin(), TrackMarker::Compare() );
    size_t mbegin = lowerBound - _markers.begin();

    for ( ;    (mbegin < _markers.size())
            && (_markers[mbegin]->getSourceU() <= interval.getVMax()) ; mbegin++ ) {
      ltrace(148) << "| @" << DbU::getValueString(_axis) << _markers[mbegin] << endl;
      if ( _markers[mbegin]->getNet() == net ) {
        ltrace(200) << "* Mark: @" << DbU::getValueString(_axis) << " " << _markers[mbegin] << endl;
        weight += _markers[mbegin]->getWeight(this);
        ++count;
      }
    }
    ltraceout(148);
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
    unsigned int  state;
    size_t        begin;
    size_t        end;

    if (_segments.empty()) return Interval(_min,_max);

    getBeginIndex( position, begin, state );
    if ( (state == InsideElement) and (_segments[begin]->getNet() != net) )
      return Interval();

    end = begin;
    return expandFreeInterval( begin, end, state, net );
  }


  Interval  Track::expandFreeInterval ( size_t& begin, size_t& end, unsigned int state, Net* net ) const
  {
    DbU::Unit minFree = _min;

    if (not (state & BeginIsTrackMin) ) {
      if (_segments[begin]->getNet() == net)
        getPrevious( begin, net );

      if (begin != npos) {
        minFree = getOccupiedInterval(begin).getVMax();
      }
    }

    if (not (state & EndIsTrackMax) ) {
      if (_segments[end]->getNet() == net) {
        getNext( end, net );

        if (end == npos) {
          end  = _segments.size() - 1;
          setMaximalFlags( state, EndIsTrackMax );
        } else {
          setMaximalFlags( state, EndIsSegmentMin );
        }
      }
    }

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
    ltrace(200) << "Track::insert() " << getLayer()->getName()
                << " @" << DbU::getValueString(getAxis()) << " " << segment << endl;

    if (   (getLayer()->getMask()         != segment->getLayer()->getMask())
       and (getBlockageLayer()->getMask() != segment->getLayer()->getMask()) ) {
      cerr << Bug("Track::insert(), segment %s has not the right layer."
                 ,getString(segment).c_str()) << endl;
    }

    segment->setAxis ( getAxis() );
    _segments.push_back ( segment );
    _segmentsValid = false;

    segment->setTrack ( this );
  }


  void  Track::setSegment ( TrackElement* segment, size_t index )
  {
    if ( index >= _segments.size() ) return;
    _segments[index] = segment;
  }


  bool  Track::check ( unsigned int& overlaps, const char* message ) const
  {
    bool coherency = true;
    bool holes     = false;

    if (message) cerr << "     o  Checking Track - " << message << endl;
    ltrace(90) << (void*)this << ":" << this << endl;

    for ( size_t i=0 ; i<_segments.size() ; i++ ) {
      if (_segments[i]) {
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
          if (_segments[i]->getTrack() != this) {
            cerr << "[CHECK] incoherency at " << i << " "
                 << _segments[i] << " is in track "
                 << _segments[i]->getTrack() << endl;
            coherency = false;
          }
          if (_segments[i]->getIndex() != i) {
            cerr << "[CHECK] incoherency at " << i << " "
                 << _segments[i] << " has bad index "
                 << _segments[i]->getIndex() << endl;
            coherency = false;
          }
        }
        if (_segments[i]->getAxis() != getAxis()) {
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


  DbU::Unit  Track::getMinimalPosition ( size_t index, unsigned int state ) const
  {
    Interval  canonical;

    switch ( state & BeginMask ) {
      case BeginIsTrackMin:   return _min;
      case BeginIsSegmentMin: return _segments[index]->getSourceU ();
      case BeginIsSegmentMax: return _segments[index]->getTargetU ();
    }

    cerr << Bug( " Track::getMinimalPosition(size_t,unsigned int) :"
                 " invalid state value %ud.", state ) << endl;

    return _min;
  }


  DbU::Unit  Track::getMaximalPosition ( size_t index, unsigned int state ) const
  {
    Interval  canonical;

    switch ( state & EndMask ) {
      case EndIsTrackMax:       return _max;
      case EndIsSegmentMin:     return _segments[index  ]->getSourceU ();
      case EndIsNextSegmentMin: if (index+1 >= getSize()) return _max;
                                     return _segments[index+1]->getSourceU ();
      case EndIsSegmentMax:     return _segments[index  ]->getTargetU ();
    }

    cerr << Bug( " Track::getMaximalPosition(size_t,unsigned int) :"
                 " invalid state value %ud.", state ) << endl;

    return _min;
  }


  Interval  Track::getOccupiedInterval ( size_t& begin ) const
  {
    if (begin == npos) return Interval();

    size_t  seed  = begin;
    Net*    owner = _segments[seed]->getNet();

    Interval  segmentInterval;
    Interval  mergedInterval;
    _segments[seed]->getCanonical( mergedInterval );

    size_t i = seed;
    while ( --i != npos ) {
      if (_segments[i]->getNet() != owner) break;

      _segments[i]->getCanonical ( segmentInterval );
      if (segmentInterval.getVMax() >= mergedInterval.getVMin()) {
        mergedInterval.merge( segmentInterval );
        begin = i;
      }
    }

    i = seed;
    while ( ++i < _segments.size() ) {
      if (_segments[i]->getNet() != owner) break;

      _segments[i]->getCanonical( segmentInterval );
      if (segmentInterval.getVMin() > mergedInterval.getVMax()) break;

      mergedInterval.merge( segmentInterval );
    }

    return mergedInterval;
  }


  size_t  Track::doRemoval ()
  {
    ltrace(148) << "Track::doRemoval() - " << this << endl;
    ltracein(148);

    size_t  size = _segments.size();

    vector<TrackElement*>::iterator  beginRemove
      = remove_if( _segments.begin(), _segments.end(), isDetachedSegment() );

    _segments.erase( beginRemove, _segments.end() );

    ltrace(148) << "After doRemoval " << this << endl;
    ltraceout(148);

    return size - _segments.size();
  }


  void  Track::doReorder ()
  {
    if (not _segmentsValid ) {
      std::sort ( _segments.begin(), _segments.end(), SegmentCompare() );
      for ( size_t i=0 ; i < _segments.size() ; i++ ) {
        _segments[i]->setIndex ( i );
      }
      _segmentsValid = true;
    }

    if (not _markersValid ) {
      std::sort ( _markers.begin(), _markers.end(), TrackMarker::Compare() );
      _markersValid = true;
    }
  }


  unsigned int  Track::checkOverlap ( unsigned int& overlaps ) const
  {
    if ( !_segments.size() ) return 0;

    size_t j = 0;
    for ( size_t i=0 ; i<_segments.size()-1 ; i++ ) {
      if ( _segments[i]->getNet() == _segments[i+1]->getNet() ) {
        if ( _segments[i]->getSourceU() == _segments[i+1]->getSourceU() ) {
          if ( _segments[i]->getTargetU() < _segments[i+1]->getTargetU() ) {
            cerr << Error(" Invalid sorting length order in %s:\n%s  \n%s  "
                         ,getString(this).c_str()
                         ,getString(_segments[i  ]).c_str()
                         ,getString(_segments[i+1]).c_str()) << endl;
          }
        }
        for ( j=i+1 ; (j<_segments.size()) && (_segments[i]->getNet() == _segments[j]->getNet()) ; j++ );
      } else {
        j = i+1;
      }

      if (   (j<_segments.size())
          && (_segments[i]->getTargetU() > _segments[j]->getSourceU()) ) {
        cerr << Error("Overlap in %s between:\n  %s\n  %s"
                     ,getString(this).c_str()
                     ,getString(_segments[i]).c_str()
                     ,getString(_segments[j]).c_str()) << endl;
        overlaps++;
      }
    }

    return overlaps;
  }


  string  Track::_getString () const
  {
    return "<" + _getTypeName() + " "
               + getString(getLayer()) + " @"
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


} // Kite namespace.
