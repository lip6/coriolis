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
#include "hurricane/Warning.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Net.h"
#include "hurricane/Name.h"
#include "hurricane/RoutingPad.h"
#include "katabatic/AutoContact.h"
#include "katabatic/GCell.h"
#include "crlcore/RoutingGauge.h"
#include "kite/DataNegociate.h"
#include "kite/TrackSegment.h"
#include "kite/Track.h"
#include "kite/Session.h"
#include "kite/RoutingEvent.h"
#include "kite/NegociateWindow.h"
#include "kite/KiteEngine.h"


namespace Kite {

  using namespace std;
  using Hurricane::tab;
  using Hurricane::ForEachIterator;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::BasicLayer;
  using Hurricane::Net;
  using Hurricane::Name;
  using Hurricane::RoutingPad;
  using Katabatic::SegSlackened;
  using Katabatic::KbPropagate;

// -------------------------------------------------------------------
// Class  :  "TrackSegment".

  size_t  TrackSegment::_allocateds = 0;


  size_t  TrackSegment::getAllocateds ()
  { return _allocateds; }


  TrackSegment::TrackSegment ( AutoSegment* segment, Track* track )
    : TrackElement  (track)
    , _base         (segment)
    , _freedomDegree(0)
    , _ppitch       (0)
    , _data         (NULL)
    , _dogLegLevel  (0)
  {
    cdebug_log(155,0) << "CTOR TrackSegment " << (void*)this    << ":" << this    << endl;
    cdebug_log(155,0) << "             over " << (void*)segment << ":" << segment << endl;

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
    base()->addObserver( getObserver() );
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

    base()->removeObserver( getObserver() );
    TrackElement::_preDestroy();
  }


  TrackElement* TrackSegment::create ( AutoSegment* segment, Track* track, bool& created )
  {
    created = false;

    TrackElement* trackElement = Session::lookup( segment->base() );
    if (not trackElement) { 
      TrackSegment* trackSegment = new TrackSegment( segment, track );
      trackSegment->_postCreate();
      created = true;
      
      trackSegment->invalidate();

      cdebug_log(159,0) << "TrackSegment::create(): " << trackSegment << endl;
      trackElement = trackSegment;
    }

    return trackElement;
  }


// Formerly Inline Functions.
// Wrappeds.
  AutoSegment*   TrackSegment::base                 () const { return _base; }
  bool           TrackSegment::isFixed              () const { return _base->isFixed(); }
  bool           TrackSegment::isHorizontal         () const { return _base->isHorizontal(); }
  bool           TrackSegment::isVertical           () const { return _base->isVertical(); }
  bool           TrackSegment::isLocal              () const { return not _base->isWeakGlobal() and not _base->isGlobal(); }
  bool           TrackSegment::isGlobal             () const { return _base->isWeakGlobal() or _base->isGlobal(); }
  bool           TrackSegment::isBipoint            () const { return _base->isBipoint(); }
  bool           TrackSegment::isTerminal           () const { return _base->isTerminal(); }
  bool           TrackSegment::isStrongTerminal     ( unsigned int flags ) const { return _base->isStrongTerminal(flags); }
  bool           TrackSegment::isStrap              () const { return _base->isStrap(); }
  bool           TrackSegment::isSlackened          () const { return _base->isSlackened(); }
  bool           TrackSegment::isDogleg             () const { return _base->isDogleg(); }
  bool           TrackSegment::isReduced            () const { return _base->isReduced(); }
  bool           TrackSegment::isUserDefined        () const { return _base->isUserDefined(); }
  bool           TrackSegment::isUTurn              () const { return _base->isUTurn(); }
// Predicates.
// Accessors.
  unsigned long  TrackSegment::getId                () const { return _base->getId(); }
  unsigned int   TrackSegment::getDirection         () const { return _base->getDirection(); }
  Net*           TrackSegment::getNet               () const { return _base->getNet(); }
  const Layer*   TrackSegment::getLayer             () const { return _base->getLayer(); }
  DbU::Unit      TrackSegment::getPitch             () const { return _base->getPitch(); }
  DbU::Unit      TrackSegment::getPPitch            () const { return _ppitch; }
  DbU::Unit      TrackSegment::getAxis              () const { return _base->getAxis(); }
  unsigned long  TrackSegment::getFreedomDegree     () const { return _freedomDegree; }
  unsigned int   TrackSegment::getDoglegLevel       () const { return _dogLegLevel; }
  Interval       TrackSegment::getSourceConstraints () const { return _base->getSourceConstraints(); }
  Interval       TrackSegment::getTargetConstraints () const { return _base->getTargetConstraints(); }
  TrackElement*  TrackSegment::getCanonical         ( Interval& i ) { return Session::lookup( _base->getCanonical(i)->base() ); }
  TrackElements  TrackSegment::getPerpandiculars    () { return new TrackElements_Perpandiculars(this); }
// Mutators.
  void           TrackSegment::invalidate           () { setFlags( TElemInvalidated ); _base->invalidate(); }


  DataNegociate* TrackSegment::getDataNegociate ( unsigned int flags ) const
  {
    if (flags & KtDataSelf) return _data;

    TrackElement* parent = getParent();
    return (parent) ? parent->getDataNegociate() : NULL;
  }


  TrackElement* TrackSegment::getNext () const
  {
    size_t dummy = _index;
    return _track->getNext( dummy, getNet() );
  }


  TrackElement* TrackSegment::getPrevious () const
  {
    size_t dummy = _index;
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

    size_t  begin = _index;
    size_t  end   = _index;

    return _track->expandFreeInterval( begin, end, Track::InsideElement, getNet() );
  }


  size_t  TrackSegment::getGCells ( Katabatic::GCellVector& gcells ) const
  {
    Katabatic::GCellVector().swap( gcells );

    Katabatic::GCell* sourceGCell = base()->getAutoSource()->getGCell();
    Katabatic::GCell* targetGCell = base()->getAutoTarget()->getGCell();

    cdebug_log(159,0) << "getGCells(): sourceGCell: " << sourceGCell << endl;
    cdebug_log(159,0) << "getGCells(): targetGCell: " << targetGCell << endl;

    for( AutoSegment* segment : base()->getAligneds() ) {
      cdebug_log(159,0) << "| " << segment << endl;

      Katabatic::GCell* gcell = segment->getAutoSource()->getGCell();
      if (gcell->getIndex() < sourceGCell->getIndex()) {
        sourceGCell = gcell;
        cdebug_log(159,0) << "getGCells(): new sourceGCell: " << sourceGCell << endl;
      }

      gcell = segment->getAutoTarget()->getGCell();
      if (gcell->getIndex() > targetGCell->getIndex()) {
        targetGCell = gcell;
        cdebug_log(159,0) << "getGCells(): new targetGCell: " << targetGCell << endl;
      }
    }

    if (not sourceGCell or not targetGCell) return 0;
    if (not sourceGCell) { gcells.push_back( targetGCell ); return 1; }
    if (not targetGCell) { gcells.push_back( sourceGCell ); return 1; }

    if (isHorizontal()) {
      gcells.push_back( sourceGCell );
      while ( sourceGCell != targetGCell ) {
        sourceGCell = sourceGCell->getRight();
        if (not sourceGCell) break;

        gcells.push_back( sourceGCell );
      }
    } else {
      gcells.push_back( sourceGCell );
      while ( sourceGCell != targetGCell ) {
        sourceGCell = sourceGCell->getUp();
        if (not sourceGCell) break;

        gcells.push_back( sourceGCell );
      }
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


  void  TrackSegment::setDoglegLevel ( unsigned int level )
  {
    if (level > 15) {
      cerr << Bug("%s has reached maximum dog leg count (15)."
                 ,_getString().c_str()) << endl;
      level = 15;
    }
    _dogLegLevel = level;
  }


  void  TrackSegment::updateFreedomDegree ()
  { _freedomDegree = _base->getSlack(); }


  void  TrackSegment::updatePPitch ()
  {
    _ppitch = _base->getPPitch();
  //cerr << "Update P/Pitch (" << DbU::toLambda(getPPitch()) << ") on " << this << endl;
  }


  void  TrackSegment::setTrack ( Track* track )
  { TrackElement::setTrack( track ); }


  void  TrackSegment::detach ()
  {
    cdebug_log(159,0) << "TrackSegment::detach() - <id:" << getId() << ">" << endl;

    setTrack( NULL );
    setIndex( (size_t)-1 );
    setFlags( TElemLocked );
  }


  void  TrackSegment::revalidate ()
  {
    unsetFlags( TElemCreated ); 
    cdebug_log(159,0) << "revalidate() - " << this << endl;

    _base->getCanonical( _sourceU, _targetU );

    if (_track) Session::addSortEvent( _track, true );
    unsetFlags( TElemInvalidated );
  }


  void  TrackSegment::setAxis ( DbU::Unit axis, unsigned int flags  )
  {
    _base->setAxis( axis, flags );
    invalidate();
  }


  void   TrackSegment::swapTrack ( TrackElement* other )
  {
    if (not other) return;

    cdebug_log(159,0) << "TrackSegment::swapTrack()" << endl;

    size_t  thisIndex   = getIndex ();
    Track*  thisTrack   = getTrack ();
    size_t  otherIndex  = other->getIndex ();
    Track*  otherTrack  = other->getTrack ();

    if (_track and otherTrack and (_track != otherTrack)) {
      cerr << Error("TrackSegment::swapTrack() - swapping TrackSegments from different tracks.") << endl;
    }

    setTrack( NULL );
    other->setTrack( NULL );

    other->setTrack( thisTrack );
    other->setIndex( thisIndex );
    if (thisTrack) thisTrack->setSegment( other, thisIndex );

    setTrack( otherTrack );
    setIndex( otherIndex );
    if (_track) _track->setSegment( this, _index );

#if defined(CHECK_DATABASE_DISABLED)
    if      (_track)            _track->_check();
    else if (other->getTrack()) other->getTrack()->_check();
#endif

    RoutingEvent* thisEvent  = getDataNegociate(KtDataSelf)->getRoutingEvent();
    RoutingEvent* otherEvent = other->getDataNegociate()->getRoutingEvent();

    if (thisEvent ) thisEvent ->setSegment( other );
    if (otherEvent) otherEvent->setSegment( this );

    cdebug_log(159,0) << "| this:  " << this << endl;
    cdebug_log(159,0) << "| other: " << other << endl;
  }


  void  TrackSegment::reschedule ( unsigned int level )
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


  float  TrackSegment::getMaxUnderDensity ( unsigned int flags ) const
  { return _base->getMaxUnderDensity( flags ); }


  bool  TrackSegment::canPivotUp ( float reserve ) const
  { return _base->canPivotUp(reserve); }


  bool  TrackSegment::canPivotDown ( float reserve ) const
  { return _base->canPivotDown( reserve ); }


  bool  TrackSegment::canMoveUp ( float reserve, unsigned int flags ) const
  { return _base->canMoveUp( reserve, flags ); }


  bool  TrackSegment::canSlacken () const
  {
    cdebug_log(159,0) << "TrackSegment::canSlacken() doglegLevel:" << getDoglegLevel() << endl;
    return (not isSlackened() and (getDoglegLevel() <= 3)) ? _base->canSlacken(KbPropagate) : false;
  }

  bool  TrackSegment::slacken ( unsigned int flags )
  {
    cdebug_log(159,0) << "TrackSegment::slacken()" << endl;

    bool success = false;

    if (not isSlackened()) {
      TrackElement* perpandicular = NULL;
      TrackElement* parallel      = NULL;

      cdebug_tabw(159,1);

      success = base()->slacken( flags|KbPropagate );
      _postDoglegs( perpandicular, parallel );
      
      cdebug_tabw(159,-1);
      return success;
    } else
      cerr << Bug("TrackSegment::slacken(): NULL base or already slackened.") << endl;

    return success;
  }


  bool  TrackSegment::moveUp ( unsigned int flags )
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


  bool  TrackSegment::moveDown ( unsigned int flags )
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


  bool  TrackSegment::moveAside ( unsigned int flags )
  {
    bool success = true;

    cdebug_log(159,1) << "TrackSegment::moveAside() - "
                      << ((flags&KtMoveToLeft )?"left" :"")
                      << ((flags&KtMoveToRight)?"rigth":"") << endl;
    if (flags & KtMoveToLeft ) base()->moveULeft ();
    if (flags & KtMoveToRight) base()->moveURight();
    cdebug_tabw(159,-1);

    return success;
  }


  TrackElement* TrackSegment::getSourceDogleg ()
  {
    if (not hasSourceDogleg()) return NULL;
   
    unsigned int  direction = perpandicularTo( getDirection() );
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
   
    unsigned int  direction = perpandicularTo( getDirection() );
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

    if (getDoglegLevel() > 3) {
      cdebug_log(159,0) << "Failed: maximum dogleg level reached (4)." << endl;
      return false;
    }

    return true;
  }


  bool  TrackSegment::canDogleg ( Katabatic::GCell* doglegGCell, unsigned int flags )
  {
    cdebug_log(159,1) << "TrackSegment::canDogleg(GCell*) " << doglegGCell << endl;

    if (doglegGCell->isUnderIoPad()) {
      cdebug_log(159,0) << "false: Cannot dogleg in a GCell under an I/O Pad." << endl;
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

    if (getDoglegLevel() > 3) {
      cdebug_log(159,0) << "Failed: maximum dogleg level reached (4)." << endl;
      cdebug_tabw(159,-1);
      return false;
    }

    vector<Katabatic::GCell*> gcells;
    getGCells( gcells );

    cdebug_log(159,0) << "Source: " << *gcells.begin () << endl;
    cdebug_log(159,0) << "Target: " << *gcells.rbegin() << endl;

    bool isGCellInside = false;
    for ( size_t igcell=0 ; igcell<gcells.size() ; ++igcell ) {
      if (doglegGCell != gcells[igcell]) continue;

      isGCellInside = true;
      if (igcell == 0) {
        if (hasSourceDogleg()) {
          if (flags & KtAllowDoglegReuse) return true;

          cdebug_log(159,0) << "false: Cannot dogleg again in source GCell." << endl;
          cdebug_tabw(159,-1);
          return false;
        }
      }

      if (hasTargetDogleg() and (igcell == gcells.size()-1)) {
        if (flags & KtAllowDoglegReuse) {
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

    if (not isLocal()) {
      cdebug_log(159,0) << "Failed: is not local" << endl;
      return false;
    }

    if (hasSourceDogleg() or hasTargetDogleg() or isSlackened()) {
      cdebug_log(159,0) << "Failed: already has source and/or target dogleg or slackened." << endl;
      return false;
    }

    if (getDoglegLevel() > 3) {
      cdebug_log(159,0) << "Failed: maximum dogleg level reached (4)." << endl;
      return false;
    }

    return _base->canDogleg(interval);
  }


  TrackElement* TrackSegment::makeDogleg ()
  {
    Katabatic::AutoContact* source = _base->getAutoSource();
    Katabatic::AutoContact* target = _base->getAutoTarget();
    Katabatic::GCell*       gcell  = _base->getAutoSource()->getGCell();

    TrackElement* dogleg   = NULL;
    TrackElement* parallel = NULL;
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


  TrackElement* TrackSegment::makeDogleg ( Katabatic::GCell* dogLegGCell
                                         , TrackElement*&    perpandicular
                                         , TrackElement*&    parallel
                                         )
  {
    cdebug_log(159,0) << "TrackSegment::makeDogleg(GCell*)" << endl;
    cdebug_log(159,0) << "Break in: " << dogLegGCell << endl;

    base()->makeDogleg( dogLegGCell );
    _postDoglegs( perpandicular, parallel );

    return perpandicular;
  }


  TrackElement* TrackSegment::makeDogleg ( Interval interval, unsigned int& flags )
  {
    TrackElement* perpandicular = NULL;
    TrackElement* parallel      = NULL;

    cdebug_log(159,0) << "TrackSegment::makeDogleg(Interval)" << endl;
    flags = base()->makeDogleg( interval );
    _postDoglegs( perpandicular, parallel );

    return perpandicular;
  }


  void  TrackSegment::_postDoglegs ( TrackElement*& perpandicular, TrackElement*& parallel )
  {
    cdebug_log(159,1) << "TrackSegment::_postDoglegs()" << endl;

    unsigned int                doglegLevel = 0;
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
        doglegLevel = segments[i+0]->getDoglegLevel();
        segments[i+0]->setDoglegLevel( doglegLevel + (segments[i]->isLocal()?1:0) );

        cdebug_log(159,0) << "Looking up new perpand:  " << doglegs[i+1] << endl;
        segments.push_back( Session::getNegociateWindow()->createTrackSegment(doglegs[i+1],0) );
        segments[i+1]->setFlags( TElemSourceDogleg|TElemTargetDogleg  );
        segments[i+1]->setDoglegLevel( doglegLevel + 1 );

        cdebug_log(159,0) << "Looking up new parallel: " << doglegs[i+2] << endl;
        segments.push_back( Session::getNegociateWindow()->createTrackSegment(doglegs[i+2],0) );
        segments[i+2]->setFlags( TElemSourceDogleg );
        segments[i+2]->getDataNegociate()->resetStateCount();
        segments[i+2]->getDataNegociate()->setState( segments[i+0]->getDataNegociate()->getState() );
        segments[i+2]->setDoglegLevel( doglegLevel + (segments[i]->isLocal()?1:0) );

        segments[i+0]->getDataNegociate()->setChildSegment( segments[i+2] );

        perpandicular = segments[i+1];
        parallel      = segments[i+2];
      }

    // TO CHECK
    // If the original TrackElement was inserted in a Track, must check
    // if the new bit takes it's place or not.
    //if ( getGCell() != originalGCell ) swapTrack ( segments[2] );

      for ( size_t i=0 ; i<doglegs.size() ; ++i ) {
        segments[i]->reschedule ( ((i%3==1) ? 0 : 1) );
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
    base()->getCanonical( min, max );
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
              +  " "   + DbU::getValueString(_targetU-_sourceU)
              +  " "   + getString(_dogLegLevel)
              + " ["   + ((_track) ? getString(_index) : "npos") + "] "
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
    record->add( getSlot( "_base",  _base ) );
    return record;
  }


} // Kite namespace.
