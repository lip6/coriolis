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
// |  C++ Module  :       "./Manipulator.cpp"                        |
// +-----------------------------------------------------------------+


#include "hurricane/DebugSession.h"
#include "hurricane/Bug.h"
#include "kite/TrackSegment.h"
#include "kite/Track.h"
#include "kite/Tracks.h"
#include "kite/DataNegociate.h"
#include "kite/RoutingPlane.h"
#include "kite/RoutingEvent.h"
#include "kite/SegmentFsm.h"
#include "kite/Manipulator.h"
#include "kite/KiteEngine.h"


namespace {

  using namespace std;
  using namespace Hurricane;
  using namespace Kite;
  using Katabatic::GCell;


// -------------------------------------------------------------------
// Class  :  "LvGCandidate".

  class LvGCandidate {
    public:
      struct Compare : public binary_function<LvGCandidate&,LvGCandidate&,bool> {
          inline bool  operator() ( const LvGCandidate& lhs, const LvGCandidate& rhs ) const;
      };
    public:
      inline                  LvGCandidate ( TrackElement* segment=NULL, Interval overlap=Interval(), size_t terminals=0 );
      inline  TrackElement*   getSegment   () const;
      inline  const Interval& getOverlap   () const;
      inline  size_t          getTerminals () const;
    private:
      TrackElement* _segment;
      Interval      _overlap;
      size_t        _terminals;
  };

  inline  LvGCandidate::LvGCandidate ( TrackElement* segment, Interval overlap, size_t terminals )
    : _segment  (segment)
    , _overlap  (overlap)
    , _terminals(terminals)
  { }

  inline  TrackElement*   LvGCandidate::getSegment   () const { return _segment; }
  inline  const Interval& LvGCandidate::getOverlap   () const { return _overlap; }
  inline  size_t          LvGCandidate::getTerminals () const { return _terminals; }

  inline bool  LvGCandidate::Compare::operator() ( const LvGCandidate& lhs, const LvGCandidate& rhs ) const
  {
    if ( lhs.getTerminals() != rhs.getTerminals() )
      return lhs.getTerminals() < rhs.getTerminals();

    if ( lhs.getOverlap() != rhs.getOverlap() )
      return lhs.getOverlap().getSize() > rhs.getOverlap().getSize();

    return lhs.getSegment()->getAxis() < rhs.getSegment()->getAxis();
  }


}  // Anonymous namespace.


namespace Kite {

  using Hurricane::Bug;

// -------------------------------------------------------------------
// Class  :  "Manipulator".

  Manipulator::Manipulator ( TrackElement* segment, SegmentFsm& S )
    : _segment(segment)
    , _data   (NULL)
    , _event  (NULL)
    , _fsm    (S)
  {
    if (not _segment)
      throw Error( "Manipulator::Manipulator(): cannot build upon a NULL TrackElement." );

    DebugSession::open( _segment->getNet(), 150, 160 );

    _data = _segment->getDataNegociate();
    if (_data) _event = _data->getRoutingEvent();
  }


  Manipulator::~Manipulator ()
  { DebugSession::close(); }


  bool  Manipulator::canRipup ( unsigned int flags ) const
  {
    if (_data) {
      if (not _event or _event->isUnimplemented()) return false;

      unsigned int limit = Session::getKiteEngine()->getRipupLimit(_segment);
      unsigned int count = _data->getRipupCount() + ((flags & NotOnLastRipup) ? 1 : 0);

      return (count < limit);
    }
    return false;
  }


  bool  Manipulator::isCaged ( DbU::Unit axis ) const
  {
    Track* track = _segment->getTrack();
    if ( not track ) return false;

    TrackElement* neighbor = _segment->getPrevious();
    if (neighbor and (neighbor->isFixed() or neighbor->isBlockage())) {
      if (abs(axis - neighbor->getTargetU()) < getPPitch()*2)
        return true;
    }

    neighbor = _segment->getNext();
    if (neighbor and (neighbor->isFixed() or neighbor->isBlockage())) {
      if (abs(axis - neighbor->getSourceU()) < getPPitch()*2)
        return true;
    }

    return false;
  }


  bool  Manipulator::ripup ( unsigned int type, DbU::Unit axisHint )
  {
    cdebug.log(159) << "Manipulator::ripup() " << endl;

    if (not canRipup()) return false;

    if (_segment->isFixed()) return false;
    if (_data == NULL) return true;

    _fsm.addAction( _segment, type, axisHint );
    return true;
  }


  bool  Manipulator::ripupPerpandiculars ( unsigned int flags )
  {
    cdebug.log(159) << "Manipulator::ripupPerpandiculars() - " << flags << endl;

    bool          success                  = true;
    bool          cagedPerpandiculars      = false;
    Interval      constraints              ( _event->getConstraints() );
    Interval      perpandicularConstraints ( constraints );
    size_t        placedPerpandiculars     = 0;
    unsigned int  parallelActionFlags      = SegmentAction::SelfRipup|SegmentAction::EventLevel4;
    unsigned int  perpandicularActionFlags = SegmentAction::SelfRipupPerpand;

    if (flags & Manipulator::PerpandicularsFirst) {
      parallelActionFlags      &= ~SegmentAction::EventLevel4;
      perpandicularActionFlags |=  SegmentAction::EventLevel4;
      if (flags & Manipulator::ToRipupLimit)
        perpandicularActionFlags |=  SegmentAction::ToRipupLimit;
    } else {
      if (flags & Manipulator::ToRipupLimit)
        parallelActionFlags |=  SegmentAction::ToRipupLimit;
    }

    cdebug.log(159) << "Pure constraints: " << constraints << endl;

    Track* track = NULL;
    const vector<TrackElement*>& perpandiculars = _event->getPerpandiculars();

    for ( size_t i=0 ; i < perpandiculars.size() ; i++ ) {
      track = perpandiculars[i]->getTrack();
      if (not track) {
      // The perpandicular is not placed yet.
        if (flags & Manipulator::PerpandicularsFirst) {
          _fsm.addAction( perpandiculars[i], perpandicularActionFlags );
        }
        continue;
      }

      bool dislodgeCaged = false;
      if (Manipulator(perpandiculars[i],_fsm).isCaged(_event->getSegment()->getAxis())) {
        cagedPerpandiculars = true;
        dislodgeCaged       = true;
      }

      placedPerpandiculars++;

    // Try to ripup the perpandicular.
      DataNegociate* data2 = perpandiculars[i]->getDataNegociate();
      cdebug.log(159) << "| " << perpandiculars[i] << endl;

      if ( (flags & Manipulator::ToMoveUp) and (data2->getState() < DataNegociate::MoveUp) )
        data2->setState( DataNegociate::MoveUp );
        
      if (Manipulator(perpandiculars[i],_fsm).ripup(perpandicularActionFlags)) {
        if (dislodgeCaged) {
          _event->setAxisHint( _event->getSegment()->getAxis() + _event->getSegment()->getPitch() );
        }
        continue;
      }

    // Cannot ripup the perpandicular, try to ripup it's neigbors.
      size_t begin;
      size_t end;
      track->getOverlapBounds( constraints, begin, end );

      for ( ; (begin < end) ; begin++ ) {
        TrackElement* other = track->getSegment(begin);

        if (other->getNet() == _event->getSegment()->getNet()) continue;

        Interval otherCanonical ( other->getCanonicalInterval() );
        if (not otherCanonical.intersect(constraints)) continue;

      // Try to ripup conflicting neighbor.
        if (Manipulator(other,_fsm).canRipup()) {
          cdebug.log(159) << "  | Ripup: " << begin << " " << other << endl;
          _fsm.addAction( other, SegmentAction::OtherRipup );
        } else {
          cdebug.log(159) << "Aborted ripup of perpandiculars, fixed or blocked." << endl;
          return false;
        }
      }
    }

    if (cagedPerpandiculars and not placedPerpandiculars) {
      cdebug.log(159) << "Aborted ripup of perpandiculars, constraints are due to fixed/blockage." << endl;
      _fsm.addAction( _segment, SegmentAction::SelfRipup );
      return true;
    }

    if (_segment->isLocal() and not placedPerpandiculars) {
      cdebug.log(159) << "No placed perpandiculars, tight native constraints, place perpandiculars FIRST." << endl;
      for ( size_t i=0 ; i < perpandiculars.size() ; i++ ) {
        _fsm.addAction( perpandiculars[i], perpandicularActionFlags|SegmentAction::EventLevel4 );
      }
      _fsm.addAction( _segment, parallelActionFlags );
      return true;
    }

    RoutingPlane* plane    = Session::getKiteEngine()->getRoutingPlaneByLayer(_segment->getLayer());
    size_t        tracksNb = 0;

    track = plane->getTrackByPosition(constraints.getVMin());

    if (track and (track->getAxis() < constraints.getVMin())) track = track->getNextTrack();
    for ( ; track && (track->getAxis() <= constraints.getVMax())
          ; track = track->getNextTrack(), tracksNb++ );

    if (_segment->isLocal() and (tracksNb < 2)) success = ripple();

    _fsm.addAction( _segment, parallelActionFlags );
    return success;
  }


  bool  Manipulator::relax ( Interval interval, unsigned int flags )
  {
    interval.inflate( - Session::getExtensionCap(getLayer()) );
    cdebug.log(159) << "Manipulator::relax() of: " << _segment << " " << interval << endl; 

    if (_segment->isFixed()) return false;
    if (not interval.intersect(_segment->getCanonicalInterval())) return false;
    if (not _data) return false;

    if (    _segment->isTerminal()
       and (_segment->getLayer() == Session::getRoutingGauge()->getRoutingLayer(1)) ) {
      if (interval.contains(_segment->base()->getAutoSource()->getX())) return false;
      if (interval.contains(_segment->base()->getAutoTarget()->getX())) return false;
    }

    cdebug.tabw(159,1);
    bool success = true;
    bool expand = _segment->isGlobal() and (flags&AllowExpand);
    cdebug.log(159) << "Expand:" << expand << endl;

    Katabatic::GCellVector gcells;
    _segment->getGCells( gcells );

    if (gcells.size() < 2 ){
      cerr << Bug( "relax() Cannot break %s,\n      only in %s."
                 , getString(_segment).c_str()
                 , getString(gcells[0]).c_str()
                 ) << endl;
      cdebug.tabw(159,-1);
      return false;
    }

    unsigned int  depth = Session::getRoutingGauge()->getLayerDepth(_segment->getLayer());
    Interval      uside;
    size_t        dogLegCount  = 0;
    size_t        iminconflict = gcells.size();
    size_t        imaxconflict = gcells.size();
    size_t        igcell;

  // Look for closest enclosing min & max GCells indexes.
    for ( igcell=0 ; igcell<gcells.size() ; igcell++ ) {
      uside = gcells[igcell]->getSide(_segment->getDirection());
      cdebug.log(159) << "| " << setw(3) << igcell << " " << gcells[igcell] << " uside: " << uside << endl;

      if (uside.contains(interval.getVMin())) {
        iminconflict = igcell;
        cdebug.log(159) << "> Min conflict: " << iminconflict << endl;
      }
      if (uside.contains(interval.getVMax())) {
        imaxconflict = igcell;
        cdebug.log(159) << "> Max conflict: " << imaxconflict << endl;
      }
    }

  // Expand min & max to enclose GCells of greatest or equal order
  // (i.e. less saturateds)
    bool minExpanded = false;
    bool maxExpanded = false;
    if (expand) {
      if (iminconflict < gcells.size()) {
      //cdebug.log(159) << "Expand min" << endl;

        size_t imindensity = 0;
        for ( size_t iexpand=1 ; iexpand<iminconflict ; ++iexpand ) {
          if (not _segment->canDogleg(gcells[iexpand],KtAllowDoglegReuse)) continue;

          // cdebug.log(159) << "<GCell [" << iexpand << "]> Density "
          //             << "Density " << Session::getRoutingGauge()->getRoutingLayer(depth)->getName()
          //             << " min. dens.:" << gcells[imindensity]->getDensity(depth)
          //             << " exp. dens.:" << gcells[iexpand    ]->getDensity(depth)
          //             << endl;

          if (gcells[imindensity]->getDensity(depth) - gcells[iexpand]->getDensity(depth) > 1e-3) {
            imindensity = iexpand;
          //cdebug.log(159) << "Accepted expand " << imindensity << endl;
          }
        }

        if (iminconflict != imindensity) minExpanded = true;
        iminconflict = (imindensity>0) ? imindensity : gcells.size();
      }

      if (imaxconflict < gcells.size()) {
      //cdebug.log(159) << "Expand max" << endl;

        size_t imindensity = imaxconflict;
        for ( size_t iexpand=imaxconflict+1 ; iexpand<gcells.size() ; ++iexpand ) {
          if (not _segment->canDogleg(gcells[iexpand],KtAllowDoglegReuse)) continue;

          // cdebug.log(159) << "<GCell [" << iexpand << "]> Density "
          //             << Session::getRoutingGauge()->getRoutingLayer(depth)->getName()
          //             << " min. dens.:" << gcells[imindensity]->getDensity(depth)
          //             << " exp. dens.:" << gcells[iexpand    ]->getDensity(depth)
          //             << endl;

          if (gcells[imindensity]->getDensity(depth) - gcells[iexpand]->getDensity(depth) > 1e-3) {
            imindensity = iexpand;
          //cdebug.log(159) << "Accepted expand " << imindensity << endl;
          }
        }
        
        if (imindensity != imaxconflict) maxExpanded = true;
        imaxconflict = (imindensity < gcells.size()) ? imindensity : gcells.size();
      }
    }
    cdebug.log(159) <<   "minExpanded:" << minExpanded << " (" << iminconflict
                << ") maxExpanded:" << maxExpanded << " (" << imaxconflict << ")" << endl;

  // Check for full enclosure.
    if (  ( (iminconflict == gcells.size()) and (imaxconflict == gcells.size()  ) )
       or ( (iminconflict == 0)             and (imaxconflict == gcells.size()-1) )) {
      cinfo << "[INFO] Manipulator::relax(): Segment fully enclosed in interval." << endl;
      cdebug.tabw(159,-1);
      return false;
    }

  // Suppress min/max if it's the first/last.
    if ((iminconflict < gcells.size()) and (imaxconflict == gcells.size()-1)) imaxconflict = gcells.size();
    if ((imaxconflict < gcells.size()) and (iminconflict == 0))               iminconflict = gcells.size();

  // Compute number of doglegs and nature of the *first* dogleg.
  // (first can be min or max, second can only be max)
    bool firstDoglegIsMin = false;
    if (iminconflict < gcells.size()) { dogLegCount++; firstDoglegIsMin = true; }
    if (imaxconflict < gcells.size())   dogLegCount++;
    
    switch ( dogLegCount ) {
      case 2:
      // Compact only if the double dogleg is at beginning or end.
        if (iminconflict == imaxconflict) {
          if (iminconflict == 0) {
          // First dogleg is max.
            dogLegCount--;
          } else if (iminconflict == gcells.size()-1) {
            dogLegCount--;
            firstDoglegIsMin = true;
          }
        }
        break;
      case 1: break;
      case 0:
        cerr << Bug( "Manipulator::relax() Can't find a GCell suitable for making dogleg."
                   , getString(interval).c_str() ) << endl;
        cdebug.tabw(159,-1);
        return false;
    }

    cdebug.log(159) << "| Has to do " << dogLegCount << " doglegs." << endl;

  // Check of "min is less than one track close the edge" (while not expanded).
  // AND we are on the first GCell AND there's one dogleg only.
    if (not minExpanded and (iminconflict == 0) and (imaxconflict == gcells.size())) {
      cdebug.log(159) << "Cannot break in first GCell only." << endl;
      cdebug.tabw(159,-1);
      return false;
    }

  // Check of "min is less than one track close the edge" (while not expanded).
    if ( /*not minExpanded and*/ (iminconflict > 0) and (iminconflict < gcells.size()) ) {
      uside = gcells[iminconflict-1]->getSide(_segment->getDirection());
      cdebug.log(159) << "GCell Edge Comparison (min): " << uside
                  << " vs. " << DbU::getValueString(interval.getVMin()) << endl;
    // Ugly: One lambda shrink.
      if (interval.getVMin()-DbU::lambda(1.0) <= uside.getVMax()) {
        cdebug.log(159) << "Using previous GCell." << endl;
        iminconflict--;
      }
    }

  // Check if there is only one dogleg AND it's the last one.
    if (not maxExpanded and (iminconflict == gcells.size()) and (imaxconflict == gcells.size()-1)) {
      cdebug.log(159) << "Cannot break in last GCell only." << endl;
      cdebug.tabw(159,-1);
      return false;
    }

  // Check of "max is less than one track close the edge" (while not expanded).
    if ((imaxconflict < gcells.size()-1)) {
      uside = gcells[imaxconflict+1]->getSide( _segment->getDirection() );
      cdebug.log(159) << "GCell Edge Comparison (max): " << uside
                  << " vs. " << DbU::getValueString(interval.getVMax()) << endl;
      if (interval.getVMax()+getPPitch() >= uside.getVMin()) {
        interval.inflate( 0, getPPitch() );
        cdebug.log(159) << "Using next GCell " << interval << endl;
        imaxconflict++;
      }
    }

    size_t ifirstDogleg  = gcells.size();
    size_t isecondDogleg = gcells.size();
    if (not firstDoglegIsMin) {
      ifirstDogleg = imaxconflict;
    } else {
      ifirstDogleg  = iminconflict;
      isecondDogleg = imaxconflict;
    }

  // Making first dogleg.
    cdebug.log(159) << "Making FIRST dogleg at " << ifirstDogleg << endl;
    TrackElement*     segment1     = NULL;
    TrackElement*     segment2     = NULL;
    Track*            track        = _segment->getTrack();
    Katabatic::GCell* dogLegGCell  = gcells[ifirstDogleg];
    TrackElement*     dogleg       = NULL;
    DbU::Unit         doglegAxis;
    bool              doglegReuse1 = false;
    bool              doglegReuse2 = false;

  // Try to reuse existing dogleg if broken at either end.
    if (ifirstDogleg == 0)               dogleg = _segment->getSourceDogleg();
    if (ifirstDogleg == gcells.size()-1) dogleg = _segment->getTargetDogleg();
    if (dogleg) {
      cdebug.log(159) << "Reusing dogleg." << endl;
      doglegReuse1 = true;
      segment1     = _segment;
    } else {
    // Try to create a new dogleg.
      if (not _segment->canDogleg(dogLegGCell)) {
        cdebug.log(159) << "Cannot create FIRST dogleg." << endl;
        cdebug.tabw(159,-1);
        return false;
      }
      _segment->makeDogleg( dogLegGCell, dogleg, segment1 );
    }

    if (firstDoglegIsMin) {
      if (minExpanded) {
        doglegAxis = dogLegGCell->getSide( _segment->getDirection() ).getCenter();
      //cdebug.log(159) << "MARK 1 doglegAxis: " << DbU::getValueString(doglegAxis) << endl;
      } else {
        doglegAxis = interval.getVMin() - getPPitch();
      //cdebug.log(159) << "MARK 2 doglegAxis: " << DbU::getValueString(doglegAxis) << endl;
      }
    } else {
      if (maxExpanded) {
        doglegAxis = dogLegGCell->getSide( _segment->getDirection() ).getVMin();
      //cdebug.log(159) << "MARK 3 doglegAxis: " << DbU::getValueString(doglegAxis) << endl;
      } else {
        doglegAxis = interval.getVMax() + getPPitch() - DbU::fromLambda(1.0);
      //cdebug.log(159) << "MARK 4 doglegAxis: " << DbU::getValueString(doglegAxis) << endl;
      }
    }
    if (doglegReuse1) _fsm.addAction( dogleg, SegmentAction::OtherRipup );
    else              dogleg->setAxis( doglegAxis );

  // If event is present, the dogleg is in the current RoutingSet.
    RoutingEvent* event = dogleg->getDataNegociate()->getRoutingEvent();
    if (event) {
      cdebug.log(159) << "Set Axis Hint: @" << DbU::getValueString(doglegAxis) << " " << dogleg << endl;
      event->setAxisHint( doglegAxis );
    } else {
      cdebug.log(159) << "Dogleg has no RoutingEvent yet." << endl;
    }

  // Making second dogleg.
    if (dogLegCount > 1) {
      cdebug.log(159) << "Making SECOND dogleg at " << isecondDogleg
                  << " on " << segment1 << endl;
      
      dogleg      = NULL;
      dogLegGCell = gcells[isecondDogleg];

      if (ifirstDogleg == isecondDogleg) {
        cdebug.log(159) << "Double break in same GCell." << endl;
        segment1->setFlags( TElemSourceDogleg );
      }

      if (isecondDogleg == gcells.size()-1) dogleg = segment1->getTargetDogleg();
      if (dogleg) {
        cdebug.log(159) << "Reusing dogleg." << endl;
        doglegReuse2 = true;
        segment2     = segment1;
      } else {
      // Try to create a new dogleg.
        if (not segment1->canDogleg(dogLegGCell)) {
          cdebug.log(159) << "Cannot create SECOND dogleg." << endl;
          cdebug.tabw(159,-1);
          return false;
        }
        segment1->makeDogleg( dogLegGCell, dogleg, segment2 );
      }
      
      if (maxExpanded) {
        doglegAxis = dogLegGCell->getSide(segment1->getDirection()/*,false*/).getCenter();
      } else {
        doglegAxis = interval.getVMax() + getPPitch();
      }
      if (doglegReuse2) _fsm.addAction( dogleg, SegmentAction::OtherRipup );
      else              dogleg->setAxis( doglegAxis );

    // If event is present, the dogleg is in the current RoutingSet.
      RoutingEvent* event = dogleg->getDataNegociate()->getRoutingEvent();
      if (event) {
        cdebug.log(159) << "Set Axis Hint: @" << DbU::getValueString(doglegAxis) << " " << dogleg << endl;
        event->setAxisHint( doglegAxis );
      } else {
        cdebug.log(159) << "Dogleg has no RoutingEvent yet." << endl;
      }

    // This cases seems never to occurs.
      const vector<AutoSegment*>& doglegs = Session::getDoglegs();
      for ( size_t i=0 ; i<doglegs.size() ; i++ ) { 
        TrackElement* segment = Session::lookup(doglegs[i]);
        if (not segment->getTrack() and track) {
          cdebug.log(159) << "Direct Track insert of: " << segment << endl;
          Session::addInsertEvent( segment, track );
        }
      }
    }

    switch ( dogLegCount ) {
      case 1:
        if (not doglegReuse1) {
          if (firstDoglegIsMin)
            _segment->getDataNegociate()->setState( DataNegociate::RipupPerpandiculars, true );
          else
            segment1->getDataNegociate()->setState( DataNegociate::RipupPerpandiculars, true );
        }
        if ((flags & NoDoglegReuse) and (doglegReuse1 or doglegReuse2 ))
          success = false;
        break;
      case 2:
        if (not doglegReuse1)
          _segment->getDataNegociate()->setState( DataNegociate::RipupPerpandiculars, true );
        if ( not doglegReuse2 )
          segment2->getDataNegociate()->setState( DataNegociate::RipupPerpandiculars, true );
        break;
    }

    if (_segment->isLocal()) {
      cdebug.log(159) << "Reset state of: " << _segment << endl;
      _segment->getDataNegociate()->setState( DataNegociate::RipupPerpandiculars, true );
    } else {
      cdebug.log(159) << "No state reset: " << _segment << endl;
    }

    if ((not doglegReuse1) and segment1 and segment1->isLocal()) {
      cdebug.log(159) << "Reset state of: " << segment1 << endl;
      segment1->getDataNegociate()->setState( DataNegociate::RipupPerpandiculars, true );
    }

    if ((not doglegReuse2) and segment2 and segment2->isLocal()) {
      cdebug.log(159) << "Reset state of: " << segment2 << endl;
      segment2->getDataNegociate()->setState( DataNegociate::RipupPerpandiculars, true );
    }

    cdebug.tabw(159,-1);
    return success;
  }


  bool  Manipulator::insertInTrack ( size_t itrack )
  {
    Track*              track            = _fsm.getTrack(itrack);
    size_t              begin            = _fsm.getBegin(itrack);
    size_t              end              = _fsm.getEnd  (itrack);
    Net*                ownerNet         = _segment->getNet();
    Interval            toFree            (_segment->getCanonicalInterval());
  //Net*                ripupNet         = NULL;
    set<TrackElement*>  canonicals;
    DbU::Unit           rightAxisHint    = 0;
    DbU::Unit           leftAxisHint     = 0;
    bool                leftIntrication  = false;
    bool                rightIntrication = false;
    bool                success          = true;

    cdebug.log(159) << "Manipulator::insertInTrack() - " << toFree << endl;

    for ( size_t i = begin ; success && (i < end) ; i++ ) {
      TrackElement* segment2 = track->getSegment(i);

      cdebug.log(159) << "* Looking // " << segment2 << endl;

      if ( segment2->getNet() == ownerNet  ) continue;
      if ( not toFree.intersect(segment2->getCanonicalInterval()) ) {
        cdebug.log(159) << "No intersection with: " << segment2->getCanonicalInterval() << endl;
        continue;
      }
      if ( segment2->isBlockage() or segment2->isFixed() ) {
        cdebug.log(159) << "Ovelap is blockage or fixed." << endl;
        success = false;
        continue;
      }
      // if ( segment2->getId() >= maxId ) {
      //   cdebug.log(159) << "Ovelap has an Id superior to AutoSegment::maxId:" << maxId << "." << endl;
      //   continue;
      // }
      // ripupNet = segment2->getNet();

      DataNegociate* data2 = segment2->getDataNegociate();
      if ( !data2 ) {
        cdebug.log(159) << "No DataNegociate, ignoring." << endl;
        continue;
      }

      if ( data2->getState() == DataNegociate::MaximumSlack ) {
        cdebug.log(159) << "At " << DataNegociate::getStateString(data2)
                    << " for " << segment2 << endl;
        success = false;
        continue;
      }

      bool shrinkLeft  = false;
      bool shrinkRight = false;

      if ( data2->getRightMinExtend() < toFree.getVMin() ) {
        cdebug.log(159) << "- Shrink right edge (push left) " << segment2 << endl;
        shrinkRight = true;
        TrackElement* rightNeighbor2 = track->getSegment(i+1);
        if ( rightNeighbor2 && (rightNeighbor2->getNet() == segment2->getNet()) ) {
          Interval interval1 = segment2->getCanonicalInterval();
          Interval interval2 = rightNeighbor2->getCanonicalInterval();

          if ( interval1.intersect(interval2) && (interval2.getVMax() > interval1.getVMax()) )
            shrinkLeft = true;
        }
      }

      if ( data2->getLeftMinExtend() > toFree.getVMax() ) {
        cdebug.log(159) << "- Shrink left edge (push right) " << segment2 << endl;
        shrinkLeft = true;
        if ( i > 0 ) {
          TrackElement* leftNeighbor2 = track->getSegment(i-1);
          if ( leftNeighbor2 && (leftNeighbor2->getNet() == segment2->getNet()) ) {
            Interval interval1 = segment2->getCanonicalInterval();
            Interval interval2 = leftNeighbor2->getCanonicalInterval();

            if ( interval1.intersect(interval2) && (interval2.getVMin() < interval1.getVMin()) )
              shrinkRight = true;
          }
        }
      }

      if ( _segment->isLocal() and segment2->isLocal() ) {
        if ( shrinkLeft and shrinkRight ) {
          Interval interval1 = segment2->getCanonicalInterval();
          if ( toFree.getCenter() < interval1.getCenter() ) shrinkRight = false;
          else shrinkLeft = false;
        }
      }

      cdebug.log(159) << "- Hard overlap/enclosure/shrink " << segment2 << endl;
      if ( _segment->isStrap() and segment2->isGlobal() ) continue;
      if ( not (success = Manipulator(segment2,_fsm).ripup(SegmentAction::OtherRipup)) )
        continue;

      canonicals.clear ();
      for( TrackElement* segment3
              : segment2->getPerpandiculars().getSubSet(TrackElements_UniqCanonical(canonicals)) ) {
        DataNegociate* data3 = segment3->getDataNegociate();
        if ( not data3 ) continue;

        RoutingEvent* event3 = data3->getRoutingEvent();
        if ( not event3 ) continue;

        if ( not toFree.intersect(event3->getConstraints()) ) {
          cdebug.log(159) << "  . " << segment3 << endl;
          continue;
        }

        cdebug.log(159) << "  | " << segment3 << endl;

        if ( shrinkRight xor shrinkLeft ) {
          if ( shrinkRight ) {
            if ( not (success=Manipulator(segment3,_fsm)
                     .ripup( SegmentAction::OtherRipupPerpandAndPushAside
                           , toFree.getVMin() - getPPitch()/2
                           )) )
              break;

            if ( event3->getTracksFree() == 1 ) {
              cdebug.log(159) << "Potential left intrication with other perpandicular." << endl;
              if ( segment3->getAxis() == segment2->getTargetU() - Session::getExtensionCap(getLayer()) ) {
                leftIntrication = true;
                leftAxisHint    = segment3->getAxis();
              }
            }
          }
          if ( shrinkLeft  ) {
            if ( not (success=Manipulator(segment3,_fsm)
                     .ripup( SegmentAction::OtherRipupPerpandAndPushAside
                           , toFree.getVMax() + getPPitch()/2
                           )) )
              break;
            if ( event3->getTracksFree() == 1 ) {
              cdebug.log(159) << "Potential right intrication with other perpandicular." << endl;
              if ( segment3->getAxis() == segment2->getSourceU() + Session::getExtensionCap(getLayer()) ) {
                rightIntrication = true;
                rightAxisHint    = segment3->getAxis();
              }
            }
          }
        } else {
          if ( not (success=Manipulator(segment3,_fsm).ripup( SegmentAction::OtherRipup
                                                              | SegmentAction::EventLevel3
                                                              )) )
            break;
        }
      }
      if ( not success ) break;
    }

    if ( success ) {
      cdebug.log(159) << "Manipulator::insertInTrack() success" << endl;

      _fsm.setState  ( SegmentFsm::OtherRipup );
      _fsm.addAction ( _segment
                     , SegmentAction::SelfInsert|SegmentAction::MoveToAxis|SegmentAction::EventLevel4 
                     , _fsm.getCost(itrack).getTrack()->getAxis() );

      unsigned int flags = 0;
      if ( rightIntrication ) flags |= RightAxisHint;
      if ( leftIntrication  ) flags |= LeftAxisHint;
      if ( flags )
        Manipulator(_segment,_fsm).shrinkToTrack(itrack,flags,leftAxisHint,rightAxisHint);
    } else
      _fsm.clearActions ();

    return success;
  }


  bool  Manipulator::forceToTrack ( size_t itrack )
  {
    Track*              track      = _fsm.getTrack(itrack);
    size_t              begin      = _fsm.getBegin(itrack);
    size_t              end        = _fsm.getEnd  (itrack);
    Net*                ownerNet   = _segment->getNet();
    Interval            toFree      (_segment->getCanonicalInterval());
  //Net*                ripupNet   = NULL;
    set<TrackElement*>  canonicals;
    bool                success    = true;

    cdebug.log(159) << "Manipulator::forceToTrack() - " << toFree << endl;

    for ( size_t i=begin ; success and (i < end) ; ++i ) {
      TrackElement* segment2 = track->getSegment(i);

      cdebug.log(159) << "* Looking // " << segment2 << endl;

      if (segment2->getNet() == ownerNet) continue;
      if (not toFree.intersect(segment2->getCanonicalInterval())) continue;
      if (segment2->isFixed()) {
        success = false;
        continue;
      }
    //ripupNet = segment2->getNet();

      DataNegociate* data2 = segment2->getDataNegociate();
      if (not data2 ) {
        cdebug.log(159) << "No DataNegociate, ignoring." << endl;
        continue;
      }

      cdebug.log(159) << "- Forced ripup " << segment2 << endl;
      if (not (success=Manipulator(segment2,_fsm).ripup(SegmentAction::OtherRipup)))
        continue;

      canonicals.clear();
      for( TrackElement* segment3
              : segment2->getPerpandiculars().getSubSet(TrackElements_UniqCanonical(canonicals)) ) {
        DataNegociate* data3 = segment3->getDataNegociate();
        if (not data3) continue;

        RoutingEvent* event3 = data3->getRoutingEvent();
        if (not event3) continue;

        if (Manipulator(segment3,_fsm).canRipup())
          _fsm.addAction( segment3, SegmentAction::OtherRipup );
      }
    }

    if (success) {
      _fsm.setState ( SegmentFsm::OtherRipup );
      _fsm.addAction( _segment
                    , SegmentAction::SelfInsert|SegmentAction::MoveToAxis
                    , _fsm.getCost(itrack).getTrack()->getAxis() );
    }

    return success;
  }


  bool  Manipulator::shrinkToTrack ( size_t i, unsigned int flags, DbU::Unit leftAxisHint, DbU::Unit rightAxisHint )
  {
#if THIS_IS_DISABLED
    Track*              track       = _fsm.getTrack(i);
    size_t              begin       = _fsm.getBegin(i);
    size_t              end         = _fsm.getEnd  (i);
    Net*                ownerNet    = _segment->getNet();
    set<TrackElement*>  canonicals;
    bool                success     = true;
    DbU::Unit           leftExtend  = _segment->getSourceU() + Session::getExtensionCap(getLayer());
    DbU::Unit           rightExtend = _segment->getSourceU() - Session::getExtensionCap(getLayer());

    cdebug.log(159) << "Manipulator::shrinkToTrack()" << endl;

    if (_segment->isLocal()) return false;
    Interval  shrunkFree = _segment->base()->getMinSpanU();

    cdebug.log(159) << "* " << shrunkFree << endl;

    for ( size_t i = begin ; success and (i < end) ; ++i ) {
      TrackElement* segment2 = track->getSegment(i);

      cdebug.log(159) << "* Looking // " << segment2 << endl;

      if (segment2->getNet() == ownerNet) continue;
      if (segment2->isFixed()) { success = false; continue; }
      if (not shrunkFree.intersect(segment2->getCanonicalInterval())) continue;

      success = false;
    }

    if (success) {
      set<TrackElement*>           perpandiculars;
      set<TrackElement*>::iterator iperpand;

      DbU::Unit axisHint;
      if (not (flags & LeftAxisHint )) leftAxisHint  = shrunkFree.getCenter();
      if (not (flags & RightAxisHint)) rightAxisHint = shrunkFree.getCenter();

      _segment->getPerpandicularsBound( perpandiculars );
      for ( iperpand = perpandiculars.begin() ; iperpand != perpandiculars.end() ; ++iperpand ) {
        DataNegociate* data2 = (*iperpand)->getDataNegociate();
        if (data2) {
          cdebug.log(159) << "| perpandicular bound:" << *iperpand << endl;
          success = Manipulator(*iperpand,_fsm).ripup( SegmentAction::SelfRipupPerpandWithAxisHint );
          if (success) {
            if      ((*iperpand)->getAxis() == leftExtend ) axisHint = leftAxisHint;
            else if ((*iperpand)->getAxis() == rightExtend) axisHint = rightAxisHint;
            else {
              cinfo << "[INFO] Bound Axis is neither left nor right\n      " << (*iperpand) << endl;
              axisHint = shrunkFree.getCenter();
            }

            _fsm.getActions()[_fsm.getActions().size()-1].setAxisHint( axisHint );
          }
        }
      }

      _fsm.addAction( _segment, SegmentAction::SelfInsert );
      _fsm.setState ( SegmentFsm::OtherRipup );

      cdebug.log(159) << "Successful shrinkToTrack." << endl;
      return true;
    }
#endif

    return false;
  }


  bool  Manipulator::forceOverLocals ()
  {
    cdebug.log(159,1) << "Manipulator::forceOverLocals()" << endl;

    vector<TrackCost>& costs = _fsm.getCosts();
    size_t itrack = 0;
    for ( ; itrack<costs.size() ; ++itrack ) {
      cdebug.log(159) << "Trying itrack:" << itrack << endl;

      if (  costs[itrack].isFixed()
         or costs[itrack].isBlockage()
         or costs[itrack].isInfinite()
         or costs[itrack].isOverlapGlobal() )
        continue;

      bool      success    = true;
      Track*    track      = _fsm.getTrack(itrack);
      size_t    begin      = _fsm.getBegin(itrack);
      size_t    end        = _fsm.getEnd  (itrack);
      Net*      ownerNet   = _segment->getNet();
      Interval  toFree      (_segment->getCanonicalInterval());
      
      for ( size_t i = begin ; success and (i < end) ; i++ ) {
        TrackElement* segment2 = track->getSegment(i);

        cdebug.log(159) << "* Looking // " << segment2 << endl;

        if ( segment2->getNet() == ownerNet  ) continue;
        if ( not toFree.intersect(segment2->getCanonicalInterval()) ) continue;
        if ( segment2->isFixed() ) {
          success = false;
          continue;
        }

        DataNegociate* data2 = segment2->getDataNegociate();
        if ( not data2 ) {
          cdebug.log(159) << "No DataNegociate, ignoring." << endl;
          success = false;
          continue;
        }

        cdebug.log(159) << "- Forced ripup " << segment2 << endl;
        if ( not (success=Manipulator(segment2,_fsm).ripup(SegmentAction::OtherRipup)) ) {
          continue;
        }
      }

      if (success) {
        _fsm.setState ( SegmentFsm::OtherRipup );
        _fsm.addAction( _segment
                      , SegmentAction::SelfInsert|SegmentAction::MoveToAxis
                      , _fsm.getCost(itrack).getTrack()->getAxis()
                      );
        break;
      }
    }

    cdebug.tabw(159,-1);
    return (itrack < costs.size());
  }


  bool  Manipulator::slacken ( unsigned int flags )
  {
    cdebug.log(159) << "Manipulator::slacken() " << _segment << endl; 

    if (    _segment->isFixed   ()) return false;
    if (not _segment->canSlacken()) return false;

    return _segment->slacken( flags );
  }


  bool  Manipulator::ripple ()
  {
    cdebug.log(159) << "Manipulator::ripple() from " << _segment << endl; 

  //if (not _segment->canRipple()) return false;
    if (not _segment->isLocal()) return false;

    Net*          net   = _segment->getNet();
    Interval      uside = _segment->base()->getAutoSource()->getGCell()->getSide ( Katabatic::perpandicularTo(_segment->getDirection())/*, false*/ );
    RoutingPlane* plane = Session::getKiteEngine()->getRoutingPlaneByLayer(_segment->getLayer());

    cdebug.tabw(159,1);
    for( Track* track : Tracks_Range::get(plane,uside)) {
      size_t begin;
      size_t end;

      track->getOverlapBounds( _segment->getCanonicalInterval(), begin, end );
      for ( ; begin < end ; begin++ ) {
        TrackElement* other = track->getSegment(begin);
        cdebug.log(159) << "| " << other << endl;

        if (other->getNet() == net) continue;
        if (not other->canRipple()) continue;

        DataNegociate* otherData = other->getDataNegociate();
        if (not otherData) continue;

        DbU::Unit     shiftedAxisHint;
        RoutingEvent* otherEvent = otherData->getRoutingEvent();

        if (other->getAxis() < _segment->getAxis()) {
        // Ugly: routing pitch.
          shiftedAxisHint = otherEvent->getAxisHint() - getPitch();
          if (shiftedAxisHint < uside.getVMin())
            shiftedAxisHint = uside.getVMin();
        } else {
        // Ugly: routing pitch.
          shiftedAxisHint = otherEvent->getAxisHint() + getPitch();
          if (shiftedAxisHint > uside.getVMax())
            shiftedAxisHint = uside.getVMax();
        }

        otherEvent->setAxisHint( shiftedAxisHint );
        _fsm.addAction( other, SegmentAction::OtherRipup );
      }
    }
    cdebug.tabw(159,-1);

    return true;
  }


  bool  Manipulator::pivotUp ()
  {
    cdebug.log(159) << "Manipulator::pivotUp() " << _segment << endl; 
    return false;

    if (_segment->isFixed()) return false;
    if (_segment->isStrap()) return false;

    float reserve = (_segment->isLocal()) ? 0.5 : 1.0;
    if (not _segment->canMoveUp(reserve)) return false;

    return _segment->moveUp( Katabatic::KbNoFlags );
  }


  bool  Manipulator::pivotDown ()
  {
    cdebug.log(159) << "Manipulator::pivotDown() " << _segment << endl; 
    return false;

    if (    _segment->isFixed ()       ) return false;
    if (    _segment->isStrap ()       ) return false;
    if (not _segment->canPivotDown(2.0)) return false;

    return _segment->moveDown( Katabatic::KbNoFlags );
  }


  bool  Manipulator::moveUp ( unsigned int flags )
  {
    cdebug.log(159) << "Manipulator::moveUp() " << _segment << endl; 

    unsigned int kflags = Katabatic::KbWithNeighbors;
  //kflags |= (flags & AllowLocalMoveUp   ) ? Katabatic::AutoSegment::AllowLocal    : 0;
    kflags |= (flags & AllowTerminalMoveUp) ? Katabatic::KbAllowTerminal : 0;

    if (_segment->isFixed()) return false;
    if (not (flags & AllowLocalMoveUp)) {
      if (_segment->isLocal()) {
        if (not _segment->canPivotUp(0.5)) return false;
      } else {
        if (_segment->getLength() < 20*getPitch()) {
          if (not (flags & AllowShortPivotUp)) return false;
          if (not _segment->canPivotUp(1.0)) return false;
        }
        if (not _segment->canMoveUp(0.5,kflags)) return false;
      }
    } else {
      if (not _segment->canMoveUp(0.5,kflags)) return false;
    }
    return _segment->moveUp( kflags );
  }


  bool  Manipulator::makeDogleg ()
  {
    cdebug.log(159) << "Manipulator::makeDogleg() " << _segment << endl; 

    if (    _segment->isFixed()) return false;
    if (not _segment->isLocal()) return false;
    if (_segment->getLength() < 5*getPitch()) return false;

    if (_fsm.getCosts().size()) {
      Track*    track    = _fsm.getTrack(0);
      size_t    begin    = _fsm.getBegin(0);
      size_t    end      = _fsm.getEnd  (0);
      Net*      ownerNet = _segment->getNet();
      Interval  toFree    (_segment->getCanonicalInterval());
      Interval  overlap;

      for ( size_t i=begin ; i<end ; ++i ) {
        TrackElement* segment2 = track->getSegment(i);

        cdebug.log(159) << "* Looking // " << segment2 << endl;

        if (    segment2->getNet() == ownerNet) continue;
        if (not toFree.intersect(segment2->getCanonicalInterval())) continue;

        if (overlap.isEmpty())
          overlap = segment2->getCanonicalInterval();
        else
          overlap.merge( segment2->getCanonicalInterval() );
      }

      if (not overlap.isEmpty() and makeDogleg(overlap)) return true;
    }

    if (not _segment->canDogleg()) return false;
    _segment->makeDogleg();

    return true;
  }


  bool  Manipulator::makeDogleg ( Interval overlap )
  {
    cdebug.log(159) << "Manipulator::makeDogleg(Interval) " << _segment << endl; 
    cdebug.log(159) << overlap << endl;

    if (    _segment->isFixed  ()       ) return false;
    if (not _segment->canDogleg(overlap)) return false;

    unsigned int  flags      = 0;
    TrackElement* dogleg     = _segment->makeDogleg(overlap,flags);
    if (dogleg) {
      cdebug.log(159) << "Manipulator::makeDogleg(Interval) - Push dogleg to the "
                  << ((flags&Katabatic::KbDoglegOnLeft)?"left":"right") << endl;
      if (_segment->isTerminal()) {
        Katabatic::AutoContact* contact =
          (flags&Katabatic::KbDoglegOnLeft) ? _segment->base()->getAutoSource()
                                            : _segment->base()->getAutoTarget();
        DbU::Unit     axisHint = (_segment->isHorizontal()) ? contact->getX() : contact->getY();
        RoutingEvent* event    = dogleg->getDataNegociate()->getRoutingEvent();
        if (event) {
          event->setAxisHint    ( axisHint );
          event->setForcedToHint( true );
          cdebug.log(159) << "Forced to axis hint @" << DbU::getValueString(axisHint) << endl;
        }
      }
      return true;
    }

    return false;
  }


  bool  Manipulator::makeDogleg ( DbU::Unit position )
  {
    cdebug.log(159) << "Manipulator::makeDogleg(position) " << _segment << endl; 
    cdebug.log(159) << "Breaking position: " << DbU::getValueString(position) << endl;

    if (_segment->isFixed()) return false;

    Katabatic::GCellVector gcells;
    _segment->getGCells( gcells );

    size_t igcell = 0;
    for ( ; igcell<gcells.size() ; igcell++ ) {
      if (gcells[igcell]->getSide(_segment->getDirection()).contains(position))
        break;
    }
    if (igcell == gcells.size()) return false;
    if (not _segment->canDogleg(gcells[igcell])) return false;

    TrackElement* dogleg   = NULL;
    TrackElement* parallel = NULL;
    _segment->makeDogleg( gcells[igcell], dogleg, parallel );
    return (dogleg != NULL);
  }


  bool  Manipulator::minimize ()
  {
    cdebug.log(159) << "Manipulator::minimize() " << _segment << endl; 

    if (_segment->isFixed()) return false;
    if (not _event->canMinimize()) return false;

    DbU::Unit  minSpan = DbU::Max;
    DbU::Unit  maxSpan = DbU::Min;
    Interval   punctualSpan ( false );

    if (_segment->base()->getAutoSource()->getAnchor()) {
      cdebug.log(159) << "  | " << _segment->base()->getAutoSource() << endl;
      Interval constraints ( _segment->base()->getAutoSource()->getUConstraints
                             (perpandicularTo(_segment->getDirection())) );
      cdebug.log(159) << "  | Constraints: " << constraints << endl;

      minSpan = min( minSpan, constraints.getVMax() );
      maxSpan = max( maxSpan, constraints.getVMin() );
      punctualSpan.intersection( constraints );
    }

    if (_segment->base()->getAutoTarget()->getAnchor()) {
      cdebug.log(159) << "  | " << _segment->base()->getAutoTarget() << endl;
      Interval constraints ( _segment->base()->getAutoTarget()->getUConstraints
                             (perpandicularTo(_segment->getDirection())) );
      cdebug.log(159) << "  | Constraints: " << constraints << endl;

      minSpan = min( minSpan, constraints.getVMax() );
      maxSpan = max( maxSpan, constraints.getVMin() );
      punctualSpan.intersection( constraints );
    }

    const vector<TrackElement*>& perpandiculars = _event->getPerpandiculars();
    for ( size_t i=0 ; i<perpandiculars.size() ; i++ ) {
      DataNegociate* data2 = perpandiculars[i]->getDataNegociate();
      if (not data2) continue;

      cdebug.log(159) << "  | " << perpandiculars[i] << endl;

      RoutingEvent* event2 = data2->getRoutingEvent();
      if (not event2) continue;

      cdebug.log(159) << "  | Constraints: " << event2->getConstraints() << endl;

      minSpan = min( minSpan, event2->getConstraints().getVMax() );
      maxSpan = max( maxSpan, event2->getConstraints().getVMin() );
      punctualSpan.intersection( event2->getConstraints() );
    }
    if (minSpan > maxSpan) swap( minSpan, maxSpan );

    cdebug.log(159) << "punctualSpan: " << punctualSpan
                << " min/max span: [" << DbU::getValueString(minSpan)
                <<                ":" << DbU::getValueString(maxSpan) << "]"
                << " long: ["         << minSpan
                <<                ":" << maxSpan << "]" << endl;

    vector<Interval> holes;
    for ( size_t itrack=0 ; itrack<_fsm.getCosts().size() ; itrack++ ) {
      size_t  begin = _fsm.getBegin(itrack);
      size_t  end   = _fsm.getEnd  (itrack);
      Track*  track = _fsm.getTrack(itrack);

      if (end < track->getSize()) end++;

      cdebug.log(159) << "Looking for holes in " << _fsm.getCost(itrack) << endl;

      TrackElement* otherPrevious = NULL;
    // ToDo: Manage disjoint but subsequent segment of a Net.
    //       (currently, that hole will not be found).
      for ( ; begin < end ; begin++ ) {
        TrackElement* otherSegment = track->getSegment(begin);
        if (otherSegment->getNet() == _segment->getNet()) continue;
        if (not otherPrevious) {
          holes.push_back( Interval(track->getMin()
                                   ,otherSegment->getSourceU()) );
          cdebug.log(159) << "| First hole: " << holes.back() << " " << otherSegment << endl;
        } else {
          if (otherSegment->getNet() == otherPrevious->getNet()) continue;

          holes.push_back( Interval(otherPrevious->getTargetU()
                                   ,otherSegment ->getSourceU()) );
          cdebug.log(159) << "| Found hole: " << holes.back()
                      << " " << otherPrevious << " <-> " << " " << otherSegment << endl;
        }
        otherPrevious = otherSegment;
      }
    }

    if (holes.empty()) {
      cdebug.log(159) << "No holes found to minimize into." << endl;
      return false;
    }

    Interval  currentSpan = _segment->getCanonicalInterval();
    Interval  biggestHole;
    for ( size_t i=0 ; i<holes.size() ; i++ ) {
      if ( currentSpan.getIntersection(holes[i]   ).getSize()
         > currentSpan.getIntersection(biggestHole).getSize() )
        biggestHole = holes[i];
    }

    DbU::Unit axisHint = 0;
    if (not punctualSpan.isEmpty()) {
      bool success = false;

      if (biggestHole.intersect(punctualSpan)) {
        cdebug.log(159) << "Go as punctual into biggest hole: " << biggestHole << endl;
        axisHint = biggestHole.intersection(punctualSpan).getCenter();
        success = true;
      } else {
        for ( size_t i=0 ; i<holes.size() ; i++ ) {
          cdebug.log(159) << "Trying secondary hole: " << holes[i] << endl;
          if (holes[i].intersect(punctualSpan)) {
            biggestHole = holes[i];
            axisHint = biggestHole.intersection(punctualSpan).getCenter();
            cdebug.log(159) << "Go as punctual into secondary hole: " << biggestHole << endl;
            success = true;
          }
        }
      }

      if (not success) {
        cdebug.log(159) << "No suitable hole found." << endl;
        return false;
      }
    } else {
      biggestHole.intersection( Interval(minSpan,maxSpan) );
      if (biggestHole.isEmpty()) {
        cdebug.log(159) << "Biggest hole is empty (under span ["
                    << DbU::getValueString(minSpan) << " "
                    << DbU::getValueString(maxSpan) << "])" << endl;
        return false;
      } else {
        cdebug.log(159) << "Go into biggest hole: " << biggestHole << endl;
        axisHint = biggestHole.getCenter();
      }
    }

    cdebug.log(159) << "Axis Hint: " << DbU::getValueString(axisHint) << endl;

    for ( size_t i=0 ; i<perpandiculars.size() ; i++ ) {
      DataNegociate* data2 = perpandiculars[i]->getDataNegociate();
      if (not data2) continue;

      cdebug.log(159) << "  | " << perpandiculars[i] << endl;

      RoutingEvent* event2 = data2->getRoutingEvent();
      if (not event2) continue;

      _fsm.addAction( perpandiculars[i], SegmentAction::SelfRipupPerpandWithAxisHint, axisHint );
    }

    _event->setMinimized();

    return true;
  }


  void  Manipulator::reprocessPerpandiculars ()
  {
    if ( _event->getAxisHistory() == _event->getAxisHint() ) return;

    bool moveLeft = (_event->getAxisHistory() > _event->getAxisHint());

    const vector<TrackElement*>& perpandiculars = _event->getPerpandiculars();
    for ( size_t iperpand=0 ; iperpand<perpandiculars.size() ; iperpand++ ) {
      TrackElement*  perpandicular = perpandiculars[iperpand];
      DataNegociate* data          = perpandicular->getDataNegociate();

      if ( perpandicular->isFixed() ) continue;
      if ( not data ) continue;
      if ( not perpandicular->getTrack() ) continue;
      if ( not Manipulator(perpandicular,_fsm).canRipup()
         or (data->getState() >= DataNegociate::MaximumSlack) ) continue;

    // Ugly: ExtensionCap usage.
      if ( moveLeft ) {
        if ( perpandicular->getTargetU()-Session::getExtensionCap(getLayer()) == _event->getAxisHistory() )
          _fsm.addAction ( perpandicular, SegmentAction::OtherRipupPerpandAndPacking );
      } else {
        if ( perpandicular->getSourceU()+Session::getExtensionCap(getLayer()) == _event->getAxisHistory() )
          _fsm.addAction ( perpandicular, SegmentAction::OtherRipupPerpandAndPacking );
      }
    }
  }


  void  Manipulator::repackPerpandiculars ()
  {
    cdebug.log(159) << "Manipulator::repackPerpandiculars()" << endl;

    const vector<TrackElement*>& perpandiculars = _event->getPerpandiculars();
    for ( size_t iperpand=0 ; iperpand<perpandiculars.size() ; iperpand++ ) {
      TrackElement*  perpandicular = perpandiculars[iperpand];
      DataNegociate* data          = perpandicular->getDataNegociate();

      if (perpandicular->isFixed ()) continue;
      if (perpandicular->isGlobal()) continue;
      if (not data) continue;

      if (RoutingEvent::getStage() == RoutingEvent::Repair) {
        data->setState( DataNegociate::Repair );
        if (data->getStateCount() > 1) data->resetStateCount();
      }
      _fsm.addAction( perpandicular, SegmentAction::SelfRipupPerpand );
    }
    _fsm.addAction( _segment, SegmentAction::SelfRipup|SegmentAction::EventLevel4 );
  }


}  // Kite namespace.
