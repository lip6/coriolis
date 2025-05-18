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
// |  C++ Module  :       "./TrackSegment.cpp"                       |
// +-----------------------------------------------------------------+


#include <sstream>
#include <limits>
#include "hurricane/Bug.h"
#include "hurricane/DebugSession.h"
#include "hurricane/Warning.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Net.h"
#include "hurricane/Name.h"
#include "hurricane/RoutingPad.h"
#include "anabatic/AutoContact.h"
#include "anabatic/GCell.h"
#include "crlcore/RoutingGauge.h"
#include "katana/DataNegociate.h"
#include "katana/RoutingPlane.h"
#include "katana/TrackSegmentRegular.h"
#include "katana/TrackSegmentNonPref.h"
#include "katana/TrackSegmentWide.h"
#include "katana/Track.h"
#include "katana/Session.h"
#include "katana/RoutingEvent.h"
#include "katana/NegociateWindow.h"
#include "katana/KatanaEngine.h"


namespace Katana {

  using namespace std;
  using Hurricane::tab;
  using Hurricane::ForEachIterator;
  using Hurricane::Bug;
  using Hurricane::DebugSession;
  using Hurricane::Error;
  using Hurricane::BasicLayer;
  using Hurricane::Net;
  using Hurricane::Name;
  using Hurricane::RoutingPad;
  using Anabatic::AutoSegment;
  using Anabatic::perpandicularTo;

// -------------------------------------------------------------------
// Class  :  "TrackSegment".

  size_t  TrackSegment::_allocateds = 0;


  size_t  TrackSegment::getAllocateds ()
  { return _allocateds; }


  TrackSegment::TrackSegment ( AutoSegment* segment, Track* track )
    : TrackElement  (track)
    , _base         (segment)
    , _symmetric    (NULL)
    , _freedomDegree(0)
    , _ppitch       (0)
    , _data         (NULL)
    , _priority     (0.0)
    , _flags        (NoFlags)
  {
    cdebug_log(155,0) << "CTOR TrackSegment " << /*(void*)this    <<*/ ":" << this    << endl;
    cdebug_log(155,0) << "             over " << /*(void*)segment <<*/ ":" << segment << endl;

    setFlags( TElemCreated|TElemLocked );
    if (segment) {
      _data = new DataNegociate( this );
      _base->getCanonical( _sourceU, _targetU );
      updateFreedomDegree();
      updatePPitch();
    }

    ++_allocateds;
  }


  void  TrackSegment::_postCreate ()
  {
    TrackElement::_postCreate();
    base()->setObserver( AutoSegment::Observable::TrackSegment, getObserver() );
  }


  TrackSegment::~TrackSegment ()
  {
    if (_data) delete _data;
    --_allocateds;
  }


  void  TrackSegment::_preDestroy ()
  {
    cdebug_log(155,0) << "TrackSegment::_preDestroy() - " << (void*)this
               << " [" << (void*)_base << ", "
               << (void*)(_base?_base->base():NULL) << "]" << endl;

    DbU::Unit length = base()->getLength();
    if ( (length > 0) and (length < getPPitch()) ) {
      BasicLayer* layer  = getLayer()->getBasicLayers().getFirst();
      DbU::Unit   width  = base()->getWidth();
      Contact*    source = base()->getAutoSource()->base();
      Contact*    target = base()->getAutoTarget()->base();
      if (isHorizontal()) {
        width = std::max( width, source->getBoundingBox(layer).getHeight() );
        width = std::max( width, target->getBoundingBox(layer).getHeight() );
      } else {
        width = std::max( width, source->getBoundingBox(layer).getWidth() );
        width = std::max( width, target->getBoundingBox(layer).getWidth() );
      }
      base()->base()->setWidth( width );
    }

    base()->setObserver( AutoSegment::Observable::TrackSegment, NULL );
    TrackElement::_preDestroy();
  }


  TrackElement* TrackSegment::create ( AutoSegment* segment, Track* track, bool& created )
  {
    DebugSession::open( segment->getNet(), 159, 160 );

    created = false;

    bool useNonPref = (segment->getDirection() xor Session::getDirection(segment->getLayer()));

    DbU::Unit     defaultWireWidth = Session::getWireWidth( segment->base()->getLayer() );
  // TO_CHECK.
  //TrackElement* trackElement     = Session::lookup( segment->base() );
    TrackElement* trackElement     = Session::lookup( segment );
    if (not trackElement) { 
      if (useNonPref) {
        trackElement = new TrackSegmentNonPref( segment );
        if (track)
          cerr << Error( "TrackSegment::create(): Must not set track when creating a non-preferred element.\n"
                         "        (on %s)", getString(segment).c_str() ) << endl;
      } else {
        if (segment->isFixed() and track and (segment->getAxis() != track->getAxis()))
          trackElement = new TrackSegmentWide( segment, track );
        else if (segment->base()->getWidth() <= defaultWireWidth)
          trackElement = new TrackSegmentRegular( segment, track );
        else
          trackElement = new TrackSegmentWide( segment, track );
      }

      trackElement->_postCreate();
      trackElement->invalidate();
      created = true;
      cdebug_log(159,0) << "TrackSegment::create(): " << "nonPref:" << useNonPref
                        << " " << trackElement << endl;
    }

    DebugSession::close();

    return trackElement;
  }


// Formerly Inline Functions.
// Wrappeds.
  AutoSegment*   TrackSegment::base                 () const { return _base; }
  Segment*       TrackSegment::getSegment           () const { return _base->base(); }
  bool           TrackSegment::isFixed              () const { return _base->isFixed(); }
  bool           TrackSegment::isFixedAxis          () const { return _base->isFixedAxis(); }
  bool           TrackSegment::isHorizontal         () const { return _base->isHorizontal(); }
  bool           TrackSegment::isVertical           () const { return _base->isVertical(); }
  bool           TrackSegment::isLocal              () const { return not _base->isGlobal() and not _base->isWeakGlobal(); }
  bool           TrackSegment::isGlobal             () const { return _base->isWeakGlobal() or _base->isGlobal(); }
  bool           TrackSegment::isWeakGlobal         () const { return _base->isWeakGlobal(); }
  bool           TrackSegment::isUnbreakable        () const { return _base->isUnbreakable() and not _base->isWeakGlobal(); }
  bool           TrackSegment::isForOffgrid         () const { return _base->isForOffgrid(); }
  bool           TrackSegment::isBipoint            () const { return _base->isBipoint(); }
  bool           TrackSegment::isTerminal           () const { return _base->isTerminal(); }
  bool           TrackSegment::isDrag               () const { return _base->isDrag(); }
  bool           TrackSegment::isStrongTerminal     ( Flags flags ) const { return _base->isStrongTerminal(flags); }
  bool           TrackSegment::isStrap              () const { return _base->isStrap() and not _base->isWeakGlobal(); }
  bool           TrackSegment::isSlackened          () const { return _base->isSlackened(); }
  bool           TrackSegment::isDogleg             () const { return _base->isDogleg(); }
  bool           TrackSegment::isShortDogleg        () const { return _flags & TElemShortDogleg; }
  bool           TrackSegment::isReduced            () const { return _base->isReduced(); }
  bool           TrackSegment::isUserDefined        () const { return _base->isUserDefined(); }
  bool           TrackSegment::isUTurn              () const { return _base->isUTurn(); }
  bool           TrackSegment::isAnalog             () const { return _base->isAnalog(); }
  bool           TrackSegment::isWide               () const { return _base->isWide(); }
  bool           TrackSegment::isShortNet           () const { return _base->isShortNet(); }
  bool           TrackSegment::isPriorityLocked     () const { return _flags & PriorityLocked; }
  bool           TrackSegment::isNonPrefOnVSmall    () const { return _base->isNonPrefOnVSmall(); }
// Predicates.
  bool           TrackSegment::hasSymmetric         () const { return _symmetric != NULL; }
// Accessors.
  unsigned long  TrackSegment::getId                () const { return _base->getId(); }
  Flags          TrackSegment::getDirection         () const { return _base->getDirection(); }
  Net*           TrackSegment::getNet               () const { return _base->getNet(); }
  DbU::Unit      TrackSegment::getWidth             () const { return _base->getWidth(); }
  const Layer*   TrackSegment::getLayer             () const { return _base->getLayer(); }
  unsigned int   TrackSegment::getDepth             () const { return _base->getDepth(); }
  DbU::Unit      TrackSegment::getPitch             () const { return _base->getPitch(); }
  DbU::Unit      TrackSegment::getPPitch            () const { return _ppitch; }
  DbU::Unit      TrackSegment::getExtensionCap      ( Flags flags ) const { return _base->getExtensionCap(flags); }
  DbU::Unit      TrackSegment::getAxis              () const { return _base->getAxis(); }
  unsigned long  TrackSegment::getFreedomDegree     () const { return _freedomDegree; }
  float          TrackSegment::getPriority          () const { return _priority; }
  uint32_t       TrackSegment::getBreakLevel        () const { return _base->getBreakLevel(); }
  Interval       TrackSegment::getSourceConstraints () const { return _base->getSourceConstraints(); }
  Interval       TrackSegment::getTargetConstraints () const { return _base->getTargetConstraints(); }
  TrackElement*  TrackSegment::getSymmetric         () { return _symmetric; }
  TrackElements  TrackSegment::getPerpandiculars    () { return new TrackElements_Perpandiculars(this); }
// Mutators.
  void           TrackSegment::invalidate           () { setFlags( TElemInvalidated ); _base->invalidate(); }


  DbU::Unit  TrackSegment::getSourceAxis () const
  {
    DbU::Unit sourceAxis = 0;
    DbU::Unit targetAxis = 0;
    base()->getEndAxes( sourceAxis, targetAxis );
    return sourceAxis;
  }


  DbU::Unit  TrackSegment::getTargetAxis () const
  {
    DbU::Unit sourceAxis = 0;
    DbU::Unit targetAxis = 0;
    base()->getEndAxes( sourceAxis, targetAxis );
    return targetAxis;
  }


  DataNegociate* TrackSegment::getDataNegociate ( Flags flags ) const
  {
    if (flags & Flags::DataSelf) return _data;

    TrackElement* parent = getParent();
    return (parent) ? parent->getDataNegociate() : NULL;
  }


  TrackElement* TrackSegment::getNext () const
  {
    size_t dummy = _track->find( this );
    return _track->getNext( dummy, getNet() );
  }


  TrackElement* TrackSegment::getPrevious () const
  {
    size_t dummy = _track->find( this );
    return _track->getPrevious( dummy, getNet() );
  }


  TrackElement* TrackSegment::getParent () const
  {
    AutoSegment* baseParent = base()->getParent();
    if (not baseParent) return NULL;

    TrackElement* element = Session::lookup( baseParent );
    return element;
  }


  Interval  TrackSegment::getFreeInterval () const
  {
    if (not _track) return Interval(false);

    size_t  begin = _track->find( this );
    size_t  end   = begin;

    if (begin == Track::npos) {
      cerr << Error( "TrackSegment::getFreeInterval(): Segment not found in it's assigned track.\n"
                   "        * %s\n"
                   "        * %s\n"
                   , getString(_track).c_str()
                   , getString(this).c_str()
                   ) << endl;
      return Interval(false);
    }

    return _track->expandFreeInterval( begin, end, Track::InsideElement, getNet() );
  }


  size_t  TrackSegment::getGCells ( vector<GCell*>& gcells ) const
  {
    vector<GCell*>().swap( gcells );

    Flags  direction   = getDirection();
    GCell* sourceGCell = base()->getAutoSource()->getGCell();
    GCell* targetGCell = base()->getAutoTarget()->getGCell();

    cdebug_log(155,0) << "getGCells(): sourceGCell: " << sourceGCell << endl;
    cdebug_log(155,0) << "getGCells(): targetGCell: " << targetGCell << endl;

    for ( AutoSegment* segment : base()->getAligneds() ) {
      cdebug_log(155,0) << "| " << segment << endl;

      Anabatic::GCell* gcell = segment->getAutoSource()->getGCell();
      if (isLess(gcell,sourceGCell,direction)) {
        sourceGCell = gcell;
        cdebug_log(155,0) << "getGCells(): new sourceGCell: " << sourceGCell << endl;
      }

      gcell = segment->getAutoTarget()->getGCell();
      if (isGreater(gcell,targetGCell,direction)) {
        targetGCell = gcell;
        cdebug_log(155,0) << "getGCells(): new targetGCell: " << targetGCell << endl;
      }
    }

    if (not sourceGCell or not targetGCell) return 0;
    if (not sourceGCell) { gcells.push_back( targetGCell ); return 1; }
    if (not targetGCell) { gcells.push_back( sourceGCell ); return 1; }

    Flags      side = (direction & Flags::Horizontal) ? Flags::EastSide : Flags::NorthSide;
    DbU::Unit  axis = getAxis();
    cdebug_log(155,0) << "* dir:" << side._getString() << " @" << DbU::getValueString(axis) << endl;

    gcells.push_back( sourceGCell );
    while ( sourceGCell != targetGCell ) {
      sourceGCell = sourceGCell->getNeighborAt( direction, axis );
      cdebug_log(155,0) << "| " << sourceGCell << endl;
      if (not sourceGCell) break;

      gcells.push_back( sourceGCell );
    }

    return gcells.size();
  }


  size_t  TrackSegment::getPerpandicularsBound ( set<TrackElement*>& bounds )
  {
    bounds.clear ();

    set<AutoSegment*>            baseBounds;
    set<AutoSegment*>::iterator  ibase;
    _base->getPerpandicularsBound( baseBounds );

    for ( ibase=baseBounds.begin() ; ibase!=baseBounds.end() ; ++ibase ) {
      TrackElement* segment = Session::lookup( *ibase );
      if (segment)
        bounds.insert( segment );
    }

    return bounds.size();
  }


  void  TrackSegment::setPriorityLock ( bool state )
  {
    if (state) _flags |=  PriorityLocked;
    else       _flags &= ~PriorityLocked;
  }


  void  TrackSegment::updateFreedomDegree ()
  { _freedomDegree = _base->getSlack(); }


  void  TrackSegment::forcePriority ( float forced )
  { if (not isPriorityLocked()) _priority = forced; }


  void  TrackSegment::computePriority ()
  {
    if (isPriorityLocked()) return;
    if (isAnalog() and isTerminal()) { _priority = 0.0; return; }

    double length = DbU::toLambda(getLength());
    double slack  = DbU::toLambda(base()->getSlack());
    double pitch  = DbU::toLambda(getPitch());

  //if (length > 200.0) length = 200.0 - std::log(length)*20.0;
  //if (length <   0.0) length =   0.0;
  //if (slack / DbU::toLambda(_segment->getPitch()) < 2.0 ) slack = 999.0;
    if (slack / pitch > 10.0) slack = 10.0*pitch;
      
  //cerr << "TrackSegment::computePriority() length:" << length << " slack:" << slack
  //     << " pri:" << (length + 1.0) * (slack + 1.0) << " pitch:" << DbU::toLambda(getPitch()) << endl;

    _priority = (length + 1.0) * (slack + 1.0);

  //if (_priority > 10000.0) cerr << "_priority:" << _priority
  //                              << " length:"   << DbU::toLambda(getLength())
  //                              << " slack:"    << DbU::toLambda(base()->getSlack()) << endl;
  }


  void  TrackSegment::updatePPitch ()
  {
    _ppitch = _base->getPPitch();
  //cerr << "Update P/Pitch (" << DbU::toLambda(getPPitch()) << ") on " << this << endl;
  }


  void  TrackSegment::forcePositions ( DbU::Unit source, DbU::Unit target )
  { _base->forcePositions( source, target ); }


  void  TrackSegment::forcePositions ( const Interval& freeSpan )
  {
    _base->forcePositions( freeSpan );
    _base->invalidate();
  }


  void  TrackSegment::setTrack ( Track* track )
  {
    if (track) {
      DbU::Unit axis = track->getAxis();
      if (isWide()) {
        DbU::Unit pitch = track->getRoutingPlane()->getLayerGauge()->getPitch();
        axis += (pitch * (getTrackSpan() - 1)) / 2;
        
        cdebug_log(155,0) << "TrackSegment::setTrack(): pitch:" << DbU::getValueString(pitch)
                          << " trackSpan:" << getTrackSpan() << endl;
      }
      if (isNonPref()) {
        axis = getAxis();
        
        cdebug_log(155,0) << "TrackSegment::setTrack(): Non-preferred, keep @" << DbU::getValueString(axis)
                          << " trackSpan:" << getTrackSpan() << endl;
      }
      
      addTrackCount( getTrackSpan() );
      setAxis( axis, AutoSegment::SegAxisSet );
    }
    TrackElement::setTrack( track );
  }


  void  TrackSegment::setSymmetric ( TrackElement* segment )
  { _symmetric = dynamic_cast<TrackSegment*>( segment ); }


  // void  TrackSegment::detach ()
  // {
  //   cdebug_log(159,0) << "TrackSegment::detach() - <id:" << getId() << ">" << endl;

  //   setTrack( NULL );
  //   setFlags( TElemLocked );
  //   addTrackCount( -1 );
  // }


  void  TrackSegment::detach ( TrackSet& removeds )
  {
    cdebug_log(159,1) << "TrackSegment::detach(TrackSet&) - <id:" << getId() << "> trackSpan:"
                      << getTrackSpan() << endl;

    Track* wtrack = getTrack();
    for ( size_t i=0 ; wtrack and (i<getTrackSpan()) ; ++i ) {
      removeds.insert( wtrack );
      cdebug_log(159,0) << "| " << wtrack << endl;
      
      wtrack = wtrack->getNextTrack();
    }
  //addTrackCount( -getTrackSpan() );
    addTrackCount( -1  );

  //detach();
    setTrack( NULL );
    setFlags( TElemLocked );

    cdebug_tabw(159,-1);
  }


  void  TrackSegment::revalidate ()
  {
    DebugSession::open( getNet(), 159, 160 );

    unsetFlags( TElemCreated ); 
    cdebug_log(159,0) << "revalidate() - " << this << endl;

    if (isNonPref()) {
      Interval perpandicularSpan ( getAxis() );
      DbU::Unit cap = std::max( base()->getExtensionCap( Anabatic::Flags::Source )
                              , base()->getExtensionCap( Anabatic::Flags::Target ));
      cdebug_log(159,0) << "  axis=" << DbU::getValueString(getAxis())
                        <<   " cap=" << DbU::getValueString(cap)
                        << endl;
      perpandicularSpan.inflate( cap );

      _sourceU = perpandicularSpan.getVMin();
      _targetU = perpandicularSpan.getVMax();

      updateTrackSpan();
    }
    else
      _base->getCanonical( _sourceU, _targetU );


    if (_track) {
      Track* wtrack = getTrack();
      for ( size_t i=0 ; wtrack and (i<getTrackSpan()) ; ++i ) {
        Session::addSortEvent( wtrack, true );
        wtrack = wtrack->getNextTrack();
      }
    }
    unsetFlags( TElemInvalidated );

    DebugSession::close();
  }


  void  TrackSegment::setAxis ( DbU::Unit axis, uint32_t flags  )
  {
    if (_data) {
      if (axis == getAxis()) _data->incSameRipup();
      else _data->resetSameRipup();
    }
    _base->setAxis( axis, flags );
    invalidate();
  }


  void   TrackSegment::swapTrack ( TrackElement* other )
  {
    if (not other) return;

    cdebug_log(159,0) << "TrackSegment::swapTrack()" << endl;

    Track*  thisTrack   = getTrack();
    Track*  otherTrack  = other->getTrack();
    size_t  thisIndex   = ( thisTrack) ?  thisTrack->find( this) : Track::npos;
    size_t  otherIndex  = (otherTrack) ? otherTrack->find(other) : Track::npos;

    if (_track and otherTrack and (_track != otherTrack)) {
      cerr << Error("TrackSegment::swapTrack() - swapping TrackSegments from different tracks.") << endl;
    }

    setTrack( NULL );
    other->setTrack( NULL );

    other->setTrack( thisTrack );
    if (thisTrack) thisTrack->setSegment( other, thisIndex );

    setTrack( otherTrack );
    if (_track) _track->setSegment( this, otherIndex );

#if defined(CHECK_DATABASE_DISABLED)
    if      (_track)            _track->_check();
    else if (other->getTrack()) other->getTrack()->_check();
#endif

    RoutingEvent* thisEvent  = getDataNegociate(Flags::DataSelf)->getRoutingEvent();
    RoutingEvent* otherEvent = other->getDataNegociate()->getRoutingEvent();

    if (thisEvent ) thisEvent ->setSegment( other );
    if (otherEvent) otherEvent->setSegment( this );

    cdebug_log(159,0) << "| this:  " << this << endl;
    cdebug_log(159,0) << "| other: " << other << endl;
  }


  void  TrackSegment::reschedule ( uint32_t level )
  {
    cdebug_log(159,1) << "TrackSegment::reschedule() - " << this << endl;

    if (not _data or not _data->hasRoutingEvent())
      Session::getNegociateWindow()->addRoutingEvent( this, level );
    else {
      if (_track != NULL)
        Session::addRemoveEvent( this );
      Session::getNegociateWindow()->rescheduleEvent( _data->getRoutingEvent(), level );
    }

    cdebug_tabw(159,-1);
  }


  void  TrackSegment::computeAlignedPriority ()
  {
    if (isPriorityLocked() or isTerminal()) return;

    computePriority();

    AutoSegment* canonical = base();

    vector<TrackElement*> sourceAligneds;
    vector<TrackElement*> targetAligneds;
    for ( AutoSegment* segment : canonical->getAligneds(Flags::Source|Flags::WithDoglegs) ) {
      if (not segment->isCanonical()) continue;
      sourceAligneds.push_back( Session::lookup(segment) );
    }
    for ( AutoSegment* segment : canonical->getAligneds(Flags::Target|Flags::WithDoglegs) ) {
      if (not segment->isCanonical()) continue;
      sourceAligneds.push_back( Session::lookup(segment) );
    }

    if (sourceAligneds.empty() and targetAligneds.empty()) return;

    setPriorityLock( true );

    cdebug_log(159,0) << "TrackSegment::computeAlignedPriority() " << this << endl;
    cdebug_log(159,0) << "Aligneds on:" << getPriority() << ":" << this << endl;
    for ( size_t i=0 ; i<sourceAligneds.size() ; ++i ) {
      sourceAligneds[i]->forcePriority( getPriority() - 2.0*(i+1) + 1.0 );
      sourceAligneds[i]->setPriorityLock( true );

      cdebug_log(159,0) << "| S:" << i << " " << sourceAligneds[i]->getPriority()
                        << ":" << sourceAligneds[i] << endl;
    }
    for ( size_t i=0 ; i<targetAligneds.size() ; ++i ) {
      targetAligneds[i]->forcePriority( getPriority() - 2.0*(i+1) );
      targetAligneds[i]->setPriorityLock( true );

      cdebug_log(159,0) << "| T:" << i << " " << targetAligneds[i]->getPriority()
                        << ":" << targetAligneds[i] << endl;
    }
  }


  float  TrackSegment::getMaxUnderDensity ( Flags flags ) const
  { return _base->getMaxUnderDensity( flags ); }
  

  bool  TrackSegment::canRealign () const
  {
    if (isGlobal() or isNonPref() /*or isReduced()*/) return false;
    for ( TrackElement* perpandicular : const_cast<TrackSegment*>(this)->getPerpandiculars() ) {
      if (perpandicular->isReduced()) return false;
    }
    return true;
  }


  bool  TrackSegment::canPivotUp ( float reserve, Flags flags ) const
  { return _base->canPivotUp( reserve, flags ); }


  bool  TrackSegment::canPivotDown ( float reserve, Flags flags ) const
  { return _base->canPivotDown( reserve, flags ); }


  bool  TrackSegment::canMoveUp ( float reserve, Flags flags ) const
  { return _base->canMoveUp( reserve, flags ); }


  bool  TrackSegment::canSlacken () const
  {
    cdebug_log(159,0) << "TrackSegment::canSlacken() breakLevel:" << getBreakLevel() << endl;
    if (isNonPref()) return false;
    return (not isSlackened() and (getBreakLevel() < 2)) ? _base->canSlacken(Flags::Propagate) : false;
  }


  bool  TrackSegment::slacken ( Flags flags )
  {
    cdebug_log(159,0) << "TrackSegment::slacken()" << endl;

    bool success = false;

    if (not isSlackened()) {
      TrackElement* perpandicular = NULL;
      TrackElement* parallel      = NULL;

      cdebug_tabw(159,1);

      success = base()->slacken( flags|Flags::Propagate );
      _postDoglegs( perpandicular, parallel );
      
      cdebug_tabw(159,-1);
      return success;
    } else
      cerr << Bug("TrackSegment::slacken(): NULL base or already slackened.") << endl;

    return success;
  }


  bool  TrackSegment::moveUp ( Flags flags )
  {
    bool success = false;

    cdebug_log(159,1) << "TrackSegment::moveUp() " << flags << endl;

    success = base()->moveUp( flags );
    if (success) {
      TrackElement* perpandicular = NULL;
      TrackElement* parallel      = NULL;

      Session::revalidateTopology();
      _postDoglegs( perpandicular, parallel );
    }
      
    cdebug_tabw(159,-1);

    return success;
  }


  bool  TrackSegment::moveDown ( Flags flags )
  {
    bool success = false;

    cdebug_log(159,1) << "TrackSegment::moveDown() " << flags << endl;

    success = base()->moveDown( flags );
    if (success) {
      TrackElement* perpandicular = NULL;
      TrackElement* parallel      = NULL;

      Session::revalidateTopology();
      _postDoglegs( perpandicular, parallel );
    }
      
    cdebug_tabw(159,-1);

    return success;
  }


  bool  TrackSegment::moveAside ( Flags flags )
  {
    bool success = true;

    cdebug_log(159,1) << "TrackSegment::moveAside() - "
                      << ((flags&Flags::MoveToLeft )?"left" :"")
                      << ((flags&Flags::MoveToRight)?"rigth":"") << endl;
    if (flags & Flags::MoveToLeft ) base()->moveULeft ();
    if (flags & Flags::MoveToRight) base()->moveURight();
    cdebug_tabw(159,-1);

    return success;
  }


  TrackElement* TrackSegment::getSourceDogleg ()
  {
    if (not hasSourceDogleg()) return NULL;
   
    Flags         direction = perpandicularTo( getDirection() );
    TrackElement* dogleg    = NULL;
    for( Segment* segment : base()->getAutoSource()->getSlaveComponents().getSubSet<Segment*>() ) {
      dogleg = Session::lookup( segment );
      if (dogleg and (dogleg->getDirection() == direction)) {
        cdebug_log(159,0) << "Source dogleg: " << dogleg << endl;
        return dogleg;
      }
    }
    return NULL;
  }


  TrackElement* TrackSegment::getTargetDogleg ()
  {
    if (not hasSourceDogleg()) return NULL;
   
    Flags         direction = perpandicularTo( getDirection() );
    TrackElement* dogleg    = NULL;
    for( Segment* segment : base()->getAutoTarget()->getSlaveComponents().getSubSet<Segment*>() ) {
      dogleg = Session::lookup( segment );
      if (dogleg and (dogleg->getDirection() == direction)) {
        cdebug_log(159,0) << "Target dogleg: " << dogleg << endl;
        return dogleg;
      }
    }
    return NULL;
  }


  bool  TrackSegment::canDogleg ()
  {
    cdebug_log(159,0) << "TrackSegment::canDogleg()" << endl;

    if (not isLocal()) {
      cdebug_log(159,0) << "Failed: is not local." << endl;
      return false;
    }

    if (isFixed()) {
      cdebug_log(159,0) << "Failed: is fixed." << endl;
      return false;
    }

    if (isRouted()) {
      cdebug_log(159,0) << "Failed: belongs to an already routed net." << endl;
      return false;
    }

    if (isSlackened()) {
      cdebug_log(159,0) << "Failed: is local & slackened." << endl;
      return false;
    }

    if (hasSourceDogleg() or hasTargetDogleg()) {
      cdebug_log(159,0) << "Failed: already has source or target dogleg." << endl;
      return false;
    }

    if (getDepth() < 3) {
      if (getBreakLevel() and (getLength() / getBreakLevel() < Session::getSliceHeight())) {
        cdebug_log(159,0) << "Failed: minimum frag. length reached (M2-M3) "
                          << DbU::getValueString( getLength() )
                          << "/" << getBreakLevel()
                          << endl;
      }
    }

    if (isGlobal() and (getDepth() > 2)) {
      if (getBreakLevel() and (getLength() / getBreakLevel() < 4*Session::getSliceHeight())) {
        cdebug_log(159,0) << "Failed: minimum frag. length reached (M4+) "
                          << DbU::getValueString( getLength() )
                          << "/" << getBreakLevel()
                          << endl;
        return false;
      }
    }

    return true;
  }


  bool  TrackSegment::canDogleg ( Anabatic::GCell* doglegGCell, Flags flags )
  {
    cdebug_log(159,1) << "TrackSegment::canDogleg(GCell*) " << doglegGCell << endl;

    if (doglegGCell->isIoPad()) {
      cdebug_log(159,0) << "false: Cannot dogleg in a GCell under an I/O Pad." << endl;
      cdebug_tabw(159,-1);
      return false;
    }

    if (doglegGCell->isStdCellRow()) {
      cdebug_log(159,0) << "false: Cannot dogleg in a Standard Cell row." << endl;
      cdebug_tabw(159,-1);
      return false;
    }

    if (isFixed()) {
      cdebug_log(159,0) << "false: Cannot dogleg a fixed segment." << endl;
      cdebug_tabw(159,-1);
      return false;
    }

    if (isRouted()) {
      cdebug_log(159,0) << "false: Cannot dogleg a segment belonging to an already routed net." << endl;
      cdebug_tabw(159,-1);
      return false;
    }

    if (isLocal()) {
      if (hasSourceDogleg() or hasTargetDogleg()) {
        cdebug_log(159,0) << "false: Cannot dogleg again a local segment." << endl;
        cdebug_tabw(159,-1);
        return false;
      }
      if (isSlackened()) {
        cdebug_log(159,0) << "false: Cannot dogleg a local slackened segment." << endl;
        cdebug_tabw(159,-1);
        return false;
      }
    }

    if (getDepth() < 3) {
      if (getBreakLevel() and (getLength() / getBreakLevel() < Session::getSliceHeight())) {
        cdebug_log(159,0) << "Failed: minimum frag. length reached (M2-M3) "
                          << DbU::getValueString( getLength() )
                          << "/" << getBreakLevel()
                          << endl;
      }
    }

    if (isGlobal() and (getDepth() > 2)) {
      if (getBreakLevel() and (getLength() / getBreakLevel() < 4*Session::getSliceHeight())) {
        cdebug_log(159,0) << "Failed: minimum frag. length reached (M4+) "
                          << DbU::getValueString( getLength() )
                          << "/" << getBreakLevel()
                          << endl;
        cdebug_tabw(159,-1);
        return false;
      }
    }

    vector<Anabatic::GCell*> gcells;
    if (not getGCells(gcells)) {
      cdebug_log(159,0) << "getGCell() has gone wrong." << endl;
      cdebug_tabw(159,-1);
      return false;
    }

    cdebug_log(159,0) << "Source: " << *gcells.begin () << endl;
    cdebug_log(159,0) << "Target: " << *gcells.rbegin() << endl;

    AutoContact* acSource = base()->getAutoSource();
    AutoContact* acTarget = base()->getAutoTarget();
    if (gcells[0] == doglegGCell) {
      if (acSource->isHDogleg() or acSource->isVDogleg()) {
        cdebug_log(159,0) << "false: Source perpandicular already has a dogleg." << endl;
        cdebug_tabw(159,-1);
        return false;
      }
    }
    if (gcells[gcells.size()-1] == doglegGCell) {
      if (acTarget->isHDogleg() or acTarget->isVDogleg()) {
        cdebug_log(159,0) << "false: Target perpandicular already has a dogleg." << endl;
        cdebug_tabw(159,-1);
        return false;
      }
    }

    bool isGCellInside = false;
    for ( size_t igcell=0 ; igcell<gcells.size() ; ++igcell ) {
      if (doglegGCell != gcells[igcell]) continue;

      isGCellInside = true;
      if (igcell == 0) {
        if (hasSourceDogleg()) {
          if (flags & Flags::AllowDoglegReuse) return true;

          cdebug_log(159,0) << "false: Cannot dogleg again in source GCell." << endl;
          cdebug_tabw(159,-1);
          return false;
        }
      }

      if (hasTargetDogleg() and (igcell == gcells.size()-1)) {
        if (flags & Flags::AllowDoglegReuse) {
          cdebug_log(159,0) << "true" << endl;
          cdebug_tabw(159,-1);
          return true;
        }

        cdebug_log(159,0) << "false: Cannot dogleg again in target GCell." << endl;
        cdebug_tabw(159,-1);
        return false;
      }

      break;
    }

    if (not isGCellInside) {
      cdebug_log(159,0) << "false: dogleg GCell is outside segment support (go outside GCell active)." << endl;
      cdebug_tabw(159,-1);
      return false;
    }

    cdebug_log(159,0) << "true" << endl;
    cdebug_tabw(159,-1);
    return true;
  }


  bool  TrackSegment::canDogleg ( Interval interval )
  {
    cdebug_log(159,0) << "TrackSegment::canDogleg(Interval) " << interval << endl;

    if (isFixed()) {
      cdebug_log(159,0) << "Failed: is fixed" << endl;
      return false;
    }

    if (isRouted()) {
      cdebug_log(159,0) << "Failed: belongs to an already routed net" << endl;
      return false;
    }

    if (isLocal()) {
      cdebug_log(159,0) << "Failed: is local" << endl;
      return false;
    }

    if (hasSourceDogleg() or hasTargetDogleg() or isSlackened()) {
      cdebug_log(159,0) << "Failed: already has source and/or target dogleg or slackened." << endl;
      return false;
    }

    if (getDepth() < 3) {
      if (getBreakLevel() and (getLength() / getBreakLevel() < Session::getSliceHeight())) {
        cdebug_log(159,0) << "Failed: minimum frag. length reached (M2-M3) "
                          << DbU::getValueString( getLength() )
                          << "/" << getBreakLevel()
                          << endl;
      }
    }

    if (isGlobal() and (getDepth() > 2)) {
      if (getBreakLevel() and (getLength() / getBreakLevel() < 4*Session::getSliceHeight())) {
        cdebug_log(159,0) << "Failed: minimum frag. length reached (M4+) "
                          << DbU::getValueString( getLength() )
                          << "/" << getBreakLevel()
                          << endl;
        return false;
      }
    }

    return _base->canDogleg(interval);
  }



  TrackElement* TrackSegment::promoteToPref ()
  {
    TrackElement* perpandicular = nullptr;
    TrackElement* parallel      = nullptr;

    base()->setObserver( AutoSegment::Observable::TrackSegment, nullptr );
    DataNegociate* data = getDataNegociate();
    if (data and data->hasRoutingEvent())
      data->getRoutingEvent()->setDisabled( true );

    base()->promoteToPref( Flags::NoFlags );
    _postDoglegs( perpandicular, parallel );

    cdebug_log(159,0) << "TrackSegment::promoteToPref() " << this << endl;

    return perpandicular;
  }
  

  TrackElement* TrackSegment::makeDogleg ()
  {
    Anabatic::AutoContact* source = _base->getAutoSource();
    Anabatic::AutoContact* target = _base->getAutoTarget();
    Anabatic::GCell*       gcell  = _base->getAutoSource()->getGCell();

    TrackElement* dogleg   = nullptr;
    TrackElement* parallel = nullptr;
    makeDogleg( gcell, dogleg, parallel );

    if (dogleg) {
      if (source->isTerminal() xor target->isTerminal()) {
        if (target->isTerminal())
          source = target;

        DbU::Unit axis = (_base->isHorizontal()) ? source->getX() : source->getY();

        cdebug_log(159,0) << "Setting dogleg axis @" << DbU::getValueString(axis) << endl;
        dogleg->setAxis( axis );
      }
    }
    return dogleg;
  }


  TrackElement* TrackSegment::makeDogleg ( Anabatic::GCell* dogLegGCell
                                         , TrackElement*&   perpandicular
                                         , TrackElement*&   parallel
                                         )
  {
    cdebug_log(159,0) << "TrackSegment::makeDogleg(GCell*)" << endl;
    cdebug_log(159,0) << "Break in: " << dogLegGCell << endl;

    base()->makeDogleg( dogLegGCell, Flags::IncBreakLevel );
    _postDoglegs( perpandicular, parallel );

    return perpandicular;
  }


  Flags  TrackSegment::makeDogleg ( Interval interval, TrackElement*& perpandicular, TrackElement*& parallel, Flags flags )
  {
    perpandicular = NULL;
    parallel      = NULL;

    cdebug_log(159,0) << "TrackSegment::makeDogleg(Interval)" << endl;
    flags |= base()->makeDogleg( interval, Flags::IncBreakLevel );
    _postDoglegs( perpandicular, parallel );

    if (flags & Flags::ShortDogleg) {
      parallel->setFlags( TElemShortDogleg );
      Session::addShortDogleg( this, parallel );
    }

    return flags;
  }


  void  TrackSegment::_postDoglegs ( TrackElement*& perpandicular, TrackElement*& parallel )
  {
    cdebug_log(159,1) << "TrackSegment::_postDoglegs()" << endl;

    const vector<AutoSegment*>& doglegs = Session::getDoglegs();
    vector<TrackElement*>       segments;

    if (not doglegs.empty()) {
      if (doglegs.size()%3 != 0)
        cerr << Error( "Session::_postDoglegs(): Number of created segments incoherent with pure doglegs (%u)."
                     , doglegs.size() ) << endl;

      for ( size_t i=0 ; i<doglegs.size() ; i+=3 ) {
        cdebug_log(159,0) << "Looking up original:     " << doglegs[i] << endl;
        segments.push_back( Session::getNegociateWindow()->createTrackSegment(doglegs[i],0) );
        segments[i+0]->setFlags( TElemTargetDogleg );
        segments[i+0]->getDataNegociate()->resetRipupCount();
      //segments[i+0]->getDataNegociate()->resetStateCount();
        segments[i+0]->getDataNegociate()->setState( DataNegociate::RipupPerpandiculars );

        cdebug_log(159,0) << "Looking up new perpand:  " << doglegs[i+1] << endl;
        segments.push_back( Session::getNegociateWindow()->createTrackSegment(doglegs[i+1],0) );
        segments[i+1]->setFlags( TElemSourceDogleg|TElemTargetDogleg  );

        cdebug_log(159,0) << "Looking up new parallel: " << doglegs[i+2] << endl;
        if (doglegs[i+2]) {
          segments.push_back( Session::getNegociateWindow()->createTrackSegment(doglegs[i+2],0) );
          segments[i+2]->setFlags( TElemSourceDogleg );
          segments[i+2]->getDataNegociate()->resetStateCount();
          segments[i+2]->getDataNegociate()->setState( segments[i+0]->getDataNegociate()->getState() );

          segments[i+0]->getDataNegociate()->setChildSegment( segments[i+2] );
        } else {
          segments.push_back( nullptr );
        }

        perpandicular = segments[i+1];
        parallel      = segments[i+2];
      }

    // TO CHECK
    // If the original TrackElement was inserted in a Track, must check
    // if the new bit takes it's place or not.
    //if ( getGCell() != originalGCell ) swapTrack ( segments[2] );

      for ( size_t i=0 ; i<doglegs.size() ; ++i ) {
        if (not segments[i]) continue;
        segments[i]->reschedule( ((i%3==1) ? 0 : 1) );
        const char* segPart = "Unknown";
        switch ( i%3 ) {
          case 0: segPart = "original "; break;
          case 1: segPart = "perpand  "; break;
          case 2: segPart = "new paral"; break;
        }
        cdebug_log(159,0) << "[" << (i/3) << ":" << i << "] " << segPart << ": "
                    << segments[i] << endl;
      }
    } else {
      reschedule( 1 );
    }      

    cdebug_tabw(159,-1);

    Session::doglegReset();
  }


  bool  TrackSegment::_check () const
  {
    if (not base()) return true;

    bool coherency = true;

    if (not base()->isCanonical()) {
      cerr << "[CHECK] " << this << " supporting AutoSegment is not canonical." << endl;
      coherency = false;
    }

    DbU::Unit min;
    DbU::Unit max;
    base()->checkPositions();

    if (isNonPref()) {
      Interval   perpandicularSpan ( getAxis() );
      DbU::Unit  cap               = base()->getExtensionCap( Anabatic::Flags::Source );
      perpandicularSpan.inflate( cap );
      if (getId() == 825)
        cerr << "  axis=" << DbU::getValueString(getAxis())
             <<   " cap=" << DbU::getValueString(cap)
             << endl;

      min = perpandicularSpan.getVMin();
      max = perpandicularSpan.getVMax();
    }
    else
      _base->getCanonical( min, max );

    if (getSourceU() != min) {
      cerr << "[CHECK] " << this << " has bad source position "
           << "cache:" << DbU::getValueString(getSourceU()) << " vs. "
           << "canon:" << DbU::getValueString(min)          << "."  << endl;
      coherency = false;
    }
    if (getTargetU() != max) {
      cerr << "[CHECK] " << this << " has bad target position "
           << "cache:" << DbU::getValueString(getTargetU()) << " vs. "
           << "canon:" << DbU::getValueString(max)          << "." << endl;
      coherency = false;
    }

    return coherency;
  }


  string  TrackSegment::_getTypeName () const
  { return "TrackSegment"; }


  string  TrackSegment::_getString () const
  {
    string s1 = _base->_getString();
    string s2 = " ["   + DbU::getValueString(_sourceU)
              +  ":"   + DbU::getValueString(_targetU) + "]"
              +  " "   + DbU::getValueString(_targetU-_sourceU) + " "
              + ((isNonPref()      ) ? "P" : "-")
              + ((isRouted()       ) ? "R" : "-")
              + ((isSlackened()    ) ? "S" : "-")
              + ((_track           ) ? "T" : "-")
              + ((canRipple()      ) ? "r" : "-")
              + ((hasSourceDogleg()) ? "s" : "-")
              + ((hasTargetDogleg()) ? "t" : "-");

    s1.insert( s1.size()-1, s2 );

    return s1;
  }


  Record* TrackSegment::_getRecord () const
  {
    Record* record = TrackElement::_getRecord();
    record->add( getSlot( "_base"     ,  _base      ) );
    record->add( getSlot( "_symmetric",  _symmetric ) );
    record->add( getSlot( "_flags"    ,  _flags     ) );
    return record;
  }


} // Katana namespace.
