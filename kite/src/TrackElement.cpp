
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
// |  C++ Module  :       "./TrackElement.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include  <limits>
#include  <sstream>

#include  "hurricane/Bug.h"
#include  "hurricane/Warning.h"
#include  "hurricane/Net.h"
#include  "hurricane/Name.h"
#include  "katabatic/AutoContact.h"
#include  "katabatic/GCell.h"
#include  "crlcore/RoutingGauge.h"
#include  "kite/DataNegociate.h"
#include  "kite/TrackElement.h"
#include  "kite/TrackCost.h"
#include  "kite/Track.h"
#include  "kite/Session.h"
#include  "kite/RoutingEvent.h"
#include  "kite/NegociateWindow.h"



namespace {


  using namespace std;
  using namespace Hurricane;
  using namespace CRL;
  using namespace Kite;


  // ---------------------------------------------------------------
  // Function  :  "DummyOverlapCost ()".


  void  DummyOverlapCost ( const TrackElement* segment, TrackCost& cost )
  {
    cerr << Warning("No overlapCost callback has been set (%s)."
                   ,getString(segment).c_str()) << endl;
  }


} // End of local namespace.


namespace Kite {


  using Hurricane::Bug;
  using Hurricane::Net;
  using Hurricane::Name;
  using Katabatic::GCell;


// -------------------------------------------------------------------
// Class  :  "TrackElement::Compare".


  bool  TrackElement::Compare::operator() ( TrackElement* lhs, TrackElement* rhs )
  {
    return lhs->getArea() > rhs->getArea();
  }


// -------------------------------------------------------------------
// Class  :  "TrackElement::CompareByPosition".
//
// Return:  lhs < rhs.


  bool  TrackElement::CompareByPosition::operator() ( const TrackElement* lhs, const TrackElement* rhs ) const
  {
    if ( lhs == rhs ) return false;

    if ( lhs->isBlockage() xor rhs->isBlockage() )
      return lhs->isBlockage();

    if ( lhs->getLength() < rhs->getLength() ) return true;
    if ( lhs->getLength() > rhs->getLength() ) return false;

    if ( lhs->isHorizontal() xor rhs->isHorizontal() )
      return rhs->isHorizontal();

    if ( lhs->getAxis() > rhs->getAxis() ) return true;
    if ( lhs->getAxis() < rhs->getAxis() ) return false;

    if ( lhs->getSourceU() > rhs->getSourceU() ) return true;
    if ( lhs->getSourceU() < rhs->getSourceU() ) return false;

    if ( lhs->isBlockage() and rhs->isBlockage() ) return false;

  //return lhs->getNet()->getName() < rhs->getNet()->getName();
    return lhs->getId() < rhs->getId();
  }


// -------------------------------------------------------------------
// Class  :  "TrackElement".


  SegmentOverlapCostCB* TrackElement::_overlapCostCallback = DummyOverlapCost;


  SegmentOverlapCostCB* TrackElement::setOverlapCostCB ( SegmentOverlapCostCB* cb )
  {
    SegmentOverlapCostCB* oldCb = _overlapCostCallback;

    _overlapCostCallback = cb;

    return oldCb;
  }


// Former inline functions.
  AutoSegment*   TrackElement::base                       () const { return NULL; }
  bool           TrackElement::isCreated                  () const { return false; }
  bool           TrackElement::isFixed                    () const { return false; }
  bool           TrackElement::isBlockage                 () const { return false; }
  bool           TrackElement::isStrap                    () const { return false; }
  bool           TrackElement::isSlackenStrap             () const { return false; }
  bool           TrackElement::isLocal                    () const { return true; }
  bool           TrackElement::isGlobal                   () const { return not isLocal(); }
  bool           TrackElement::isLocked                   () const { return false; }
  bool           TrackElement::isTerminal                 () const { return false; }
  bool           TrackElement::isRevalidated              () const { return false; }
  bool           TrackElement::isRouted                   () const { return true; }
  bool           TrackElement::isSlackened                () const { return false; }
  bool           TrackElement::isSlackenDogLeg            () const { return false; }
  bool           TrackElement::hasSourceDogLeg            () const { return false; }
  bool           TrackElement::hasTargetDogLeg            () const { return false; }
  bool           TrackElement::allowOutsideGCell          () const { return false; }
  bool           TrackElement::canGoOutsideGCell          () const { return false; }
  bool           TrackElement::canRipple                  () const { return false; }
  unsigned long  TrackElement::getId                      () const { return 0; }
  unsigned long  TrackElement::getArea                    () const { return 0; }
  unsigned int   TrackElement::getDogLegLevel             () const { return 0; }
  unsigned int   TrackElement::getDogLegOrder             () const { return 0; }
  Interval       TrackElement::getSourceConstraints       () const { return Interval(); }
  Interval       TrackElement::getTargetConstraints       () const { return Interval(); }
  DataNegociate* TrackElement::getDataNegociate           () const { return NULL; }
  TrackElements  TrackElement::getCollapsedPerpandiculars () { return new TrackElements_CollapsedPerpandicular(NULL); }
  void           TrackElement::setAllowOutsideGCell       ( bool ) { }
  void           TrackElement::setLock                    ( bool ) { }
  void           TrackElement::setRevalidated             ( bool ) { }
  void           TrackElement::invalidate                 () { }
  void           TrackElement::setCanRipple               ( bool ) { }
  void           TrackElement::setSourceDogLeg            ( bool ) { }
  void           TrackElement::setTargetDogLeg            ( bool ) { }
  TrackElement*  TrackElement::getCanonical               ( Interval& i ) { i=Interval(getSourceU(),getTargetU()); return this; }
  bool           TrackElement::canSlacken                 () const { return false; }


  bool           TrackElement::canDesalignate             () const { return false; }
  bool           TrackElement::canPivotUp                 ( float ) const { return false; };
  bool           TrackElement::canMoveUp                  ( float, unsigned int ) const { return false; };
  bool           TrackElement::canDogLeg                  () { return false; };
  bool           TrackElement::canDogLeg                  ( Interval ) { return false; };
  bool           TrackElement::canDogLegAt                ( Katabatic::GCell*, bool allowReuse ) { return false; };
  TrackElement*  TrackElement::getSourceDogLeg            () { return NULL; }
  TrackElement*  TrackElement::getTargetDogLeg            () { return NULL; }
  void           TrackElement::dataInvalidate             () { }
  void           TrackElement::eventInvalidate            () { }
  void           TrackElement::setArea                    () { }
  void           TrackElement::setRouted                  ( bool ) { }
  void           TrackElement::setTrack                   ( Track* track ) { _track = track; }
  void           TrackElement::setDogLegLevel             ( unsigned int ) { }
  void           TrackElement::setDogLegOrder             ( unsigned int ) { }
  void           TrackElement::swapTrack                  ( TrackElement* ) { }
  void           TrackElement::reschedule                 ( unsigned int ) { }
  void           TrackElement::detach                     () { }
  void           TrackElement::revalidate                 ( bool invalidEvent ) { }
  void           TrackElement::setAxis                    ( DbU::Unit, unsigned int flags ) { }
  void           TrackElement::slacken                    () { }
  bool           TrackElement::moveUp                     ( unsigned int  ) { return false; }
  bool           TrackElement::moveAside                  ( bool onLeft ) { return false; }
  TrackElement*  TrackElement::makeDogLeg                 () { return NULL; }
  TrackElement*  TrackElement::makeDogLeg                 ( Interval, bool& leftDogleg ) { return NULL; }
  TrackElement*  TrackElement::makeDogLeg                 ( Katabatic::GCell* ) { return NULL; }
  TrackElement*  TrackElement::_postDogLeg                ( Katabatic::GCell* ) { return NULL; }
  void           TrackElement::_postModify                () { }
  void           TrackElement::desalignate                () { }
  bool           TrackElement::_check                     () const { return true; }


  TrackElement::TrackElement ( Track* track )
    : _track(track)
    , _index((size_t)-1)
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
    size_t dummy = _index;
    return _track->getNext ( dummy, getNet() );
  }


  TrackElement* TrackElement::getPrevious () const
  {
    size_t dummy = _index;
    return _track->getPrevious ( dummy, getNet() );
  }


  Interval  TrackElement::getFreeInterval () const
  {
    if ( !_track ) return Interval(false);

    size_t  begin = _index;
    size_t  end   = _index;
    return _track->expandFreeInterval ( begin, end, Track::Inside, getNet() );
  }


  size_t  TrackElement::getGCells ( vector<GCell*>& gcells ) const
  {
    vector<GCell*>().swap ( gcells );
    return gcells.size();
  }


  size_t  TrackElement::getPerpandicularsBound ( set<TrackElement*>& bounds )
  {
    bounds.clear ();
    return 0;
  }


  unsigned int  TrackElement::getOrder () const
  { return numeric_limits<unsigned int>::max(); }


  void  TrackElement::incOverlapCost ( Net* net, TrackCost& cost ) const
  {
    if ( not _track or (getNet() == net) ) return;

    _overlapCostCallback ( this, cost );
  }


  string  TrackElement::_getTypeName () const
  { return "TrackElement"; }


  string  TrackElement::_getString () const
  { return "<" + _getTypeName() + ">"; }


  Record* TrackElement::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add ( getSlot ( "_track",  _track ) );
    record->add ( getSlot ( "_index",  _index ) );

    return record;
  }


} // End of Kite namespace.
