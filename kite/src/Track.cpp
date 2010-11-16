
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
// |  C++ Module  :       "./Track.cpp"                              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include  <cstdlib>
#include  <sstream>
#include  <memory>
#include  <algorithm>

#include  "hurricane/Warning.h"
#include  "hurricane/Bug.h"
#include  "hurricane/Layer.h"
#include  "hurricane/Net.h"
#include  "kite/RoutingPlane.h"
#include  "kite/Track.h"
#include  "kite/TrackMarker.h"
#include  "kite/DataNegociate.h"


namespace {


  using namespace std;
  using namespace CRL;
  using namespace Kite;


  struct isDetachedSegment {
      bool operator() ( const TrackElement* s ) { return !s->getTrack(); };
  };


  DbU::Unit  getPositionByIterator ( const vector<TrackElement*>& v, size_t i )
  { return (*(v.begin()+i))->getSourceU(); }


} // End of local namespace.


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


  const size_t  Track::NPOS = (size_t)-1;


  Track::Track ( RoutingPlane* routingPlane, unsigned int index )
    : _routingPlane (routingPlane)
    , _index        (index)
    , _axis         (routingPlane->getTrackPosition(index))
    , _min          (routingPlane->getTrackMin())
    , _max          (routingPlane->getTrackMax())
    , _segments     ()
    , _markers      ()
    , _segmentsValid(false)
    , _markersValid (false)
  { }


  void  Track::_postCreate ()
  { }


  Track::~Track ()
  {
    ltrace(90) << "Track::~Track() - " << (void*)this << endl;
  }


  void  Track::_preDestroy ()
  {
    ltrace(90) << "Track::_preDestroy() - " << (void*)this << " " << this << endl;
    ltracein(90);

    for ( size_t i=0 ; i<_segments.size() ; i++ )
      if ( _segments[i] ) _segments[i]->detach();

    for ( size_t i=0 ; i<_markers.size() ; i++ )
      if ( _markers[i] ) _markers[i]->destroy();

    ltraceout(90);
  }


  void  Track::destroy ()
  {
    ltrace(90) << "Track::destroy() - " << (void*)this << " " << this << endl;

    Track::_preDestroy ();
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


  TrackElement* Track::getSegment ( size_t index ) const
  {
    if ( (index == NPOS) || (index >= getSize()) ) return NULL;
    return _segments[index];
  }


  TrackElement* Track::getSegment ( DbU::Unit position ) const
  {
    unsigned int  state;
    size_t        begin;
    size_t        end;

    getIBounds ( position, begin, end, state );

    if ( state & (MinTrackMin|MaxTrackMax) ) return NULL;

    return getSegment(begin);
  }


  TrackElement* Track::getNext ( size_t& index, Net* net, bool useOrder ) const
  {
    for ( index++ ; index < _segments.size() ; index++ ) {
      if ( _segments[index]->getNet() == net ) continue;
      if ( useOrder
         and  _segments[index]->getDataNegociate()
         and (_segments[index]->getDataNegociate()->getGCellOrder() >= Session::getOrder()) )
        continue;
      return _segments[index];
    }
    index = NPOS;

    return NULL;
  }


  TrackElement* Track::getPrevious ( size_t& index, Net* net, bool useOrder ) const
  {
    for ( index-- ; index != NPOS ; index-- ) {
      if ( inltrace(148) ) {
        cerr << tab << index << ":"; cerr.flush();
        cerr << _segments[index] << endl;
      }
      if ( _segments[index]->getNet() == net ) continue;
      if ( useOrder
         and  _segments[index]->getDataNegociate()
         and (_segments[index]->getDataNegociate()->getGCellOrder() >= Session::getOrder()) )
        continue;
      return _segments[index];
    }
    index = NPOS;

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


  void  Track::getIBounds ( DbU::Unit position, size_t& begin, size_t& end, unsigned int& state ) const
  {
    if ( _segments.empty() ) {
      state = EmptyTrack;
      begin = end = 0;
      return;
    }

    if ( position < _min ) {
      cerr << Warning ( " Position %s inferior to the lower bound of %s. Returning NPOS."
                      , DbU::getValueString(position).c_str()
                      , getString(this).c_str() ) << endl;
      state = BeforeFirst;
      begin = end = 0;
      return;
    }

    if ( position > _max ) {
      cerr << Warning ( " Position %s superior to the upper bound of %s. Returning NPOS."
                      , DbU::getValueString(position).c_str()
                      , getString(this).c_str() ) << endl;
      state = AfterLast;
      begin = end =  _segments.size() - 1;
      return;
    }

    vector<TrackElement*>::const_iterator  lowerBound
      = lower_bound ( _segments.begin(), _segments.end(), position, SourceCompare() );
    begin = end = lowerBound - _segments.begin();

    if ( begin < _segments.size() )
      for ( ; (begin > 0) && (_segments[begin-1]->getNet() == _segments[begin]->getNet()) ; --begin );

    state = 0;
    if ( (begin == 0) && (position < _segments[0]->getSourceU()) ) {
      state = BeforeFirst;
    } else {
      if ( begin ) end = begin -= 1;

      size_t   usedBegin    = begin;
      size_t   usedEnd      = begin;
      Interval usedInterval = expandUsedInterval ( usedBegin, usedEnd );

      if ( position < usedInterval.getVMax() )
        state = Inside;
      else
        if ( begin+1 == _segments.size() )
          state = AfterLast;
        else
          state = Outside;
    }
  }


  void  Track::getOverlapBounds ( Interval interval, size_t& begin, size_t& end ) const
  {
    unsigned int  iState;
    size_t        iBegin;
    size_t        iEnd;

    if (     _segments.empty()
         || (interval.getVMax() <= _min)
         || (interval.getVMin() >= _max)) {
      begin = end = NPOS;
      return;
    }

    getIBounds ( interval.getVMin(), begin, iEnd, iState );
    expandUsedInterval ( begin, iEnd );

    getIBounds ( interval.getVMax(), iBegin, end, iState );
    while ( end < _segments.size() ) {
      if ( _segments[end++]->getSourceU() >= interval.getVMax() ) break;
    }
  }


  TrackCost  Track::getOverlapCost ( Interval interval, Net* net, size_t begin, size_t end ) const
  {
    TrackCost  cost ( const_cast<Track*>(this), interval, begin, end );

    ltrace(190) << "getOverlapCost() @" << DbU::getValueString(_axis)
                << " [" << DbU::getValueString(interval.getVMin())
                << ":"  << DbU::getValueString(interval.getVMax()) << "]"
                << "<-> [" << begin << ":"  << end << "]"
                << endl;

    ltracein(148);

    vector<TrackMarker*>::const_iterator lowerBound
      = lower_bound ( _markers.begin(), _markers.end(), interval.getVMin(), TrackMarker::Compare() );
    size_t mbegin = lowerBound - _markers.begin();

    for ( ;    (mbegin < _markers.size())
            && (_markers[mbegin]->getSourceU() <= interval.getVMax()) ; mbegin++ ) {
      ltrace(148) << "| @" << DbU::getValueString(_axis) << _markers[mbegin] << endl;
      if ( _markers[mbegin]->getNet() != net ) {
        ltrace(148) << "* Mark: @" << DbU::getValueString(_axis) << " " << _markers[mbegin] << endl;
        cost.incTerminals ( _markers[mbegin]->getWeight(this) );
      }
    }

    if ( begin == NPOS ) {
      ltrace(148) << "  begin == NPOS (after last TrackElement)." << endl;
      ltraceout(148);
      return cost;
    }

    for ( ; begin < end ; begin++ ) {
      if ( _segments[begin]->getNet() == net ) {
        cost.incDeltaShared ( _segments[begin]->getCanonicalInterval().intersection(interval).getSize() );
      }
      ltrace(190) << "| overlap: " << _segments[begin] << endl;
      _segments[begin]->incOverlapCost ( net, cost );
      if ( cost.isInfinite() ) break;
    }

    ltraceout(148);

    return cost;
  }


  TrackCost  Track::getOverlapCost ( Interval interval, Net* net ) const
  {
    size_t begin;
    size_t end;

    getOverlapBounds ( interval, begin, end );

    return getOverlapCost ( interval, net, begin, end );
  }


  TrackCost  Track::getOverlapCost ( TrackElement* segment ) const
  {
    return getOverlapCost ( segment->getCanonicalInterval(), segment->getNet() );
  }


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


  Track* Track::getNext () const
  {
    return getRoutingPlane()->getTrackByIndex ( getIndex()+1 );
  }


  Track* Track::getPrevious () const
  {
    if ( !getIndex() ) return NULL;

    return getRoutingPlane()->getTrackByIndex ( getIndex()-1 );
  }


  size_t  Track::find ( const TrackElement* segment ) const
  {
    if ( !_segments.size() ) return NPOS;

    vector<TrackElement*>::const_iterator  lowerBound
      = lower_bound ( _segments.begin()
                    , _segments.end()
                    , segment
                    , SegmentCompare()
                    );

    if ( lowerBound != _segments.end() ) {
      while ( segment->getSourceU() == (*lowerBound)->getSourceU() ) {
        if ( *lowerBound == segment ) return (size_t)(lowerBound-_segments.begin());
        lowerBound++;
      }
    }

    return NPOS;
  }


  Interval  Track::getFreeInterval ( DbU::Unit position, Net* net ) const
  {
    unsigned int  state;
    size_t        begin;
    size_t        end;

    if ( !_segments.size() ) return Interval(_min,_max);

    getIBounds ( position, begin, end, state );
    if ( (state == Inside) && (_segments[begin]->getNet() != net) )
      return Interval();

    return expandFreeInterval ( begin, end, state, net );
  }


  Interval  Track::expandFreeInterval ( size_t& begin, size_t& end, unsigned int state, Net* net, bool useOrder ) const
  {
    DbU::Unit minFree = _min;

    if ( !(state & MinTrackMin) ) {
      if ( _segments[begin]->getNet() == net )
        getPrevious ( begin, net, useOrder );

      if ( begin != NPOS ) {
        size_t usedEnd;
        minFree = expandUsedInterval ( begin, usedEnd, useOrder ).getVMax();
      }
    }

    if ( !(state & MaxTrackMax) ) {
      if ( _segments[end]->getNet() == net ) {
        getNext ( end, net, useOrder );

        if ( end == NPOS ) {
          end  = _segments.size() - 1;
          setMaximalFlags ( state, MaxTrackMax );
        } else {
          setMaximalFlags ( state, MaxSegmentMin );
        }
      }
    }

    return Interval ( minFree, getMaximalPosition(end,state) );
  }


  void  Track::forceSort ()
  {
    _segmentsValid = false;
  }


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


  bool  Track::_check ( unsigned int& overlaps, const char* message ) const
  {
    bool coherency = true;
    bool holes     = false;

    if ( message ) cerr << "     o  Checking Track - " << message << endl;
    ltrace(90) << "[CHECK] " << (void*)this << ":" << this << endl;

    for ( size_t i=0 ; i<_segments.size() ; i++ ) {
      if ( _segments[i] ) {
        if ( i ) {
          if ( _segments[i-1] == _segments[i] ) {
            cerr << "[CHECK] incoherency at " << i << " "
                 << _segments[i] << " is duplicated. " << endl;
            coherency = false;
          }
        }
        if ( !_segments[i]->getTrack() ) {
          cerr << "[CHECK] incoherency at " << i << " "
               << _segments[i] << " is detached." << endl;
          coherency = false;
        } else { 
          if ( _segments[i]->getTrack() != this ) {
            cerr << "[CHECK] incoherency at " << i << " "
                 << _segments[i] << " is in track "
                 << _segments[i]->getTrack() << endl;
            coherency = false;
          }
          if ( _segments[i]->getIndex() != i ) {
            cerr << "[CHECK] incoherency at " << i << " "
                 << _segments[i] << " has bad index "
                 << _segments[i]->getIndex() << endl;
            coherency = false;
          }
        }
        if ( _segments[i]->getAxis() != getAxis() ) {
          cerr << "[CHECK] incoherency at " << i << " "
               << _segments[i] << " is not on Track axis "
               << DbU::getValueString(getAxis()) << "." << endl;
          coherency = false;
        } 
        
        coherency = coherency && _segments[i]->_check ();
      } else {
        cerr << "[CHECK] Hole at position " << i << "." << endl;
        holes     = true;
        coherency = false;
      }
    }

    if ( !holes )
      coherency = coherency && (checkOverlap(overlaps) == 0);

    return coherency;
  }


  DbU::Unit  Track::getSourcePosition ( size_t  i ) const
  {
    if ( i == NPOS) return 0;

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

    switch ( state & MinMask ) {
      case MinTrackMin:   return _min;
      case MinSegmentMin: return _segments[index]->getSourceU ();
      case MinSegmentMax: return _segments[index]->getTargetU ();
    }

    cerr << Bug ( " Track::getMinimalPosition(size_t,unsigned int) :"
                  " invalid state value %ud.", state ) << endl;

    return _min;
  }


  DbU::Unit  Track::getMaximalPosition ( size_t index, unsigned int state ) const
  {
    Interval  canonical;

    switch ( state & MaxMask ) {
      case MaxTrackMax:       return _max;
      case MaxSegmentMin:     return _segments[index  ]->getSourceU ();
      case MaxNextSegmentMin: return _segments[index+1]->getSourceU ();
      case MaxSegmentMax:     return _segments[index  ]->getTargetU ();
    }

    cerr << Bug ( " Track::getMaximalPosition(size_t,unsigned int) :"
                  " invalid state value %ud.", state ) << endl;

    return _min;
  }


  Interval  Track::expandUsedInterval ( size_t& begin, size_t& end, bool useOrder ) const
  {
    if ( begin == NPOS ) return Interval();

    size_t  seed  = begin;
    Net*    owner = _segments[seed]->getNet();

    Interval  expandInterval;
    Interval  ownerInterval;
    _segments[seed]->getCanonical ( ownerInterval );

    size_t i = seed;
    while ( --i != NPOS ) {
      if ( _segments[i]->getNet() != owner ) break;
      if ( useOrder
         and  _segments[i]->getDataNegociate()
         and (_segments[i]->getDataNegociate()->getGCellOrder() >= Session::getOrder()) )
        continue;

      _segments[i]->getCanonical ( expandInterval );
      if ( expandInterval.getVMax() >= ownerInterval.getVMin() ) {
        ownerInterval.merge ( expandInterval );
        begin = i;
      }
    }

    end = i = seed;
    while ( ++i < _segments.size() ) {
      if ( _segments[i]->getNet() != owner ) break;
      if ( useOrder
         and  _segments[i]->getDataNegociate()
         and (_segments[i]->getDataNegociate()->getGCellOrder() >= Session::getOrder()) )
        continue;

      _segments[i]->getCanonical ( expandInterval );
      if ( expandInterval.getVMin() > ownerInterval.getVMax() ) break;
      if ( expandInterval.getVMax() > ownerInterval.getVMax() ) end = i;

      ownerInterval.merge ( expandInterval );
    }

    return ownerInterval;
  }


  size_t  Track::pack ()
  {
    ltrace(148) << "Track::pack() - " << this << endl;
    ltracein(148);

    size_t  size = _segments.size();

    vector<TrackElement*>::iterator  beginRemove
      = remove_if ( _segments.begin(), _segments.end(), isDetachedSegment() );

    _segments.erase ( beginRemove, _segments.end() );

# if 0
    size_t  first    = 0;
    size_t  last     = 0;
    bool    erase    = false;

    while ( last < _segments.size() ) {
      if ( _segments[last] ) {
        if ( erase ) {
          _segments.erase ( _segments.begin()+first, _segments.begin()+last );

          erase = false;
          last  = first;
        } else {
          first = last;
        }
      } else {
        erase = true;
      }
    }

    if ( erase )
      _segments.erase ( _segments.begin()+first, _segments.end() );
# endif

    ltrace(148) << "After packing " << this << endl;
    ltraceout(148);

    return size - _segments.size();
  }


  void  Track::sort ()
  {
    if ( !_segmentsValid ) {
      std::sort ( _segments.begin(), _segments.end(), SegmentCompare() );
      for ( size_t i=0 ; i < _segments.size() ; i++ ) {
        _segments[i]->setIndex ( i );
      }
      _segmentsValid = true;
    }

    if ( !_markersValid ) {
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
            cerr << Warning(" Invalid sorting length order in %s:\n%s  \n%s  "
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
        cerr << Warning("Overlap in %s between:\n  %s\n  %s"
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
    record->add ( getSlot ( "_routingPlane",  _routingPlane ) );
    record->add ( getSlot ( "_index"       , &_index        ) );
    record->add ( DbU::getValueSlot ( "_axis"        , &_axis         ) );
    record->add ( getSlot ( "_segments"    , &_segments     ) );
                                     
    return record;
  }


} // End of Kite namespace.
