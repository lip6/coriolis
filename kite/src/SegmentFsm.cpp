// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./SegmentFsm.cpp"                         |
// +-----------------------------------------------------------------+


#include <algorithm>
#include "hurricane/Bug.h"
#include "hurricane/DebugSession.h"
#include "kite/TrackElement.h"
#include "kite/Tracks.h"
#include "kite/RoutingPlane.h"
#include "kite/DataNegociate.h"
#include "kite/RoutingEvent.h"
#include "kite/RoutingEventQueue.h"
#include "kite/RoutingEventHistory.h"
#include "kite/Manipulator.h"
#include "kite/SegmentFsm.h"
#include "kite/KiteEngine.h"


namespace {

  using namespace std;
  using namespace Hurricane;
  using namespace Kite;


// -------------------------------------------------------------------
// Class  :  "Cs1Candidate".

  class Cs1Candidate {
    public:
      inline            Cs1Candidate       ( Track* track=NULL, DbU::Unit ppitch=0 );
      inline Track*     getTrack           () const;
      inline size_t     getBegin           () const;
      inline size_t     getEnd             () const;
      inline size_t     getLength          () const;
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
  inline size_t     Cs1Candidate::getLength          () const { return _conflicts.size(); }
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
    ltrace(200) << "UnionInterval::addInterval() - " << interval << endl;

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
    RoutingPlane*   plane         = Session::getKiteEngine()->getRoutingPlaneByLayer(_masterEvent->getSegment()->getLayer());
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


namespace Kite {

  using std::sort;
  using Hurricane::tab;
  using Hurricane::inltrace;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::DebugSession;
  using Hurricane::Bug;
  using Hurricane::ForEachIterator;
  using Katabatic::KbHalfSlacken;


// -------------------------------------------------------------------
// Class  :  "SegmentAction".

  SegmentAction::SegmentAction ( TrackElement* segment
                               , unsigned int  type
                               , DbU::Unit     axisHint
                               , unsigned int  toState
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

    DebugSession::open( _segment->getNet(), 200 );

    if (_type & Perpandicular) {
      ltrace(200) << "* Riping Pp " << _segment << endl;
    } else {
      ltrace(200) << "* Riping // " << _segment << endl;
    }

    if (_segment->isFixed()) { DebugSession::close(); return true; }

    DataNegociate* data = _segment->getDataNegociate();
    if (data == NULL) { DebugSession::close(); return true; }

    if (_type & ResetRipup) data->resetRipupCount();

    if (_type & ToState) {
      data->setState     ( _toState );
      data->setRipupCount( Session::getKiteEngine()->getRipupLimit(_segment) );
    }

    if (_segment->getTrack()) Session::addRemoveEvent( _segment );

    RoutingEvent* event = data->getRoutingEvent();
    if (event == NULL) {
      cerr << Bug( "Missing Event on %p:%s"
                 , _segment->base()->base(),getString(_segment).c_str() ) << endl;
      DebugSession::close();
      return true;
    }

    if ( (_type & AxisHint) /*and not _segment->isSlackenDogleg()*/ ) {
      ltrace(200) << "Setting Axis Hint: @" << DbU::getValueString(_axisHint) << endl;
      event->setAxisHint( _axisHint );
    }

  // It is possible that this code could be disabled.
  // There should be no need to move the axis of the segment to be inserted,
  // it will automatically slot into the empty track, if any.
    if (_type & MoveToAxis) {
      ltrace(200) << "Moving Axis To: @" << DbU::getValueString(_axisHint) << endl;
      _segment->setAxis( _axisHint );
    }

    if (_type & ToRipupLimit) {
      unsigned int limit = Session::getKiteEngine()->getRipupLimit(_segment);
      if (limit > data->getRipupCount())
        data->setRipupCount( limit );
    }

    unsigned int eventLevel = 0;
    if (_type & EventLevel1) eventLevel = 1;
    if (_type & EventLevel2) eventLevel = 2;
    if (_type & EventLevel3) eventLevel = 3;
    if (_type & EventLevel4) eventLevel = 4;
    if (_type & EventLevel5) eventLevel = 5;
    event->setRipedByLocal( _type&RipedByLocal );

    RoutingEvent* fork = event->reschedule( queue, eventLevel );

    if (fork) {
      unsigned int mode = RoutingEvent::Repair;
      if (RoutingEvent::getStage() < RoutingEvent::Repair)
        mode = (_type&PackingMode) ? RoutingEvent::Pack : RoutingEvent::Negociate;
      fork->setMode( mode );
    }

    DebugSession::close();
    return true;
  }


// -------------------------------------------------------------------
// Class  :  "SegmentFsm".
  

  SegmentFsm::SegmentFsm ( RoutingEvent* event, RoutingEventQueue& queue, RoutingEventHistory& history )
    : _event              (event)
    , _queue              (queue)
    , _history            (history)
    , _state              (0)
    , _data               (NULL)
    , _constraint         ()
    , _optimal            ()
    , _costs              ()
    , _actions            ()
    , _fullBlocked        (true)
  {
    TrackElement* segment = _event->getSegment();
    unsigned int  depth   = Session::getRoutingGauge()->getLayerDepth(segment->getLayer());
    _event->setTracksFree ( 0 );

    _data = segment->getDataNegociate();
    if ( !_data ) {
      _state = MissingData;
      return;
    }

    _data->update();
    _event->revalidate();

    _constraint = _event->getConstraints();
    _optimal    = _event->getOptimal();

    const Interval& perpandicular = _event->getPerpandicularFree ();

    ltrace(148) << "Katabatic intervals:"                  << endl;
    ltrace(148) << "* Optimal:        "  << _optimal       << endl;
    ltrace(148) << "* Constraints:    "  << _constraint    << endl;
    ltrace(148) << "* Perpandicular:  "  << perpandicular  << endl;
    ltrace(148) << "* AxisHint:       "  << DbU::getValueString(_event->getAxisHint()) << endl;

    if ( _event->getTracksNb() ) {
      if ( _constraint.getIntersection(perpandicular).isEmpty() ) {
        ltrace(200) << "Perpandicular free is too tight." << endl;
        _state = EmptyTrackList;
      } else
        _constraint.intersection ( perpandicular );
    } else {
      ltrace(200) << "No Track in perpandicular free." << endl;
      _state = EmptyTrackList;
    }

    if ( _state == EmptyTrackList ) return;

    ltrace(148) << "Negociate intervals:" << endl;
    ltrace(148) << "* Optimal:     "      << _optimal    << endl;
    ltrace(148) << "* Constraints: "      << _constraint << endl;
    ltracein(148);

    // if ( segment->isLocal() and (_data->getState() >= DataNegociate::MaximumSlack) )
    //   _constraint.inflate ( 0, DbU::lambda(1.0) );

    bool inLocalDepth    = (depth < 3);
    bool isOneLocalTrack = (segment->isLocal())
      and (segment->base()->getAutoSource()->getGCell()->getGlobalsCount(depth) >= 9.0);

    RoutingPlane* plane = Session::getKiteEngine()->getRoutingPlaneByLayer(segment->getLayer());
    forEach ( Track*, itrack, Tracks_Range::get(plane,_constraint)) {
      unsigned int costflags = 0;
      costflags |= (segment->isLocal() and (depth >= 3)) ? TrackCost::LocalAndTopDepth : 0;

      _costs.push_back ( itrack->getOverlapCost(segment,costflags) );
      _costs.back().setAxisWeight   ( _event->getAxisWeight(itrack->getAxis()) );
      _costs.back().incDeltaPerpand ( _data->getWiringDelta(itrack->getAxis()) );
      if (segment->isGlobal()) {
        ltrace(500) << "Deter| setForGlobal() on " << *itrack << endl;
        _costs.back().setForGlobal();
      }

      if ( inLocalDepth and (_costs.back().getDataState() == DataNegociate::MaximumSlack) )
        _costs.back().setInfinite ();

      if ( isOneLocalTrack
         and  _costs.back().isOverlapGlobal()
         and (_costs.back().getDataState() >= DataNegociate::ConflictSolveByHistory) )
        _costs.back().setInfinite ();

      _costs.back().consolidate ();
      if ( _fullBlocked and (not _costs.back().isBlockage() and not _costs.back().isFixed()) ) 
        _fullBlocked = false;

      ltrace(149) << "| " << _costs.back() << ((_fullBlocked)?" FB ": " -- ") << (*itrack) << endl;
    }
    ltraceout(148);

    if ( _costs.empty() ) {
      Track* nearest = plane->getTrackByPosition(_constraint.getCenter());

      if (  (nearest->getAxis() < _constraint.getVMin())
         or (nearest->getAxis() > _constraint.getVMax()) ) {
      //setUnimplemented ();
      //cerr << "[UNIMPLEMENTED] " << segment << " no Track in constraint interval "
      //     << _constraint << " " <<  "." << endl;
      } else {
        cerr << Bug(" %s Track_Range() failed to find Tracks in %s (they exists)."
                   ,getString(segment).c_str()
                   ,getString(_constraint).c_str()
                   ) << endl;
      }
      _state = EmptyTrackList;
    }

    unsigned int flags = 0;
    flags |= (segment->isStrap()) ? TrackCost::IgnoreAxisWeight : 0;
    flags |= (segment->isLocal()
             and (_data->getState() < DataNegociate::Minimize)
             and (_data->getRipupCount() < 5))
             ? TrackCost::DiscardGlobals : 0;
    flags |= (RoutingEvent::getStage() == RoutingEvent::Repair) ? TrackCost::IgnoreSharedLength : 0;

    if ( flags & TrackCost::DiscardGlobals ) {
      ltrace(200) << "TrackCost::Compare() - DiscardGlobals" << endl;
    }

    sort ( _costs.begin(), _costs.end(), TrackCost::Compare(flags) );

    size_t i=0;
    for ( ; (i<_costs.size()) and _costs[i].isFree() ; i++ );
    _event->setTracksFree ( i );
  }


  void  SegmentFsm::addAction ( TrackElement* segment
                         , unsigned int  type
                         , DbU::Unit     axisHint
                         , unsigned int  toSegmentFsm  )
  {
    if ( not segment->isFixed() ) {
      _actions.push_back ( SegmentAction(segment,type,axisHint,toSegmentFsm) );
      ltrace(200) << "SegmentFsm::addAction(): " << segment << endl;
    }
  }


  void  SegmentFsm::doActions ()
  {
    ltrace(200) << "SegmentFsm::doActions() - " << _actions.size() << endl;

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

      DebugSession::open ( _actions[i].getSegment()->getNet(), 200 );
      if ( not _actions[i].doAction(_queue) ) {
        cinfo << "[INFO] Failed action on " << _actions[i].getSegment() << endl;
      }
      DebugSession::close ();
    }

    _actions.clear ();
  }


  bool  SegmentFsm::insertInTrack ( size_t i )
  {
    ltrace(200) << "SegmentFsm::insertInTrack() istate:" << _event->getInsertState()
                << " track:" << i << endl;

    _event->incInsertState();
    switch ( _event->getInsertState() ) {
      case 1:
        if ( Manipulator(_event->getSegment(),*this).insertInTrack(i) ) return true;
        _event->incInsertState();
      case 2:
        if ( Manipulator(_event->getSegment(),*this).shrinkToTrack(i) ) return true;
        _event->incInsertState();
      case 3:
        if ( Manipulator(_event->getSegment(),*this).forceToTrack(i) ) return true;
        _event->incInsertState();
    }
    return false;
  }


  bool  SegmentFsm::conflictSolveByHistory ()
  {
    bool          success = false;
    RipupHistory  ripupHistory ( _event );
    RoutingEvent* event;
    TrackElement* segment = _event->getSegment();

    ltrace(200) << "SegmentFsm::conflictSolveByHistory()" << endl;

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
          ltrace(200) << breakPoint << endl;
        } else {
          breakPoint = Point( segment->getAxis(), minConflict );
          ltrace(200) << breakPoint << endl;
        }

        Katabatic::GCell* dogLegGCell = Session::getGCellUnder( breakPoint.getX(), breakPoint.getY() );
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
          ltrace(200) << breakPoint << endl;
        } else {
          breakPoint = Point( segment->getAxis(), maxConflict );
          ltrace(200) << breakPoint << endl;
        }

        Katabatic::GCell* dogLegGCell = Session::getGCellUnder( breakPoint.getX(), breakPoint.getY() );
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
      ltrace(200) << "No disloggers found @" << DbU::getValueString(segment->getAxis()) << endl;

      Interval freeSpan = Session::getKiteEngine()->
        getTrackByPosition(segment->getLayer(),segment->getAxis())->
        getFreeInterval(segment->getSourceU(),segment->getNet());

      if (freeSpan.contains(segment->getCanonicalInterval())) {
        ltrace(200) << "Disloggers vanished, Segment can be re-inserted." << endl;
        success = true;
      }
    }

    return success;
  }


  bool  SegmentFsm::conflictSolveByPlaceds ()
  {
    bool                  success    = false;
    Interval              constraints;
    vector<Cs1Candidate>  candidates;
    TrackElement*         segment    = _event->getSegment();
    bool                  canMoveUp  = (segment->isLocal()) ? segment->canPivotUp(0.5) : segment->canMoveUp(1.0); // MARK 1
    unsigned int          relaxFlags = Manipulator::NoDoglegReuse
                                     | ((_data and (_data->getStateCount() < 2)) ? Manipulator::AllowExpand
                                                                                 : Manipulator::NoExpand);

    ltrace(200) << "SegmentFsm::conflictSolveByPlaceds()" << endl;
    ltrace(200) << "| Candidates Tracks: " << endl;

    segment->base()->getConstraints( constraints );
    Interval      overlap    = segment->getCanonicalInterval();
    RoutingPlane* plane      = Session::getKiteEngine()->getRoutingPlaneByLayer(segment->getLayer());
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

      size_t        begin;
      size_t        end;
      TrackElement* other;
      Net*          otherNet      = NULL;
      Interval      otherOverlap;
      bool          otherIsGlobal = false;

      track->getOverlapBounds( overlap, begin, end );
      candidates.back().setBegin( begin );
      candidates.back().setEnd  ( end );

      ltrace(200) << "* " << track << " [" << begin << ":" << end << "]" << endl;

      for ( ; (begin < end) ; ++begin ) {
        other = track->getSegment( begin );

        if (other->getNet() == segment->getNet()) {
          ltrace(200) << "  | " << begin << " Same net: " << " " << other << endl;
          continue;
        }
        if (not other->getCanonicalInterval().intersect(overlap)) {
          ltrace(200) << "  | " << begin << " No Conflict: " << " " << other << endl;
          if (otherNet == NULL) candidates.back().setBegin( begin+1 );
          continue;
        }
        ltrace(200) << "  | " << begin << " Conflict: " << " " << other << endl;

        if (otherNet != other->getNet()) {
          if (otherNet) {
            if (otherIsGlobal) {
              candidates.back().addConflict( otherOverlap );
              ltrace(200) << "  | Other overlap G: " << otherOverlap << endl;
            } else {
              ltrace(200) << "  | Other overlap L: " << otherOverlap << " ignored." << endl;
            }
          }
          otherNet      = other->getNet();
          otherOverlap  = other->getCanonicalInterval();
          otherIsGlobal = other->isGlobal() or other->isBlockage() or other->isFixed();
        } else {
          otherOverlap.merge(other->getCanonicalInterval());
          otherIsGlobal = otherIsGlobal or other->isGlobal() or other->isBlockage() or other->isFixed();
        }
      }
      if (not otherOverlap.isEmpty()) {
        if (otherIsGlobal) {
          candidates.back().addConflict( otherOverlap );
          ltrace(200) << "  | Other overlap G: " << otherOverlap << endl;
        } else {
          ltrace(200) << "  | Other overlap L: " << otherOverlap << " ignored." << endl;
        }
      }

      candidates.back().consolidate();
    }

    sort( candidates.begin(), candidates.end() );

    for ( size_t icandidate=0 ; icandidate<candidates.size() ; ++icandidate ) {
      ltrace(200) << "Trying l:" << candidates[icandidate].getLength()
                  << " " << candidates[icandidate].getTrack() << endl;

      Interval overlap0 = candidates[icandidate].getLongestConflict();
      ltrace(200) << "overlap0: " << overlap0 << endl;

      if (overlap0.isEmpty()) {
        ltrace(200) << "overlap0 is empty, no conflict, ignoring Track candidate." << endl;
        continue;
      }

      Track*        track = candidates[icandidate].getTrack();
      TrackElement* other = track->getSegment( overlap.getCenter() );
      if (not other) {
        cbug << Error("conflictSolveByPlaceds(): No segment under overlap center.") << endl;
        continue;
      }

      if (other->isGlobal()) {
        ltrace(200) << "conflictSolveByPlaceds() - Conflict with global, other move up" << endl;
        if ((success = Manipulator(other,*this).moveUp())) break;
      }

      ltrace(200) << "conflictSolveByPlaceds() - Relaxing self" << endl;

      if (Manipulator(segment,*this).relax(overlap0,relaxFlags)) {
        success = true;
        break;
      } else {
        if ( not canMoveUp
           and (relaxFlags != Manipulator::NoExpand)
           and Manipulator(segment,*this).relax(overlap0,Manipulator::NoExpand|Manipulator::NoDoglegReuse) ) {
          ltrace(200) << "Cannot move up but successful narrow breaking." << endl;
          success = true;
          break;
        }
      }
    }

    if ( not success and segment->isGlobal() and (_costs.size() <= 1) ) {
      ltrace(200) << "Overconstrained perpandiculars, rip them up. On track:" << endl;
      ltrace(200) << "  " << track << endl;
      Manipulator(segment,*this).ripupPerpandiculars ();
      success = true;
    }

    return success;
  }


  bool  SegmentFsm::solveTerminalVsGlobal ()
  {
    TrackElement* segment = getEvent()->getSegment();
    ltrace(200) << "SegmentFsm::solveTerminalVsGlobal: " << " " << segment << endl;

    if (not (segment->isTerminal() and segment->isLocal())) return false;

    for ( size_t icost=0 ; icost<_costs.size() ; ++icost ) {
      Interval overlap = segment->getCanonicalInterval();
      size_t   begin;
      size_t   end;
      getCost(icost).getTrack()->getOverlapBounds( overlap, begin, end );

      for ( ; begin<end ; ++begin ) {
        TrackElement* other        = getCost(icost).getTrack()->getSegment(begin);
        Interval      otherOverlap = other->getCanonicalInterval();
        
        if (other->getNet() == segment->getNet()) continue;
        if (not other->isGlobal()) continue;
        if (not otherOverlap.contains(overlap)) continue;

        ltrace(200) << "| Global candidate:" << other << endl;
        if (Manipulator(other,*this).moveUp(Manipulator::AllowTerminalMoveUp)) {
          ltrace(200) << "| Global candidate selected." << endl;
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

    ltrace(200) << "SegmentFsm::solveFullBlockages: " << " " << segment << endl;
    ltracein(200);

    if ( segment->isLocal() ) {
      success = Manipulator(segment,*this).pivotUp();
      if ( not success ) {
        ltrace(200) << "Tightly constrained local segment overlapping a blockage, move up." << endl;
        ltrace(200) << segment << endl;
        success = Manipulator(segment,*this).moveUp
          (Manipulator::AllowLocalMoveUp|Manipulator::AllowTerminalMoveUp);
      }
    } else {
      Interval overlap = segment->getCanonicalInterval();
      size_t   begin;
      size_t   end;

      getCost(0).getTrack()->getOverlapBounds ( overlap, begin, end );
      for ( ; begin<end ; ++begin ) {
        TrackElement* other        = getCost(0).getTrack()->getSegment(begin);
        Interval      otherOverlap = other->getCanonicalInterval();
        
        if ( other->getNet() == segment->getNet() ) continue;
        if ( not otherOverlap.intersect(overlap) ) continue;
        
        ltrace(200) << "| " << begin << " Blockage conflict: " << " " << other << endl;
        if ( (success = Manipulator(segment,*this).relax
             (otherOverlap,Manipulator::NoDoglegReuse|Manipulator::NoExpand)) ) {
          break;
          }
      }
    }
    if ( not success ) {
      cparanoid << Error( "Tighly constrained segment overlapping a blockage:\n        %s"
                        , getString(segment).c_str() ) << endl;
      ltrace(200) << "Segment is hard blocked, bypass to Unimplemented." << endl;
    }
    
    ltraceout(200);
    return success;
  }


  bool  SegmentFsm::desaturate ()
  {
    ltrace(200) << "SegmentFsm::desaturate()" << endl;
    ltracein(200);

    size_t        itrack  = 0;

#if THIS_IS_DISABLED
    TrackElement* segment = _event->getSegment();
    for ( ; itrack<getCosts().size() ; ++itrack ) {
      ltrace(200) << "Trying track:" << itrack << endl;

      if ( getCost(itrack).isGlobalEnclosed() ) {
        Track*    track      = getTrack(itrack);
        size_t    begin      = getBegin(itrack);
        size_t    end        = getEnd  (itrack);
        Net*      ownerNet   = segment->getNet();
        Interval  toFree      (segment->getCanonicalInterval());
        bool      success    = true;
      
        for ( size_t i = begin ; success and (i < end) ; i++ ) {
          TrackElement* segment2 = track->getSegment(i);

          ltrace(200) << "* Looking // " << segment2 << endl;

          if ( segment2->getNet() == ownerNet  ) continue;
          if ( not toFree.intersect(segment2->getCanonicalInterval()) ) continue;
          if ( segment2->isFixed() or not segment2->isBipoint() ) {
            success = false;
            continue;
          }

          DataNegociate* data2 = segment2->getDataNegociate();
          if ( not data2 ) {
            ltrace(200) << "No DataNegociate, ignoring." << endl;
            success = false;
            continue;
          }

          ltrace(200) << "- Forced moveUp " << segment2 << endl;
          if ( not (success=Manipulator(segment2,*this).moveUp(Manipulator::AllowTerminalMoveUp)) ) {
            continue;
          }
        }

        if ( success ) {
          setState ( SegmentFsm::OtherRipup );
          addAction( segment
                   , SegmentAction::SelfInsert|SegmentAction::MoveToAxis
                   , getCost(itrack).getTrack()->getAxis()
                   );
          break;
        }
      }
    }

#endif
    ltraceout(200);
    return (itrack < _costs.size());
  }



  bool  SegmentFsm::_slackenStrap ( TrackElement*& segment, DataNegociate*& data, unsigned int flags )
  {
    ltrace(200) << "Strap segment Fsm." << endl;

    bool          success   = false;
    unsigned int  nextState = data->getState();

    switch ( data->getState() ) {
      case DataNegociate::RipupPerpandiculars:
        nextState = DataNegociate::Minimize;
        success = Manipulator(segment,*this).ripupPerpandiculars();
        if (success) break;
      case DataNegociate::Minimize:
        if (data->getStateCount() >= 2) {
          nextState = DataNegociate::MaximumSlack;
        }
        success = Manipulator(segment,*this).minimize();
        if (success) break;
      case DataNegociate::Dogleg:
      case DataNegociate::Slacken:
      case DataNegociate::ConflictSolveByHistory:
      case DataNegociate::ConflictSolveByPlaceds:
      case DataNegociate::MoveUp:
      case DataNegociate::MaximumSlack:
      case DataNegociate::Unimplemented:
        nextState = DataNegociate::Unimplemented;
        break;
    }
    
    if (not success and (nextState != DataNegociate::Unimplemented))
      success = Manipulator(segment,*this).ripupPerpandiculars(Manipulator::ToRipupLimit);

    if (not (flags&NoTransition)) {
      data->setState( nextState );
      ltrace(200) << "Incrementing state (after): " << nextState << " count:" << data->getStateCount() << endl;
    }

    return success;
  }


  bool  SegmentFsm::_slackenLocal ( TrackElement*& segment, DataNegociate*& data, unsigned int flags )
  {
    ltrace(200) << "Local segment Fsm." << endl;

    bool          success   = false;
    unsigned int  nextState = data->getState();

    switch (data->getState()) {
      case DataNegociate::RipupPerpandiculars:
        nextState = DataNegociate::Minimize;
        success = Manipulator(segment,*this).ripupPerpandiculars();
        if (success) break;
      case DataNegociate::Minimize:
        if (isFullBlocked() and not segment->isTerminal()) {
          ltrace(200) << "Is Fully blocked." << endl;
          nextState = DataNegociate::Unimplemented;
          break;
        }
        nextState = DataNegociate::Dogleg;
        success = Manipulator(segment,*this).minimize();
        if (success) break;
      case DataNegociate::Dogleg:
        nextState = DataNegociate::Slacken;
        success = Manipulator(segment,*this).makeDogleg();
        if (success) break;
      case DataNegociate::Slacken:
        nextState = DataNegociate::ConflictSolveByPlaceds;
        success = Manipulator(segment,*this).slacken();
        if (success) break;
      case DataNegociate::ConflictSolveByHistory:
      case DataNegociate::ConflictSolveByPlaceds:
        nextState = DataNegociate::LocalVsGlobal;
        success = conflictSolveByHistory();
        break;
      case DataNegociate::LocalVsGlobal:
        nextState = DataNegociate::MoveUp;
        success = solveTerminalVsGlobal();
        if (success) break;
        break;
      case DataNegociate::MoveUp:
        nextState = DataNegociate::MaximumSlack;
        success = Manipulator(segment,*this).moveUp();
        if (success) break;
      case DataNegociate::MaximumSlack:
        if (segment->isStrap()) { 
          if ( (nextState < DataNegociate::MaximumSlack) or (data->getStateCount() < 2) ) {
            nextState = DataNegociate::MaximumSlack;
            success = conflictSolveByPlaceds();
            if (success) break;
          }
        }
      case DataNegociate::Unimplemented:
        nextState = DataNegociate::Unimplemented;
        break;
    }

    if (not success and (nextState != DataNegociate::Unimplemented)) {
      if (data->getStateCount() < 6)
        success = Manipulator(segment,*this).ripupPerpandiculars(Manipulator::ToRipupLimit);
    }

    if (not success
       and (nextState == DataNegociate::Unimplemented)
       and segment->isSlackened()
       and isFullBlocked()) {
      if (solveFullBlockages()) nextState = DataNegociate::MoveUp;
    }

    if (not (flags&NoTransition)) {
      data->setState( nextState );
      ltrace(200) << "Incrementing state (after): " << nextState << " count:" << data->getStateCount() << endl;
    }

    return success;
  }


  bool  SegmentFsm::_slackenGlobal ( TrackElement*& segment, DataNegociate*& data, unsigned int flags )
  {
    bool          success   = false;
    unsigned int  nextState = data->getState();

    switch ( data->getState() ) {
      case DataNegociate::RipupPerpandiculars:
      case DataNegociate::Minimize:
      case DataNegociate::Dogleg:
        ltrace(200) << "Global, SegmentFsm: RipupPerpandiculars." << endl;
        nextState = DataNegociate::Slacken;
        break;
      case DataNegociate::Slacken:
        ltrace(200) << "Global, SegmentFsm: Slacken."  << endl;
        if ((success = Manipulator(segment,*this).slacken(KbHalfSlacken))) {
          nextState = DataNegociate::RipupPerpandiculars;
          break;
        }
      case DataNegociate::MoveUp:
        ltrace(200) << "Global, SegmentFsm: MoveUp." << endl;
        if ((success = Manipulator(segment,*this).moveUp(Manipulator::AllowShortPivotUp))) {
          break;
        }
        nextState = DataNegociate::ConflictSolveByHistory;
        break;
      case DataNegociate::ConflictSolveByHistory:
      case DataNegociate::ConflictSolveByPlaceds:
        ltrace(200) << "Global, SegmentFsm: ConflictSolveByHistory or ConflictSolveByPlaceds." << endl;
        if ((success = conflictSolveByPlaceds())) {
          if (segment->canMoveUp(1.0))
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
        if ((success=Manipulator(segment,*this).forceOverLocals())) {
          break;
        }
      case DataNegociate::Unimplemented:
        ltrace(200) << "Global, SegmentFsm: MaximumSlack or Unimplemented." << endl;
        nextState = DataNegociate::Unimplemented;
        break;
    }

    if (not success and (nextState != DataNegociate::Unimplemented)) {
      if (data->getStateCount() < 6)
        success = Manipulator(segment,*this).ripupPerpandiculars(Manipulator::ToRipupLimit);
    }

  // Special case: all tracks are overlaping a blockage.
    if (not success
       and (nextState == DataNegociate::Unimplemented)
       and segment->isSlackened() ) {
      if (solveFullBlockages()) nextState = DataNegociate::MoveUp;
    }

    if (not (flags&NoTransition)) {
      if (data->getChildSegment()) {
        TrackElement* child = segment;
        ltrace(200) << "Incrementing state of childs (after): " << endl;
        while ( child ) {
          ltrace(200) << "| " << child << endl;
          if (child->base()->isGlobal()) {
            child->getDataNegociate()->setState( nextState );
            ltrace(200) << "| Update:" << nextState << " count:" << child->getDataNegociate()->getStateCount() << endl;
          }
          TrackElement* parent = child;
          child = parent->getDataNegociate()->getChildSegment();
          parent->getDataNegociate()->setChildSegment( NULL );
        }
      } else {
        data->setState( nextState );
        ltrace(200) << "Incrementing state (after): " << segment << endl;
        ltrace(200) << "| " << nextState << " count:" << data->getStateCount() << endl;
      }
    }

    return success;
  }


  bool  SegmentFsm::slackenTopology ( unsigned int flags )
  {
    bool           success     = false;
    TrackElement*  segment     = getEvent()->getSegment();
    DataNegociate* data        = segment->getDataNegociate ();
    unsigned int   actionFlags = SegmentAction::SelfInsert|SegmentAction::EventLevel5;

    DebugSession::open( segment->getNet(), 200 );
    ltrace(200) << "Slacken Topology for " << segment->getNet()
                << " " << segment << endl;
    ltracein(200);

    if (not segment or not data) { ltraceout(200); DebugSession::close(); return false; }

    _event->resetInsertState();
    data->resetRipupCount();

    if      (segment->isStrap()) { success = _slackenStrap ( segment, data, flags ); }
    else if (segment->isLocal()) { success = _slackenLocal ( segment, data, flags ); }
    else                         { success = _slackenGlobal( segment, data, flags ); }

    if (success) {
      actionFlags |= SegmentAction::ResetRipup;
      addAction( segment, actionFlags );
    } else {
      clearActions();
      if (data->getState() == DataNegociate::Unimplemented) {
        cinfo << "[UNSOLVED] " << segment << " unable to slacken topology." << endl;
      }
    }

    ltraceout(200);
    DebugSession::close();

    return success;
  }


}  // Kite namespace.
