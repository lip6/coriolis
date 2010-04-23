
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
// |  C++ Module  :       "./TrackSegment.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include  <sstream>

#include  "hurricane/Bug.h"
#include  "hurricane/Warning.h"
#include  "hurricane/Net.h"
#include  "hurricane/Name.h"
#include  "hurricane/RoutingPad.h"
#include  "katabatic/AutoContact.h"
#include  "crlcore/RoutingGauge.h"
#include  "kite/GCell.h"
#include  "kite/DataNegociate.h"
#include  "kite/TrackSegment.h"
#include  "kite/TrackCost.h"
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
    , _gcell        (NULL)
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
    , _dogLegOrder  (0)
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

    vector<GCell*> gcells;
    getGCells ( gcells );

#if ENABLE_STIFFNESS
    updateGCellsStiffness ( TrackElement::AddToGCells );
#endif
  }


  TrackSegment::~TrackSegment ()
  {
    if ( _data ) delete _data;
  }


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
    if ( !trackElement ) { 
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
  bool           TrackSegment::isCreated                  () const { return _created; }
  bool           TrackSegment::isFixed                    () const { return _base->isFixed(); }
  bool           TrackSegment::isStrap                    () const { return _base->isCanonicalStrap(); }
  bool           TrackSegment::isSlackenStrap             () const { return _base->isSlackenStrap(); }
  bool           TrackSegment::isLocal                    () const { return _base->isCanonicalLocal(); }
  bool           TrackSegment::isGlobal                   () const { return !isLocal(); }
  bool           TrackSegment::isLocked                   () const { return _lock; }
  bool           TrackSegment::isTerminal                 () const { return _base->isTerminal(); }
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
  GCell*         TrackSegment::getGCell                   () const { return _gcell; }
  DbU::Unit      TrackSegment::getAxis                    () const { return _base->getAxis(); }
  unsigned long  TrackSegment::getArea                    () const { return _area; }
  unsigned int   TrackSegment::getDogLegLevel             () const { return _dogLegLevel; }
  unsigned int   TrackSegment::getDogLegOrder             () const { return _dogLegOrder; }
  Interval       TrackSegment::getSourceConstraints       () const { return _base->getSourceConstraints(); }
  Interval       TrackSegment::getTargetConstraints       () const { return _base->getTargetConstraints(); }
  DataNegociate* TrackSegment::getDataNegociate           () const { return _data; }
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


  // DbU::Unit  TrackSegment::getSourceU () const
  // { return _base->getSourceU(); }


  // DbU::Unit  TrackSegment::getTargetU () const
  // { return _base->getTargetU(); }


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


  Interval  TrackSegment::getFreeInterval ( bool useOrder ) const
  {
    if ( !_track ) return Interval(false);

    size_t  begin = _index;
    size_t  end   = _index;

    return _track->expandFreeInterval ( begin, end, Track::Inside, getNet(), useOrder );
  }


  size_t  TrackSegment::getGCells ( vector<GCell*>& gcells ) const
  {
    vector<GCell*>().swap ( gcells );

    GCell* sourceGCell = Session::lookup(base()->getAutoSource()->getGCell());
    GCell* targetGCell = Session::lookup(base()->getAutoTarget()->getGCell());

    ltrace(148) << "getGCells(): sourceGCell: " << sourceGCell << endl;
    ltrace(148) << "getGCells(): targetGCell: " << targetGCell << endl;

    forEach ( AutoSegment*, isegment, base()->getCollapseds() ) {
      GCell* gcell = Session::lookup(isegment->getAutoSource()->getGCell());
      if ( gcell->getIndex() < sourceGCell->getIndex() ) {
        sourceGCell = gcell;
        ltrace(148) << "getGCells(): new sourceGCell: " << sourceGCell << endl;
      }

      gcell = Session::lookup(isegment->getAutoTarget()->getGCell());
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


  void  TrackSegment::setGCell ( GCell* gcell )
  {
    _gcell = gcell;
    if (_data and not (_data->isBorder() or _data->isRing()))
      _data->setGCellOrder( (_gcell) ? _gcell->getOrder() : (unsigned int)-1 );
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


  unsigned int  TrackSegment::getOrder () const
  {
    if ( _data == NULL ) return numeric_limits<unsigned int>::max();
    return _data->getGCellOrder ();
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


  void  TrackSegment::setDogLegOrder ( unsigned int order )
  {
    ltrace(200) << "setDogLegOrder(): " << order << " " << this << endl;

    if ( order > 32768 ) {
      cerr << Bug("%s has reached maximum dog leg order (32768)."
                 ,_getString().c_str()) << endl;
      order = 32768;
    }

    _dogLegOrder = order;
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
  {
    TrackElement::setTrack ( track );
#if ENABLE_STIFFNESS
    updateGCellsStiffness ( ((track != NULL) ? TrackElement::Routed : TrackElement::UnRouted ) );
#endif
  }


  void  TrackSegment::detach ()
  {
    ltrace(200) << "TrackSegment::detach() - <id:" << getId() << ">" << endl;

    setTrack ( NULL );
    setIndex ( (size_t)-1 );
    setLock  ( true );
  //updateGCellsStiffness ( TrackElement::UnRouted );
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


  void  TrackSegment::updateGCellsStiffness ( unsigned int flags )
  {
    if ( Session::getKiteEngine()->getState() > Katabatic::StateDriving ) return;

    vector<GCell*> gcells;
    getGCells ( gcells );

    int count = 0;
    if ( flags & TrackElement::AddToGCells ) {
      if ( getTrack() ) { flags |= TrackSegment::Routed; _routed = false; }
    //cerr << "INC:";
      count = +1;
    }
    if ( flags & TrackElement::RemoveFromGCells ) {
      if ( getTrack() ) { flags |= TrackSegment::UnRouted; _routed = true; }
    //cerr << "DEC:";
      count = -1;
    }
    if ( count ) {
    //cerr << count << " SegmentCount() for " << this << endl;
      for ( size_t i=0 ; i<gcells.size() ; ++i ) {
        gcells[i]->incSegmentCount ( count );
      //cerr << "| " << gcells[i] << endl;
      }
    }

    count = 0;
    if ( not _routed and (flags & TrackElement::Routed  ) ) { count = +1; _routed = true; }
    if (     _routed and (flags & TrackElement::UnRouted) ) { count = -1; _routed = false; }

    if ( count ) {
      for ( size_t i=0 ; i<gcells.size() ; ++i ) {
        gcells[i]->incRoutedCount ( count );
      //cerr << "Update (" << count << ") " << gcells[i] << " for: " << this << endl;
      }
    }
  }


  void   TrackSegment::swapTrack ( TrackElement* other )
  {
    if ( !other ) return;

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
    // if ( _track     )        updateGCellsStiffness ( TrackElement::UnRouted );
    // if ( otherTrack ) other->updateGCellsStiffness ( TrackElement::UnRouted );

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

    RoutingEvent* thisEvent  = getDataNegociate()->getRoutingEvent();
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
    ltrace(200) << "GCell: " << _gcell << endl;
    ltrace(200) << "GCell::order:" << _gcell->getOrder()
                << " Data::order:" << getOrder()
                << " Session::order:" << Session::getOrder() << endl;

    if ( getOrder() == Session::getOrder() ) {
      if ( not _data or not _data->hasRoutingEvent() )
        Session::getNegociateWindow()->addInsertEvent ( this, level );
      else {
        if ( _track != NULL )
          Session::addRemoveEvent ( this );
        Session::getNegociateWindow()->rescheduleEvent ( _data->getRoutingEvent(), level );
      }
    } else {
      if ( _data and _data->hasRoutingEvent() )
        _data->getRoutingEvent()->setDisabled();

      if ( getOrder() > Session::getOrder() ) {
        if ( _track != NULL )
          Session::addRemoveEvent ( this );
      }
    }
    ltraceout(200);
  }


  void  TrackSegment::slacken ()
  {
    if ( not isSlackened() ) {
      ltrace(200) << "TrackSegment::slacken()" << endl;
      ltracein(200);

      // set<AutoSegment*> collapseds;

      // collapseds.insert ( _base );
      // forEach ( AutoSegment*, isegment, _base->getCollapseds() ) {
      //   collapseds.insert ( *isegment );
      // }

    //unsigned int direction   = Constant::perpandicular ( getDirection() );
    //unsigned int doglegLevel = getDogLegLevel() + 1;

    //setSlackened ( true );
#if ENABLE_STIFFNESS
      updateGCellsStiffness ( TrackElement::RemoveFromGCells );
#endif
      base()->slacken ( true );
#if ENABLE_STIFFNESS
      updateGCellsStiffness ( TrackElement::AddToGCells );
#endif

      _postModify ();

      // const vector<AutoSegment*>& invalidateds = Session::getInvalidateds();
      // if ( not invalidateds.empty() ) {
      //   vector<TrackElement*> segments;
      //   for ( size_t i=0 ; i<invalidateds.size() ; i++ ) {
      //     ltrace(200) << "slacken: " << invalidateds[i] << endl;
      //     TrackElement* segment = GCell::addTrackSegment(NULL,invalidateds[i],false);
      //     segments.push_back ( segment );

      //   //if ( segment->base()->isTerminal() or (collapseds.find(segment->base()) != collapseds.end()) )
      //   //  segment->setSlackened ( true );

      //     if ( segment->isCreated() ) {
      //     //segment->setSlackened ( true );
      //       if ( segment->getDirection() == direction ) {
      //         ltrace(200) << "Increasing dogleg level to: " << doglegLevel << endl;
      //         segment->setDogLegLevel ( doglegLevel );

      //         if ( segment->getOrder() > Session::getOrder() ) {
      //           ltrace(200) << "Adding to ring: " << segment << endl;
      //           Session::getNegociateWindow()->addToRing ( segment );
      //         }
      //       }
      //     }

      //     segment->reschedule ( 0 );
      //   }

      //   for ( size_t i=0 ; i<segments.size() ; i++ ) {
      //     ltrace(200) << "slacken: " << segments[i] << endl;
      //   }
      // }
      
      ltraceout(200);
    } else
      throw Bug("TrackSegment::slacken(): NULL base or already slackened.");
  }


  bool  TrackSegment::canPivotUp ( float reserve ) const
  {
    return _base->canPivotUp(reserve);
  }


  bool  TrackSegment::canMoveUp ( float reserve ) const
  {
    // if ( isLocal() /*and (hasSourceDogLeg() or hasTargetDogLeg())*/ ) {
    //   return _base->canPivotUp();
    // }

    return _base->canMoveUp ( true, reserve );
  }


  bool  TrackSegment::moveUp ()
  {
    bool success = false;

    ltrace(160) << "TrackSegment::moveUp()" << endl;
    ltracein(200);

#if ENABLE_STIFFNESS
    updateGCellsStiffness ( TrackElement::RemoveFromGCells );
#endif
    success = base()->moveUp ( true );
#if ENABLE_STIFFNESS
    updateGCellsStiffness ( TrackElement::AddToGCells );
#endif

    const vector<AutoSegment*>& invalidateds = Session::getInvalidateds();
    if ( not invalidateds.empty() ) {
      vector<TrackElement*> segments;
      for ( size_t i=0 ; i<invalidateds.size() ; i++ ) {
        ltrace(200) << "moved: " << invalidateds[i] << endl;
        TrackElement* segment = GCell::addTrackSegment(NULL,invalidateds[i],false);
        segments.push_back ( segment );
        // if (  (segment->getTrack() == NULL)
        //    or (segment->getLayer() != segment->getTrack()->getLayer()) )
        segment->reschedule ( 0 );
      }

      for ( size_t i=0 ; i<segments.size() ; i++ ) {
        ltrace(200) << "moved: " << segments[i] << endl;
      }
    }

    if ( _data ) {
      _data->setState ( DataNegociate::ConflictSolve1, true );
      _data->resetRipupCount ();
    }
      
    ltraceout(200);

    return success;
  }


  bool  TrackSegment::moveAside ( bool onLeft )
  {
    bool success = true;

    ltrace(200) << "TrackSegment::moveAside() - " << (onLeft?"left":"right") << endl;
    ltracein(200);

    unsigned int order = _data->getGCellOrder();

#if ENABLE_STIFFNESS
    updateGCellsStiffness ( TrackElement::RemoveFromGCells );
#endif
    if ( onLeft ) base()->moveULeft  ();
    else          base()->moveURight ();
#if ENABLE_STIFFNESS
    updateGCellsStiffness ( TrackElement::AddToGCells );
#endif

    const vector<AutoSegment*>& invalidateds = Session::getInvalidateds();
    if ( not invalidateds.empty() ) {
      vector<TrackElement*> segments;
      for ( size_t i=0 ; i<invalidateds.size() ; i++ ) {
        ltrace(200) << "moved: " << invalidateds[i] << endl;
        segments.push_back ( GCell::addTrackSegment(NULL,invalidateds[i],false) );
        segments.back()->reschedule ( 0 );
      }

      if ( _data->getGCellOrder() < order ) {
        cinfo << "[INFO] Putting TrackSegment of order " << order
              << " into GCell " << getGCell() << endl;
        _data->setGCellOrder ( order );
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
    if ( !hasSourceDogLeg() ) return NULL;
   
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
    if ( !hasSourceDogLeg() ) return NULL;
   
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

    if ( getDogLegOrder() < Session::getOrder() ) {
      ltrace(200) << "No dogleg in current order " << Session::getOrder()
                  << " yet, allowing (previous:" << _dogLegOrder << ")" << endl;
      setDogLegOrder ( Session::getOrder() );
      setSourceDogLeg ( false );
      setTargetDogLeg ( false );
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

    GCell* originalGCell = getGCell ();

#if ENABLE_STIFFNESS
    updateGCellsStiffness ( TrackElement::RemoveFromGCells );
#endif
    base()->makeDogLeg ( interval, upLayer, leftDogleg );
#if ENABLE_STIFFNESS
    updateGCellsStiffness ( TrackElement::AddToGCells );
#endif

    return _postDogLeg ( originalGCell );
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

    if ( getDogLegOrder() < Session::getOrder() ) {
      ltrace(200) << "No dogleg in current order " << Session::getOrder()
                  << " yet, allowing (previous:" << _dogLegOrder << ")" << endl;
      setDogLegOrder ( Session::getOrder() );
      setSourceDogLeg ( false );
      setTargetDogLeg ( false );
    }
    if ( hasSourceDogLeg() || hasTargetDogLeg() ) return false;

    return true;
  }


  TrackElement* TrackSegment::makeDogLeg ()
  {
    AutoContact* source = _base->getAutoSource();
    AutoContact* target = _base->getAutoTarget();
    GCell*       gcell  = Session::lookup ( _base->getAutoSource()->getGCell() );

#if ENABLE_STIFFNESS
    updateGCellsStiffness ( TrackElement::RemoveFromGCells );
#endif
    TrackElement* dogleg = makeDogLeg ( gcell );
#if ENABLE_STIFFNESS
    updateGCellsStiffness ( TrackElement::AddToGCells );
#endif

    if ( dogleg ) {
      if ( source->isTerminal() xor target->isTerminal() ) {
        if ( target->isTerminal() )
          source = target;

        DbU::Unit axis = (_base->isHorizontal()) ? source->getX() : source->getY();

        ltrace(200) << "Setting dogleg axis @" << DbU::getValueString(axis) << endl;
        dogleg->setAxis ( axis );
      }
      return _postDogLeg(gcell);
    }

    return NULL;
  }


  bool  TrackSegment::canDogLegAt ( GCell* dogLegGCell, bool allowReuse )
  {
    ltrace(200) << "TrackSegment::canDogLegAt(GCell*) " << dogLegGCell << endl;

    if ( getDogLegOrder() < Session::getOrder() ) {
      ltrace(200) << "No dogleg in current order " << Session::getOrder()
                  << " yet, allowing (previous:" << _dogLegOrder << ")" << endl;
      setDogLegOrder ( Session::getOrder() );
      setSourceDogLeg ( false );
      setTargetDogLeg ( false );
    }

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
//     if ( Session::getRoutingGauge()->getLayerDepth(getLayer()) > 3 ) {
//       ltrace(200) << "Cannot dogleg on the last top layer." << endl;
//       return false;
//     }

    vector<GCell*> gcells;
    getGCells ( gcells );

    ltrace(190) << "Source: " << *gcells.begin () << endl;
    ltrace(190) << "Target: " << *gcells.rbegin() << endl;

    bool isGCellInside = false;
    for ( size_t igcell=0 ; igcell<gcells.size() ; igcell++ ) {
      if ( dogLegGCell != gcells[igcell] ) continue;

      isGCellInside = true;
      if ( igcell == 0 ) {
        if ( hasSourceDogLeg() ) {
          if ( allowReuse ) return true;

          ltrace(200) << "Cannot dogleg again in source GCell." << endl;
          return false;
        }
      }

      if ( hasTargetDogLeg() && (igcell == gcells.size()-1) ) {
        if ( allowReuse ) return true;

        ltrace(200) << "Cannot dogleg again in target GCell." << endl;
        return false;
      }

    //if ( dogLegGCell->getUSide(getDirection(),false).intersect(getCanonicalInterval()) ) {
    //  ltrace(200) << "Segment is almost outside the breaking GCell." << endl;
    // return false;
    //}

      break;
    }

    if ( not isGCellInside ) {
      ltrace(200) << "dogleg GCell is outside segment support (go outside GCell active)." << endl;
      return false;
    }

    return true;
  }


  TrackElement* TrackSegment::makeDogLeg ( GCell* dogLegGCell )
  {
    ltrace(200) << "TrackSegment::makeDogLeg(GCell*)" << endl;
    ltrace(200) << "Break in: " << dogLegGCell << endl;

    bool upLayer = (Session::getRoutingGauge()->getLayerDepth(getLayer()) < 2);

    GCell* originalGCell = getGCell ();

#if ENABLE_STIFFNESS
    updateGCellsStiffness ( TrackElement::RemoveFromGCells );
#endif
    base()->makeDogLeg ( dogLegGCell->base(), upLayer );
#if ENABLE_STIFFNESS
    updateGCellsStiffness ( TrackElement::AddToGCells );
#endif

    return _postDogLeg ( originalGCell );
  }


  TrackElement* TrackSegment::_postDogLeg ( GCell* originalGCell )
  {
    ltracein(200);

    TrackElement* perpandicular = NULL;

    const vector<AutoSegment*>& dogLegs = Session::getDogLegs();
    if ( not dogLegs.empty() ) {
      vector<TrackElement*> segments;
      for ( size_t i=0 ; i<dogLegs.size() ; i++ ) {
        ltrace(200) << "Looking up: " << dogLegs[i] << endl;

        segments.push_back ( GCell::addTrackSegment(NULL,dogLegs[i]) );

      //if ( isSlackened() /*&& (segments[i]->base() == dogLegs[i])*/ )
      //  segments[i]->setSlackened (true);

        segments[i]->setDogLegOrder ( Session::getOrder() );

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

      ltrace(200) << "Before break: " << originalGCell << endl;
      ltrace(200) << "After break:  " << getGCell() << endl;
      if ( getGCell() != originalGCell ) swapTrack ( segments[2] );

      for ( size_t i=0 ; i<dogLegs.size() ; i++ ) {
        segments[i]->reschedule ( ((i==1) ? 0 : 1) );
        if ( i == 1 ) {
          ltrace(200) << "Set canDogleg: dogleg:" << segments[1]->getDataNegociate()->getGCellOrder()
                      << " vs. session:" << Session::getOrder() << endl;

          if ( segments[1]->getOrder() > Session::getOrder()) {
            segments[i]->setCanRipple ( true );
            ltrace(200) << "Adding to ring: " << segments[i] << endl;
            Session::getNegociateWindow()->addToRing ( segments[i] );
          }
        } else {
          if ( segments[i]->getOrder() > Session::getOrder()) {
            AutoContact* source = segments[i]->base()->getAutoSource();
            AutoContact* target = segments[i]->base()->getAutoTarget();
            if (  (source and dynamic_cast<RoutingPad*>(source->getAnchor()))
               or (target and dynamic_cast<RoutingPad*>(target->getAnchor()))) {
              ltrace(200) << "Adding to ring: " << segments[i] << endl;
              Session::getNegociateWindow()->addToRing ( segments[i] );
            }
          }
        }
      //if ( i == 1 ) {
      //  RoutingEvent* event = segments[i]->getDataNegociate()->getRoutingEvent();
      //  if ( event )
      //    event->setCanGoOutsideGCell ( true );
      //}
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

//     Interval baseSpan = _base->getSpanU ();
//     if ( baseSpan.getVMin() - Session::getExtensionCap() > _sourceCanonical ) return _base->canDesalignate();   
//     if ( baseSpan.getVMax() + Session::getExtensionCap() < _targetCanonical ) return _base->canDesalignate();   

//     return false;
  }


  void  TrackSegment::desalignate ()
  {
    ltrace(200) << "TrackSegment::desalignate()" << endl;
    ltracein(200);

#if ENABLE_STIFFNESS
    updateGCellsStiffness ( TrackElement::RemoveFromGCells );
#endif

    _base->desalignate ();

#if ENABLE_STIFFNESS
    updateGCellsStiffness ( TrackElement::AddToGCells );
#endif

    _postModify ();

    // const vector<AutoSegment*>& invalidateds = Session::getInvalidateds();
    // if ( not invalidateds.empty() ) {
    //   vector<TrackElement*> segments;
    //   for ( size_t i=0 ; i<invalidateds.size() ; i++ ) {
    //     ltrace(200) << "desaligned: " << invalidateds[i] << endl;
    //     segments.push_back ( GCell::addTrackSegment(NULL,invalidateds[i],false) );
    //     segments[i]->reschedule ( 0 );

    //     if ( segments[i]->getDataNegociate()->getGCellOrder() > Session::getOrder() ) {
    //       ltrace(200) << "Adding to ring: " << segments[i] << endl;
    //       Session::getNegociateWindow()->addToRing ( segments[i] );
    //     }
    //   }

    //   for ( size_t i=0 ; i<segments.size() ; i++ ) {
    //     ltrace(200) << "desaligned: " << segments[i] << endl;
    //   }
    // }      

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
        TrackElement* segment = GCell::addTrackSegment(NULL,invalidateds[i],false);
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

      if ( segment->getDataNegociate()->getGCellOrder() == Session::getOrder() ) {
        if ( segment->getDirection() == parallelDir ) {
          forEach ( TrackElement*, iperpandicular, segment->getCollapsedPerpandiculars() ) {
            ltrace(200) << "| pp: " << *iperpandicular << endl;

            if ( iperpandicular->getDataNegociate()->getGCellOrder() == Session::getOrder() ) {
              iperpandicular->reschedule ( 0 );
            } else if ( iperpandicular->getDataNegociate()->getGCellOrder() > Session::getOrder() ) {
              ltrace(200) << "Adding to ring: " << *iperpandicular << endl;
              Session::getNegociateWindow()->addToRing ( *iperpandicular );
            }
          }
          segment->reschedule ( 0 );
        }
      } else if ( segment->getDataNegociate()->getGCellOrder() > Session::getOrder()) {
        if ( segment->getDirection() == parallelDir ) {
          AutoContact* source = segment->base()->getAutoSource();
          AutoContact* target = segment->base()->getAutoTarget();
          if (  (source and dynamic_cast<RoutingPad*>(source->getAnchor()))
             or (target and dynamic_cast<RoutingPad*>(target->getAnchor()))) {
            ltrace(200) << "Adding to ring: " << segment << endl;
            Session::getNegociateWindow()->addToRing ( segment );
          }
        }
      }
    }

    ltraceout(200);
  }


  bool  TrackSegment::_check () const
  {
    if ( !base() ) return true;

    bool coherency = true;

    if ( !base()->isCanonical() ) {
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
#if ENABLE_STIFFNESS
    if ( _routed xor (_track != NULL) ) {
      cerr << "[CHECK] " << this << " incoherency between routed/track flags." << endl;
    }
#endif

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
              +  " o:" + getString(_data->getGCellOrder())
              + " ["   + ((_track) ? getString(_index) : "npos") + "] "
              + ((isSlackened()    ) ? "S" : "-")
              + ((_track           ) ? "T" : "-")
              + ((_canRipple       ) ? "r" : "-")
              + ((_data->isBorder()) ? "B" : "-")
              + ((_data->isRing  ()) ? "R" : "-")
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
