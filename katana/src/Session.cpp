// -*- mode: C++; explicit-buffer-name: "Session.cpp<katana>" -*-
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
// |  C++ Module  :       "./Session.cpp"                            |
// +-----------------------------------------------------------------+


#include "hurricane/Bug.h"
#include "hurricane/Point.h"
#include "hurricane/Error.h"
#include "hurricane/RoutingPad.h"
#include "anabatic/AutoContactTerminal.h"
#include "katana/Session.h"
#include "katana/Track.h"
#include "katana/TrackElement.h"
#include "katana/KatanaEngine.h"
#include "katana/RoutingPlane.h"
#include "katana/NegociateWindow.h"


namespace {

  using namespace std;
  using namespace Hurricane;
  using namespace CRL;
  using namespace Katana;
  using Anabatic::perpandicularTo;
  using Anabatic::AutoSegment;
  using Anabatic::AutoContactTerminal;
  using Anabatic::AutoSegments_OnContact;
  using Anabatic::AutoSegments;


  const char* reopenSession = "Katana::Session::_open(): Session already open for %s (internal error).";


  void  metal2protect ( AutoContactTerminal* contact )
  {
    const Layer*  metal2         = Session::getRoutingLayer(1);
    RoutingPlane* metal3plane    = Session::getKatanaEngine()->getRoutingPlaneByIndex( 2 );
    DbU::Unit     metal3axis     = metal3plane->getTrackByPosition( contact->getY() )->getAxis();
    RoutingPad*   rp             = dynamic_cast<RoutingPad*>( contact->getAnchor() );
    DbU::Unit     viaSideProtect = Session::getViaWidth((size_t)0);
    Point         position       ( contact->getX(), metal3axis );

    AutoContact* sourceVia12 = AutoContactTerminal::create( contact->getGCell()
                                                          , rp
                                                          , metal2
                                                          , position
                                                          , viaSideProtect, viaSideProtect
                                                          );
    AutoContact* targetVia12 = AutoContactTerminal::create( contact->getGCell()
                                                          , rp
                                                          , metal2
                                                          , position
                                                          , viaSideProtect, viaSideProtect
                                                          );
    
    AutoSegment* segment = AutoSegment::create( sourceVia12, targetVia12, Flags::Vertical );
    
    sourceVia12->setFlags( Anabatic::CntFixed|Anabatic::CntIgnoreAnchor );
    targetVia12->setFlags( Anabatic::CntFixed|Anabatic::CntIgnoreAnchor );
    segment->setFlags( AutoSegment::SegFixed );

    Session::getNegociateWindow()->createTrackSegment( segment, Flags::NoFlags );
    
    cdebug_log(145,0) << "Hard protect: " << contact << endl;
    cdebug_log(145,0) << "X:" << DbU::getValueString(position.getX())
                      << " Metal3 Track Y:" << DbU::getValueString(metal3axis) << endl;
  }


} // Anonymous namespace.


namespace Katana {

  using std::cerr;
  using std::endl;
  using Hurricane::tab;
  using Hurricane::Error;
  using Hurricane::Bug;
  using Hurricane::Point;


// -------------------------------------------------------------------
// Class  :  "Session".

  Session::Session ( KatanaEngine* katana )
    : Super(katana)
    , _indirectInvalids()
    , _insertEvents    ()
    , _removeEvents    ()
    , _sortEvents      ()
  { }


  void  Session::_postCreate ()
  { Super::_postCreate(); }


  Session::~Session ()
  { }


  void  Session::_preDestroy ()
  {
    Super::_preDestroy();
    _isEmpty();
  }


  Session* Session::_open ( KatanaEngine* katana )
  {
    cdebug_log(159,0) << "Katana::Session::_open()" << endl;

    Session* session = Session::get();
    if (session) {
      if (session->_getKatanaEngine() != katana)
        throw Error( reopenSession, getString(session->getKatanaEngine()).c_str() );

      return session;
    }

    session = new Session ( katana );
    session->_postCreate();

    return session;
  }


  Session* Session::get ( const char* message )
  { return dynamic_cast<Session*>( Super::get(message) ); }


  Configuration* Session::getConfiguration ()
  { return Session::getKatanaEngine()->getConfiguration(); }


  uint32_t  Session::getStage ()
  { return get("getStage()")->_getKatanaEngine()->getStage(); }


  AutoContact* Session::lookup ( Contact* contact )
  { return Super::lookup(contact); }


  TrackElement* Session::lookup ( Segment* segment )
  { return Session::get("Session::lookup(Segment*)")->_getKatanaEngine()->_lookup(segment); }


  TrackElement* Session::lookup ( AutoSegment* segment )
  { return Session::get("lookup(AutoSegment*)")->_getKatanaEngine()->_lookup ( segment ); }


  void  Session::addShortDogleg ( TrackElement* segmentA, TrackElement* segmentB )
  { Session::get("addShortDogleg(AutoSegment*)")->_getKatanaEngine()->_addShortDogleg( segmentA, segmentB ); }

  
  TrackElement* Session::getDoglegPaired ( TrackElement* segment )
  { return Session::get("getDoglegPaired(AutoSegment*)")->_getKatanaEngine()->_getDoglegPaired( segment ); }


  void  Session::setInterrupt ( bool state )
  { Session::get("setInterrupt()")->_getKatanaEngine()->setInterrupt(state); }


  KatanaEngine* Session::_getKatanaEngine ()
  { return static_cast<KatanaEngine*>(_anabatic); }


  Net* Session::_getBlockageNet ()
  { return _getKatanaEngine()->getBlockageNet(); };


  NegociateWindow* Session::_getNegociateWindow ()
  { return _getKatanaEngine()->getNegociateWindow(); };


  uint32_t  Session::_getRipupCost ()
  { return _getKatanaEngine()->getRipupCost(); };


  Anabatic::GCell* Session::_getGCellUnder ( DbU::Unit x, DbU::Unit y )
  { return _getKatanaEngine()->getGCellUnder(Point(x,y)); };


  Interval& Session::_toAxisInterval ( Interval& interval, size_t depth ) const
  {
    RoutingLayerGauge* rg = getLayerGauge(depth);
    if (not rg) return interval;

    Box ab = getKatanaEngine()->getCell()->getAbutmentBox();
    DbU::Unit abMin = (rg->isHorizontal()) ? ab.getYMin() : ab.getXMin();
    DbU::Unit abMax = (rg->isHorizontal()) ? ab.getYMax() : ab.getXMax() ;

    DbU::Unit trackMin = rg->getTrackPosition( abMin, abMax, interval.getVMin(), Constant::Superior );
    DbU::Unit trackMax = rg->getTrackPosition( abMin, abMax, interval.getVMax(), Constant::Inferior );
    interval = Interval( trackMin, trackMax );

    return interval;
  }


  void  Session::_doRemovalEvents ( bool reschedule )
  {
    cdebug_log(159,1) << "Katana::Session::_doRemovalEvents()" << endl;

    TrackSet  packTracks;

    for ( size_t i=0 ; i<_removeEvents.size() ; ++i ) {
      cdebug_log(159,0) << "Remove event for:" << _removeEvents[i]._segment << endl;

      if (not _removeEvents[i]._segment->getTrack()) continue;
      _removeEvents[i]._segment->detach( packTracks );
      if (reschedule) _removeEvents[i]._segment->reschedule( 0 );
    }
    _removeEvents.clear();

    for ( TrackSet::iterator it=packTracks.begin() ; it != packTracks.end() ; ++it )
      (*it)->doRemoval();

    cdebug_tabw(159,-1);
  }


  void  Session::_doLockEvents ()
  {
    for ( size_t i=0 ; i<_lockEvents.size() ; ++i ) {
      AutoContactTerminal* source = dynamic_cast<AutoContactTerminal*>( _lockEvents[i]._segment->base()->getAutoSource() );
      AutoContactTerminal* target = dynamic_cast<AutoContactTerminal*>( _lockEvents[i]._segment->base()->getAutoTarget() );
      if (source and source->canDrag()) metal2protect( source );
      if (target and target->canDrag()) metal2protect( target );
    }
    _lockEvents.clear();
  }


  size_t  Session::_revalidate ()
  {
    cdebug_log(159,1) << "Katana::Session::_revalidate()" << endl;
    uint32_t overlaps = 0;

  //Track* checkTrack = NULL;
  //if (getKatanaEngine()->getRoutingPlaneByIndex( 1 ))
  //  checkTrack = getKatanaEngine()->getRoutingPlaneByIndex( 1 )->getTrackByIndex( 87 );

    _doLockEvents();
    _doRemovalEvents();
  //if (checkTrack) checkTrack->check( overlaps, "Session::_revalidate() - check track 82 @270." );

    for ( const Event& event : _insertEvents ) {
      if (event._segment) {
        if (event._segment->getAxis() != event._axis) event._segment->setAxis( event._axis );
        if (not event._segment->isReduced())          event._track->insert( event._segment );
      }
      if (event._marker) event._track->insert( event._marker );
    }
    _insertEvents.clear();
    _doRemovalEvents( false );
  //if (checkTrack) checkTrack->check( overlaps, "Session::_revalidate() - check track 82 @270." );

  // Check if to be destroyeds are not associateds with TrackSegments.
    const set<AutoSegment*>&          destroyeds = getDestroyeds();
    set<AutoSegment*>::const_iterator idestroyed = destroyeds.begin();
    for ( ; idestroyed != destroyeds.end() ; ++idestroyed ) {
      if (lookup(*idestroyed)) {
        cdebug_tabw(155,-1);
        throw Error( "Destroyed AutoSegment is associated with a TrackSegment\n"
                     "        (%s)"
                   , getString(*idestroyed).c_str());
      }
    }
    
    size_t count = Super::_revalidate();

    Interval                    span;
    const vector<AutoSegment*>& revalidateds     = getRevalidateds();
  //const set<Net*>&            netsModificateds = getNetsModificateds();

    for ( size_t i=0 ; i<revalidateds.size() ; ++i ) {
      if (not revalidateds[i]->isCanonical()) continue;
      
    //Net*          currentNet   = NULL;
      TrackElement* trackSegment = lookup( revalidateds[i] );

      if (trackSegment and trackSegment->isInvalidated()) {
        trackSegment->revalidate();
      }
    }
    _doglegReset();

    for ( Track* track : _sortEvents ) {
      track->doReorder();
      if (cdebug.enabled()) {
        track->check( overlaps, "Session::_revalidate() - track sorting." );
      }
# if defined(CHECK_DATABASE)
      track->check( overlaps, "Session::_revalidate() - track sorting." );
# endif
    }

# if defined(CHECK_DATABASE)
    for ( set<Track*>::iterator it=packTracks.begin() ; it != packTracks.end() ; ++it )
      (*it)->check( overlaps, "Session::_revalidate() - on packed track." );

    for ( size_t i=0 ; i<revalidateds.size() ; ++i ) {
      revalidateds[i]->check();
    }
# endif

    _sortEvents.clear();

  // Looking for reduced/raised segments.
    for ( size_t i=0 ; i<revalidateds.size() ; ++i ) {
      if (revalidateds[i]->mustRaise()) {
        cdebug_log(159,0) << "Session: raise:" << revalidateds[i] << endl;
        revalidateds[i]->raise();
        TrackElement* trackSegment = lookup( revalidateds[i] );
        if (trackSegment) trackSegment->reschedule( 0 );
      }
      // if (revalidateds[i]->isUnderMinLength()) {
      //   cdebug_log(159,0) << "Session: under min length:" << revalidateds[i] << endl;
      //   revalidateds[i]->expandToMinLength();
      //   TrackElement* trackSegment = lookup( revalidateds[i] );
      //   if (trackSegment) {
      //     trackSegment->invalidate();
      //     trackSegment->reschedule( 0 );
      //   }
      // } else {
      //   if (revalidateds[i]->isAtMinArea()) {
      //     if (revalidateds[i]->unexpandToMinLength()) {
      //       TrackElement* trackSegment = lookup( revalidateds[i] );
      //       if (trackSegment) {
      //         trackSegment->invalidate();
      //         trackSegment->reschedule( 0 );
      //       }
      //     }
      //   }
      // }
      if (revalidateds[i]->canReduce()) {
        revalidateds[i]->reduce();
        TrackElement* trackSegment = lookup( revalidateds[i] );
        if (trackSegment and trackSegment->getTrack()) _addRemoveEvent( trackSegment );
        cdebug_log(159,0) << "Session: reduce:" << revalidateds[i] << endl;
      }
    }
    
    // for ( TrackElement* trackSegment : _indirectInvalids ) {
    //   cdebug_log(159,0) << "Indirect reschedule:" << trackSegment << endl;
    //   trackSegment->reschedule( 0 );
    //   // addRemoveEvent( trackSegment );

    //   // if (trackSegment->getDataNegociate() and trackSegment->getDataNegociate()->hasRoutingEvent()) {
    //   //   RoutingEvent* event = trackSegment->getDataNegociate()->getRoutingEvent();
    //   //   if (   not event->isDisabled()
    //   //      and not event->isUnimplemented()
    //   //      and     event->isProcessed()) {
    //   //     trackSegment->reschedule( 0 );
    //   //   }
    //   // }
    // }
    _indirectInvalids.clear();

    _doRemovalEvents();
    for ( Track* track : _sortEvents ) track->doReorder();
    _sortEvents.clear();

    cdebug_tabw(159,-1);
    return count;
  }


  bool  Session::_isEmpty () const
  {
    if ( not _insertEvents.empty() or not _removeEvents.empty() or not _sortEvents.empty() ) {
      cerr << Bug( "Session::_isEmpty() failed :\n"
                   "       %u inserts, %u removes and %u sort events remains."
                 , _insertEvents.size()
                 , _removeEvents.size()
                 , _sortEvents  .size() ) << endl;
      if (not _sortEvents.empty()) {
        cerr << "      Remaining sort events on Tracks:" << endl;
        for ( Track* track : _sortEvents ) {
          cerr << "      | " << track << endl;
        }
      }
      return false;
    }

    return true;
  }


  void  Session::_addInsertEvent ( TrackMarker* marker, Track* track )
  {
    _insertEvents.push_back( Event(marker,track) );
    _addSortEvent( track, true );
  }


  void  Session::_addInsertEvent ( TrackElement* segment, Track* track, DbU::Unit axis, bool check )
  {
    cdebug_log(159,0) <<  "addInsertEvent() " << segment
                      << "\n               @" << DbU::getValueString(axis)
                      << " " << track << endl;

    if ( check and (segment->getTrack() != NULL) ) {
      cerr << Bug("Session::addInsertEvent(): Segment already in Track."
                 "\n      %s."
                 "\n      to %s."
                 ,getString(segment).c_str()
                 ,getString(track).c_str()
                 ) << endl;
      return;
    }

    _insertEvents.push_back( Event(segment,track,axis) );
    _addSortEvent( track, true );
  }


  void  Session::_addRemoveEvent ( TrackElement* segment )
  {
    if (not segment->getTrack()) {
      cerr << Bug( " Katana::Session::addRemoveEvent() : %s is not in any Track."
                 , getString(segment).c_str() ) << endl;
      return;
    }

    cdebug_log(159,0) << "Ripup: @" << DbU::getValueString(segment->getAxis()) << " " << segment << endl;
    _removeEvents.push_back( Event(segment,segment->getTrack(),segment->getAxis()) );
    _addSortEvent( segment->getTrack(), true );
  }


  void  Session::_addLockEvent ( TrackElement* segment )
  {
    if (not segment->isTerminal()) {
      cerr << Bug( " Katana::Session::addLockEvent() : %s is not connected to a terminal."
                 , getString(segment).c_str() ) << endl;
      return;
    }

    cdebug_log(159,0) << "Lock: @" << DbU::getValueString(segment->getAxis()) << " " << segment << endl;
    _lockEvents.push_back( Event(segment,NULL,0) );
  }


  void  Session::_addMoveEvent ( TrackElement* segment, Track* track, DbU::Unit axis )
  {
    if (not segment->getTrack()) {
      if (not segment->isNonPref() and not segment->isReduced()) {
        cerr << Bug( " Katana::Session::addMoveEvent() : %s is not yet in a track."
                   , getString(segment).c_str() ) << endl;
      }
    } else {
      _addRemoveEvent( segment );
    }
    _addInsertEvent( segment, track, axis, false );
  }


  void  Session::_addSortEvent ( Track* track, bool forced )
  {
    if (not track ) {
      cerr << Bug( " Katana::Session::addSortEvent() : no Track to sort." ) << endl;
      return;
    }
    if (forced) track->invalidate();
    for ( Track* elem : _sortEvents ) if (elem == track) return;
    _sortEvents.push_back( track );
  }


  void  Session::setStage ( uint32_t stage )
  { get("setStage()")->_getKatanaEngine()->setStage(stage); }


  string  Session::_getTypeName () const
  { return "Katana::Session"; }


  Record* Session::_getRecord () const
  {
    Record* record = Super::_getRecord ();
    record->add( getSlot( "_sortEvents"  , &_sortEvents ) );
                                     
    return record;
  }


} // Katana namespace.
