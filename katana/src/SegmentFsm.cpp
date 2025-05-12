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
// |  C++ Module  :       "./SegmentFsm.cpp"                         |
// +-----------------------------------------------------------------+


#include <algorithm>
#include "hurricane/Bug.h"
#include "hurricane/DebugSession.h"
#include "katana/TrackElement.h"
#include "katana/Tracks.h"
#include "katana/RoutingPlane.h"
#include "katana/DataNegociate.h"
#include "katana/RoutingEvent.h"
#include "katana/RoutingEventQueue.h"
#include "katana/RoutingEventHistory.h"
#include "katana/Manipulator.h"
#include "katana/SegmentFsm.h"
#include "katana/KatanaEngine.h"


namespace {

  using namespace std;
  using namespace Hurricane;
  using namespace Katana;


// -------------------------------------------------------------------
// Class  :  "Cs1Candidate".

  class Cs1Candidate {
    public:
      inline            Cs1Candidate       ( Track* track=NULL, DbU::Unit ppitch=0 );
      inline Track*     getTrack           () const;
      inline size_t     getBegin           () const;
      inline size_t     getEnd             () const;
      inline size_t     getSize            () const;
      inline Interval   getConflict        ( size_t );
      inline Interval   getLongestConflict () const;
      inline DbU::Unit  getBreakPos        () const;
      inline DbU::Unit  getConflictLength  () const;
      inline void       setBegin           ( size_t );
      inline void       setEnd             ( size_t );
      inline void       addConflict        ( const Interval& );
             void       consolidate        ();
    public:
      friend inline bool  operator< ( const Cs1Candidate&, const Cs1Candidate& );
    private:
      Track*             _track;
      DbU::Unit          _ppitch;
      size_t             _begin;
      size_t             _end;
      vector<Interval>   _conflicts;
      Interval           _longestConflict;
      DbU::Unit          _breakPos;
      DbU::Unit          _conflictLength;
  };


  inline  Cs1Candidate::Cs1Candidate ( Track* track, DbU::Unit ppitch )
    : _track          (track)
    , _ppitch         (ppitch)
    , _begin          (0)
    , _end            (0)
    , _conflicts      ()
    , _longestConflict()
    , _breakPos       (0)
    , _conflictLength (0)
  { }

  inline Track*     Cs1Candidate::getTrack           () const { return _track; }
  inline size_t     Cs1Candidate::getBegin           () const { return _begin; }
  inline size_t     Cs1Candidate::getEnd             () const { return _end; }
  inline size_t     Cs1Candidate::getSize            () const { return _conflicts.size(); }
  inline Interval   Cs1Candidate::getLongestConflict () const { return _longestConflict; }
  inline DbU::Unit  Cs1Candidate::getBreakPos        () const { return _breakPos; }
  inline void       Cs1Candidate::setBegin           ( size_t i ) { _begin=i; }
  inline void       Cs1Candidate::setEnd             ( size_t i ) { _end=i; }

  inline void  Cs1Candidate::addConflict  ( const Interval& conflict )
  {
    _conflicts.push_back(conflict);
    _conflictLength += conflict.getSize();

    if (conflict.getSize() > _longestConflict.getSize())
      _longestConflict = conflict;
  }

  inline Interval  Cs1Candidate::getConflict ( size_t i )
  {
    if (i >= _conflicts.size()) return Interval();
    return _conflicts[i];
  }

  inline bool  operator< ( const Cs1Candidate& lhs, const Cs1Candidate& rhs )
  {
    DbU::Unit delta = lhs._longestConflict.getSize() - rhs._longestConflict.getSize();
    if (delta < 0) return true;
    if (delta > 0) return false;

    return lhs._conflictLength < rhs._conflictLength;
  }


  void  Cs1Candidate::consolidate ()
  {
    if (_conflicts.size() > 0) {
      DbU::Unit  halfConflict = 0;
      size_t i = 0;
      for ( ; i<_conflicts.size()-1 ; ++i ) {
        halfConflict += _conflicts[i].getSize();
        if (halfConflict > _conflictLength/2)
          break;
      }

    // Ugly: hard-coded pitch.
      _breakPos = _conflicts[i].getVMin() - _ppitch;
    }
  }


// -------------------------------------------------------------------
// Class  :  "UnionItervals".

  class UnionIntervals {
    public:
      inline                                 UnionIntervals ();
             void                            addInterval    ( Interval& );
      inline size_t                          size           () const;
      inline bool                            empty          () const;
      inline list<Interval>::const_iterator  begin          () const;
      inline list<Interval>::const_iterator  end            () const;
      inline DbU::Unit                       getVMin        () const;
      inline DbU::Unit                       getVMax        () const;
             string                          _getString     ();
    private:
      list<Interval>  _intervals;
  };


  inline                                 UnionIntervals::UnionIntervals () : _intervals() { }
  inline list<Interval>::const_iterator  UnionIntervals::begin          () const { return _intervals.begin(); }
  inline list<Interval>::const_iterator  UnionIntervals::end            () const { return _intervals.end(); }
  inline size_t                          UnionIntervals::size           () const { return _intervals.size(); }
  inline bool                            UnionIntervals::empty          () const { return _intervals.empty(); }
  inline DbU::Unit                       UnionIntervals::getVMin        () const { return (empty()) ? DbU::Max : (*begin()).getVMin(); }
  inline DbU::Unit                       UnionIntervals::getVMax        () const { return (empty()) ? DbU::Min : (*begin()).getVMax(); }


  void  UnionIntervals::addInterval ( Interval& interval )
  {
    cdebug_log(159,0) << "UnionInterval::addInterval() - " << interval << endl;

    list<Interval>::iterator iintv = _intervals.begin ();

    bool merged = false;
    while ( iintv != _intervals.end() ) {
      if (not merged) {
        if (interval.getVMax() < (*iintv).getVMin()) { _intervals.insert( iintv, interval ); return; }
        if (interval.getVMin() > (*iintv).getVMax()) { ++iintv; continue; }

        merged   = true;
        interval = (*iintv).merge( interval );
        ++iintv;
      } else {
        if ((*iintv).intersect( interval )) {
          interval = (*iintv).merge( interval );
          iintv    = _intervals.erase( iintv );
          continue;
        } else
          break;
      }
    }

    if (not merged) _intervals.push_back( interval );
  }


  string  UnionIntervals::_getString ()
  {
    ostringstream s;

    list<Interval>::iterator iintv = _intervals.begin();
    for ( ; iintv != _intervals.end() ; ++iintv ) {
      s << " [" << DbU::getValueString((*iintv).getVMin())
        <<  ":" << DbU::getValueString((*iintv).getVMax()) << "]";
    }
    return s.str();
  }


// -------------------------------------------------------------------
// Class  :  "RipupHistory".

  class RipupHistory {
    public:
                             RipupHistory       ( RoutingEvent* );
      inline bool            isDislodger        ( RoutingEvent* ) const;
      inline size_t          size               () const;
      inline size_t          getDislodgersCount () const;
             void            addAxis            ( DbU::Unit );
             void            addAxis            ( RoutingEvent* );
             bool            hasAxis            ( DbU::Unit ) const;
             UnionIntervals* getUnionIntervals  ( DbU::Unit );
             void            addDislodger       ( RoutingEvent* );
             void            addDislodger       ( TrackElement* );
             void            print              ( ostream& );
    private:
      RoutingEvent*                 _masterEvent;
      map<DbU::Unit,UnionIntervals> _dislodgers;
      size_t                        _dislodgersCount;
  };


  RipupHistory::RipupHistory ( RoutingEvent* event )
    : _masterEvent    (event)
    , _dislodgers     ()
    , _dislodgersCount(0)
  {
    const Interval& perpandicular = _masterEvent->getPerpandicularFree();
    RoutingPlane*   plane         = Session::getKatanaEngine()->getRoutingPlaneByLayer(_masterEvent->getSegment()->getLayer());
    Track*          track;

    if (not perpandicular.isEmpty()) {
      track = plane->getTrackByPosition(perpandicular.getVMin());

      if (track and (track->getAxis() < perpandicular.getVMin())) track = track->getNextTrack();
      for ( ; track && (track->getAxis() <= perpandicular.getVMax())
            ; track = track->getNextTrack() )
        addAxis( track->getAxis() );
    }

    track = plane->getTrackByPosition(_masterEvent->getSegment()->getAxis());
    if (track) {
      size_t   begin    = Track::npos;
      size_t   end      = Track::npos;
      Interval interval = _masterEvent->getSegment()->getCanonicalInterval();
      track->getOverlapBounds( interval, begin, end );

      if (begin != Track::npos) {
        for ( ; begin < end ; ++begin ) {
          TrackElement* other = track->getSegment(begin);
          if (other->getNet() == _masterEvent->getSegment()->getNet() ) continue;
          if (not interval.intersect(other->getCanonicalInterval())) continue;

          addDislodger( other );
        }
      }
    }
  }


  inline bool    RipupHistory::isDislodger        ( RoutingEvent* event ) const { return hasAxis(event->getSegment()->getAxis()); }
  inline size_t  RipupHistory::size               () const { return _dislodgers.size(); }
  inline size_t  RipupHistory::getDislodgersCount () const { return _dislodgersCount; }


  void  RipupHistory::addAxis ( DbU::Unit axis )
  {
    if (hasAxis(axis)) return;
    _dislodgers.insert( make_pair(axis,UnionIntervals()) );
  }


  void  RipupHistory::addAxis ( RoutingEvent* event )
  { addAxis( event->getAxisHistory() ); }


  bool  RipupHistory::hasAxis ( DbU::Unit axis ) const
  { return _dislodgers.find(axis) != _dislodgers.end(); }


  UnionIntervals* RipupHistory::getUnionIntervals ( DbU::Unit axis )
  {
    map<DbU::Unit,UnionIntervals>::iterator iunion = _dislodgers.find ( axis );
    if (iunion == _dislodgers.end()) return NULL;

    return &(iunion->second);
  }


  void  RipupHistory::addDislodger ( RoutingEvent* event )
  {
    if (event->getSegment() == _masterEvent->getSegment()) return;
    if (event->getSegment()->getLayer() != _masterEvent->getSegment()->getLayer()) return;

    UnionIntervals* intervals = getUnionIntervals( event->getAxisHistory() );
    if (not intervals) return;

    Interval canonical = event->getSegment()->getCanonicalInterval();
    intervals->addInterval( canonical );

    ++_dislodgersCount;
  }


  void  RipupHistory::addDislodger ( TrackElement* segment )
  {
    if (_masterEvent->getSegment()->getNet() == segment->getNet()) return;

    UnionIntervals* intervals = getUnionIntervals( segment->getAxis() );
    if (not intervals) return;

    Interval canonical = segment->getCanonicalInterval();
    intervals->addInterval( canonical );

    ++_dislodgersCount;
  }


  void  RipupHistory::print ( ostream& o )
  {
    o << "[HISTORY] " << _masterEvent << endl;

    map<DbU::Unit,UnionIntervals>::iterator iunion = _dislodgers.begin();
    for ( ; iunion != _dislodgers.end() ; ++iunion )
      o << "  @" << DbU::getValueString(iunion->first)
        << " " << (iunion->second)._getString() << endl;
  }


} // Anonymous namespace.


namespace Katana {

  using std::sort;
  using Hurricane::tab;
  using Hurricane::DebugSession;
  using Hurricane::Bug;
  using Hurricane::ForEachIterator;


// -------------------------------------------------------------------
// Class  :  "SegmentAction".

  SegmentAction::SegmentAction ( TrackElement* segment
                               , uint32_t      type
                               , DbU::Unit     axisHint
                               , uint32_t      toState
                               )
    : _segment (segment)
    , _type    (type)
    , _axisHint(axisHint)
    , _toState (toState)
  { }


  bool  SegmentAction::doAction ( RoutingEventQueue& queue )
  {
  // Note:
  //   "_immediate" ripup flags was associated with "perpandicular", as they
  //   must be re-inserted *before* any parallel. Must look to solve the redundancy.

    DebugSession::open( _segment->getNet(), 156, 160 );
    cdebug_log(159,1) << "* doAction() on "<< _segment << endl;

    if (_type & Lock) {
      cdebug_log(159,0) << "* Lock // " << _segment << endl;
    } else if (_type & Perpandicular) {
      cdebug_log(159,0) << "* Riping Pp " << _segment << endl;
    } else if (_type & OtherRipup) {
      cdebug_log(159,0) << "* Riping Other " << _segment << endl;
    } else {
      cdebug_log(159,0) << "* Riping // " << _segment << endl;
    }

    if (_segment->isFixed()) {
      cdebug_tabw(159,-1);
      DebugSession::close();
      return true;
    }

    DataNegociate* data = _segment->getDataNegociate();
    if (data == NULL) {
      cdebug_tabw(159,-1);
      DebugSession::close();
      return true;
    }

    if (_type & ResetRipup   ) data->resetRipupCount();
    if (_type & DecreaseRipup) data->decRipupCount  ();

    if (_type & ToState) {
      data->setState     ( _toState );
      data->setRipupCount( Session::getKatanaEngine()->getRipupLimit(_segment) );
    }

    if (_segment->getTrack()) Session::addRemoveEvent( _segment );

    RoutingEvent* event = data->getRoutingEvent();
    if (event == NULL) {
      cerr << Bug( "Missing Event on %p:%s"
                 , _segment->base()->base(),getString(_segment).c_str() ) << endl;
      DebugSession::close();
      return true;
    }

    if (_type&Lock) Session::addLockEvent( _segment );

    if ( (_type & AxisHint) /*and not _segment->isSlackenDogleg()*/ ) {
      cdebug_log(159,0) << "Setting Axis Hint: @" << DbU::getValueString(_axisHint) << endl;
      event->setAxisHint( _axisHint );
    }

  // It is possible that this code could be disabled.
  // There should be no need to move the axis of the segment to be inserted,
  // it will automatically slot into the empty track, if any.
    if (_type & MoveToAxis) {
      cdebug_log(159,0) << "Moving Axis To: @" << DbU::getValueString(_axisHint) << endl;
      _segment->setAxis( _axisHint );
    }

    if (_type & ToRipupLimit) {
      uint32_t limit = Session::getKatanaEngine()->getRipupLimit(_segment);
      if (limit > data->getRipupCount())
        data->setRipupCount( limit );
    }

    uint32_t eventLevel = 0;
    if (_type & EventLevel1) eventLevel = 1;
    if (_type & EventLevel2) eventLevel = 2;
    if (_type & EventLevel3) eventLevel = 3;
    if (_type & EventLevel4) eventLevel = 4;
    if (_type & EventLevel5) eventLevel = 5;
    event->setRipedByLocal( _type&RipedByLocal );

    if (_type & ToPref)
      event->_rescheduleAsPref();
    event->reschedule( queue, eventLevel );

    cdebug_tabw(159,-1);
    DebugSession::close();
    return true;
  }


// -------------------------------------------------------------------
// Class  :  "SegmentFsm".
  

  SegmentFsm::SegmentFsm ( RoutingEvent*        event1
                         , RoutingEventQueue&   queue
                         , RoutingEventHistory& history )
    : _event1      (event1)
    , _event2      (NULL)
    , _queue       (queue)
    , _history     (history)
    , _state       (0)
    , _data1       (NULL)
    , _data2       (NULL)
    , _constraint  ()
    , _optimal     ()
    , _costs       ()
    , _actions     ()
    , _fullBlocked (true)
    , _sameAxis    (false)
    , _useEvent2   (false)
    , _minimizeDrag(false)
  {
    DataSymmetric* symData  = NULL;
    TrackElement*  segment1 = _event1->getSegment();
    TrackElement*  segment2 = segment1->getSymmetric();
    _event1->setTracksFree( 0 );

    _data1 = segment1->getDataNegociate();
    if (not _data1) {
      _state = MissingData;
      return;
    }

    _data1->update();
    _event1->revalidate();

    if (segment2) {
      symData = Session::getKatanaEngine()->getDataSymmetric( segment1->getNet() );

      _data2 = segment2->getDataNegociate();
      if (not _data2 or not symData) {
        _state = MissingData;
        return;
      }

      _event2 = _data2->getRoutingEvent();
      _event2->setTracksFree( 0 );

      cdebug_log(159,1) << "Coupled:" << _event2 << endl;
      _data2->update();
      _event2->revalidate();
      cdebug_tabw(159,-1);

      _sameAxis = (segment1->isVertical() xor symData->isSymVertical());
    }

    Interval perpandicular = _event1->getPerpandicularFree();
    cdebug_log(159,0) << "* Perpandicular (master):  "  << perpandicular  << endl;

    _constraint = _event1->getConstraints();
    cdebug_log(159,0) << "* Constraints:    "  << _constraint    << " (" << _constraint.getVMin() << " " << _constraint.getVMax() << ")" << endl;

    _optimal    = _event1->getOptimal();
    if (_event2) {
      if (_sameAxis) {
        _constraint  .intersection( _event2->getConstraints() );
        perpandicular.intersection( _event2->getPerpandicularFree() );

        cdebug_log(159,0) << "* Perpandicular (slave): same axis "
                          << _event2->getPerpandicularFree()  << endl;
      } else {
        _constraint  .intersection( symData->getSymmetrical( _event2->getConstraints() ) );
        perpandicular.intersection( symData->getSymmetrical( _event2->getPerpandicularFree() ) );

        cdebug_log(159,0) << "* Perpandicular (slave): PP axis "
                          << symData->getSymmetrical(_event2->getPerpandicularFree())  << endl;
        cdebug_log(159,0) << "* Constraints:    "  << _constraint    << " (" << _constraint.getVMin() << " " << _constraint.getVMax() << ")" << endl;
      }
    }

    cdebug_log(159,0) << "Anabatic intervals:" << endl;
    cdebug_log(159,0) << "* Optimal:        "  << _optimal       << " (" << _optimal.getVMin()    << " " << _optimal.getVMax()    << ")" << endl;
    cdebug_log(159,0) << "* Constraints:    "  << _constraint    << " (" << _constraint.getVMin() << " " << _constraint.getVMax() << ")" << endl;
    cdebug_log(159,0) << "* Perpandicular:  "  << perpandicular  << endl;
    cdebug_log(159,0) << "* AxisHint:       "  << DbU::getValueString(_event1->getAxisHint()) << endl;

    if (_event1->getTracksNb()) {
      if (_constraint.getIntersection(perpandicular).isEmpty()) {
        cdebug_log(159,0) << "Perpandicular free is too tight." << endl;
        _state = EmptyTrackList;
      } else
        _constraint.intersection( perpandicular );
    } else {
      cdebug_log(159,0) << "No Track in perpandicular free." << endl;
      if (not segment1->isNonPref()) _state = EmptyTrackList;
      else {
        cdebug_log(159,0) << "But in non-preferred direction, so that may happen." << endl;
      }
    }

    if (_state == EmptyTrackList) return;

    cdebug_log(159,0) << "Negociate intervals:" << endl;
    cdebug_log(159,0) << "* Optimal:     "      << _optimal    << endl;
    cdebug_log(159,0) << "* Constraints: "      << _constraint << endl;
    cdebug_log(159,1) << "* _sameAxis:   "      << _sameAxis   << endl;

    // if ( segment->isLocal() and (_data->getState() >= DataNegociate::MaximumSlack) )
    //   _constraint.inflate ( 0, DbU::lambda(1.0) );

    RoutingPlane* plane = Session::getKatanaEngine()->getRoutingPlaneByLayer(segment1->getLayer());

    if (segment1->isNonPref()) {
      Track*        baseTrack = plane->getTrackByPosition( segment1->base()->getSourcePosition(), Constant::Superior );
      RoutingPlane* perpPlane = plane->getTop();
      if (not perpPlane) perpPlane = plane->getBottom();
      cdebug_log(155,0) << "sourcePosition():" << DbU::getValueString(segment1->base()->getSourcePosition()) << endl;
      cdebug_log(155,0) << "  -> baseTrack:" << baseTrack << endl;

      for ( Track* ptrack : Tracks_Range::get(perpPlane,_constraint) ) {
        cdebug_log(155,0) << "Align on (top) preferred: " << ptrack << endl;
        _costs.push_back( new TrackCost(segment1,NULL,baseTrack,NULL,ptrack->getAxis(),0) );
      
        cdebug_log(155,0) << "AxisWeight:" << DbU::getValueString(_costs.back()->getRefCandidateAxis())
                          << " sum:" << DbU::getValueString(_costs.back()->getAxisWeight())
                          << endl;
        
        if ( _fullBlocked and (not _costs.back()->isBlockage() and not _costs.back()->isFixed()) ) 
          _fullBlocked = false;

        cdebug_log(155,0) << "| " << _costs.back() << ((_fullBlocked)?" FB ": " -- ") << ptrack << endl;
      }
      if (_costs.empty()) {
        _costs.push_back( new TrackCost(segment1,NULL,baseTrack,NULL,segment1->getAxis(),0) );
        if ( _fullBlocked and (not _costs.back()->isBlockage() and not _costs.back()->isFixed()) ) 
          _fullBlocked = false;
      }
    } else {
      for ( Track* track1 : Tracks_Range::get(plane,_constraint) ) {
        Track*     track2  = NULL;
        DbU::Unit  symAxis = 0;
        if (_event2) {
          track2 =
            (_sameAxis) ? track1 : plane->getTrackByPosition
              ( segment2->getSymmetricAxis( symData->getSymmetrical( track1->getAxis() ) ) );
          
          if (track2) symAxis = track2->getAxis();

          cdebug_log(155,0) << "refTrack:"      << track1 << endl;
          cdebug_log(155,0) << "symTrack:"      << track2 << endl;
          cdebug_log(155,0) << "by symData:   " << DbU::getValueString( symData->getSymmetrical(track1->getAxis()) ) << endl;
          cdebug_log(155,0) << "plus segment2:" << DbU::getValueString( segment2->getSymmetricAxis(symData->getSymmetrical(track1->getAxis())) ) << endl;
        }

        _costs.push_back( new TrackCost(segment1,segment2,track1,track2,track1->getAxis(),symAxis) );
        cdebug_log(155,0) << "Same Ripup:" << _data1->getSameRipup() << endl;
        if ((_data1->getSameRipup() > 10) and (track1->getAxis() == segment1->getAxis())) {
          cdebug_log(155,0) << "Track blacklisted" << endl;
          _costs.back()->setBlacklisted();
        }
      
        cdebug_log(155,0) << "AxisWeight:" << DbU::getValueString(_costs.back()->getRefCandidateAxis())
                          << " sum:" << DbU::getValueString(_costs.back()->getAxisWeight())
                          << endl;
        
        if ( _fullBlocked and (not _costs.back()->isBlockage() and not _costs.back()->isFixed()) ) 
          _fullBlocked = false;

        cdebug_log(155,0) << "| " << _costs.back() << ((_fullBlocked)?" FB ": " -- ") << track1 << endl;
      }
    }
    cdebug_tabw(159,-1);

    if (_costs.empty()) {
      Track* nearest = plane->getTrackByPosition(_constraint.getCenter());

      if (  (nearest->getAxis() < _constraint.getVMin())
         or (nearest->getAxis() > _constraint.getVMax()) ) {
      //setUnimplemented ();
      //cerr << "[UNIMPLEMENTED] " << segment << " no Track in constraint interval "
      //     << _constraint << " " <<  "." << endl;
      } else {
        cerr << Bug( " %s Track_Range() failed to find Tracks in %s (they exists)."
                   , getString(segment1).c_str()
                   , getString(_constraint).c_str()
                   ) << endl;
      }
      cdebug_log(155,0) << "-> No candidate track found." << endl;
      _state = EmptyTrackList;
    }

    uint32_t flags = 0;
    flags |= (segment1->isStrap()) ? TrackCost::IgnoreAxisWeight : 0;
    flags |= (segment1->isLocal()
             and (_data1->getState() < DataNegociate::Minimize)
             and (_data1->getRipupCount() < 5))
             ? TrackCost::DiscardGlobals : 0;
    flags |= (Session::getStage() == StageRepair ) ? TrackCost::IgnoreSharedLength : 0;
    flags |= (Session::getStage() == StageRealign) ? TrackCost::IgnoreTerminals    : 0;

    if (flags & TrackCost::DiscardGlobals) {
      cdebug_log(159,0) << "TrackCost::Compare() - DiscardGlobals" << endl;
    }

  // FOR ANALOG ONLY.
  //flags |= TrackCost::IgnoreSharedLength;
    sort( _costs.begin(), _costs.end(), TrackCost::Compare(flags) );

    size_t i=0;
    for ( ; (i<_costs.size()) and _costs[i]->isFree() ; i++ );
    _event1->setTracksFree( i );
    if (_event2) _event2->setTracksFree( i );

    // if (not segment1->isDrag()) {
    //   cdebug_log(159,0) << "Not a draggable segment and _costs.size():" << _costs.size() << endl;
    //   if (_costs.size() == 1) {
    //     segment1->base()->setFlags( AutoSegment::SegFixedAxis );
    //     if (segment2) segment2->base()->setFlags( AutoSegment::SegFixedAxis );
    //   } else {
    //     segment1->base()->unsetFlags( AutoSegment::SegFixedAxis );
    //     if (segment2) segment2->base()->unsetFlags( AutoSegment::SegFixedAxis );
    //   }
    // }
  }


  SegmentFsm::~SegmentFsm ()
  {
    for ( TrackCost* cost : _costs ) delete cost;
  }


  void  SegmentFsm::setDataState ( uint32_t state )
  {
    _data1->setState( state );
    if (_data2) _data2->setState( state );
  }


  void  SegmentFsm::addAction ( TrackElement* segment
                              , uint32_t      type
                              , DbU::Unit     axisHint
                              , uint32_t      toSegmentFsm  )
  {
    if (segment->isFixed()) return;
    
    _actions.push_back ( SegmentAction(segment,type,axisHint,toSegmentFsm) );
    cdebug_log(159,0) << "SegmentFsm::addAction(): " << segment << endl;
  }


  void  SegmentFsm::doActions ()
  {
    cdebug_log(159,1) << "SegmentFsm::doActions() - " << _actions.size() << endl;

    for ( AutoSegment* base : Session::getInvalidateds() ) {
      if (not base->isInvalidatedLayer()) continue;
      TrackElement* segment = Session::lookup( base );
      if (not segment) continue;
      if (hasAction(segment)) continue;
      addAction( segment, SegmentAction::SelfRipup );
    }

    bool ripupOthersParallel = false;
    bool ripedByLocal        = getEvent()->getSegment()->isLocal();

    for ( size_t i=0 ; i<_actions.size() ; i++ ) {
      if ( ripedByLocal ) _actions[i].setFlag ( SegmentAction::RipedByLocal );
      if ( _actions[i].getType() & SegmentAction::OtherRipup ) {
        ripupOthersParallel = true;
      }
    }

    for ( size_t i=0 ; i<_actions.size() ; i++ ) {
      if ( (_actions[i].getType() & SegmentAction::SelfInsert) and ripupOthersParallel )
        _actions[i].setFlag ( SegmentAction::EventLevel3 );

      DebugSession::open ( _actions[i].getSegment()->getNet(), 156, 160 );
      if ( not _actions[i].doAction(_queue) ) {
        cinfo << "[INFO] Failed action on " << _actions[i].getSegment() << endl;
      }
      DebugSession::close ();
    }

    _actions.clear ();

    cdebug_tabw(159,-1);
  }


  void  SegmentFsm::incRipupCount ()
  {
    _data1->incRipupCount();
    if (_data2) _data2->incRipupCount();
  }


  bool  SegmentFsm::insertInTrack ( size_t i )
  {
    cdebug_log(159,0) << "SegmentFsm::insertInTrack() istate:" << _event1->getInsertState()
                      << " track:" << i << endl;

    bool success = true;

    _event1->incInsertState();
    switch ( _event1->getInsertState() ) {
      case 1:
        success =                             Manipulator(_event1->getSegment(),useEvent1()).insertInTrack(i);
        success = success and (not _event2 or Manipulator(_event2->getSegment(),useEvent2()).insertInTrack(i));
        if (success) break;
        _event1->incInsertState();
        clearActions();
      case 2:
        success =                             Manipulator(_event1->getSegment(),useEvent1()).shrinkToTrack(i);
        success = success and (not _event2 or Manipulator(_event2->getSegment(),useEvent2()).shrinkToTrack(i));
        if (success) break;
        _event1->incInsertState();
        clearActions();
      case 3:
        success =                             Manipulator(_event1->getSegment(),useEvent1()).forceToTrack(i);
        success = success and (not _event2 or Manipulator(_event2->getSegment(),useEvent2()).forceToTrack(i));
        if (success) break;
        _event1->incInsertState();
        clearActions();
    }

    useEvent1();
    if (_event2) _event2->setInsertState( _event1->getInsertState() );

    if (success and Session::disableStackedVias())
      Manipulator( _event1->getSegment(), useEvent1() ).avoidStackedVias( getCandidateAxis1(i) );

    return success;
  }


  void  SegmentFsm::bindToTrack ( size_t i )
  {
    cdebug_log(159,0) << "SegmentFsm::bindToTrack() track:" << i << endl;

    _event1->resetInsertState();
    _event1->updateAxisHistory();
    _event1->setEventLevel( 0 );

    cdebug_log(9000,0) << "Deter| addInsertEvent() @" << getTrack1(i) << endl;
    if (not (getSegment1()->isGlobal() or getSegment1()->isFixed()))
      getSegment1()->forcePositions( getCost(i)->getRefTrackFree() );
    Session::addInsertEvent( getSegment1(), getTrack1(i), getCandidateAxis1(i) );

    if (_event2) {
      _event2->resetInsertState();
      _event2->updateAxisHistory();
      _event2->setEventLevel( 0 );
      _event2->setProcessed( true );

      cdebug_log(9000,0) << "Deter| addInsertEvent() @" << getTrack2(i) << endl;
      Session::addInsertEvent( getSegment2(), getTrack2(i), getCandidateAxis2(i) );
    }

    setState( SegmentFsm::SelfInserted );

    if (Session::disableStackedVias())
      Manipulator( _event1->getSegment(), useEvent1() ).avoidStackedVias( getCandidateAxis1(i) );
  }


  void  SegmentFsm::moveToTrack ( size_t i )
  {
    cdebug_log(159,0) << "SegmentFsm::moveToTrack() :" << " track:" << i << endl;

    Session::addMoveEvent( getSegment1(), getTrack1(i), getCandidateAxis1(i) );

    if (_event2) {
      cdebug_log(9000,0) << "Deter| addInsertEvent() @" << getTrack1(i) << endl;
      Session::addMoveEvent( getSegment2(), getTrack2(i), getCandidateAxis2(i) );
    }

    setState( SegmentFsm::SelfInserted );

    if (Session::disableStackedVias())
      Manipulator( _event1->getSegment(), useEvent1() ).avoidStackedVias( getCandidateAxis1(i) );
  }


  void  SegmentFsm::ripupPerpandiculars ()
  {
    Manipulator(getSegment1(),*this).ripupPerpandiculars();
    if (_event2)
      Manipulator(getSegment2(),*this).ripupPerpandiculars();
  }


  bool  SegmentFsm::canRipup ( uint32_t flags )
  {
    return                Manipulator(getSegment1(),*this).canRipup(flags)
      and (not _event2 or Manipulator(getSegment2(),*this).canRipup(flags));
  }


  bool  SegmentFsm::conflictSolveByHistory ()
  {
    bool          success = false;
    RipupHistory  ripupHistory ( _event1 );
    RoutingEvent* event;
    TrackElement* segment = getEvent()->getSegment();

    cdebug_log(159,0) << "SegmentFsm::conflictSolveByHistory()" << endl;

    size_t maxDepth   = min( getHistory().size(), (size_t)300 );
    size_t depth      = 0;
    while ( (ripupHistory.getDislodgersCount() < 3) and (depth < maxDepth) ) {
      event = getHistory().getRNth(depth++);
      if (not event) continue;
      if ( (event->getSegment() != segment) and ripupHistory.isDislodger(event) )
        ripupHistory.addDislodger( event );
    }

  //ripupHistory.print ( cout );

    UnionIntervals* intervals = ripupHistory.getUnionIntervals( segment->getAxis() );
    if (intervals and not intervals->empty()) {

      DbU::Unit minConflict  = intervals->getVMin();
      DbU::Unit maxConflict  = intervals->getVMax();
      Interval  canonical    = segment->getCanonicalInterval();
      bool      sourceDogleg = canonical.contains(minConflict);
      bool      targetDogleg = canonical.contains(maxConflict);
      Point     breakPoint;

      if (sourceDogleg) {
        if (segment->isHorizontal()) {
          breakPoint = Point( minConflict, segment->getAxis() );
          cdebug_log(159,0) << breakPoint << endl;
        } else {
          breakPoint = Point( segment->getAxis(), minConflict );
          cdebug_log(159,0) << breakPoint << endl;
        }

        Anabatic::GCell* dogLegGCell = Session::getGCellUnder( breakPoint.getX(), breakPoint.getY() );
        if (dogLegGCell) {
          if (segment->canDogleg(dogLegGCell))
            success = segment->makeDogleg(dogLegGCell);
        } else {
          cerr << Bug( "No GCell under source %s for:\n      %s."
                     , getString(breakPoint).c_str(), getString(segment).c_str() ) << endl;
        }
      }

      if (not success and targetDogleg) {
        if (segment->isHorizontal()) {
          breakPoint = Point( maxConflict, segment->getAxis() );
          cdebug_log(159,0) << breakPoint << endl;
        } else {
          breakPoint = Point( segment->getAxis(), maxConflict );
          cdebug_log(159,0) << breakPoint << endl;
        }

        Anabatic::GCell* dogLegGCell = Session::getGCellUnder( breakPoint.getX(), breakPoint.getY() );
        if (dogLegGCell) {
          if (segment->canDogleg(dogLegGCell)) {
            success = segment->makeDogleg(dogLegGCell);
          }
        } else {
          cerr << Bug( "No GCell under target %s for:\n      %s."
                     , getString(breakPoint).c_str(), getString(segment).c_str() ) << endl;
        }
      }
    } else {
      cdebug_log(159,0) << "No disloggers found @" << DbU::getValueString(segment->getAxis()) << endl;

      Track* track = Session::getKatanaEngine()->getTrackByPosition( segment->getLayer()
                                                                   , segment->getAxis () );
      if (track) {
        Interval freeSpan = track->getFreeInterval( segment->getSourceU(), segment->getNet() );

        if (freeSpan.contains(segment->getCanonicalInterval())) {
          cdebug_log(159,0) << "Disloggers vanished, Segment can be re-inserted." << endl;
          success = true;
        }
      }
    }

    return success;
  }


  bool  SegmentFsm::conflictSolveByPlaceds ()
  {
    bool                  success    = false;
    Interval              constraints;
    vector<Cs1Candidate>  candidates;
    TrackElement*         segment    = getEvent()->getSegment();
    bool                  canMoveUp  = (segment->isLocal()) ? segment->canPivotUp(0.5,Flags::NoFlags) : segment->canMoveUp(1.0,Flags::CheckLowDensity); // MARK 1
    uint32_t              relaxFlags = Manipulator::NoDoglegReuse
                                     | ((_data1 and (_data1->getStateCount() < 2)) ? Manipulator::AllowExpand
                                                                                   : Manipulator::NoExpand);

    cdebug_log(159,0) << "SegmentFsm::conflictSolveByPlaceds()" << endl;
    cdebug_log(159,0) << "| Candidates Tracks: " << endl;

    segment->base()->getConstraints( constraints );
    Interval      overlap    = segment->getCanonicalInterval();
    RoutingPlane* plane      = Session::getKatanaEngine()->getRoutingPlaneByLayer(segment->getLayer());
    Track*        track      = plane->getTrackByPosition(constraints.getVMin(),Constant::Superior);

    if (not track) {
      cerr << Bug( "SegmentFsm::conflictSolveByPlaceds():\n"
                   "      For: %s\n"
                   "      In %s, no Track near %s"
                 , getString(segment).c_str()
                 , getString(plane).c_str()
                 , DbU::getValueString(constraints.getVMin()).c_str()
                 ) << endl;
      return false;
    }

    for ( ; track and track->getAxis() <= constraints.getVMax() ; track = track->getNextTrack() ) {
      candidates.push_back( Cs1Candidate(track,segment->getPPitch()) );

      size_t        begin         = Track::npos;
      size_t        end           = Track::npos;
      TrackElement* other         = nullptr;
      Net*          otherNet      = nullptr;
      Interval      otherOverlap;
      bool          otherIsGlobal = false;

      track->getOverlapBounds( overlap, begin, end );
      candidates.back().setBegin( begin );
      candidates.back().setEnd  ( end );

      cdebug_log(159,0) << "* " << track << " [" << begin << ":" << end << "]" << endl;

      for ( ; (begin < end) ; ++begin ) {
        other         = track->getSegment( begin );
        otherIsGlobal =    otherIsGlobal
                        or other->isGlobal()
                        or other->isBlockage()
                        or other->isFixed()
                        or other->isFixedAxis();

        if (other->getNet() == segment->getNet()) {
          cdebug_log(159,0) << "  | " << begin << " Same net: " << " " << other << endl;
          continue;
        }
        if (not other->getCanonicalInterval().intersect(overlap)) {
          cdebug_log(159,0) << "  | " << begin << " No Conflict: " << " " << other << endl;
          if (otherNet == NULL) candidates.back().setBegin( begin+1 );
          continue;
        }
        cdebug_log(159,0) << "  | " << begin << " Conflict: " << " " << other << endl;

        if (otherNet != other->getNet()) {
          if (otherNet) {
            if (otherIsGlobal) {
              candidates.back().addConflict( otherOverlap );
              cdebug_log(159,0) << "  | Other overlap G: " << otherOverlap << endl;
            } else {
              cdebug_log(159,0) << "  | Other overlap L: " << otherOverlap << " ignored." << endl;
            }
          }
          otherNet      = other->getNet();
          otherOverlap  = other->getCanonicalInterval();
          otherIsGlobal = other->isGlobal() or other->isBlockage() or other->isFixed();
        } else {
          otherOverlap.merge(other->getCanonicalInterval());
          otherIsGlobal = otherIsGlobal or other->isGlobal() or other->isBlockage() or other->isFixed() or other->isFixedAxis();
        }
      }
      if (not otherOverlap.isEmpty()) {
        if (otherIsGlobal) {
          candidates.back().addConflict( otherOverlap );
          cdebug_log(159,0) << "  | Other overlap G: " << otherOverlap << endl;
        } else {
          cdebug_log(159,0) << "  | Other overlap L: " << otherOverlap << " ignored." << endl;
        }
      }

      candidates.back().consolidate();
    }

    sort( candidates.begin(), candidates.end() );

    for ( size_t icandidate=0 ; icandidate<candidates.size() ; ++icandidate ) {
      cdebug_log(159,0) << "Trying l:" << candidates[icandidate].getSize()
                  << " " << candidates[icandidate].getTrack() << endl;

      Interval overlap0 = candidates[icandidate].getLongestConflict();
      cdebug_log(159,0) << "| overlap0: " << overlap0 << endl;

      if (overlap0.isEmpty()) {
        cdebug_log(159,0) << "overlap0 is empty, no conflict, ignoring Track candidate." << endl;
        continue;
      }

      Track*        track = candidates[icandidate].getTrack();
      TrackElement* other = track->getSegment( overlap.getCenter() );
      if (not other) {
        cdebug_log(159,0) << "conflictSolveByPlaceds(): No segment under overlap center." << endl;
        other = track->getSegment( overlap0.getCenter() );

        if (not other) {
          cdebug_log(159,0) << "conflictSolveByPlaceds(): No segment under overlap0 center." << endl;
          continue;
        }
      }
      cdebug_log(159,0) << "| other: " << other << endl;
      cdebug_log(159,0) << "| overlap0: " << overlap0 << endl;

      if (Session::getConfiguration()->isVH() and (segment->getDepth() == 1)) {
        if (Manipulator(segment,*this).makeDogleg(overlap0,Flags::ShortDogleg)) {
        //cerr << "Break using ShortDogleg." << endl;
          success = true;
          break;
        }
      } else {
        cdebug_log(159,0) << "conflictSolveByPlaceds() other->isGlobal():" << other->isGlobal() << endl;
        if (other->isGlobal()) {
          cdebug_log(159,0) << "conflictSolveByPlaceds() - Conflict with global, other move up" << endl;
          if ((success = Manipulator(other,*this).moveUp(Manipulator::IgnoreContacts))) break;
        }
        
        cdebug_log(159,0) << "conflictSolveByPlaceds() - Relaxing self" << endl;
        cdebug_log(159,0) << "| overlap0: " << overlap0 << endl;

        if (Manipulator(segment,*this).relax(overlap0,relaxFlags)) {
          success = true;
          break;
        } else {
          if ( not canMoveUp
             and (relaxFlags != Manipulator::NoExpand)
             and Manipulator(segment,*this).relax(overlap0,Manipulator::NoExpand|Manipulator::NoDoglegReuse) ) {
            cdebug_log(159,0) << "Cannot move up but successful narrow breaking." << endl;
            success = true;
            break;
          }
        }
      }
    }

    if (not success and Session::getConfiguration()->isVH() and (segment->getDepth() == 1)) {
      cdebug_log(159,0) << "Fallback for VH topologies, try break in low density area." << endl;
      success = Manipulator(segment,*this).relaxVH( Manipulator::NoExpand|Manipulator::NoDoglegReuse );
    }

    if ( not success and segment->isGlobal() and (_costs.size() <= 1) ) {
      cdebug_log(159,0) << "Overconstrained perpandiculars, rip them up. On track:" << endl;
      cdebug_log(159,0) << "  " << track << endl;
      Manipulator(segment,*this).ripupPerpandiculars ();
      success = true;
    }

    return success;
  }


  bool  SegmentFsm::solveTerminalVsGlobal ()
  {
    TrackElement* segment = getEvent()->getSegment();
    cdebug_log(159,0) << "SegmentFsm::solveTerminalVsGlobal: " << " " << segment << endl;

    if (not (segment->isTerminal() and segment->isLocal())) return false;

    for ( size_t icost=0 ; icost<_costs.size() ; ++icost ) {
      Interval overlap = segment->getCanonicalInterval();
      size_t   begin;
      size_t   end;
      getTrack1(icost)->getOverlapBounds( overlap, begin, end );

      for ( ; begin<end ; ++begin ) {
        TrackElement* other        = getTrack1(icost)->getSegment(begin);
        Interval      otherOverlap = other->getCanonicalInterval();
        
        if (other->getNet() == segment->getNet()) continue;
        if (not other->isGlobal()) continue;
        if (not otherOverlap.contains(overlap)) continue;

        cdebug_log(159,0) << "| Global candidate:" << other << endl;
        if (Manipulator(other,*this).moveUp(Manipulator::AllowTerminalMoveUp)) {
          cdebug_log(159,0) << "| Global candidate selected." << endl;
          return true;
        }
      }
    }

    return false;
  }


  bool  SegmentFsm::solveFullBlockages ()
  {
    bool          success = false;
    TrackElement* segment = getEvent()->getSegment();

    cdebug_log(159,1) << "SegmentFsm::solveFullBlockages: " << " " << segment << endl;

    if ( segment->isLocal() ) {
      success = Manipulator(segment,*this).pivotUp();
      if ( not success ) {
        cdebug_log(159,0) << "Tightly constrained local segment overlapping a blockage, move up." << endl;
        cdebug_log(159,0) << segment << endl;
        success = Manipulator(segment,*this).moveUp
          (Manipulator::AllowLocalMoveUp|Manipulator::AllowTerminalMoveUp);
      }
    } else if (not _costs.empty()) {
      Interval overlap = segment->getCanonicalInterval();
      size_t   begin;
      size_t   end;

      getTrack1(0)->getOverlapBounds ( overlap, begin, end );
      for ( ; begin<end ; ++begin ) {
        TrackElement* other        = getTrack1(0)->getSegment(begin);
        Interval      otherOverlap = other->getCanonicalInterval();
        
        if ( other->getNet() == segment->getNet() ) continue;
        if ( not otherOverlap.intersect(overlap) ) continue;
        
        cdebug_log(159,0) << "| " << begin << " Blockage conflict: " << " " << other << endl;
        if ( (success = Manipulator(segment,*this).relax
             (otherOverlap,Manipulator::NoDoglegReuse|Manipulator::NoExpand)) ) {
          break;
          }
      }
    }
    if ( not success ) {
      cparanoid << Error( "Tighly constrained segment overlapping a blockage:\n        %s"
                        , getString(segment).c_str() ) << endl;
      cdebug_log(159,0) << "Segment is hard blocked, bypass to Unimplemented." << endl;
    }
    
    cdebug_tabw(159,-1);
    return success;
  }


  bool  SegmentFsm::desaturate ()
  {
    cdebug_log(159,1) << "SegmentFsm::desaturate()" << endl;

    size_t        itrack  = 0;

#if THIS_IS_DISABLED
    TrackElement* segment = getEvent()->getSegment();
    for ( ; itrack<getCosts().size() ; ++itrack ) {
      cdebug_log(159,0) << "Trying track:" << itrack << endl;

      if ( getCost(itrack)->isGlobalEnclosed() ) {
        Track*    track      = getTrack(itrack);
        size_t    begin      = getBegin(itrack);
        size_t    end        = getEnd  (itrack);
        Net*      ownerNet   = segment->getNet();
        Interval  toFree      (segment->getCanonicalInterval());
        bool      success    = true;
      
        for ( size_t i = begin ; success and (i < end) ; i++ ) {
          TrackElement* segment2 = track->getSegment(i);

          cdebug_log(159,0) << "* Looking // " << segment2 << endl;

          if ( segment2->getNet() == ownerNet  ) continue;
          if ( not toFree.intersect(segment2->getCanonicalInterval()) ) continue;
          if ( segment2->isFixed() or not segment2->isBipoint() ) {
            success = false;
            continue;
          }

          DataNegociate* data2 = segment2->getDataNegociate();
          if ( not data2 ) {
            cdebug_log(159,0) << "No DataNegociate, ignoring." << endl;
            success = false;
            continue;
          }

          cdebug_log(159,0) << "- Forced moveUp " << segment2 << endl;
          if ( not (success=Manipulator(segment2,*this).moveUp(Manipulator::AllowTerminalMoveUp)) ) {
            continue;
          }
        }

        if ( success ) {
          setState ( SegmentFsm::OtherRipup );
          addAction( segment
                   , SegmentAction::SelfInsert|SegmentAction::MoveToAxis
                   , getTrack1(itrack)->getAxis()
                   );
          break;
        }
      }
    }

#endif
    cdebug_tabw(159,-1);
    return (itrack < _costs.size());
  }


  bool  SegmentFsm::_slackenStrap ( TrackElement*& segment, DataNegociate*& data, uint32_t flags )
  {
    cdebug_log(159,0) << "Strap segment Fsm." << endl;

    bool        success     = false;
    uint32_t    nextState   = data->getState();
    Manipulator manipulator ( segment, *this );

    if (segment->isNonPref()
       and not getCosts().empty()
       and (getCost(0)->isBlockage() or getCost(0)->isAtRipupLimit())) {
      cdebug_log(159,0) << "Non-preferred conflicts with a blockage or other's at ripup limit." << endl;
      success = manipulator.avoidBlockage();
    }

    if (not success) {
      switch ( data->getState() ) {
        case DataNegociate::RipupPerpandiculars:
          nextState = DataNegociate::Minimize;
          success = manipulator.ripupPerpandiculars();
          if (success) break;
        case DataNegociate::Minimize:
          if (data->getStateCount() >= 2) {
            nextState = DataNegociate::Slacken;
          }
          success = manipulator.minimize();
          if (success) break;
        case DataNegociate::Dogleg:
        case DataNegociate::Slacken:
          if ((success = manipulator.slacken(Flags::HalfSlacken))) {
            nextState = DataNegociate::LocalVsGlobal;
            break;
          }
        case DataNegociate::LocalVsGlobal:
          if (segment->isUnbreakable()) {
            nextState = DataNegociate::MaximumSlack;
            success   = true;
            break;
          }
        case DataNegociate::ConflictSolveByHistory:
        case DataNegociate::ConflictSolveByPlaceds:
        case DataNegociate::MoveUp:
        case DataNegociate::MaximumSlack:
        case DataNegociate::Unimplemented:
          nextState = DataNegociate::Unimplemented;
          break;
      }
    }
    
    if (not success and (nextState != DataNegociate::Unimplemented))
      success = manipulator.ripupPerpandiculars(Manipulator::ToRipupLimit);

    // if (not (flags&NoTransition)) {
      data->setState( nextState );
      cdebug_log(159,0) << "Incrementing state (after): " << nextState << " count:" << data->getStateCount() << endl;
    // }

    return success;
  }


  bool  SegmentFsm::_slackenLocal ( TrackElement*& segment, DataNegociate*& data, uint32_t flags )
  {
    cdebug_log(159,0) << "Local segment Fsm." << endl;

    bool        success     = false;
    uint32_t    nextState   = data->getState();
    Manipulator manipulator ( segment, *this );


    switch (data->getState()) {
      case DataNegociate::RipupPerpandiculars:
        if (segment->isDrag() and getCost(0)->isInfinite()) {
          nextState = DataNegociate::Slacken;
          success   = manipulator.dragMinimize();
          if (success) _minimizeDrag = true;
        } else {
          nextState = DataNegociate::Minimize;
          success   = manipulator.ripupPerpandiculars();
        }
        if (success) break;
      case DataNegociate::Minimize:
        if (nextState == DataNegociate::Minimize) {
          if (not segment->isTerminal()) {
            if (isFullBlocked()) {
              cdebug_log(159,0) << "Is Fully blocked." << endl;
              if (_costs.size() > 7) {
                success = manipulator.moveUp( Manipulator::AllowLocalMoveUp );
                if (success) {
                  cdebug_log(159,0) << "Was able to move up." << endl;
                  break;
                }
              }
              nextState = DataNegociate::Unimplemented;
              break;
            }
            if (segment->isNonPref() and (data->getRipupCount() > 3)) {
              cdebug_log(159,0) << "Non pref. "  << data->getRipupCount() << endl;
              success = manipulator.moveUp( Manipulator::AllowLocalMoveUp|Manipulator::IgnoreContacts );
              if (success) {
                cdebug_log(159,0) << "Was able to move up." << endl;
                break;
              }
            }
          }
          nextState = DataNegociate::Dogleg;
          success   = manipulator.minimize();
          if (success) break;
        }
      case DataNegociate::Dogleg:
          if (nextState == DataNegociate::Dogleg) {
            nextState = DataNegociate::Slacken;
            success   = manipulator.makeDogleg();
          }
          if (success) break;
      case DataNegociate::Slacken:
        if (nextState == DataNegociate::Slacken) {
          nextState = DataNegociate::ConflictSolveByPlaceds;
          success   = manipulator.slacken();
          if (success) break;
        }
      case DataNegociate::ConflictSolveByHistory:
      case DataNegociate::ConflictSolveByPlaceds:
        if (  (nextState == DataNegociate::ConflictSolveByHistory) 
           or (nextState == DataNegociate::ConflictSolveByPlaceds) ) {
          nextState = DataNegociate::LocalVsGlobal;
          success   = conflictSolveByHistory();
          break;
        }
      case DataNegociate::LocalVsGlobal:
        if (nextState == DataNegociate::LocalVsGlobal) {
          nextState = DataNegociate::MoveUp;
          success   = solveTerminalVsGlobal();
          if (success) break;
        }
      case DataNegociate::MoveUp:
        if (nextState == DataNegociate::LocalVsGlobal) {
          nextState = DataNegociate::MaximumSlack;
          success   = manipulator.moveUp();
          if (success) break;
        }
      case DataNegociate::MaximumSlack:
        if (nextState == DataNegociate::MaximumSlack) {
          if (segment->isStrap()) { 
            if ( (nextState < DataNegociate::MaximumSlack) or (data->getStateCount() < 2) ) {
              nextState = DataNegociate::MaximumSlack;
              success = conflictSolveByPlaceds();
              if (success) break;
            }
          }
        }
      case DataNegociate::Unimplemented:
      //if (segment->isDrag()) cerr << "Slacken DRAG:" << segment << endl;
        nextState = DataNegociate::Unimplemented;
        break;
    }

    if (not success and (nextState != DataNegociate::Unimplemented)) {
      if (data->getStateCount() < 6)
        success = manipulator.ripupPerpandiculars(Manipulator::ToRipupLimit);
    }

    if (not success
       and (nextState == DataNegociate::Unimplemented)
       and segment->isSlackened()
       and isFullBlocked()) {
      if (solveFullBlockages()) nextState = DataNegociate::MoveUp;
    }

    // if (not (flags&NoTransition)) {
      data->setState( nextState );
      cdebug_log(159,0) << "Incrementing state (after): "
                        << DataNegociate::getStateString(nextState,data->getStateCount()) << endl;
    // }

    return success;
  }


  bool  SegmentFsm::_slackenGlobal ( TrackElement*& segment, DataNegociate*& data, uint32_t flags )
  {
    bool         success     = false;
    uint32_t     nextState   = data->getState();
    Manipulator  manipulator ( segment, *this );
    uint32_t     moveUpFlags = Manipulator::AllowShortPivotUp|Manipulator::IgnoreContacts;

    switch ( data->getState() ) {
      case DataNegociate::RipupPerpandiculars:
      case DataNegociate::Minimize:
      case DataNegociate::Dogleg:
        if (NetRoutingExtension::isAnalog(segment->getNet())) {
          cdebug_log(159,0) << "Global, SegmentFsm / Analogic: Try to dogleg once." << endl;
          success = manipulator.makeDogleg( segment->getCanonicalInterval().getCenter() );
          if (success) break;
        } else {
          nextState = DataNegociate::Slacken;
          if (Session::getConfiguration()->isVH()
             and segment->getLength() < 6*Session::getSliceHeight()) {
            cdebug_log(159,0) << "Global, SegmentFsm: RipupPerpandiculars." << endl;
            break;
          }
        }
      case DataNegociate::Slacken:
        cdebug_log(159,0) << "Global, SegmentFsm: Slacken "
                          << ((manipulator.getEvent())
                             ? manipulator.getEvent()->getConstraints() : "(no event yet)") << endl;
        if (  manipulator.getEvent()
           and (  manipulator.getEvent()->getConstraints().isPonctual()
               or (isFullBlocked() and (_costs.size() > 7)))
           and segment->canMoveUp(1.0,Flags::CheckLowUpDensity|Flags::AllowTerminal) ) {
          cdebug_log(159,0) << "Next state: MoveUp." << endl;
          moveUpFlags |= Manipulator::AllowTerminalMoveUp;
        } else {
          if ((success = manipulator.slacken(Flags::HalfSlacken))) {
            nextState = DataNegociate::RipupPerpandiculars;
            cdebug_log(159,0) << "Next state: RipupPerpandiculars (half-slacken succeeded)." << endl;
            break;
          }
        }
      case DataNegociate::MoveUp:
        cdebug_log(159,0) << "Global, SegmentFsm: MoveUp." << endl;
        if ((success = manipulator.moveUp(moveUpFlags))) {
          break;
        }
        nextState = DataNegociate::ConflictSolveByHistory;
        break;
      case DataNegociate::ConflictSolveByHistory:
      case DataNegociate::ConflictSolveByPlaceds:
        cdebug_log(159,0) << "Global, SegmentFsm: ConflictSolveByHistory or ConflictSolveByPlaceds." << endl;
        if ((success = conflictSolveByPlaceds())) {
          if (segment->canMoveUp(1.0,Flags::CheckLowDensity))
            nextState = DataNegociate::MoveUp;
          else {
            if (data->getStateCount() > 3)
              nextState = DataNegociate::MaximumSlack;
          }
          if (segment->getDataNegociate()->getState() < DataNegociate::ConflictSolveByHistory)
            nextState = segment->getDataNegociate()->getState();
          break;
        }
      case DataNegociate::MaximumSlack:
        if ((success=manipulator.forceOverLocals())) {
          break;
        }
      case DataNegociate::Unimplemented:
        cdebug_log(159,0) << "Global, SegmentFsm: MaximumSlack or Unimplemented." << endl;
        nextState = DataNegociate::Unimplemented;
        break;
    }

    if (not success and (nextState != DataNegociate::Unimplemented)) {
      if (data->getStateCount() < 6)
        success = manipulator.ripupPerpandiculars(Manipulator::ToRipupLimit);
    }

  // Special case: all tracks are overlaping a blockage.
    if (not success
       and (nextState == DataNegociate::Unimplemented)
       and segment->isSlackened() ) {
      if (solveFullBlockages()) nextState = DataNegociate::MoveUp;
    }

  //if (not (flags&NoTransition)) {
      if (data->getChildSegment()) {
        TrackElement* child = segment;
        cdebug_log(159,0) << "Incrementing state of childs (after): " << endl;
        while ( child ) {
          cdebug_log(159,0) << "| " << child << endl;
          if (child->base()->isGlobal()) {
            child->getDataNegociate()->setState( nextState );
            cdebug_log(159,0) << "| Update:" << nextState << " count:" << child->getDataNegociate()->getStateCount() << endl;
          }
          TrackElement* parent = child;
          child = parent->getDataNegociate()->getChildSegment();
          parent->getDataNegociate()->setChildSegment( NULL );
        }
      } else {
        data->setState( nextState );
        cdebug_log(159,0) << "Incrementing state (after): " << segment << endl;
        cdebug_log(159,0) << "| " << nextState << " count:" << data->getStateCount() << endl;
      }
  //}

    return success;
  }


  bool  SegmentFsm::slackenTopology ( uint32_t flags )
  {
    bool          success     = false;
    TrackElement* segment1    = getSegment1();
    uint32_t      actionFlags = SegmentAction::SelfInsert|SegmentAction::EventLevel5;

    DebugSession::open( segment1->getNet(), 156, 160 );
    cdebug_log(159,1) << "Slacken Topology for " << segment1->getNet()
                      << " " << segment1 << endl;

    if (_data2) {
      cdebug_log(159,0) << "Symmetric segments are not allowed to slacken (yet)" << endl;
      cdebug_tabw(159,-1);
      DebugSession::close();
      return false;
    }

    if (segment1->isShortNet()) {
      cdebug_log(159,0) << "Short net segments are not allowed to slacken" << endl;
      cdebug_tabw(159,-1);
      DebugSession::close();
      return false;
    }

    if (not segment1 or not _data1) { cdebug_tabw(159,-1); DebugSession::close(); return false; }

    if (segment1->isStrap() or (segment1->isUnbreakable() and not segment1->isGlobal()))
      success = _slackenStrap ( segment1, _data1, flags );
    else if (segment1->isLocal())
      success = _slackenLocal ( segment1, _data1, flags );
    else
      success = _slackenGlobal( segment1, _data1, flags );

    _event1->resetInsertState();
    _data1->resetRipupCount();
    if (_event2) {
      _event2->resetInsertState();
      _data2->resetRipupCount();
    }

    if (success) {
      if (not hasAction(segment1)) {
        actionFlags |= SegmentAction::ResetRipup;
        if (isMinimizeDrag()) {
          actionFlags &= ~SegmentAction::EventLevel5;
          actionFlags |=  SegmentAction::EventLevel3;
        }
        if (segment1->isNonPref()) {
          actionFlags &= ~SegmentAction::AllEventLevels;
        }
        addAction( segment1, actionFlags );
      }
    } else {
      clearActions();
      if (_data1->getState() == DataNegociate::Unimplemented) {
        cinfo << "[UNSOLVED] " << segment1 << " unable to slacken topology." << endl;
      }
    }

    cdebug_tabw(159,-1);
    DebugSession::close();

    return success;
  }


}  // Katana namespace.
