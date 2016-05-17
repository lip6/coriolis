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
// |  C++ Module  :       "./RoutingEvent.cpp"                       |
// +-----------------------------------------------------------------+


#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "vlsisapd/configuration/Configuration.h"
#include "hurricane/Bug.h"
#include "hurricane/DebugSession.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/Net.h"
#include "hurricane/Layer.h"
#include "katabatic/AutoContact.h"
#include "kite/DataNegociate.h"
#include "kite/TrackSegment.h"
#include "kite/Track.h"
#include "kite/Tracks.h"
#include "kite/RoutingPlane.h"
#include "kite/RoutingEvent.h"
#include "kite/RoutingEventHistory.h"
#include "kite/RoutingEventQueue.h"
#include "kite/RoutingEventLoop.h"
#include "kite/NegociateWindow.h"
#include "kite/Session.h"
#include "kite/KiteEngine.h"
#include "kite/Manipulator.h"
#include "kite/SegmentFsm.h"


namespace Kite {

  using std::cerr;
  using std::endl;
  using std::setw;
  using std::min;
  using std::ostringstream;
  using Hurricane::tab;
  using Hurricane::DebugSession;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::ForEachIterator;
  using Hurricane::Net;
  using Hurricane::Layer;
  using Katabatic::GCell;
  using Katabatic::KbPropagate;


// -------------------------------------------------------------------
// Class  :  "RoutingEvent::Compare".


  bool  RoutingEvent::Compare::operator() ( const RoutingEvent* lhs, const RoutingEvent* rhs ) const
  {
    if (lhs == rhs) return false;
    return RoutingEvent::Key::Compare()( lhs->getKey(), rhs->getKey() );
  }


// -------------------------------------------------------------------
// Class  :  "RoutingEvent::Key".


  bool  RoutingEvent::Key::Compare::operator() ( const RoutingEvent::Key& lhs, const RoutingEvent::Key& rhs ) const
  {
    if (lhs._eventLevel > rhs._eventLevel) return false;
    if (lhs._eventLevel < rhs._eventLevel) return true;

  // Process all M2 (terminal access) before any others.
    if ((lhs._layerDepth == 1) and (rhs._layerDepth != 1)) return false;
    if ((lhs._layerDepth != 1) and (rhs._layerDepth == 1)) return true;

    if (lhs._priority > rhs._priority) return false;
    if (lhs._priority < rhs._priority) return true;

    if (lhs._length > rhs._length) return false;
    if (lhs._length < rhs._length) return true;

    if ((lhs._segFlags & KbHorizontal) xor (rhs._segFlags & KbHorizontal))
      return (rhs._segFlags & KbHorizontal);

    if (lhs._axis > rhs._axis) return true;
    if (lhs._axis < rhs._axis) return false;

    if (lhs._sourceU > rhs._sourceU) return true;
    if (lhs._sourceU < rhs._sourceU) return false;

    if (lhs._net->getName() != rhs._net->getName())
      return lhs._net->getName() < rhs._net->getName();

    return lhs._id < rhs._id;
  }


  RoutingEvent::Key::Key ( const RoutingEvent* event )
    : _tracksNb    (event->getTracksNb())
    , _priority    (event->getPriority())
    , _eventLevel  (event->getEventLevel())
    , _segFlags    (event->getSegment()->base()->getFlags())
    , _layerDepth  (Session::getRoutingGauge()->getLayerDepth(event->getSegment()->getLayer()))
    , _length      (event->getSegment()->getLength())
    , _axis        (event->getSegment()->getAxis())
    , _sourceU     (event->getSegment()->getSourceU())
    , _net         (event->getSegment()->getNet())
    , _id          (event->getSegment()->getId())
  { }


  void  RoutingEvent::Key::update ( const RoutingEvent* event )
  {
    if (not event) return;

    _tracksNb     = event->getTracksNb();
    _priority     = event->getPriority();
    _eventLevel   = event->getEventLevel();
    _segFlags     = event->getSegment()->base()->getFlags();
    _layerDepth   = Session::getRoutingGauge()->getLayerDepth(event->getSegment()->getLayer());
    _length       = event->getSegment()->getLength();
    _axis         = event->getSegment()->getAxis();
    _sourceU      = event->getSegment()->getSourceU();
    _id           = event->getSegment()->getId();
  }


// -------------------------------------------------------------------
// Class  :  "RoutingEvent".


  unsigned int  RoutingEvent::_idCounter  = 0;
  unsigned int  RoutingEvent::_stage      = RoutingEvent::Negociate;
  size_t        RoutingEvent::_allocateds = 0;
  size_t        RoutingEvent::_processeds = 0;
  size_t        RoutingEvent::_cloneds    = 0;


  unsigned int  RoutingEvent::getStage        () { return _stage; }
  size_t        RoutingEvent::getAllocateds   () { return _allocateds; }
  size_t        RoutingEvent::getProcesseds   () { return _processeds; }
  size_t        RoutingEvent::getCloneds      () { return _cloneds; }
  void          RoutingEvent::setStage        ( unsigned int stage ) { _stage = stage; }
  void          RoutingEvent::resetProcesseds () { _processeds = 0; }


  RoutingEvent::RoutingEvent ( TrackElement* segment, unsigned int mode )
    : _cloned              (false)
    , _processed           (false)
    , _disabled            (false)
    , _overConstrained     (false)
    , _minimized           (false)
    , _forceToHint         (false)
    , _ripedByLocal        (false)
    , _id                  (_idCounter++)
    , _segment             (segment)
    , _dataNegociate       (segment->getDataNegociate())
    , _axisHistory         (segment->getAxis())
    , _axisHint            (segment->getAxis())
    , _constraints         ()
    , _optimal             ()
    , _tracksNb            (0)
    , _tracksFree          (0)
    , _insertState         (0)
    , _mode                (mode)
    , _rippleState         (0)
    , _eventLevel          (0)
    , _priority            (0.0)
    , _key                 (this)
  {
    if (_idCounter == std::numeric_limits<unsigned int>::max()) {
      throw Error( "RoutingEvent::RoutingEvent(): Identifier counter has reached it's limit (%d bits)."
                 , std::numeric_limits<unsigned int>::digits );
    }

    DataNegociate* data = _segment->getDataNegociate();
    if (data) data->setRoutingEvent( this );

    cdebug.log(159) << "create: " << this << endl;
    cdebug.log(159) << "Initial setAxisHint @" << DbU::getValueString(getAxisHint()) << endl;

    if (_segment->getTrack()) {
      cerr << Bug( "RoutingEvent::create() - TrackElement is already inserted in a Track."
                   "\n      %s.", getString(_segment).c_str()
                 ) << endl;
    }
  }


  RoutingEvent* RoutingEvent::create ( TrackElement* segment, unsigned int mode )
  {
    // if (not dynamic_cast<TrackSegment*>(segment)) {
    //   cerr << Error( "RoutingEvent::create() Can only create event from TrackSegment:\n"
    //                  "        %s", getString(segment).c_str()
    //                ) << endl;
    // }

    RoutingEvent* event = new RoutingEvent ( segment, mode );
    ++_allocateds;

    return event;
  }


  RoutingEvent* RoutingEvent::clone () const
  {
    _cloned = true;

    RoutingEvent* clone = new RoutingEvent ( *this );
    ++_allocateds;
    ++_cloneds;

    clone->_cloned     = false;
    clone->_disabled   = false;
    clone->_eventLevel = 0;

    cdebug.log(159) << "RoutingEvent::clone() " << clone
                << " (from: " << ")" <<  endl;

    return clone;
  }


  RoutingEvent::~RoutingEvent ()
  {
    cdebug.log(159) << "~RoutingEvent() " << endl;

    DataNegociate* data = _segment->getDataNegociate();
    if ( data and (data->getRoutingEvent() == this) )
      data->setRoutingEvent( NULL );
  }


  void  RoutingEvent::destroy ()
  {
    cdebug.log(159) << "RoutingEvent::destroy() " << this << endl;
    if (_allocateds > 0) --_allocateds;

    delete this;
  }


  bool  RoutingEvent::isUnimplemented () const
  { return getState() == DataNegociate::Unimplemented; }


  void  RoutingEvent::setMode ( unsigned int mode )
  { _mode = mode; }


  unsigned int  RoutingEvent::getState () const
  {
    DataNegociate* data = _segment->getDataNegociate();
    return (data) ? data->getState() : 0;
  }


  void  RoutingEvent::setState ( unsigned int state )
  {
    DataNegociate* data = _segment->getDataNegociate();
    if (data) data->setState( state );
  }


  void  RoutingEvent::setAxisHint ( DbU::Unit axis )
  {
    cdebug.log(159) << "setAxisHint @" << DbU::getValueString(axis) << " " << _segment << endl;
    _axisHint = axis;
  }


  void  RoutingEvent::setAxisHintFromParent ()
  {
    if (getStage() == Repair) return;

    TrackElement* parent = _segment->getParent();
    if (not parent) return;

    RoutingEvent* parentEvent = parent->getDataNegociate()->getRoutingEvent();
    if (parentEvent == this) {
      cbug << Error( "RoutingEvent::setAxisHintFromParent(): Parentage loop between\n"
                     "        this  :%p:%s\n        parent:%p:%s"
                   , _segment->base(),getString(_segment->base()).c_str()
                   , parent  ->base(),getString(parent  ->base()).c_str()
                   ) << endl;
      _axisHint = parent->getAxis();
      return;
    }
    _axisHint = parent->getAxis();

    cdebug.log(159) << "setAxisHintFromParent() - hint:" << DbU::getValueString(_axisHint)
                << " axis:" << DbU::getValueString(parent->getAxis()) << " parent:" << parent << endl;
    return;
  }


  RoutingEvent* RoutingEvent::reschedule ( RoutingEventQueue& queue, unsigned int eventLevel )
  {
    RoutingEvent* active = _segment->getDataNegociate()->getRoutingEvent();
    if (active != this) return active->reschedule( queue, eventLevel );

    RoutingEvent* fork = NULL;

    if ( (getStage() != Repair) and isUnimplemented() ) {
      cdebug.log(159) << "Reschedule: cancelled (Unimplemented) "
                  << " -> " << fork << endl;
      return NULL;
    }

    if (not isProcessed()) {
      fork = this;
      cdebug.log(159) << "Reschedule/Self: "
                  << " -> "
                  << eventLevel << ":" << fork << endl;
    } else {
      fork = clone();
      fork->_processed  = false;

      _segment->getDataNegociate()->setRoutingEvent( fork );

      cdebug.log(159) << "Reschedule/Fork: "
                  << " -> " << fork << endl;
    }

    if (fork->_eventLevel < eventLevel)
      fork->_eventLevel = eventLevel;

    if (getStage() == Repair) {
      fork->setMode( RoutingEvent::Repair );
      _segment->getDataNegociate()->setState( DataNegociate::Repair );
    } else if (getStage() == RoutingEvent::Pack) {
      fork->setMode( RoutingEvent::Pack );
    }

    queue.repush( fork );

    return fork;
  }


  void  RoutingEvent::setSegment ( TrackElement* segment )
  {
    if (_segment) _segment->getDataNegociate()->setRoutingEvent( NULL );
    _segment = segment;
    _segment->getDataNegociate()->setRoutingEvent( this );
  }


  void  RoutingEvent::process ( RoutingEventQueue&   queue
                              , RoutingEventHistory& history
                              , RoutingEventLoop&    loop
                              )
  {
    loop.update( _segment->getId() );
    if (loop.isLooping()) {
        loop.erase( _segment->getId() );
        setState( DataNegociate::Unimplemented );

#if LOOP_DEBUG
      if (loop.getMaxCount() > 500) {
        cbug << Error("Loop detected, removing event %s.",_getString().c_str()) << endl;

        ostringstream message;
        message << "Kite has detected a loop between the following Segments:\n<tt>";

        const vector<RoutingEventLoop::Element>& elements = loop.getElements();
        for ( size_t i=0 ; i<elements.size() ; ++i ) {
          message << setw(2) << elements[i]._count << "| id:"
                  << elements[i]._id << "\n";
        }
        message << "</tt>";

        throw Error( message.str().c_str() );
      }
#else
      ostringstream message;
      message << "[BUG] Kite has detected a loop between the following Segments:";

      const vector<RoutingEventLoop::Element>& elements = loop.getElements();
      for ( size_t i=0 ; i<elements.size() ; ++i ) {
        message << "\n" << setw(10) << elements[i]._count << "| id:" << elements[i]._id;
      }
      cbug << message.str() << endl;
#endif
    }

    DebugSession::open( _segment->getNet(), 150, 160 );

    cdebug.log(9000) << "Deter| Event "
                     <<         getProcesseds()
                     << ","  << getEventLevel()
                     << ","  << setw(6) << getPriority()
                     << ": " << _segment << endl;
    _processeds++;

    cdebug.tabw(159,1);
    cdebug.log(159) << "State: *before* "
                    << DataNegociate::getStateString(_segment->getDataNegociate())
                    << " ripup:" << _segment->getDataNegociate()->getRipupCount()
                    << endl;
    cdebug.log(159) << "Level: " << getEventLevel()
                    << ", area: " << _segment->getFreedomDegree() << endl;

  //_preCheck( _segment );
    _eventLevel = 0;

    if (_mode != Pack) history.push( this );

    if ( isProcessed() or isDisabled() ) {
      cdebug.log(159) << "Already processed or disabled." << endl;
    } else {
      setProcessed();
      setTimeStamp( _processeds );

      switch ( _mode ) {
        case Negociate: _processNegociate( queue, history ); break;
        case Pack:      _processPack     ( queue, history ); break;
        case Repair:    _processRepair   ( queue, history ); break;
        default:
          cerr << Bug( "RoutingEvent::process() - Unknown mode value:%d.", _mode ) << endl;
          break;
      }
    }
    cdebug.tabw(159,-1);

    queue.repushInvalidateds();
    Session::revalidate();
    queue.commit();

  //_postCheck( _segment );
#if defined(CHECK_DATABASE)
    Session::getKiteEngine()->_check( _segment->getNet() );
#endif
    DebugSession::close();

    if (Session::getKiteEngine()->getPostEventCb() != NULL)
      Session::getKiteEngine()->getPostEventCb()();
  }


  void  RoutingEvent::_processNegociate ( RoutingEventQueue& queue, RoutingEventHistory& history )
  {
    cdebug.log(159) << "* Mode:Negociation." << endl;

    SegmentFsm fsm ( this, queue, history );

    if (fsm.getState() == SegmentFsm::MissingData) {
      cbug << Error("RoutingEvent::process() - Missing datas.") << endl;
      return;
    }

    cdebug.tabw(159,1);

    fsm.getData()->incRipupCount();

    size_t itrack = 0;
    for ( itrack = 0 ; itrack < fsm.getCosts().size() ; itrack++ )
      cdebug.log(159) << "| " << fsm.getCost(itrack) << endl;

    itrack = 0;
    if ( (not isOverConstrained()) and Manipulator(_segment,fsm).canRipup() ) {
      if (fsm.getCosts().size() and fsm.getCost(itrack).isFree()) {
        cdebug.log(159) << "Insert in free space " << this << endl;
        resetInsertState();

        _axisHistory = _segment->getAxis();
        _eventLevel  = 0;
        cdebug.log(9000) << "Deter| addInsertEvent() @" << fsm.getCost(itrack).getTrack() << endl;
        if (not _segment->isReduced())
          Session::addInsertEvent( _segment, fsm.getCost(itrack).getTrack() );
        fsm.setState( SegmentFsm::SelfInserted );
      } else {
      // Do ripup.
        if (fsm.getState() == SegmentFsm::EmptyTrackList) {
          Manipulator(_segment,fsm).ripupPerpandiculars();
        } else {
          if (Manipulator(_segment,fsm).canRipup(Manipulator::NotOnLastRipup)) {
            if (cdebug.enabled(9000)) {
              for ( itrack=0 ; itrack<fsm.getCosts().size() ; itrack++ ) {
                cdebug.log(9000) << "Deter| | Candidate Track: " << fsm.getCost(itrack) << endl;
              }
            }
            for ( itrack=0 ; itrack<fsm.getCosts().size() ; itrack++ ) {
              cdebug.log(159) << "Trying Track: " << itrack << endl;
              if (fsm.getCost(itrack).isInfinite()) break;
              if (fsm.insertInTrack(itrack)) break;
              resetInsertState();
            } // Next ripup is possible.
          }

          if (fsm.getState() != SegmentFsm::OtherRipup) {
            fsm.slackenTopology();
          }
        }
      }
    } else {
    // Ripup limit has been reached.
      if (isOverConstrained()) {
        cdebug.log(159) << "Immediate slackening due to overconstraint" << endl;
        fsm.getData()->setState( DataNegociate::Slacken );
      }
      if (not fsm.slackenTopology()) {
        fsm.setState( SegmentFsm::SelfMaximumSlack );
      }
    }

    fsm.doActions();

    if (itrack < fsm.getCosts().size()) {
      cdebug.log(159) << "Placed: @" << DbU::getValueString(fsm.getCost(itrack).getTrack()->getAxis())
                  << " " << this << endl;
    }

    cdebug.tabw(159,-1);
  }


  void  RoutingEvent::_processPack ( RoutingEventQueue& queue, RoutingEventHistory& history )
  {
    cdebug.log(159) << "* Mode:Pack." << endl;

    if (not _segment->isUTurn()) return;

    SegmentFsm fsm ( this, queue, history );
    if (fsm.getState() == SegmentFsm::MissingData   ) return;
    if (fsm.getState() == SegmentFsm::EmptyTrackList) return;

    cdebug.tabw(159,1);
    for ( size_t i = 0 ; i < fsm.getCosts().size() ; i++ )
      cdebug.log(159) << "| " << fsm.getCost(i) << endl;
    cdebug.tabw(159,-1);

    if (    _segment->getTrack()
       and  fsm.getCosts().size()
       and  fsm.getCost(0).isFree()
       and (fsm.getCost(0).getTrack() != _segment->getTrack()) ) {

      cerr << "_processPack(): move to " << fsm.getCost(0).getTrack() << endl;
      Session::addMoveEvent( _segment, fsm.getCost(0).getTrack() );
      fsm.setState( SegmentFsm::SelfInserted );
    }
  }


  void  RoutingEvent::_processRepair ( RoutingEventQueue& queue, RoutingEventHistory& history )
  {
    cdebug.log(159) << "* Mode:Repair." << endl;

    if ( _segment->getTrack() != NULL ) {
      cdebug.log(159) << "* Cancel: already in Track." << endl;
      return;
    }

    SegmentFsm fsm ( this, queue, history );
    if (fsm.getState() == SegmentFsm::MissingData   ) return;
    if (fsm.getState() == SegmentFsm::EmptyTrackList) return;

    cdebug.tabw(159,1);
    for ( size_t i = 0 ; i < fsm.getCosts().size() ; i++ )
      cdebug.log(159) << "| " << fsm.getCost(i) << endl;
    cdebug.tabw(159,-1);

    if (fsm.getCosts().size() and fsm.getCost(0).isFree()) {
      cdebug.log(159) << "Insert in free space." << endl;
      Session::addInsertEvent( _segment, fsm.getCost(0).getTrack() );
      fsm.setState( SegmentFsm::SelfInserted );
    } else {
      switch ( fsm.getData()->getStateCount() ) {
        case 1:
        // First try: minimize.
          Manipulator(_segment,fsm).minimize ();
          fsm.addAction( _segment, SegmentAction::SelfInsert );
          fsm.doActions();
          queue.commit();
          break;
        case 2:
        // Second try: failed re-inserted first.
          Manipulator(_segment,fsm).repackPerpandiculars ();
          fsm.addAction( _segment, SegmentAction::SelfInsert );
          fsm.doActions();
          queue.commit();
          break;
        default:
          cdebug.log(159) << "Repair failed." << endl;
          break;
      }
    }
  }


  void  RoutingEvent::revalidate ()
  {
    DebugSession::open( _segment->getNet(), 150, 160 );

    cdebug.log(159,1) << "RoutingEvent::revalidate() - " << this << endl;

  //_dataNegociate->update();

    setAxisHintFromParent();
    cdebug.log(159) << "axisHint:" << DbU::getValueString(getAxisHint()) << endl;

    _overConstrained = false;
    _segment->base()->getConstraints( _constraints );
    _segment->base()->getOptimal    ( _optimal );

    cdebug.log(159) << "Stage:" << RoutingEvent::getStage() << endl;
    if (RoutingEvent::getStage() == RoutingEvent::Repair) {
      if (_segment->isStrongTerminal(KbPropagate)) {
        cdebug.log(159) << "Not expanding on Terminals:" << _constraints << endl;
      } else {
        cdebug.log(159) << "Expanding:" << _constraints << endl;
        _constraints.inflate( Session::getSliceHeight() );
        cdebug.log(159) << "Expanding (after):" << _constraints << endl;
      }
    }
    cdebug.log(159) << "| Raw Track Constraint: " << _constraints << endl;

    _tracksNb = 0;

    Interval perpandicular = _constraints;
    perpandicular.intersection( getPerpandicularFree());
    cdebug.log(159) << "| Perpandicular Free: " << perpandicular << endl;

    if (not perpandicular.isEmpty()) {
      RoutingPlane* plane   = Session::getKiteEngine()->getRoutingPlaneByLayer(_segment->getLayer());
      Track*        track   = plane->getTrackByPosition(perpandicular.getVMin());

      if ( track and (track->getAxis() < perpandicular.getVMin()) ) track = track->getNextTrack();
      for ( ; track and (track->getAxis() <= perpandicular.getVMax())
            ; track = track->getNextTrack(), _tracksNb++ );
    }
    if (not _tracksNb) {
      cdebug.log(159) << "| Reverting to pure constraints." << endl;
      RoutingPlane* plane   = Session::getKiteEngine()->getRoutingPlaneByLayer(_segment->getLayer());
      Track*        track   = plane->getTrackByPosition(_constraints.getVMin());

      if ( track && (track->getAxis() < _constraints.getVMin()) ) track = track->getNextTrack();
      for ( ; track && (track->getAxis() <= _constraints.getVMax())
            ; track = track->getNextTrack(), _tracksNb++ );
    }
    if (not _tracksNb) {
      cdebug.log(159) << "| Pure constraints are too tight." << endl;
      if (_segment->base())
        _overConstrained =     _segment->base()->getAutoSource()->isTerminal()
                           and _segment->base()->getAutoTarget()->isTerminal();
    }
      
    _priority
      = (DbU::toLambda(_segment->getLength())        + 1.0)
      * (DbU::toLambda(_segment->base()->getSlack()) + 1.0);

    cdebug.log(159) << _segment << " has " << _tracksNb << " choices " << perpandicular << endl;
    cdebug.tabw(159,-1);

    DebugSession::close();
  }


  string  RoutingEvent::_getTypeName () const
  { return "RoutingEvent"; }


  string  RoutingEvent::_getString () const
  {
    string s = "<" + _getTypeName();

    s += " "  + getString(_eventLevel);
    s += ","  + getString((long)getPriority());
    s += " @" + DbU::getValueString(getAxisHistory());
    s += ":"  + getString(_segment);
    s += " "  + getString(_segment->getDataNegociate()->getRipupCount());
    s += " ";
    s +=        (isCloned       ()?"C":"-");
    s +=        (isDisabled     ()?"d":"-");
    s +=        (isUnimplemented()?"u":"-");
    s += ">";

    return s;
  }


  Record* RoutingEvent::_getRecord () const
  {
    Record* record = new Record ( getString(this) );
    record->add ( getSlot ( "_segment"   , _segment    ) );
    record->add ( getSlot ( "_processed" , _processed  ) );
                                     
    return record;
  }


} // Kite namespace.
