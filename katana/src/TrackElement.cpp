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
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./TrackElement.cpp"                       |
// +-----------------------------------------------------------------+


#include <limits>
#include <sstream>
#include "hurricane/Bug.h"
#include "hurricane/Warning.h"
#include "hurricane/Net.h"
#include "hurricane/Name.h"
#include "anabatic/AutoContact.h"
#include "anabatic/GCell.h"
#include "crlcore/RoutingGauge.h"
#include "katana/DataNegociate.h"
#include "katana/TrackElement.h"
#include "katana/TrackCost.h"
#include "katana/Track.h"
#include "katana/Session.h"
#include "katana/RoutingEvent.h"
#include "katana/NegociateWindow.h"


namespace {

  using namespace std;
  using namespace Hurricane;
  using namespace CRL;
  using namespace Katana;


  void  dummyOverlapCost ( const TrackElement* segment, TrackCost& cost )
  {
    cerr << Warning("No overlapCost callback has been set (%s)."
                   ,getString(segment).c_str()) << endl;
  }


} // Anonymous namespace.


namespace Katana {

  using Hurricane::tab;
  using Hurricane::Bug;
  using Hurricane::Net;
  using Hurricane::Name;
  using Anabatic::GCell;


// -------------------------------------------------------------------
// Comparison Classes.
//
// Return:  lhs < rhs.


  bool  Compare::operator() ( TrackElement* lhs, TrackElement* rhs )
  { return lhs->getFreedomDegree() > rhs->getFreedomDegree(); }


  bool  CompareByPosition::operator() ( const TrackElement* lhs, const TrackElement* rhs ) const
  {
    if (lhs == rhs) return false;

    if (lhs->isBlockage() xor rhs->isBlockage()) return lhs->isBlockage();

    if (lhs->getLength() < rhs->getLength()) return true;
    if (lhs->getLength() > rhs->getLength()) return false;

    if (lhs->isHorizontal() xor rhs->isHorizontal()) return rhs->isHorizontal();

    if (lhs->getAxis() > rhs->getAxis()) return true;
    if (lhs->getAxis() < rhs->getAxis()) return false;

    if (lhs->getSourceU() > rhs->getSourceU()) return true;
    if (lhs->getSourceU() < rhs->getSourceU()) return false;

    if (lhs->isBlockage() and rhs->isBlockage()) return false;

    return lhs->getId() < rhs->getId();
  }


// -------------------------------------------------------------------
// Class  :  "TrackElement".


  SegmentOverlapCostCB* TrackElement::_overlapCostCallback = dummyOverlapCost;


  SegmentOverlapCostCB* TrackElement::setOverlapCostCB ( SegmentOverlapCostCB* cb )
  {
    SegmentOverlapCostCB* oldCb = _overlapCostCallback;
    _overlapCostCallback = cb;
    return oldCb;
  }


  void  TrackElement::notify ( TrackElement* segment, unsigned int flags )
  {
    if (flags & AutoSegment::Invalidate) {
      if (not segment->isInvalidated()) {
        cdebug_log(159,0) << "::notify() <Invalidate> on " << segment << endl;
        segment->invalidate();
      }
    }

    if (flags & AutoSegment::Revalidate) {
    // Revalidation must be delayed until *all* the AutoSegments have been revalidated.
    // if (segment->isInvalidated()) {
    //   cdebug_log(159,0) << "::notify() <Revalidate> on " << segment << endl;
    //   segment->revalidate( true );
    // }
    }

    if (flags & AutoSegment::RevalidatePPitch) {
      segment->updatePPitch();
    }
  }


// Wrapped AutoSegment Functions.
  AutoSegment*   TrackElement::base                 () const { return NULL; }
  bool           TrackElement::isFixed              () const { return false; }
  bool           TrackElement::isLocal              () const { return true; }
  bool           TrackElement::isGlobal             () const { return not isLocal(); }
  bool           TrackElement::isBipoint            () const { return false; }
  bool           TrackElement::isTerminal           () const { return false; }
  bool           TrackElement::isStrongTerminal     ( Flags ) const { return false; }
  bool           TrackElement::isStrap              () const { return false; }
  bool           TrackElement::isSlackened          () const { return false; }
  bool           TrackElement::isDogleg             () const { return false; }
  bool           TrackElement::isReduced            () const { return false; }
  bool           TrackElement::isUTurn              () const { return false; }
  bool           TrackElement::isUserDefined        () const { return false; }
  bool           TrackElement::isAnalog             () const { return false; }
  bool           TrackElement::isWide               () const { return false; }
// Predicates.
  bool           TrackElement::hasSymmetric         () const { return false; }
  bool           TrackElement::canSlacken           () const { return false; }
  bool           TrackElement::canPivotUp           ( float, Flags ) const { return false; };
  bool           TrackElement::canPivotDown         ( float, Flags ) const { return false; };
  bool           TrackElement::canMoveUp            ( float, Flags ) const { return false; };
  bool           TrackElement::canDogleg            () { return false; };
  bool           TrackElement::canDogleg            ( Interval ) { return false; };
  bool           TrackElement::canDogleg            ( Anabatic::GCell*, Flags ) { return false; };
// Accessors.
  unsigned long  TrackElement::getId                () const { return 0; }
  unsigned long  TrackElement::getFreedomDegree     () const { return 0; }
  uint32_t       TrackElement::getTrackCount        () const { return 0; }
  DbU::Unit      TrackElement::getPitch             () const { return 0; }
  DbU::Unit      TrackElement::getPPitch            () const { return 0; }
  float          TrackElement::getMaxUnderDensity   ( Flags ) const { return 0.0; };
  uint32_t       TrackElement::getDoglegLevel       () const { return 0; }
  TrackElement*  TrackElement::getParent            () const { return NULL; }
  Interval       TrackElement::getSourceConstraints () const { return Interval(); }
  Interval       TrackElement::getTargetConstraints () const { return Interval(); }
  DataNegociate* TrackElement::getDataNegociate     ( Flags ) const { return NULL; }
  TrackElements  TrackElement::getPerpandiculars    () { return new TrackElements_Perpandiculars(NULL); }
  void           TrackElement::invalidate           () { }
  TrackElement*  TrackElement::getCanonical         ( Interval& i ) { i=Interval(getSourceU(),getTargetU()); return this; }
  TrackElement*  TrackElement::getSourceDogleg      () { return NULL; }
  TrackElement*  TrackElement::getTargetDogleg      () { return NULL; }
  TrackElement*  TrackElement::getSymmetric         () { return NULL; }
// Mutators.
  void           TrackElement::addTrackCount        ( int32_t ) { }
  void           TrackElement::setTrack             ( Track* track ) { _track = track; }
  void           TrackElement::setSymmetric         ( TrackElement* ) { }
  void           TrackElement::updateFreedomDegree  () { }
  void           TrackElement::setDoglegLevel       ( uint32_t ) { }
  void           TrackElement::swapTrack            ( TrackElement* ) { }
  void           TrackElement::reschedule           ( uint32_t ) { }
  void           TrackElement::detach               () { }
  void           TrackElement::detach               ( set<Track*>& ) { }
  void           TrackElement::revalidate           () { }
  void           TrackElement::updatePPitch         () { }
  void           TrackElement::setAxis              ( DbU::Unit, uint32_t flags ) { }
  TrackElement*  TrackElement::makeDogleg           () { return NULL; }
  TrackElement*  TrackElement::makeDogleg           ( Interval, Flags&  ) { return NULL; }
  TrackElement*  TrackElement::makeDogleg           ( Anabatic::GCell*, TrackElement*&, TrackElement*& ) { return NULL; }
  void           TrackElement::_postDoglegs         ( TrackElement*&, TrackElement*& ) { }
  bool           TrackElement::moveAside            ( Flags ) { return false; }
  bool           TrackElement::slacken              ( Flags ) { return false; }
  bool           TrackElement::moveUp               ( Flags ) { return false; }
  bool           TrackElement::moveDown             ( Flags ) { return false; }
#if THIS_IS_DISABLED
  void           TrackElement::desalignate          () { }
#endif
  bool           TrackElement::_check               () const { return true; }


  TrackElement::TrackElement ( Track* track )
    : _flags   (0)
    , _track   (track)
    , _sourceU (0)
    , _targetU (0)
    , _observer(this)
  { }


  void  TrackElement::_postCreate ()
  { }


  TrackElement::~TrackElement ()
  { }


  void  TrackElement::_preDestroy ()
  { }


  void  TrackElement::destroy ()
  {
    _preDestroy ();
    delete this;
  }


  TrackElement* TrackElement::getNext () const
  {
    size_t dummy = _track->find( this );
    return _track->getNext( dummy, getNet() );
  }


  TrackElement* TrackElement::getPrevious () const
  {
    size_t dummy = _track->find( this );
    return _track->getPrevious( dummy, getNet() );
  }


  Interval  TrackElement::getFreeInterval () const
  {
    if (not _track) return Interval(false);

    size_t  begin = _track->find( this );
    size_t  end   = begin;
    return _track->expandFreeInterval( begin, end, Track::InsideElement, getNet() );
  }


  size_t  TrackElement::getGCells ( vector<GCell*>& gcells ) const
  {
    vector<GCell*>().swap( gcells );
    return gcells.size();
  }


  void  TrackElement::incOverlapCost ( TrackCost& cost ) const
  {
    if (not _track or (getNet() == cost.getNet())) return;
    _overlapCostCallback( this, cost );
  }


  string  TrackElement::_getTypeName () const
  { return "TrackElement"; }


  string  TrackElement::_getString () const
  { return "<"+_getTypeName()+">"; }


  Record* TrackElement::_getRecord () const
  {
    Record* record = new Record( _getString() );
    record->add( getSlot( "_flags",  _track ) );
    record->add( getSlot( "_track",  _track ) );
    record->add( DbU::getValueSlot( "_sourceU",  &_sourceU ) );
    record->add( DbU::getValueSlot( "_targetU",  &_targetU ) );

    return record;
  }


} // Katana namespace.
