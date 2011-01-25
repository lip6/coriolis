
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
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
// |  C++ Module  :       "./TrackSegment.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include  <sstream>
#include  <limits>

#include  "hurricane/Bug.h"
#include  "hurricane/Warning.h"
#include  "hurricane/Net.h"
#include  "hurricane/Name.h"
#include  "hurricane/RoutingPad.h"
#include  "katabatic/AutoContact.h"
#include  "katabatic/GCell.h"
#include  "crlcore/RoutingGauge.h"
#include  "kite/DataNegociate.h"
#include  "kite/TrackSegment.h"
#include  "kite/Track.h"
#include  "kite/Session.h"
#include  "kite/RoutingEvent.h"
#include  "kite/NegociateWindow.h"
#include  "kite/KiteEngine.h"


namespace Kite {


  using namespace std;
  using Hurricane::inltrace;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::tab;
  using Hurricane::ForEachIterator;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Net;
  using Hurricane::Name;
  using Hurricane::RoutingPad;


// -------------------------------------------------------------------
// Class  :  "TrackSegment".


  TrackSegment::TrackSegment ( AutoSegment* segment, Track* track )
    : TrackElement  (track)
    , _base         (segment)
    , _created      (true)
    , _lock         (true)
    , _revalidated  (false)
    , _sourceDogLeg (false)
    , _targetDogLeg (false)
    , _canRipple    (false)
    , _routed       (false)
    , _area         (0)
    , _data         (NULL)
    , _dogLegLevel  (0)
  {
    if (segment) {
      _data = new DataNegociate ( this );
      _base->getCanonical ( _sourceU, _targetU );
      setArea ();
    //update  ();
    }
  }


  void  TrackSegment::_postCreate ()
  {
    TrackElement::_postCreate ();
    Session::link ( this );
  }


  TrackSegment::~TrackSegment ()
  { if ( _data ) delete _data; }


  void  TrackSegment::_preDestroy ()
  {
    ltrace(90) << "TrackSegment::_preDestroy() - " << (void*)this
               << " [" << (void*)_base << ", "
               << (void*)(_base?_base->base():NULL) << "]" << endl;

    Session::unlink ( this );

    TrackElement::_preDestroy ();
  }


  TrackElement* TrackSegment::create ( AutoSegment* segment, Track* track, bool& created )
  {
    created = false;

    TrackElement* trackElement = Session::lookup ( segment->base() );
    if ( not trackElement ) { 
      TrackSegment* trackSegment = new TrackSegment ( segment, track );
      trackSegment->_postCreate ();
      created = true;
      
      trackSegment->invalidate ();

      ltrace(200) << "TrackSegment::create(): " << trackSegment << endl;
      trackElement = trackSegment;
    }

    return trackElement;
  }


// Formerly Inline Functions.
  AutoSegment*   TrackSegment::base                       () const { return _base; }
  bool           TrackSegment::isBipoint                  () const { return _base->isBipoint(); }
  bool           TrackSegment::isCreated                  () const { return _created; }
  bool           TrackSegment::isFixed                    () const { return _base->isFixed(); }
  bool           TrackSegment::isStrap                    () const { return _base->isCanonicalStrap(); }
  bool           TrackSegment::isSlackenStrap             () const { return _base->isSlackenStrap(); }
  bool           TrackSegment::isLocal                    () const { return _base->isCanonicalLocal(); }
  bool           TrackSegment::isGlobal                   () const { return !isLocal(); }
  bool           TrackSegment::isLocked                   () const { return _lock; }
  bool           TrackSegment::isTerminal                 () const { return _base->isTerminal(); }
  bool           TrackSegment::isDogleg                   () const { return _base->isDogleg(); }
  bool           TrackSegment::isRevalidated              () const { return _revalidated; }
  bool           TrackSegment::isRouted                   () const { return _routed; }
  bool           TrackSegment::isSlackened                () const { return _base->isSlackened(); }
  bool           TrackSegment::isSlackenDogLeg            () const { return isSlackened() and (_dogLegLevel > 0); }
  bool           TrackSegment::hasSourceDogLeg            () const { return _sourceDogLeg; }
  bool           TrackSegment::hasTargetDogLeg            () const { return _targetDogLeg; }
  bool           TrackSegment::allowOutsideGCell          () const { return _base->allowOutsideGCell(); }
  bool           TrackSegment::canGoOutsideGCell          () const { return _base->canGoOutsideGCell(); }
  bool           TrackSegment::canRipple                  () const { return _canRipple; }
  unsigned long  TrackSegment::getId                      () const { return _base->getId(); }
  DbU::Unit      TrackSegment::getAxis                    () const { return _base->getAxis(); }
  unsigned long  TrackSegment::getArea                    () const { return _area; }
  unsigned int   TrackSegment::getDogLegLevel             () const { return _dogLegLevel; }
  Interval       TrackSegment::getSourceConstraints       () const { return _base->getSourceConstraints(); }
  Interval       TrackSegment::getTargetConstraints       () const { return _base->getTargetConstraints(); }
  TrackElements  TrackSegment::getCollapsedPerpandiculars () { return new TrackElements_CollapsedPerpandicular(this); }
  void           TrackSegment::setAllowOutsideGCell       ( bool   state ) { _base->setAllowOutsideGCell(state,true); }
  void           TrackSegment::setLock                    ( bool   state ) { _lock  = state; }
  void           TrackSegment::setRevalidated             ( bool   state ) { _revalidated  = state; }
  void           TrackSegment::invalidate                 () { _base->invalidate(); }
  void           TrackSegment::setCanRipple               ( bool state ) { _canRipple = state; }
  void           TrackSegment::setSourceDogLeg            ( bool state ) { _sourceDogLeg = state; }
  void           TrackSegment::setTargetDogLeg            ( bool state ) { _targetDogLeg = state; }


  TrackElement*  TrackSegment::getCanonical ( Interval& i )
  { return Session::lookup ( _base->getCanonical(i)->base() ); }


  bool  TrackSegment::canSlacken () const
  { return (not isSlackened())?(_base->canSlacken(true)):false; }


  bool  TrackSegment::isHorizontal () const
  { return _base->isHorizontal(); }


  bool  TrackSegment::isVertical () const
  { return _base->isVertical(); }


  unsigned int  TrackSegment::getDirection () const
  { return _base->getDirection(); }


  Net* TrackSegment::getNet () const
  { return _base->getNet(); }


  const Layer* TrackSegment::getLayer () const
  { return _base->getLayer(); }


  DataNegociate* TrackSegment::getDataNegociate ( unsigned int flags ) const
  {
    if ( flags & TrackElement::DataSelf ) return _data;

    TrackElement* parent = getParent();
    return (parent) ? parent->getDataNegociate() : NULL;
  }


  TrackElement* TrackSegment::getNext () const
  {
    size_t dummy = _index;
    return _track->getNext ( dummy, getNet() );
  }


  TrackElement* TrackSegment::getPrevious () const
  {
    size_t dummy = _index;
    return _track->getPrevious ( dummy, getNet() );
  }


  TrackElement* TrackSegment::getParent () const
  {
    AutoSegment* baseParent = base()->getParent();
    if ( not baseParent ) return NULL;

    TrackElement* element = Session::lookup ( baseParent );
    return element;
  }


  Interval  TrackSegment::getFreeInterval () const
  {
    if ( not _track ) return Interval(false);

    size_t  begin = _index;
    size_t  end   = _index;

    return _track->expandFreeInterval ( begin, end, Track::Inside, getNet() );
  }


  size_t  TrackSegment::getGCells ( vector<Katabatic::GCell*>& gcells ) const
  {
    vector<Katabatic::GCell*>().swap ( gcells );

    Katabatic::GCell* sourceGCell = base()->getAutoSource()->getGCell();
    Katabatic::GCell* targetGCell = base()->getAutoTarget()->getGCell();

    ltrace(148) << "getGCells(): sourceGCell: " << sourceGCell << endl;
    ltrace(148) << "getGCells(): targetGCell: " << targetGCell << endl;

    forEach ( AutoSegment*, isegment, base()->getCollapseds() ) {
      Katabatic::GCell* gcell = isegment->getAutoSource()->getGCell();
      if ( gcell->getIndex() < sourceGCell->getIndex() ) {
        sourceGCell = gcell;
        ltrace(148) << "getGCells(): new sourceGCell: " << sourceGCell << endl;
      }

      gcell = isegment->getAutoTarget()->getGCell();
      if ( gcell->getIndex() > targetGCell->getIndex() ) {
        targetGCell = gcell;
        ltrace(148) << "getGCells(): new targetGCell: " << targetGCell << endl;
      }
    }

    if ( not sourceGCell or not targetGCell ) return 0;
    if ( not sourceGCell ) { gcells.push_back ( targetGCell ); return 1; }
    if ( not targetGCell ) { gcells.push_back ( sourceGCell ); return 1; }

    if ( isHorizontal() ) {
      gcells.push_back ( sourceGCell );
      while ( sourceGCell != targetGCell ) {
        sourceGCell = sourceGCell->getRight();
        if ( not sourceGCell ) break;

      //cerr << "     Pushing: " << sourceGCell << endl;
        gcells.push_back ( sourceGCell );
      }
    } else {
      gcells.push_back ( sourceGCell );
      while ( sourceGCell != targetGCell ) {
        sourceGCell = sourceGCell->getUp();
        if ( not sourceGCell ) break;

        gcells.push_back ( sourceGCell );
      }
    }

    return gcells.size();
  }


  size_t  TrackSegment::getPerpandicularsBound ( set<TrackElement*>& bounds )
  {
    bounds.clear ();

    set<AutoSegment*>            baseBounds;
    set<AutoSegment*>::iterator  ibase;
    _base->getPerpandicularsBound ( baseBounds );

    for ( ibase = baseBounds.begin() ; ibase != baseBounds.end() ; ibase++ ) {
      TrackElement* segment = Session::lookup ( *ibase );
      if ( segment )
        bounds.insert ( segment );
    }

    return bounds.size();
  }


  void  TrackSegment::setDogLegLevel ( unsigned int level )
  {
    if ( level > 15 ) {
      cerr << Bug("%s has reached maximum dog leg count (15)."
                 ,_getString().c_str()) << endl;
      level = 15;
    }

    _dogLegLevel = level;
  }


  void  TrackSegment::dataInvalidate ()
  { if (_data) _data->invalidate(); }


  void  TrackSegment::eventInvalidate ()
  {
    if ( !_data ) return;

    RoutingEvent* event = _data->getRoutingEvent();
    if ( event ) event->invalidate ( true );
  }


  void  TrackSegment::setArea ()
  {
  //float length = getValue ( getTargetU() - getSourceU() );
  //float height = getValue ( getSlack() );
  //_area = (unsigned long)( length * height );
    _area = _base->getSlack();
  }


  void  TrackSegment::setRouted ( bool state )
  { _routed = state; }


  void  TrackSegment::setTrack ( Track* track )
  { TrackElement::setTrack ( track ); }


  void  TrackSegment::detach ()
  {
    ltrace(200) << "TrackSegment::detach() - <id:" << getId() << ">" << endl;

    setTrack ( NULL );
    setIndex ( (size_t)-1 );
    setLock  ( true );
  }


  void  TrackSegment::revalidate ( bool invalidEvent )
  {
    _created = false;
    ltrace(148) << "revalidate() - " << this << endl;

    _base->getCanonical ( _sourceU, _targetU );
    _data->update ();
    if ( invalidEvent )
      _data->invalidate ( true, true );

    if ( _track ) Session::addSortEvent ( _track );
    _revalidated = true;
  }


  void  TrackSegment::setAxis ( DbU::Unit axis, unsigned int flags  )
  {
    _base->setAxis ( axis, flags );
    invalidate ();
  }


  void   TrackSegment::swapTrack ( TrackElement* other )
  {
    if ( not other ) return;

    ltrace(200) << "TrackSegment::swapTrack()" << endl;

    size_t  thisIndex   = getIndex ();
    Track*  thisTrack   = getTrack ();
    size_t  otherIndex  = other->getIndex ();
    Track*  otherTrack  = other->getTrack ();

    if ( _track and otherTrack and (_track != otherTrack) ) {
      cerr << Error("TrackSegment::swapTrack() - swapping TrackSement from different tracks.") << endl;
    }

    // detach ();
    // other->detach ();
    // if ( thisTrack  ) thisTrack ->insert ( other );
    // if ( otherTrack ) otherTrack->insert ( this  );

    setTrack ( NULL );
    other->setTrack ( NULL );

  //other->setRouted ( thisRouted );
    other->setTrack  ( thisTrack );
    other->setIndex  ( thisIndex );
    if ( thisTrack ) thisTrack->setSegment ( other, thisIndex );

  //setRouted ( otherRouted );
    setTrack  ( otherTrack );
    setIndex  ( otherIndex );
    if ( _track ) _track->setSegment ( this, _index );

#if defined(CHECK_DATABASE_DISABLED)
    if ( _track )
      _track->_check();
    else if ( other->getTrack() )
      other->getTrack()->_check();
#endif

    RoutingEvent* thisEvent  = getDataNegociate(TrackElement::DataSelf)->getRoutingEvent();
    RoutingEvent* otherEvent = other->getDataNegociate()->getRoutingEvent();

    if ( thisEvent  ) thisEvent ->setSegment ( other );
    if ( otherEvent ) otherEvent->setSegment ( this );

    ltrace(200) << "| this:  " << this << endl;
    ltrace(200) << "| other: " << other << endl;
  }


  void  TrackSegment::reschedule ( unsigned int level )
  {
    ltrace(200) << "TrackSegment::reschedule() - " << this << endl;
    ltracein(200);

    if ( not _data or not _data->hasRoutingEvent() )
      Session::getNegociateWindow()->addInsertEvent ( this, level );
    else {
      if ( _track != NULL )
        Session::addRemoveEvent ( this );
      Session::getNegociateWindow()->rescheduleEvent ( _data->getRoutingEvent(), level );
    }

    ltraceout(200);
  }


  void  TrackSegment::slacken ()
  {
    if ( not isSlackened() ) {
      ltrace(200) << "TrackSegment::slacken()" << endl;
      ltracein(200);

      base()->slacken ( true );
      _postModify ();
      
      ltraceout(200);
    } else
      throw Bug("TrackSegment::slacken(): NULL base or already slackened.");
  }


  float  TrackSegment::getMaxUnderDensity ( unsigned int flags ) const
  { return _base->getMaxUnderDensity ( flags ); }


  bool  TrackSegment::canPivotUp ( float reserve ) const
  { return _base->canPivotUp(reserve); }


  bool  TrackSegment::canPivotDown ( float reserve ) const
  { return _base->canPivotDown(reserve); }


  bool  TrackSegment::canMoveUp ( float reserve, unsigned int flags ) const
  { return _base->canMoveUp ( reserve, flags ); }


  bool  TrackSegment::moveUp ( unsigned int flags )
  {
    bool success = false;

    ltrace(200) << "TrackSegment::moveUp() " << flags << endl;
    ltracein(200);

    success = base()->moveUp ( flags );

    const vector<AutoSegment*>& invalidateds = Session::getInvalidateds();
    if ( not invalidateds.empty() ) {
      vector<TrackElement*> segments;
      for ( size_t i=0 ; i<invalidateds.size() ; i++ ) {
        TrackElement* segment = Session::getNegociateWindow()->addTrackSegment(invalidateds[i],false);
        if ( segment != NULL ) {
          ltrace(200) << "moved up: " << invalidateds[i] << endl;
          segments.push_back ( segment );
        // if (  (segment->getTrack() == NULL)
        //    or (segment->getLayer() != segment->getTrack()->getLayer()) )
          segment->reschedule ( 0 );
        }
      }
      for ( size_t i=0 ; i<segments.size() ; i++ ) {
        ltrace(200) << "moved up: " << segments[i] << endl;
      }
    }

    // if ( _data and success ) {
    //   _data->setState ( DataNegociate::ConflictSolve1, true );
    //   _data->resetRipupCount ();
    // }
      
    ltraceout(200);

    return success;
  }


  bool  TrackSegment::moveDown ( unsigned int flags )
  {
    bool success = false;

    ltrace(200) << "TrackSegment::moveDown() " << flags << endl;
    ltracein(200);

    success = base()->moveDown ( flags );

    const vector<AutoSegment*>& invalidateds = Session::getInvalidateds();
    if ( not invalidateds.empty() ) {
      vector<TrackElement*> segments;
      for ( size_t i=0 ; i<invalidateds.size() ; i++ ) {
        TrackElement* segment = Session::getNegociateWindow()->addTrackSegment(invalidateds[i],false);
        if ( segment != NULL ) {
          ltrace(200) << "moved down: " << invalidateds[i] << endl;
          segments.push_back ( segment );
        // if (  (segment->getTrack() == NULL)
        //    or (segment->getLayer() != segment->getTrack()->getLayer()) )
          segment->reschedule ( 0 );
        }
      }
      for ( size_t i=0 ; i<segments.size() ; i++ ) {
        ltrace(200) << "moved down: " << segments[i] << endl;
      }
    }

    // if ( _data and success ) {
    //   _data->setState ( DataNegociate::ConflictSolve1, true );
    //   _data->resetRipupCount ();
    // }
      
    ltraceout(200);

    return success;
  }


  bool  TrackSegment::moveAside ( bool onLeft )
  {
    bool success = true;

    ltrace(200) << "TrackSegment::moveAside() - " << (onLeft?"left":"right") << endl;
    ltracein(200);

    if ( onLeft ) base()->moveULeft  ();
    else          base()->moveURight ();

    const vector<AutoSegment*>& invalidateds = Session::getInvalidateds();
    if ( not invalidateds.empty() ) {
      vector<TrackElement*> segments;
      for ( size_t i=0 ; i<invalidateds.size() ; i++ ) {
        ltrace(200) << "moved: " << invalidateds[i] << endl;
        segments.push_back ( Session::getNegociateWindow()->addTrackSegment(invalidateds[i],false) );
        segments.back()->reschedule ( 0 );
      }

      for ( size_t i=0 ; i<segments.size() ; i++ ) {
        ltrace(200) << "moved: " << segments[i] << endl;
      }
    }
      
    ltraceout(200);

    return success;
  }


  TrackElement* TrackSegment::getSourceDogLeg ()
  {
    if ( not hasSourceDogLeg() ) return NULL;
   
    unsigned int  direction = Constant::perpandicular ( getDirection() );
    TrackElement* dogleg    = NULL;
    forEach ( Segment*, isegment, base()->getAutoSource()->getSlaveComponents().getSubSet<Segment*>() ) {
      dogleg = Session::lookup ( *isegment );
      if ( dogleg and (dogleg->getDirection() == direction) ) {
        ltrace(200) << "Source dogleg: " << dogleg << endl;
        return dogleg;
      }
    }
    return NULL;
  }


  TrackElement* TrackSegment::getTargetDogLeg ()
  {
    if ( not hasSourceDogLeg() ) return NULL;
   
    unsigned int  direction = Constant::perpandicular ( getDirection() );
    TrackElement* dogleg    = NULL;
    forEach ( Segment*, isegment, base()->getAutoTarget()->getSlaveComponents().getSubSet<Segment*>() ) {
      dogleg = Session::lookup ( *isegment );
      if ( dogleg and (dogleg->getDirection() == direction) ) {
        ltrace(200) << "Target dogleg: " << dogleg << endl;
        return dogleg;
      }
    }
    return NULL;
  }


  bool  TrackSegment::canDogLeg ( Interval interval )
  {
    ltrace(200) << "TrackSegment::canDogLeg(Interval) " << interval << endl;

    if ( isFixed() ) {
      ltrace(200) << "Failed: is fixed" << endl;
      return false;
    }

    if ( not isLocal() ) {
      ltrace(200) << "Failed: is not local" << endl;
      return false;
    }

    if ( hasSourceDogLeg() or hasTargetDogLeg() or isSlackened() ) {
      ltrace(200) << "Failed: already has source and/or target dogleg or slackened." << endl;
      return false;
    }

    return _base->canDogLeg(interval);
  }


  TrackElement* TrackSegment::makeDogLeg ( Interval interval, bool& leftDogleg )
  {
    ltrace(200) << "TrackSegment::makeDogLeg(Interval)" << endl;

    bool upLayer = (Session::getRoutingGauge()->getLayerDepth(getLayer()) < 2);

    base()->makeDogLeg ( interval, upLayer, leftDogleg );

    return _postDogLeg ();
  }


  bool  TrackSegment::canDogLeg ()
  {
    ltrace(200) << "TrackSegment::canDogLeg()" << endl;

    if ( not isLocal() ) {
      ltrace(200) << "Failed: is not local." << endl;
      return false;
    }

    if ( isFixed() ) {
      ltrace(200) << "Failed: is fixed." << endl;
      return false;
    }

    if ( isSlackened() ) {
      ltrace(200) << "Failed: is local & slackened." << endl;
      return false;
    }

    if ( hasSourceDogLeg() || hasTargetDogLeg() ) return false;

    return true;
  }


  TrackElement* TrackSegment::makeDogLeg ()
  {
    Katabatic::AutoContact* source = _base->getAutoSource();
    Katabatic::AutoContact* target = _base->getAutoTarget();
    Katabatic::GCell*       gcell  = _base->getAutoSource()->getGCell();

    TrackElement* dogleg = makeDogLeg ( gcell );

    if ( dogleg ) {
      if ( source->isTerminal() xor target->isTerminal() ) {
        if ( target->isTerminal() )
          source = target;

        DbU::Unit axis = (_base->isHorizontal()) ? source->getX() : source->getY();

        ltrace(200) << "Setting dogleg axis @" << DbU::getValueString(axis) << endl;
        dogleg->setAxis ( axis );
      }
      return _postDogLeg();
    }

    return NULL;
  }


  bool  TrackSegment::canDogLegAt ( Katabatic::GCell* dogLegGCell, unsigned int flags )
  {
    ltrace(200) << "TrackSegment::canDogLegAt(GCell*) " << dogLegGCell << endl;

    if ( isFixed() ) {
      ltrace(200) << "Cannot dogleg a fixed segment." << endl;
      return false;
    }

    if ( isLocal() ) {
      if ( hasSourceDogLeg() or hasTargetDogLeg() ) {
        ltrace(200) << "Cannot dogleg again a local segment." << endl;
        return false;
      }
      if ( isSlackened() ) {
        ltrace(200) << "Cannot dogleg a local slackened segment." << endl;
        return false;
      }
    }

    vector<Katabatic::GCell*> gcells;
    getGCells ( gcells );

    ltrace(190) << "Source: " << *gcells.begin () << endl;
    ltrace(190) << "Target: " << *gcells.rbegin() << endl;

    bool isGCellInside = false;
    for ( size_t igcell=0 ; igcell<gcells.size() ; igcell++ ) {
      if ( dogLegGCell != gcells[igcell] ) continue;

      isGCellInside = true;
      if ( igcell == 0 ) {
        if ( hasSourceDogLeg() ) {
          if ( flags & TrackElement::AllowDoglegReuse ) return true;

          ltrace(200) << "Cannot dogleg again in source GCell." << endl;
          return false;
        }
      }

      if ( hasTargetDogLeg() && (igcell == gcells.size()-1) ) {
        if ( flags & TrackElement::AllowDoglegReuse ) return true;

        ltrace(200) << "Cannot dogleg again in target GCell." << endl;
        return false;
      }

      break;
    }

    if ( not isGCellInside ) {
      ltrace(200) << "dogleg GCell is outside segment support (go outside GCell active)." << endl;
      return false;
    }

    return true;
  }


  TrackElement* TrackSegment::makeDogLeg ( Katabatic::GCell* dogLegGCell )
  {
    ltrace(200) << "TrackSegment::makeDogLeg(GCell*)" << endl;
    ltrace(200) << "Break in: " << dogLegGCell << endl;

    bool upLayer = (Session::getRoutingGauge()->getLayerDepth(getLayer()) < 2);

    base()->makeDogLeg ( dogLegGCell, upLayer );

    return _postDogLeg ();
  }


  TrackElement* TrackSegment::_postDogLeg ()
  {
    ltracein(200);

    TrackElement* perpandicular = NULL;

    const vector<AutoSegment*>& dogLegs = Session::getDogLegs();
    if ( not dogLegs.empty() ) {
      vector<TrackElement*> segments;
      for ( size_t i=0 ; i<dogLegs.size() ; i++ ) {
        ltrace(200) << "Looking up: " << dogLegs[i] << endl;

        segments.push_back ( Session::getNegociateWindow()->addTrackSegment(dogLegs[i],false) );

        switch ( i ) {
          case 0:
            segments[i]->setTargetDogLeg();
            segments[i]->getDataNegociate()->resetRipupCount();

            if ( segments[i] != this ) {
              cerr << Error ( "Incoherency in TrackSegment LUT, lookup of %p gives %p instead of %p (this)"
                            , dogLegs[i], segments[i], this
                            ) << endl;
            }
            break;
          case 1:
            perpandicular = segments[i];
            segments[i]->setSourceDogLeg ();
            segments[i]->setTargetDogLeg ();
            segments[i]->setDogLegLevel ( getDogLegLevel()+1 );
            break;
          case 2:
            segments[i]->setSourceDogLeg ();
            break;
        }
      }

    // TO CHECK
    // If the original TrackElement was inserted in a Track, must check
    // if the new bit takes it's place or not.
    //if ( getGCell() != originalGCell ) swapTrack ( segments[2] );

      for ( size_t i=0 ; i<dogLegs.size() ; i++ ) {
        segments[i]->reschedule ( ((i==1) ? 0 : 1) );
      }

      ltrace(200) << "original:  " << segments[0] << endl;
      ltrace(200) << "perpand:   " << segments[1] << endl;
      ltrace(200) << "new paral: " << segments[2] << endl;
    }      

    ltraceout(200);

    return perpandicular;
  }


  bool  TrackSegment::canDesalignate () const
  {
    ltrace(200) << "TrackSegment::canDesalignate()" << endl;

    return _base->canDesalignate();
  }


  void  TrackSegment::desalignate ()
  {
    ltrace(200) << "TrackSegment::desalignate()" << endl;
    ltracein(200);

    _base->desalignate ();
    _postModify ();

    ltraceout(200);
  }


  void  TrackSegment::_postModify ()
  {
    ltrace(200) << "TrackSegment::_postModify()" << endl;
    ltracein(200);

    unsigned int doglegLevel      = getDogLegLevel() + 1;
    unsigned int parallelDir      = getDirection ();
    unsigned int perpandicularDir = Constant::perpandicular ( parallelDir );

    const vector<AutoSegment*>& invalidateds = Session::getInvalidateds();
    vector<TrackElement*>       segments;

    if ( not invalidateds.empty() ) {
      for ( size_t i=0 ; i<invalidateds.size() ; i++ ) {
        ltrace(200) << "invalidated: " << invalidateds[i] << endl;
        TrackElement* segment = Session::getNegociateWindow()->addTrackSegment(invalidateds[i],false);
        if ( segment != NULL )
          segments.push_back ( segment );
      }
    }      

    for ( size_t i=0 ; i<segments.size() ; ++i ) {
      TrackElement* segment = segments[i];
      ltrace(200) << "Post proccess: " << segment << endl;

      if ( segment->isCreated() and (segment->getDirection() == perpandicularDir) ) {
        ltrace(200) << "Increasing dogleg level to: " << doglegLevel << endl;
        segment->setDogLegLevel ( doglegLevel );
      }

      if ( segment->getDirection() == parallelDir ) {
        forEach ( TrackElement*, iperpandicular, segment->getCollapsedPerpandiculars() ) {
          ltrace(200) << "| pp: " << *iperpandicular << endl;
          iperpandicular->reschedule ( 0 );
        }
        segment->reschedule ( 0 );
      }
    }

    ltraceout(200);
  }


  bool  TrackSegment::_check () const
  {
    if ( not base() ) return true;

    bool coherency = true;

    if ( not base()->isCanonical() ) {
      cerr << "[CHECK] " << this << " supporting AutoSegment is not canonical." << endl;
      coherency = false;
    }

    DbU::Unit min;
    DbU::Unit max;
    base()->getCanonical ( min, max );
    if ( getSourceU() != min ) {
      cerr << "[CHECK] " << this << " has bad source position " << DbU::getValueString(min) << "." << endl;
      coherency = false;
    }
    if ( getTargetU() != max ) {
      cerr << "[CHECK] " << this << " has bad target position " << DbU::getValueString(max) << "." << endl;
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
              + ((isSlackened()    ) ? "S" : "-")
              + ((_track           ) ? "T" : "-")
              + ((_canRipple       ) ? "r" : "-")
              + ((_sourceDogLeg    ) ? "s" : "-")
              + ((_targetDogLeg    ) ? "t" : "-");

    s1.insert ( s1.size()-1, s2 );

    return s1;
  }


  Record* TrackSegment::_getRecord () const
  {
    Record* record = TrackElement::_getRecord ();
    record->add ( getSlot ( "_base"       ,  _base        ) );
    record->add ( getSlot ( "_lock"       ,  _lock        ) );
    record->add ( getSlot ( "_revalidated",  _revalidated ) );

    return record;
  }


} // End of Kite namespace.
