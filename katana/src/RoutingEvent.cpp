// -*- mode: C++; explicit-buffer-name: "RoutingEvent.cpp<katana>" -*-
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
// |  C++ Module  :       "./RoutingEvent.cpp"                       |
// +-----------------------------------------------------------------+


#include <cstdlib>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "hurricane/configuration/Configuration.h"
#include "hurricane/Bug.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/DebugSession.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/Net.h"
#include "hurricane/Layer.h"
#include "anabatic/AutoContact.h"
#include "katana/DataNegociate.h"
#include "katana/TrackSegment.h"
#include "katana/Track.h"
#include "katana/Tracks.h"
#include "katana/RoutingPlane.h"
#include "katana/RoutingEvent.h"
#include "katana/RoutingEventHistory.h"
#include "katana/RoutingEventQueue.h"
#include "katana/RoutingEventLoop.h"
#include "katana/NegociateWindow.h"
#include "katana/Session.h"
#include "katana/KatanaEngine.h"
#include "katana/Manipulator.h"
#include "katana/SegmentFsm.h"


namespace Katana {

  using std::cerr;
  using std::endl;
  using std::setw;
  using std::min;
  using std::ostringstream;
  using Hurricane::tab;
  using Hurricane::Breakpoint;
  using Hurricane::DebugSession;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::BaseFlags;
  using Hurricane::UpdateSession;
  using Hurricane::ForEachIterator;
  using Hurricane::Net;
  using Hurricane::Layer;
  using Anabatic::GCell;
  using Anabatic::AutoSegment;


// -------------------------------------------------------------------
// Class  :  "RoutingEvent::Compare".


  bool  RoutingEvent::Compare::operator() ( const RoutingEvent* lhs, const RoutingEvent* rhs ) const
  {
    if (lhs == rhs) return false;
    bool value = RoutingEvent::Key::Compare()( lhs->getKey(), rhs->getKey() );
    // if (  (lhs->getSegment()->base()->getFlags() & AutoSegment::SegFixedAxis)
    //    or (lhs->getSegment()->base()->getFlags() & AutoSegment::SegFixedAxis)) {
    //   cerr << "Compare: lhs < rhs = " << value << endl;
    //   cerr << "  lhs L:" << lhs->getEventLevel() << " " << lhs << endl;
    //   cerr << "  rhs L:" << rhs->getEventLevel() << " " << rhs << endl;
    // }
    return value;
  }


// -------------------------------------------------------------------
// Class  :  "RoutingEvent::Key".


  bool  RoutingEvent::Key::Compare::operator() ( const RoutingEvent::Key& lhs, const RoutingEvent::Key& rhs ) const
  {
    if (lhs._eventLevel > rhs._eventLevel) return false;
    if (lhs._eventLevel < rhs._eventLevel) return true;

  //if (lhs._net->getName() != rhs._net->getName())
  //  return lhs._net->getName() < rhs._net->getName();

  // Process all M2 (terminal access) before any others.
  //if ((lhs._layerDepth == 1) and (rhs._layerDepth != 1)) return false;
  //if ((lhs._layerDepth != 1) and (rhs._layerDepth == 1)) return true;

  // For VH gauge, process fixed axis first.
    if (    (lhs._segFlags & AutoSegment::SegFixedAxis) and not (rhs._segFlags & AutoSegment::SegFixedAxis)) return false;
    if (not (lhs._segFlags & AutoSegment::SegFixedAxis) and     (rhs._segFlags & AutoSegment::SegFixedAxis)) return true;

    if (lhs._rpDistance > rhs._rpDistance) return true;
    if (lhs._rpDistance < rhs._rpDistance) return false;

    if (lhs._layerDepth > rhs._layerDepth) return true;
    if (lhs._layerDepth < rhs._layerDepth) return false;

    if (lhs._rpDistance == 0) {
      if (lhs._length > rhs._length) return true;
      if (lhs._length < rhs._length) return false;
    }

    if (lhs._priority > rhs._priority) return false;
    if (lhs._priority < rhs._priority) return true;

    if (lhs._length > rhs._length) return false;
    if (lhs._length < rhs._length) return true;

    if ((lhs._segFlags & AutoSegment::SegHorizontal) xor (rhs._segFlags & AutoSegment::SegHorizontal))
      return (rhs._segFlags & AutoSegment::SegHorizontal);

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
    , _rpDistance  (event->getSegment()->base()->getRpDistance())
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
    _rpDistance   = event->getSegment()->base()->getRpDistance();
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


  uint32_t  RoutingEvent::_idCounter  = 0;
  uint32_t  RoutingEvent::_allocateds = 0;
  uint32_t  RoutingEvent::_processeds = 0;
  uint32_t  RoutingEvent::_cloneds    = 0;


  uint32_t  RoutingEvent::getStage        () { return Session::getStage(); }
  uint32_t  RoutingEvent::getAllocateds   () { return _allocateds; }
  uint32_t  RoutingEvent::getProcesseds   () { return _processeds; }
  uint32_t  RoutingEvent::getCloneds      () { return _cloneds; }
  void      RoutingEvent::resetProcesseds () { _processeds = 0; }


  RoutingEvent::RoutingEvent ( TrackElement* segment )
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
    , _rippleState         (0)
    , _eventLevel          (0)
    , _key                 (this)
  {
    if (_idCounter == std::numeric_limits<uint32_t>::max()) {
      throw Error( "RoutingEvent::RoutingEvent(): Identifier counter has reached it's limit (%d bits)."
                 , std::numeric_limits<uint32_t>::digits );
    }

    DataNegociate* data = _segment->getDataNegociate();
    if (data) data->setRoutingEvent( this );

    cdebug_log(159,0) << "create: " << this << endl;
    cdebug_log(159,0) << "Initial setAxisHint @" << DbU::getValueString(getAxisHint()) << endl;

    if (_segment->getTrack()) {
      cerr << Bug( "RoutingEvent::create() - TrackElement is already inserted in a Track."
                   "\n      %s.", getString(_segment).c_str()
                 ) << endl;
    }
  }


  RoutingEvent* RoutingEvent::create ( TrackElement* segment )
  {
    // if (not dynamic_cast<TrackSegment*>(segment)) {
    //   cerr << Error( "RoutingEvent::create() Can only create event from TrackSegment:\n"
    //                  "        %s", getString(segment).c_str()
    //                ) << endl;
    // }

    RoutingEvent* event = new RoutingEvent ( segment  );
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

    cdebug_log(159,0) << "RoutingEvent::clone() " << clone
                << " (from: " << ")" <<  endl;

    return clone;
  }


  RoutingEvent::~RoutingEvent ()
  {
    cdebug_log(159,0) << "~RoutingEvent() " << endl;

    DataNegociate* data = _segment->getDataNegociate();
    if ( data and (data->getRoutingEvent() == this) )
      data->setRoutingEvent( NULL );
  }


  void  RoutingEvent::destroy ()
  {
    cdebug_log(159,0) << "RoutingEvent::destroy() " << this << endl;
    if (_allocateds > 0) --_allocateds;

    delete this;
  }


  bool  RoutingEvent::isUnimplemented () const
  { return getState() == DataNegociate::Unimplemented; }


  uint32_t  RoutingEvent::getState () const
  {
    DataNegociate* data = _segment->getDataNegociate();
    return (data) ? data->getState() : 0;
  }


  void  RoutingEvent::setState ( uint32_t state )
  {
    DataNegociate* data = _segment->getDataNegociate();
    if (data) data->setState( state );
  }


  void  RoutingEvent::setAxisHint ( DbU::Unit axis )
  {
    cdebug_log(159,0) << "setAxisHint @" << DbU::getValueString(axis) << " " << _segment << endl;
    _axisHint = axis;
  }


  void  RoutingEvent::setAxisHintFromParent ()
  {
    if (Session::getStage() == StageRepair) return;

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

    cdebug_log(159,0) << "setAxisHintFromParent() - hint:" << DbU::getValueString(_axisHint)
                << " axis:" << DbU::getValueString(parent->getAxis()) << " parent:" << parent << endl;
    return;
  }


  RoutingEvent* RoutingEvent::reschedule ( RoutingEventQueue& queue, uint32_t eventLevel )
  {
    RoutingEvent* active = _segment->getDataNegociate()->getRoutingEvent();
    if (active != this) return active->reschedule( queue, eventLevel );

    RoutingEvent* fork = NULL;

    if (getState() == DataNegociate::RepairFailed) {
      cdebug_log(159,0) << "Reschedule: cancelled (RepairFailed) -> " << fork << endl;
      return NULL;
    }

    if ( (Session::getStage() != StageRepair) and isUnimplemented() ) {
      cdebug_log(159,0) << "Reschedule: cancelled (Unimplemented) -> " << fork << endl;
      return NULL;
    }

    if (not isProcessed()) {
      fork = this;
      cdebug_log(159,0) << "Reschedule/Self: -> "
                        << eventLevel << ":" << fork << endl;
    } else {
      fork = clone();
      fork->_processed  = false;

      _segment->getDataNegociate()->setRoutingEvent( fork );

      cdebug_log(159,0) << "Reschedule/Fork: -> "
                        << eventLevel << ":" << fork << endl;
    }

    if (fork->_eventLevel < eventLevel)
      fork->_eventLevel = eventLevel;

    if (Session::getStage() == StageRepair) {
      if (_segment->getDataNegociate()->getState() < DataNegociate::Repair)
        _segment->getDataNegociate()->resetRipupCount();
      _segment->getDataNegociate()->setState( DataNegociate::Repair );
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
    loop.update( _segment );
    if (loop.isLooping()) {

#if LOOP_DEBUG
      if (loop.getMaxCount() > 500) {
        cbug << Error("Loop detected, removing event %s.",_getString().c_str()) << endl;

        ostringstream message;
        message << "Katana has detected a loop between the following Segments:\n<tt>";

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
      message << "[BUG] Katana has detected a loop between the following Segments:";

      const vector<RoutingEventLoop::Element>& elements = loop.getElements();
      for ( size_t i=0 ; i<elements.size() ; ++i ) {
        message << "\n" << setw(10) << elements[i]._count << "| " << elements[i]._segment;
      }
      cerr << message.str() << endl;
#endif

      loop.erase( _segment );
      setState( DataNegociate::RepairFailed );
      setDisabled( true );
      setProcessed();
    }

  //DebugSession::open( _segment->getNet(), 155, 160 );
    DebugSession::open( _segment->getNet(), 149, 160 );

    cdebug_log(9000,0) << "Deter| Event "
                     <<         getProcesseds()
                     << ","  << getEventLevel()
                     << ","  << tsetw(6) << getPriority()
                     << ": " << _segment << endl;
    _processeds++;

    cdebug_tabw(159,1);
    cdebug_log(159,0) << "State: *before* "
                      << DataNegociate::getStateString(_segment->getDataNegociate())
                      << " ripup:" << _segment->getDataNegociate()->getRipupCount()
                      << endl;
    cdebug_log(159,0) << "Level: " << getEventLevel()
                      << ", p-slack: " << _segment->getFreedomDegree() << endl;

  //_preCheck( _segment );
    _eventLevel = 0;

    if (Session::getStage() != StagePack) history.push( this );

    if ( isProcessed() or isDisabled() ) {
      cdebug_log(159,0) << "Already processed or disabled." << endl;
    } else {
      if (_segment->hasSymmetric()) {
      }

      setProcessed();
      setTimeStamp( _processeds );

      switch ( Session::getStage() ) {
        case StageNegociate: _processNegociate( queue, history ); break;
        case StagePack:      _processPack     ( queue, history ); break;
        case StageRepair:    _processRepair   ( queue, history ); break;
        case StageRealign:   _processRealign  ( queue, history ); break;
        default:
          cerr << Bug( "RoutingEvent::process() - Unknown stage value:%d.", Session::getStage() ) << endl;
          break;
      }
    }
    cdebug_tabw(159,-1);

    queue.repushInvalidateds();

    // if (getProcesseds() == 48172 + 1) {
    //   UpdateSession::close();
    //   Breakpoint::stop( 0, "Stopping before revalidating event 3107." );
    //   UpdateSession::open();
    // }
    
    Session::revalidate();
    queue.commit();

  //_postCheck( _segment );
#if defined(CHECK_DATABASE)
    Session::getKatanaEngine()->_check( _segment->getNet() );
#endif
    DebugSession::close();

    if (Session::getKatanaEngine()->getPostEventCb() != NULL)
      Session::getKatanaEngine()->getPostEventCb()();
  }


  void  RoutingEvent::_processNegociate ( RoutingEventQueue& queue, RoutingEventHistory& history )
  {
    cdebug_log(159,0) << "* Mode:Negociation." << endl;

    SegmentFsm fsm ( this, queue, history );

    if (fsm.getState() == SegmentFsm::MissingData) {
      cbug << Error("RoutingEvent::process() - Missing datas.") << endl;
      return;
    }

    cdebug_tabw(159,1);

    fsm.incRipupCount();

    cdebug_log(159,0) << "| Candidate Tracks:" << endl;
    size_t itrack = 0;
    for ( itrack = 0 ; itrack < fsm.getCosts().size() ; itrack++ )
      cdebug_log(159,0) << "| " << itrack << ":" << fsm.getCost(itrack) << endl;

    itrack = 0;
    if ( (not isOverConstrained()) and fsm.canRipup() ) {
      if (fsm.getCosts().size() and fsm.getCost(itrack)->isFree()) {
        cdebug_log(159,0) << "Insert in free space " << this << endl;
        fsm.bindToTrack( itrack );
      } else {
      // Do ripup.
        if (fsm.getState() == SegmentFsm::EmptyTrackList) {
          fsm.ripupPerpandiculars();
        } else {
          if (fsm.canRipup(Manipulator::NotOnLastRipup)) {
            if (cdebug.enabled(9000)) {
              for ( itrack=0 ; itrack<fsm.getCosts().size() ; itrack++ ) {
                cdebug_log(9000,0) << "Deter| | Candidate Track: " << fsm.getCost(itrack) << endl;
              }
            }
            for ( itrack=0 ; itrack<fsm.getCosts().size() ; itrack++ ) {
              cdebug_log(159,0) << "Trying Track: " << itrack << endl;
              if (fsm.getCost(itrack)->isInfinite()) break;
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
        cdebug_log(159,0) << "Immediate slackening due to overconstraint" << endl;
        fsm.setDataState( DataNegociate::Slacken );
      }
      if (not fsm.slackenTopology()) {
        fsm.setState( SegmentFsm::SelfMaximumSlack );
      }
    }

    fsm.doActions();

    if (itrack < fsm.getCosts().size()) {
      cdebug_log(159,0) << "Placed: @" << DbU::getValueString(fsm.getTrack1(itrack)->getAxis())
                  << " " << this << endl;
    }

    cdebug_tabw(159,-1);
  }


  void  RoutingEvent::_processPack ( RoutingEventQueue& queue, RoutingEventHistory& history )
  {
    cdebug_log(159,0) << "* Mode:Pack." << endl;

    if (not _segment->isUTurn()) return;

    SegmentFsm fsm ( this, queue, history );
    if (fsm.getState() == SegmentFsm::MissingData   ) return;
    if (fsm.getState() == SegmentFsm::EmptyTrackList) return;

    cdebug_tabw(159,1);
    for ( size_t i = 0 ; i < fsm.getCosts().size() ; i++ )
      cdebug_log(159,0) << "| " << fsm.getCost(i) << endl;
    cdebug_tabw(159,-1);

    if (    _segment->getTrack()
       and  fsm.getCosts().size()
       and  fsm.getCost(0)->isFree()
       and (fsm.getTrack1(0) != _segment->getTrack()) ) {

      cerr << "_processPack(): move to " << fsm.getTrack1(0) << endl;
      fsm.moveToTrack( 0 );
    }
  }


  void  RoutingEvent::_processRepair ( RoutingEventQueue& queue, RoutingEventHistory& history )
  {
    cdebug_log(159,0) << "* Mode:Repair." << endl;

    if ( _segment->getTrack() != NULL ) {
      cdebug_log(159,0) << "* Cancel: already in Track." << endl;
      return;
    }

    SegmentFsm fsm ( this, queue, history );
    if (fsm.getState() == SegmentFsm::MissingData   ) return;
    if (fsm.getState() == SegmentFsm::EmptyTrackList) return;
    if (fsm.isSymmetric()) return;


    cdebug_log(159,0) << "| Candidate Tracks:" << endl;
    size_t itrack = 0;
    for ( itrack = 0 ; itrack < fsm.getCosts().size() ; itrack++ )
      cdebug_log(159,0) << "| " << itrack << ":" << fsm.getCost(itrack) << endl;

    if (fsm.getCosts().size() and fsm.getCost(0)->isFree()) {
      cdebug_log(159,0) << "Insert in free space." << endl;
      fsm.bindToTrack( 0 );

      cdebug_log(159,0) << "Re-try perpandiculars:" << endl;
      for ( TrackElement* perpandicular : getPerpandiculars() ) {
        if (not perpandicular->getTrack() ) {
          cdebug_log(159,0) << "| " << perpandicular << endl;
          fsm.addAction( perpandicular, SegmentAction::SelfInsert );
          DataNegociate* data = perpandicular->getDataNegociate();
          if (data and (data->getState() < DataNegociate::Repair)) {
            data->setState( DataNegociate::Repair );
            data->resetRipupCount();
          }
        }
      }
      fsm.doActions();
      queue.commit();
    } else {
      switch ( fsm.getData()->getStateCount() ) {
        case 1:
        // First try: minimize or replace perpandiculars first.
          if (Manipulator(_segment,fsm).minimize())
            fsm.addAction( _segment, SegmentAction::SelfInsert );
          else
            Manipulator(_segment,fsm).repackPerpandiculars( Manipulator::PerpandicularsFirst );
          fsm.doActions();
          queue.commit();
          break;
        case 2:
        // Second try: failed re-inserted first.
          Manipulator(_segment,fsm).repackPerpandiculars( 0 );
        //fsm.addAction( _segment, SegmentAction::SelfInsert );
          fsm.doActions();
          queue.commit();
          break;
        default:
          cdebug_log(159,0) << "Repair failed." << endl;
          setState( DataNegociate::RepairFailed );
          break;
      }
    }
  }


  void  RoutingEvent::_processRealign ( RoutingEventQueue& queue, RoutingEventHistory& history )
  {
    cdebug_log(159,0) << "* Mode:Realign." << endl;

    SegmentFsm fsm ( this, queue, history );
    if (fsm.getState() == SegmentFsm::MissingData   ) return;
    if (fsm.getState() == SegmentFsm::EmptyTrackList) return;
    if (fsm.isSymmetric()) return;

    cdebug_log(159,0) << "| Candidate Tracks:" << endl;
    size_t itrack = 0;
    for ( itrack = 0 ; itrack < fsm.getCosts().size() ; itrack++ )
      cdebug_log(159,0) << "| " << itrack << ":" << fsm.getCost(itrack) << endl;

    if (   fsm.getCosts().size()
       and fsm.getCost(0)->isFree()
       and (fsm.getCost(0)->getTrack() != getSegment()->getTrack())) {
      cdebug_log(159,0) << "Insert in free space." << endl;
      fsm.moveToTrack( 0 );
      cdebug_log(155,0) << "          @" << DbU::getValueString(getSegment()->getAxis())
                        << "  -> "       << DbU::getValueString(fsm.getCost(0)->getTrack()->getAxis()) << endl;
      fsm.doActions();
      queue.commit();
    }
  }


  void  RoutingEvent::revalidate ()
  {
    if (_segment->isNonPref()) { _revalidateNonPref(); return; }
    
    DebugSession::open( _segment->getNet(), 156, 160 );

    cdebug_log(159,1) << "RoutingEvent::revalidate() - " << this << endl;

  //_dataNegociate->update();

    setAxisHintFromParent();
    cdebug_log(159,0) << "axisHint:" << DbU::getValueString(getAxisHint()) << endl;

    _overConstrained = false;
    _segment->base()->getConstraints( _constraints );
    _segment->base()->getOptimal    ( _optimal );

    cdebug_log(159,0) << "Stage:" << RoutingEvent::getStage() << endl;
    if (Session::getStage() == StageRepair) {
      if (_segment->isStrongTerminal(Flags::Propagate)) {
        cdebug_log(159,0) << "Not expanding on Terminals:" << _constraints << endl;
      } else if (  _segment->base()->getAutoSource()->isFixed()
                or _segment->base()->getAutoTarget()->isFixed()) {
        cdebug_log(159,0) << "Not expanding on fixed segments:" << _constraints << endl;
      } else if (   (       Session::getKatanaEngine()->isChannelStyle()
                    and not Session::getKatanaEngine()->isHybridStyle())
                and _segment->base()->isHorizontal()) {
        cdebug_log(159,0) << "Not expanding on horizontal segments in channel mode:" << _constraints << endl;
      } else {
        cdebug_log(159,0) << "Expanding:" << _constraints << endl;
        _constraints.inflate( Session::getSliceHeight() );
        cdebug_log(159,0) << "Expanding (after):" << _constraints << endl;
      }
    }

    if (_segment->isShortDogleg()) {
      TrackElement* parallel = Session::getDoglegPaired( _segment );
      if (parallel and parallel->getTrack()) {
        Interval delta ( parallel->getAxis() - _segment->getPitch()
                       , parallel->getAxis() + _segment->getPitch() );
        _constraints.intersection( delta );
        cdebug_log(159,0) << "Short parallel to: " << parallel << endl;
        cdebug_log(159,0) << "Constraints restricted to: " << delta << endl;
      }
    }

    cdebug_log(159,0) << "| Raw Track Constraint: " << _constraints
                      << " [" << _constraints.getVMin()
                      <<  "," << _constraints.getVMax() << "]" << endl;

    _tracksNb = 0;

    Interval perpandicular = _constraints;
    perpandicular.intersection( getPerpandicularFree());
    cdebug_log(159,0) << "| Perpandicular Free: " << perpandicular << endl;

    if (not perpandicular.isEmpty()) {
      RoutingPlane* plane   = Session::getKatanaEngine()->getRoutingPlaneByLayer(_segment->getLayer());
      Track*        track   = plane->getTrackByPosition(perpandicular.getVMin());

      if ( track and (track->getAxis() < perpandicular.getVMin()) ) track = track->getNextTrack();
      for ( ; track and (track->getAxis() <= perpandicular.getVMax())
            ; track = track->getNextTrack(), _tracksNb++ );
    }
    if (not _tracksNb) {
      cdebug_log(159,0) << "| Reverting to pure constraints." << endl;
      RoutingPlane* plane   = Session::getKatanaEngine()->getRoutingPlaneByLayer(_segment->getLayer());
      Track*        track   = plane->getTrackByPosition(_constraints.getVMin());

      if ( track and (track->getAxis() < _constraints.getVMin()) ) track = track->getNextTrack();
      cdebug_log(159,0) << "| Nearest " << track << endl;
      while ( track and (track->getAxis() <= _constraints.getVMax()) ) {
        _tracksNb++;
        track = track->getNextTrack();
        cdebug_log(159,0) << "| Next " << track << endl;
      }
    }
    cdebug_log(159,0) << "| _tracksNb " << _tracksNb << endl;
    if (not _tracksNb) {
      cdebug_log(159,0) << "| Pure constraints are too tight." << endl;
      if (_segment->base())
        _overConstrained =     _segment->base()->getAutoSource()->isTerminal()
                           and _segment->base()->getAutoTarget()->isTerminal();
    }
    cdebug_log(159,0) << "| _tracksNb " << _tracksNb << endl;

    _segment->computePriority();

    cdebug_log(159,0) << _segment << " has " << (int)_tracksNb << " choices " << perpandicular << endl;
    cdebug_tabw(159,-1);

    DebugSession::close();
  }


  void  RoutingEvent::_revalidateNonPref ()
  {
    DebugSession::open( _segment->getNet(), 156, 160 );

    cdebug_log(159,1) << "RoutingEvent::_revalidateNonPref() - " << this << endl;

    setAxisHintFromParent();
    cdebug_log(159,0) << "axisHint:" << DbU::getValueString(getAxisHint()) << endl;

    _overConstrained = false;
    _segment->base()->getConstraints( _constraints );
    _segment->base()->getOptimal    ( _optimal );

    cdebug_log(159,0) << "Stage:" << RoutingEvent::getStage() << endl;
    cdebug_log(159,0) << "| Raw Track Constraint: " << _constraints
                      << " [" << _constraints.getVMin()
                      <<  "," << _constraints.getVMax() << "]" << endl;

    _tracksNb = 0;

    Interval perpandicular = _constraints;
    perpandicular.intersection( getPerpandicularFree() );
    cdebug_log(159,0) << "| Perpandicular Free: " << perpandicular << endl;

    size_t depth = Session::getRoutingGauge()->getLayerDepth( _segment->getLayer() );

    depth += (depth+1 <= Session::getConfiguration()->getAllowedDepth()) ? 1 : -1;
    RoutingPlane* plane = Session::getKatanaEngine()->getRoutingPlaneByIndex( depth );

    if (not perpandicular.isEmpty()) {
      Track* track = plane->getTrackByPosition( perpandicular.getVMin() );

      if ( track and (track->getAxis() < perpandicular.getVMin()) ) track = track->getNextTrack();
      for ( ; track and (track->getAxis() <= perpandicular.getVMax())
            ; track = track->getNextTrack(), _tracksNb++ );
    }
    if (not _tracksNb) {
      cdebug_log(159,0) << "| Reverting to pure constraints." << endl;
      Track* track = plane->getTrackByPosition( _constraints.getVMin() );

      if ( track && (track->getAxis() < _constraints.getVMin()) ) track = track->getNextTrack();
      for ( ; track && (track->getAxis() <= _constraints.getVMax())
            ; track = track->getNextTrack(), _tracksNb++ );
    }
    if (not _tracksNb) {
      cdebug_log(159,0) << "| Pure constraints are too tight." << endl;
      if (_segment->base())
        _overConstrained =     _segment->base()->getAutoSource()->isTerminal()
                           and _segment->base()->getAutoTarget()->isTerminal();
    }

    _segment->computePriority();

    cdebug_log(159,0) << _segment << " has " << (int)_tracksNb << " choices " << perpandicular << endl;
    cdebug_tabw(159,-1);

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


} // Katana namespace.
