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
// |  C++ Module  :       "./Manipulator.cpp"                        |
// +-----------------------------------------------------------------+


#include "hurricane/DebugSession.h"
#include "hurricane/Bug.h"
#include "katana/TrackSegment.h"
#include "katana/Track.h"
#include "katana/Tracks.h"
#include "katana/DataNegociate.h"
#include "katana/RoutingPlane.h"
#include "katana/RoutingEvent.h"
#include "katana/SegmentFsm.h"
#include "katana/Manipulator.h"
#include "katana/KatanaEngine.h"


namespace {

  using namespace std;
  using namespace Hurricane;
  using namespace Katana;
  using Anabatic::GCell;


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


namespace Katana {

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

    DebugSession::open( _segment->getNet(), 156, 160 );

    _data = _segment->getDataNegociate();
    if (_data) _event = _data->getRoutingEvent();
  }


  Manipulator::~Manipulator ()
  { DebugSession::close(); }


  bool  Manipulator::canRipup ( uint32_t flags ) const
  {
    if (_data) {
      if (not _event or _event->isUnimplemented()) return false;

      uint32_t limit = Session::getKatanaEngine()->getRipupLimit(_segment);
      uint32_t count = _data->getRipupCount() + ((flags & NotOnLastRipup) ? 1 : 0);

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


  bool  Manipulator::ripup ( uint32_t type, DbU::Unit axisHint )
  {
    cdebug_log(159,1) << "Manipulator::ripup()" << endl;

    if (not canRipup())      { cdebug_tabw(159,-1); return false; }
    if (_segment->isFixed()) { cdebug_tabw(159,-1); return false; }
    if (_data == NULL)       { cdebug_tabw(159,-1); return true;  }

    _fsm.addAction( _segment, type, axisHint );
    cdebug_tabw(159,-1);
    return true;
  }


  bool  Manipulator::ripupPerpandiculars ( uint32_t flags )
  {
    cdebug_log(159,1) << "Manipulator::ripupPerpandiculars() - " << flags << endl;

    bool      success                  = true;
    bool      cagedPerpandiculars      = false;
    Interval  constraints              ( _event->getConstraints() );
    Interval  perpandicularConstraints ( constraints );
    size_t    placedPerpandiculars     = 0;
    uint32_t  parallelActionFlags      = SegmentAction::SelfRipup|SegmentAction::EventLevel4;
    uint32_t  perpandicularActionFlags = SegmentAction::SelfRipupPerpand;

    if (flags & Manipulator::PerpandicularsFirst) {
      parallelActionFlags      &= ~SegmentAction::EventLevel4;
      perpandicularActionFlags |=  SegmentAction::EventLevel4;
      if (flags & Manipulator::ToRipupLimit)
        perpandicularActionFlags |=  SegmentAction::ToRipupLimit;
    } else {
      if (flags & Manipulator::ToRipupLimit)
        parallelActionFlags |=  SegmentAction::ToRipupLimit;
    }

    cdebug_log(159,0) << "Pure constraints: " << constraints << endl;

    Track* track = NULL;
    const vector<TrackElement*>& perpandiculars = _event->getPerpandiculars();

    for ( size_t i=0 ; i < perpandiculars.size() ; i++ ) {
      cdebug_log(159,0) << "| " << perpandiculars[i] << endl;

      track = perpandiculars[i]->getTrack();
      if (not track) {
      // The perpandicular is not placed yet.
        if (perpandiculars[i]->isFixedAxis()) {
          RoutingPlane* plane = Session::getKatanaEngine()->getRoutingPlaneByLayer(perpandiculars[i]->getLayer());
          Track*        track = plane->getTrackByPosition( perpandiculars[i]->getAxis() );
          if (track) {
            TrackElement* other = track->getSegment( _segment->getAxis() );
            if (other and (other->getNet() != _segment->getNet()) ) {
              _fsm.addAction( other, SegmentAction::OtherRipup );
            }
          }
        } else {
          if (flags & Manipulator::PerpandicularsFirst) {
            _fsm.addAction( perpandiculars[i], perpandicularActionFlags );
          }
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

      if ( (flags & Manipulator::ToMoveUp) and (data2->getState() < DataNegociate::MoveUp) ) {
        cdebug_log(159,0) << "Force move up of perpandicular." << endl;
        data2->setState( DataNegociate::MoveUp );
      }
        
      if (Manipulator(perpandiculars[i],_fsm).ripup(perpandicularActionFlags)) {
        if (dislodgeCaged) {
          _event->setAxisHint( _event->getSegment()->getAxis() + _event->getSegment()->getPitch() );
        }
        continue;
      }

    // Cannot ripup the perpandicular, try to ripup it's neigbors.
      cdebug_log(159,0) << "Try to ripup neighbors." << endl;
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
          cdebug_log(159,0) << "  | Ripup: " << begin << " " << other << endl;
          _fsm.addAction( other, SegmentAction::OtherRipup );
        } else {
          cdebug_log(159,0) << "Aborted ripup of perpandiculars, fixed or blocked." << endl;
          cdebug_tabw(159,-1);
          return false;
        }
      }
    }

    if (cagedPerpandiculars and not placedPerpandiculars) {
      cdebug_log(159,0) << "Aborted ripup of perpandiculars, constraints are due to fixed/blockage." << endl;
      _fsm.addAction( _segment, SegmentAction::SelfRipup );
      cdebug_tabw(159,-1);
      return true;
    }

    if (_segment->isLocal() and not placedPerpandiculars) {
      cdebug_log(159,0) << "No placed perpandiculars, tight native constraints, place perpandiculars FIRST." << endl;
      for ( size_t i=0 ; i < perpandiculars.size() ; i++ ) {
        _fsm.addAction( perpandiculars[i], perpandicularActionFlags|SegmentAction::EventLevel4 );
      }
      _fsm.addAction( _segment, parallelActionFlags );
      cdebug_tabw(159,-1);
      return true;
    }

    RoutingPlane* plane    = Session::getKatanaEngine()->getRoutingPlaneByLayer(_segment->getLayer());
    size_t        tracksNb = 0;

    track = plane->getTrackByPosition(constraints.getVMin());

    if (track and (track->getAxis() < constraints.getVMin())) track = track->getNextTrack();
    for ( ; track && (track->getAxis() <= constraints.getVMax())
          ; track = track->getNextTrack(), tracksNb++ );

    if (_segment->isLocal() and (tracksNb < 2)) success = ripple();

    _fsm.addAction( _segment, parallelActionFlags );

    cdebug_tabw(159,-1);
    return success;
  }


  bool  Manipulator::relax ( Interval interval, uint32_t flags )
  {
    Session::toAxisInterval( interval, Session::getLayerDepth(_segment->getLayer())+1 );
    cdebug_log(159,0) << "Manipulator::relax() of: " << _segment << " " << interval << endl; 

    if (_segment->isFixed()) return false;
    if (not interval.intersect(_segment->getCanonicalInterval())) return false;
    if (not _data) return false;

    if (    _segment->isTerminal()
       and (_segment->getLayer() == Session::getRoutingGauge()->getRoutingLayer(1)) ) {
      if (interval.contains(_segment->base()->getAutoSource()->getX())) return false;
      if (interval.contains(_segment->base()->getAutoTarget()->getX())) return false;
    }

    cdebug_tabw(159,1);
    bool success = true;
    bool expand = _segment->isGlobal() and (flags&AllowExpand);
    cdebug_log(159,0) << "Expand:" << expand << endl;

    vector<GCell*> gcells;
    _segment->getGCells( gcells );

    if (gcells.size() < 2) {
      cerr << Bug( "relax() Cannot break %s,\n      only in %s."
                 , getString(_segment).c_str()
                 , getString(gcells[0]).c_str()
                 ) << endl;
      cdebug_tabw(159,-1);
      return false;
    }

    uint32_t  depth = Session::getRoutingGauge()->getLayerDepth(_segment->getLayer());
    Interval  uside;
    size_t    dogLegCount  = 0;
    size_t    iminconflict = gcells.size();
    size_t    imaxconflict = gcells.size();
    size_t    igcell;

  // Look for closest enclosing min & max GCells indexes.
    for ( igcell=0 ; igcell<gcells.size() ; igcell++ ) {
      uside = gcells[igcell]->getSide(_segment->getDirection());
      cdebug_log(159,0) << "| " << setw(3) << igcell << " " << gcells[igcell] << " uside: " << uside << endl;

      if (uside.contains(interval.getVMin())) {
        iminconflict = igcell;
        cdebug_log(159,0) << "> Min conflict: " << iminconflict << endl;
      }
      if (uside.contains(interval.getVMax())) {
        imaxconflict = igcell;
        cdebug_log(159,0) << "> Max conflict: " << imaxconflict << endl;
      }
    }

  // Expand min & max to enclose GCells of greatest or equal order
  // (i.e. less saturateds)
    bool minExpanded = false;
    bool maxExpanded = false;
    if (expand) {
      if (iminconflict < gcells.size()) {
      //cdebug_log(159,0) << "Expand min" << endl;

        size_t imindensity = 0;
        for ( size_t iexpand=1 ; iexpand<iminconflict ; ++iexpand ) {
          if (not _segment->canDogleg(gcells[iexpand],Flags::AllowDoglegReuse)) continue;

          // cdebug_log(159,0) << "<GCell [" << iexpand << "]> Density "
          //             << "Density " << Session::getRoutingGauge()->getRoutingLayer(depth)->getName()
          //             << " min. dens.:" << gcells[imindensity]->getDensity(depth)
          //             << " exp. dens.:" << gcells[iexpand    ]->getDensity(depth)
          //             << endl;

          if (gcells[imindensity]->getDensity(depth) - gcells[iexpand]->getDensity(depth) > 1e-3) {
            imindensity = iexpand;
          //cdebug_log(159,0) << "Accepted expand " << imindensity << endl;
          }
        }

        if (iminconflict != imindensity) minExpanded = true;
        iminconflict = (imindensity>0) ? imindensity : gcells.size();
      }

      if (imaxconflict < gcells.size()) {
      //cdebug_log(159,0) << "Expand max" << endl;

        size_t imindensity = imaxconflict;
        for ( size_t iexpand=imaxconflict+1 ; iexpand<gcells.size() ; ++iexpand ) {
          if (not _segment->canDogleg(gcells[iexpand],Flags::AllowDoglegReuse)) continue;

          // cdebug_log(159,0) << "<GCell [" << iexpand << "]> Density "
          //             << Session::getRoutingGauge()->getRoutingLayer(depth)->getName()
          //             << " min. dens.:" << gcells[imindensity]->getDensity(depth)
          //             << " exp. dens.:" << gcells[iexpand    ]->getDensity(depth)
          //             << endl;

          if (gcells[imindensity]->getDensity(depth) - gcells[iexpand]->getDensity(depth) > 1e-3) {
            imindensity = iexpand;
          //cdebug_log(159,0) << "Accepted expand " << imindensity << endl;
          }
        }
        
        if (imindensity != imaxconflict) maxExpanded = true;
        imaxconflict = (imindensity < gcells.size()) ? imindensity : gcells.size();
      }
    }
    cdebug_log(159,0) <<   "minExpanded:" << minExpanded << " (" << iminconflict
                << ") maxExpanded:" << maxExpanded << " (" << imaxconflict << ")" << endl;

  // Check for full enclosure.
    if (  ( (iminconflict == gcells.size()) and (imaxconflict == gcells.size()  ) )
       or ( (iminconflict == 0)             and (imaxconflict == gcells.size()-1) )) {
      cinfo << "[INFO] Manipulator::relax(): Segment fully enclosed in interval." << endl;
      cdebug_tabw(159,-1);
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
        cdebug_tabw(159,-1);
        return false;
    }

    cdebug_log(159,0) << "| Has to do " << dogLegCount << " doglegs." << endl;

  // Check of "min is less than one track close the edge" (while not expanded).
  // AND we are on the first GCell AND there's one dogleg only.
    if (not minExpanded and (iminconflict == 0) and (imaxconflict == gcells.size())) {
      cdebug_log(159,0) << "Cannot break in first GCell only." << endl;
      cdebug_tabw(159,-1);
      return false;
    }

  // Check of "min is less than one track close the edge" (while not expanded).
    if ( /*not minExpanded and*/ (iminconflict > 0) and (iminconflict < gcells.size()) ) {
      uside = gcells[iminconflict-1]->getSide(_segment->getDirection());
      cdebug_log(159,0) << "GCell Edge Comparison (min): " << uside
                  << " vs. " << DbU::getValueString(interval.getVMin()) << endl;
    // Ugly: One lambda shrink.
      if (interval.getVMin()-DbU::lambda(1.0) <= uside.getVMax()) {
        cdebug_log(159,0) << "Using previous GCell." << endl;
        iminconflict--;
      }
    }

  // Check if there is only one dogleg AND it's the last one.
    if (not maxExpanded and (iminconflict == gcells.size()) and (imaxconflict == gcells.size()-1)) {
      cdebug_log(159,0) << "Cannot break in last GCell only." << endl;
      cdebug_tabw(159,-1);
      return false;
    }

  // Check of "max is less than one track close the edge" (while not expanded).
    if ((imaxconflict < gcells.size()-1)) {
      uside = gcells[imaxconflict+1]->getSide( _segment->getDirection() );
      cdebug_log(159,0) << "GCell Edge Comparison (max): " << uside
                  << " vs. " << DbU::getValueString(interval.getVMax()) << endl;
      if (interval.getVMax()+getPPitch() >= uside.getVMin()) {
        interval.inflate( 0, getPPitch() );
        cdebug_log(159,0) << "Using next GCell " << interval << endl;
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
    cdebug_log(159,0) << "Making FIRST dogleg at " << ifirstDogleg << endl;
    TrackElement*     segment1     = NULL;
    TrackElement*     segment2     = NULL;
    Track*            track        = _segment->getTrack();
    Anabatic::GCell* dogLegGCell  = gcells[ifirstDogleg];
    TrackElement*     dogleg       = NULL;
    DbU::Unit         doglegAxis;
    bool              doglegReuse1 = false;
    bool              doglegReuse2 = false;

  // Try to reuse existing dogleg if broken at either end.
    if (ifirstDogleg == 0)               dogleg = _segment->getSourceDogleg();
    if (ifirstDogleg == gcells.size()-1) dogleg = _segment->getTargetDogleg();
    if (dogleg) {
      cdebug_log(159,0) << "Reusing dogleg." << endl;
      doglegReuse1 = true;
      segment1     = _segment;
    } else {
    // Try to create a new dogleg.
      if (not _segment->canDogleg(dogLegGCell)) {
        cdebug_log(159,0) << "Cannot create FIRST dogleg." << endl;
        cdebug_tabw(159,-1);
        return false;
      }
      if (not _segment->makeDogleg( dogLegGCell, dogleg, segment1 )) {
        cdebug_log(159,0) << "FIRST makeDogleg() call failed (BUG)." << endl;
        cdebug_tabw(159,-1);
        return false;
      }
    }

    if (firstDoglegIsMin) {
      if (minExpanded) {
        doglegAxis = dogLegGCell->getSide( _segment->getDirection() ).getCenter();
      //cdebug_log(159,0) << "MARK 1 doglegAxis: " << DbU::getValueString(doglegAxis) << endl;
      } else {
        doglegAxis = interval.getVMin() - getPPitch();
      //cdebug_log(159,0) << "MARK 2 doglegAxis: " << DbU::getValueString(doglegAxis) << endl;
      }
    } else {
      if (maxExpanded) {
        doglegAxis = dogLegGCell->getSide( _segment->getDirection() ).getVMin();
      //cdebug_log(159,0) << "MARK 3 doglegAxis: " << DbU::getValueString(doglegAxis) << endl;
      } else {
        doglegAxis = interval.getVMax() + getPPitch() - DbU::fromLambda(1.0);
      //cdebug_log(159,0) << "MARK 4 doglegAxis: " << DbU::getValueString(doglegAxis) << endl;
      }
    }
    if (doglegReuse1) _fsm.addAction( dogleg, SegmentAction::OtherRipup );
    else              dogleg->setAxis( doglegAxis );

  // If event is present, the dogleg is in the current RoutingSet.
    RoutingEvent* event = dogleg->getDataNegociate()->getRoutingEvent();
    if (event) {
      cdebug_log(159,0) << "Set Axis Hint: @" << DbU::getValueString(doglegAxis) << " " << dogleg << endl;
      event->setAxisHint( doglegAxis );
    } else {
      cdebug_log(159,0) << "Dogleg has no RoutingEvent yet." << endl;
    }

  // Making second dogleg.
    if (dogLegCount > 1) {
      cdebug_log(159,0) << "Making SECOND dogleg at " << isecondDogleg
                  << " on " << segment1 << endl;
      
      dogleg      = NULL;
      dogLegGCell = gcells[isecondDogleg];

      if (ifirstDogleg == isecondDogleg) {
        cdebug_log(159,0) << "Double break in same GCell." << endl;
        segment1->setFlags( TElemSourceDogleg );
      }

      if (isecondDogleg == gcells.size()-1) dogleg = segment1->getTargetDogleg();
      if (dogleg) {
        cdebug_log(159,0) << "Reusing dogleg." << endl;
        doglegReuse2 = true;
        segment2     = segment1;
      } else {
      // Try to create a new dogleg.
        if (not segment1->canDogleg(dogLegGCell)) {
          cdebug_log(159,0) << "Cannot create SECOND dogleg." << endl;
          cdebug_tabw(159,-1);
          return false;
        }
        if (not segment1->makeDogleg( dogLegGCell, dogleg, segment2 )) {
          cdebug_log(159,0) << "SECOND makeDogleg() call failed (BUG)." << endl;
          cdebug_tabw(159,-1);
          return false;
        }
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
        cdebug_log(159,0) << "Set Axis Hint: @" << DbU::getValueString(doglegAxis) << " " << dogleg << endl;
        event->setAxisHint( doglegAxis );
      } else {
        cdebug_log(159,0) << "Dogleg has no RoutingEvent yet." << endl;
      }

    // This cases seems never to occurs.
      const vector<AutoSegment*>& doglegs = Session::getDoglegs();
      for ( size_t i=0 ; i<doglegs.size() ; i++ ) { 
        TrackElement* segment = Session::lookup(doglegs[i]);
        if (not segment->getTrack() and track) {
          cdebug_log(159,0) << "Direct Track insert of: " << segment << endl;
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
      cdebug_log(159,0) << "Reset state of: " << _segment << endl;
      _segment->getDataNegociate()->setState( DataNegociate::RipupPerpandiculars, true );
    } else {
      cdebug_log(159,0) << "No state reset: " << _segment << endl;
    }

    if ((not doglegReuse1) and segment1 and segment1->isLocal()) {
      cdebug_log(159,0) << "Reset state of: " << segment1 << endl;
      segment1->getDataNegociate()->setState( DataNegociate::RipupPerpandiculars, true );
    }

    if ((not doglegReuse2) and segment2 and segment2->isLocal()) {
      cdebug_log(159,0) << "Reset state of: " << segment2 << endl;
      segment2->getDataNegociate()->setState( DataNegociate::RipupPerpandiculars, true );
    }

    cdebug_tabw(159,-1);
    return success;
  }


  bool  Manipulator::insertInTrack ( size_t icost )
  {
    cdebug_log(159,1) << "Manipulator::insertInTrack(size_t)" << endl;
    cdebug_log(159,0) << _segment << endl;

    bool success = true;

    for ( size_t itrack=0 ; success and (itrack<_segment->getTrackSpan()) ; ++itrack ) {
      success = success and _insertInTrack( icost, itrack );
    }

    if (success) {
      cdebug_log(159,0) << "Manipulator::insertInTrack() success" << endl;

      _fsm.setState ( SegmentFsm::OtherRipup );
      _fsm.addAction( _segment
                    , SegmentAction::SelfInsert|SegmentAction::MoveToAxis|SegmentAction::EventLevel4 
                    , _fsm.getTrack1(icost)->getAxis() );

#if THIS_IS_DISABLED
      uint32_t flags = 0;
      if (rightIntrication) flags |= RightAxisHint;
      if (leftIntrication ) flags |= LeftAxisHint;
      if (flags)
        Manipulator( _segment, _fsm ).shrinkToTrack( icost, flags, leftAxisHint, rightAxisHint );
#endif
    } else
      _fsm.clearActions();

    cdebug_tabw(159,-1);
    return success;
  }


  bool  Manipulator::_insertInTrack ( size_t icost, size_t itrack )
  {
    Track*              track            = _fsm.getTrack(icost,itrack);
    size_t              begin            = _fsm.getBegin(icost,itrack);
    size_t              end              = _fsm.getEnd  (icost,itrack);
    Net*                ownerNet         = _segment->getNet();
    Interval            toFree            (_segment->getCanonicalInterval());
  //Net*                ripupNet         = NULL;
    set<TrackElement*>  canonicals;
  //DbU::Unit           rightAxisHint    = 0;
  //DbU::Unit           leftAxisHint     = 0;
  //bool                leftIntrication  = false;
  //bool                rightIntrication = false;
    bool                success          = true;

    cdebug_log(159,1) << "Manipulator::_insertInTrack(size_t) - " << toFree << endl;
    cdebug_log(159,0) << _segment << endl;

    for ( size_t i = begin ; success and (i < end) ; i++ ) {
      TrackElement* segment2 = track->getSegment(i);

      cdebug_log(159,0) << "* Looking // " << segment2 << endl;

      if ( segment2->getNet() == ownerNet  ) continue;
      if ( not toFree.intersect(segment2->getCanonicalInterval()) ) {
        cdebug_log(159,0) << "No intersection with: " << segment2->getCanonicalInterval() << endl;
        continue;
      }
      if ( segment2->isBlockage() or segment2->isFixed() ) {
        cdebug_log(159,0) << "Ovelap is blockage or fixed." << endl;
        success = false;
        continue;
      }
      // if ( segment2->getId() >= maxId ) {
      //   cdebug_log(159,0) << "Ovelap has an Id superior to AutoSegment::maxId:" << maxId << "." << endl;
      //   continue;
      // }
      // ripupNet = segment2->getNet();

      DataNegociate* data2 = segment2->getDataNegociate();
      if ( !data2 ) {
        cdebug_log(159,0) << "No DataNegociate, ignoring." << endl;
        continue;
      }

      if ( data2->getState() == DataNegociate::MaximumSlack ) {
        cdebug_log(159,0) << "At " << DataNegociate::getStateString(data2)
                    << " for " << segment2 << endl;
        success = false;
        continue;
      }

      bool shrinkLeft  = false;
      bool shrinkRight = false;

      if ( data2->getRightMinExtend() < toFree.getVMin() ) {
        cdebug_log(159,0) << "- Shrink right edge (push left) " << segment2 << endl;
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
        cdebug_log(159,0) << "- Shrink left edge (push right) " << segment2 << endl;
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

      cdebug_log(159,0) << "- Hard overlap/enclosure/shrink " << segment2 << endl;
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
          cdebug_log(159,0) << "  . " << segment3 << endl;
          continue;
        }

        cdebug_log(159,0) << "  | " << segment3 << endl;

        if ( shrinkRight xor shrinkLeft ) {
          if ( shrinkRight ) {
            if ( not (success=Manipulator(segment3,_fsm)
                     .ripup( SegmentAction::OtherRipupPerpandAndPushAside
                           , toFree.getVMin() - getPPitch()/2
                           )) )
              break;

            if ( event3->getTracksFree() == 1 ) {
              cdebug_log(159,0) << "Potential left intrication with other perpandicular." << endl;
              if ( segment3->getAxis() == segment2->getTargetAxis() ) {
              //leftIntrication = true;
              //leftAxisHint    = segment3->getAxis();
              }
            }
          }
          if ( shrinkLeft  ) {
            cdebug_log(159,0) << "Move PP to right: "
                              << DbU::getValueString(toFree.getVMax()) << " + "
                              << DbU::getValueString(getPPitch()/2)
                              << endl;
            if ( not (success=Manipulator(segment3,_fsm)
                     .ripup( SegmentAction::OtherRipupPerpandAndPushAside
                           , toFree.getVMax() + getPPitch()/2
                           )) )
              break;
            if ( event3->getTracksFree() == 1 ) {
              cdebug_log(159,0) << "Potential right intrication with other perpandicular." << endl;
              if ( segment3->getAxis() == segment2->getSourceAxis() ) {
              //rightIntrication = true;
              //rightAxisHint    = segment3->getAxis();
              }
            }
          }
        } else {
          if ( not (success=Manipulator(segment3,_fsm).ripup( SegmentAction::OtherRipup
                                                            | SegmentAction::EventLevel3 )) )
            break;
        }
      }
    }

    cdebug_tabw(159,-1);
    return success;
  }


  bool  Manipulator::forceToTrack ( size_t icost )
  {
    cdebug_log(159,1) << "Manipulator::forceToTrack(size_t)" << endl;
    cdebug_log(159,0) << _segment << endl;

    bool success = true;

    for ( size_t itrack=0 ; success and (itrack<_segment->getTrackSpan()) ; ++itrack ) {
      success = success and _forceToTrack( icost, itrack );
    }

    if (success) {
      _fsm.setState ( SegmentFsm::OtherRipup );
      _fsm.addAction( _segment
                    , SegmentAction::SelfInsert|SegmentAction::MoveToAxis
                    , _fsm.getTrack(icost)->getAxis() );
    } else
      _fsm.clearActions();

    cdebug_tabw(159,-1);
    return success;
  }


  bool  Manipulator::_forceToTrack ( size_t icost, size_t itrack )
  {
    Track*              track      = _fsm.getTrack(icost,itrack);
    size_t              begin      = _fsm.getBegin(icost,itrack);
    size_t              end        = _fsm.getEnd  (icost,itrack);
    Net*                ownerNet   = _segment->getNet();
    Interval            toFree      (_segment->getCanonicalInterval());
  //Net*                ripupNet   = NULL;
    set<TrackElement*>  canonicals;
    bool                success    = true;

    cdebug_log(159,1) << "Manipulator::_forceToTrack(size_t) - " << toFree << endl;

    for ( size_t i=begin ; success and (i < end) ; ++i ) {
      TrackElement* segment2 = track->getSegment(i);

      cdebug_log(159,0) << "* Looking // " << segment2 << endl;

      if (segment2->getNet() == ownerNet) continue;
      if (not toFree.intersect(segment2->getCanonicalInterval())) continue;
      if (segment2->isFixed()) {
        success = false;
        continue;
      }
    //ripupNet = segment2->getNet();

      DataNegociate* data2 = segment2->getDataNegociate();
      if (not data2 ) {
        cdebug_log(159,0) << "No DataNegociate, ignoring." << endl;
        continue;
      }

      cdebug_log(159,0) << "- Forced ripup " << segment2 << endl;
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

    cdebug_tabw(159,-1);
    return success;
  }


  bool  Manipulator::shrinkToTrack ( size_t icost, uint32_t flags, DbU::Unit leftAxisHint, DbU::Unit rightAxisHint )
  {
    cdebug_log(159,1) << "Manipulator::shrinkToTrack(size_t)" << endl;
    cdebug_log(159,0) << _segment << endl;

    bool success = true;

    for ( size_t itrack=0 ; success and (itrack<_segment->getTrackSpan()) ; ++itrack ) {
      success = success and _shrinkToTrack( icost, itrack, flags, leftAxisHint, rightAxisHint );
    }

    cdebug_tabw(159,-1);
    return success;
  }


  bool  Manipulator::_shrinkToTrack ( size_t icost, size_t itrack, uint32_t flags, DbU::Unit leftAxisHint, DbU::Unit rightAxisHint )
  {
#if THIS_IS_DISABLED
    Track*              track       = _fsm.getTrack(icost,itrack);
    size_t              begin       = _fsm.getBegin(icost,itrack);
    size_t              end         = _fsm.getEnd  (icost,itrack);
    Net*                ownerNet    = _segment->getNet();
    set<TrackElement*>  canonicals;
    bool                success     = true;
    DbU::Unit           leftExtend  = _segment->getSourceU() + Session::getExtensionCap(getLayer());
    DbU::Unit           rightExtend = _segment->getSourceU() - Session::getExtensionCap(getLayer());

    cdebug_log(159,0) << "Manipulator::shrinkToTrack()" << endl;

    if (_segment->isLocal()) return false;
    Interval  shrunkFree = _segment->base()->getMinSpanU();

    cdebug_log(159,0) << "* " << shrunkFree << endl;

    for ( size_t i = begin ; success and (i < end) ; ++i ) {
      TrackElement* segment2 = track->getSegment(i);

      cdebug_log(159,0) << "* Looking // " << segment2 << endl;

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
          cdebug_log(159,0) << "| perpandicular bound:" << *iperpand << endl;
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

      cdebug_log(159,0) << "Successful shrinkToTrack." << endl;
      return true;
    }
#endif

    return false;
  }


  bool  Manipulator::forceOverLocals ()
  {
    cdebug_log(159,1) << "Manipulator::forceOverLocals()" << endl;

    vector<TrackCost*>& costs = _fsm.getCosts();
    size_t itrack = 0;
    for ( ; itrack<costs.size() ; ++itrack ) {
      cdebug_log(159,0) << "Trying itrack:" << itrack << endl;

      if (  costs[itrack]->isFixed()
         or costs[itrack]->isBlockage()
         or costs[itrack]->isInfinite()
         or costs[itrack]->isOverlapGlobal() )
        continue;

      bool      success    = true;
      Track*    track      = _fsm.getTrack(itrack);
      size_t    begin      = _fsm.getBegin(itrack);
      size_t    end        = _fsm.getEnd  (itrack);
      Net*      ownerNet   = _segment->getNet();
      Interval  toFree      (_segment->getCanonicalInterval());
      
      for ( size_t i = begin ; success and (i < end) ; i++ ) {
        TrackElement* segment2 = track->getSegment(i);

        cdebug_log(159,0) << "* Looking // " << segment2 << endl;

        if ( segment2->getNet() == ownerNet  ) continue;
        if ( not toFree.intersect(segment2->getCanonicalInterval()) ) continue;
        if ( segment2->isFixed() ) {
          success = false;
          continue;
        }

        DataNegociate* data2 = segment2->getDataNegociate();
        if ( not data2 ) {
          cdebug_log(159,0) << "No DataNegociate, ignoring." << endl;
          success = false;
          continue;
        }

        cdebug_log(159,0) << "- Forced ripup " << segment2 << endl;
        if ( not (success=Manipulator(segment2,_fsm).ripup(SegmentAction::OtherRipup)) ) {
          continue;
        }
      }

      if (success) {
        _fsm.setState ( SegmentFsm::OtherRipup );
        _fsm.addAction( _segment
                      , SegmentAction::SelfInsert|SegmentAction::MoveToAxis
                      , _fsm.getTrack(itrack)->getAxis()
                      );
        break;
      }
    }

    cdebug_tabw(159,-1);
    return (itrack < costs.size());
  }


  bool  Manipulator::slacken ( Flags flags )
  {
    cdebug_log(159,0) << "Manipulator::slacken() " << _segment << endl; 

    if (    _segment->isFixed   ()) return false;
    if (not _segment->canSlacken()) return false;

    return _segment->slacken( flags );
  }


  bool  Manipulator::ripple ()
  {
    cdebug_log(159,0) << "Manipulator::ripple() from " << _segment << endl; 

  //if (not _segment->canRipple()) return false;
    if (not _segment->isLocal()) return false;

    Net*          net   = _segment->getNet();
    Interval      uside = _segment->base()->getAutoSource()->getGCell()->getSide ( Anabatic::perpandicularTo(_segment->getDirection())/*, false*/ );
    RoutingPlane* plane = Session::getKatanaEngine()->getRoutingPlaneByLayer(_segment->getLayer());

    cdebug_tabw(159,1);
    for( Track* track : Tracks_Range::get(plane,uside)) {
      size_t begin;
      size_t end;

      track->getOverlapBounds( _segment->getCanonicalInterval(), begin, end );
      for ( ; begin < end ; begin++ ) {
        TrackElement* other = track->getSegment(begin);
        cdebug_log(159,0) << "| " << other << endl;

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
    cdebug_tabw(159,-1);

    return true;
  }


  bool  Manipulator::pivotUp ()
  {
    cdebug_log(159,0) << "Manipulator::pivotUp() " << _segment << endl; 
    return false;

    if (_segment->isFixed()) return false;
    if (_segment->isStrap()) return false;

    float reserve = (_segment->isLocal()) ? 0.5 : 1.0;
    if (not _segment->canMoveUp(reserve)) return false;

    return _segment->moveUp( Flags::NoFlags );
  }


  bool  Manipulator::pivotDown ()
  {
    cdebug_log(159,0) << "Manipulator::pivotDown() " << _segment << endl; 
    return false;

    if (    _segment->isFixed () ) return false;
    if (    _segment->isStrap () ) return false;
    if (not _segment->canPivotDown(2.0,Flags::NoFlags)) return false;

    return _segment->moveDown( Flags::NoFlags );
  }


  bool  Manipulator::moveUp ( uint32_t flags )
  {
    cdebug_log(159,0) << "Manipulator::moveUp() " << _segment << endl; 

    Flags kflags = Flags::WithNeighbors;
  //kflags |= (flags & AllowLocalMoveUp   ) ? Flags::AutoSegment::AllowLocal    : 0;
    kflags |= (flags & AllowTerminalMoveUp) ? Flags::AllowTerminal  : Flags::NoFlags;
    kflags |= (flags & IgnoreContacts     ) ? Flags::IgnoreContacts : Flags::NoFlags;

    if (_segment->isFixed()) return false;
    if (not (flags & AllowLocalMoveUp)) {
      if (_segment->isLocal()) {
        if (not _segment->canPivotUp(0.5,kflags)) return false;
      } else {
        if (_segment->getLength() < 20*getPitch()) {
          if (not (flags & AllowShortPivotUp)) return false;
          if (not _segment->canPivotUp(1.0,kflags)) return false;
        }
        if (not _segment->canMoveUp(0.5,kflags)) return false;
      }
    } else {
      if (not _segment->canMoveUp(0.5,kflags)) return false;
    }
    return _segment->moveUp( kflags|Flags::Propagate );
  }


  bool  Manipulator::makeDogleg ()
  {
    cdebug_log(159,0) << "Manipulator::makeDogleg() " << _segment << endl; 

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

        cdebug_log(159,0) << "* Looking // " << segment2 << endl;

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


  bool  Manipulator::makeDogleg ( Interval overlap, Flags flags )
  {
    cdebug_log(159,0) << "Manipulator::makeDogleg(Interval) " << _segment << endl; 
    cdebug_log(159,0) << overlap << endl;

    if (    _segment->isFixed  ()       ) return false;
    if (not _segment->canDogleg(overlap)) return false;

    TrackElement* dogleg   = NULL;
    TrackElement* parallel = NULL;
    _segment->makeDogleg( overlap, dogleg, parallel, flags );
    if (dogleg) {
      cdebug_log(159,0) << "Manipulator::makeDogleg(Interval) - Push dogleg to the "
                  << ((flags&Flags::DoglegOnLeft)?"left":"right") << endl;
      if (flags & Flags::ShortDogleg)
        Session::addShortDogleg( _segment, parallel );
      
      if (_segment->isTerminal()) {
        Anabatic::AutoContact* contact =
          (flags&Flags::DoglegOnLeft) ? _segment->base()->getAutoSource()
                                      : _segment->base()->getAutoTarget();
        DbU::Unit     axisHint = (_segment->isHorizontal()) ? contact->getX() : contact->getY();
        RoutingEvent* event    = dogleg->getDataNegociate()->getRoutingEvent();
        if (event) {
          event->setAxisHint    ( axisHint );
          event->setForcedToHint( true );
          cdebug_log(159,0) << "Forced to axis hint @" << DbU::getValueString(axisHint) << endl;
        }
      }
      return true;
    }

    return false;
  }


  bool  Manipulator::makeDogleg ( DbU::Unit position )
  {
    cdebug_log(159,0) << "Manipulator::makeDogleg(position) " << _segment << endl; 
    cdebug_log(159,0) << "Breaking position: " << DbU::getValueString(position) << endl;

    if (_segment->isFixed()) return false;

    vector<GCell*> gcells;
    _segment->getGCells( gcells );

    size_t igcell = 0;
    for ( ; igcell<gcells.size() ; igcell++ ) {
      if (gcells[igcell]->getSide(_segment->getDirection()).contains(position))
        break;
    }
    if (igcell == gcells.size()) return false;
    if (gcells[igcell]->isDevice()) {
      if      (igcell > 0)               --igcell;
      else if (igcell < gcells.size()-1) ++igcell;
      else return false;
    }
    
    if (not _segment->canDogleg(gcells[igcell])) return false;

    TrackElement* dogleg   = NULL;
    TrackElement* parallel = NULL;
    _segment->makeDogleg( gcells[igcell], dogleg, parallel );
    return (dogleg != NULL);
  }


  bool  Manipulator::minimize ()
  {
    cdebug_log(159,0) << "Manipulator::minimize() " << _segment << endl; 

    if (_segment->isFixed()) return false;
    if (not _event->canMinimize()) return false;

    DbU::Unit  minSpan = DbU::Max;
    DbU::Unit  maxSpan = DbU::Min;
    Interval   punctualSpan ( false );

    if (_segment->base()->getAutoSource()->getAnchor()) {
      cdebug_log(159,0) << "  | " << _segment->base()->getAutoSource() << endl;
      Interval constraints ( _segment->base()->getAutoSource()->getUConstraints
                             (perpandicularTo(_segment->getDirection())) );
      cdebug_log(159,0) << "  | Constraints: " << constraints << endl;

      minSpan = min( minSpan, constraints.getVMax() );
      maxSpan = max( maxSpan, constraints.getVMin() );
      punctualSpan.intersection( constraints );
    }

    if (_segment->base()->getAutoTarget()->getAnchor()) {
      cdebug_log(159,0) << "  | " << _segment->base()->getAutoTarget() << endl;
      Interval constraints ( _segment->base()->getAutoTarget()->getUConstraints
                             (perpandicularTo(_segment->getDirection())) );
      cdebug_log(159,0) << "  | Constraints: " << constraints << endl;

      minSpan = min( minSpan, constraints.getVMax() );
      maxSpan = max( maxSpan, constraints.getVMin() );
      punctualSpan.intersection( constraints );
    }

    const vector<TrackElement*>& perpandiculars = _event->getPerpandiculars();
    for ( size_t i=0 ; i<perpandiculars.size() ; i++ ) {
      DataNegociate* data2 = perpandiculars[i]->getDataNegociate();
      if (not data2) continue;

      cdebug_log(159,0) << "  | " << perpandiculars[i] << endl;

      RoutingEvent* event2 = data2->getRoutingEvent();
      if (not event2) continue;

      cdebug_log(159,0) << "  | Constraints: " << event2->getConstraints() << endl;

      minSpan = min( minSpan, event2->getConstraints().getVMax() );
      maxSpan = max( maxSpan, event2->getConstraints().getVMin() );
      punctualSpan.intersection( event2->getConstraints() );
    }
    if (minSpan > maxSpan) swap( minSpan, maxSpan );

    cdebug_log(159,0) << "punctualSpan: " << punctualSpan
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

      cdebug_log(159,0) << "Looking for holes in " << _fsm.getCost(itrack) << endl;

      TrackElement* otherPrevious = NULL;
    // ToDo: Manage disjoint but subsequent segment of a Net.
    //       (currently, that hole will not be found).
      for ( ; begin < end ; begin++ ) {
        TrackElement* otherSegment = track->getSegment(begin);
        if (otherSegment->getNet() == _segment->getNet()) continue;
        if (not otherPrevious) {
          holes.push_back( Interval(track->getMin()
                                   ,otherSegment->getSourceU()) );
          cdebug_log(159,0) << "| First hole: " << holes.back() << " " << otherSegment << endl;
        } else {
          if (otherSegment->getNet() == otherPrevious->getNet()) continue;

          holes.push_back( Interval(otherPrevious->getTargetU()
                                   ,otherSegment ->getSourceU()) );
          cdebug_log(159,0) << "| Found hole: " << holes.back()
                      << " " << otherPrevious << " <-> " << " " << otherSegment << endl;
        }
        otherPrevious = otherSegment;
      }
    }

    if (holes.empty()) {
      cdebug_log(159,0) << "No holes found to minimize into." << endl;
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
        cdebug_log(159,0) << "Go as punctual into biggest hole: " << biggestHole << endl;
        axisHint = biggestHole.intersection(punctualSpan).getCenter();
        success = true;
      } else {
        for ( size_t i=0 ; i<holes.size() ; i++ ) {
          cdebug_log(159,0) << "Trying secondary hole: " << holes[i] << endl;
          if (holes[i].intersect(punctualSpan)) {
            biggestHole = holes[i];
            axisHint = biggestHole.intersection(punctualSpan).getCenter();
            cdebug_log(159,0) << "Go as punctual into secondary hole: " << biggestHole << endl;
            success = true;
          }
        }
      }

      if (not success) {
        cdebug_log(159,0) << "No suitable hole found." << endl;
        return false;
      }
    } else {
      biggestHole.intersection( Interval(minSpan,maxSpan) );
      if (biggestHole.isEmpty()) {
        cdebug_log(159,0) << "Biggest hole is empty (under span ["
                    << DbU::getValueString(minSpan) << " "
                    << DbU::getValueString(maxSpan) << "])" << endl;
        return false;
      } else {
        cdebug_log(159,0) << "Go into biggest hole: " << biggestHole << endl;
        axisHint = biggestHole.getCenter();
      }
    }

    cdebug_log(159,0) << "Axis Hint: " << DbU::getValueString(axisHint) << endl;

    for ( size_t i=0 ; i<perpandiculars.size() ; i++ ) {
      DataNegociate* data2 = perpandiculars[i]->getDataNegociate();
      if (not data2) continue;

      cdebug_log(159,0) << "  | " << perpandiculars[i] << endl;

      RoutingEvent* event2 = data2->getRoutingEvent();
      if (not event2) continue;

      _fsm.addAction( perpandiculars[i], SegmentAction::SelfRipupPerpandWithAxisHint, axisHint );
    }

    _event->setMinimized();

    return true;
  }


  bool  Manipulator::dragMinimize ()
  {
    cdebug_log(159,1) << "Manipulator::dragMinimize() " << _segment << endl; 

    if (_segment->isFixed())    { cdebug_tabw(159,-1); return false; }
    if (not _segment->isDrag()) { cdebug_tabw(159,-1); return false; }

    Interval termConstraints;

    if (_segment->base()->getAutoSource()->canDrag()) {
      cdebug_log(159,0) << _segment->base()->getAutoSource() << endl;
      termConstraints = _segment->base()->getAutoSource()->getUConstraints( _segment->getDirection() );
      cdebug_log(159,0) << "Terminal Constraints (source): " << termConstraints << endl;
    }
    if (_segment->base()->getAutoTarget()->canDrag()) {
      cdebug_log(159,0) << _segment->base()->getAutoTarget() << endl;
      termConstraints = _segment->base()->getAutoTarget()->getUConstraints( _segment->getDirection() );
      cdebug_log(159,0) << "Terminal Constraints (target): " << termConstraints << endl;
    }

    if (_fsm.getCosts().size() == 0) {
      cerr << Error( "Manipulator::dragMinimize(): The segment cannot be put in any track.\n"
                     "        On: %s"
                   , getString(_segment).c_str()
                   ) << endl;
      cdebug_tabw(159,-1);
      return false;
    }

    if (_fsm.getCosts().size() > 1)
      cerr << Error( "Manipulator::dragMinimize(): The segment can be put in more than one track (%d).\n"
                     "        On: %s"
                   , _fsm.getCosts().size()
                   , getString(_segment).c_str()
                   ) << endl;

    size_t     begin    = _fsm.getBegin( 0 );
    size_t     end      = _fsm.getEnd  ( 0 );
    Track*     track    = _fsm.getTrack( 0 );
    DbU::Unit  axisHint = 0;

#if DISABLED
    if (termConstraints.getSize() < _segment->getPPitch()*2) {
      cdebug_log(159,0) << "Constraints less than two perpandicular pitches, lock." << endl;

      for ( ; begin < end ; ++begin ) {
        cdebug_log(159,0) << "| Ripup:" << track->getSegment(begin) << endl;
        _fsm.addAction( track->getSegment(begin), SegmentAction::OtherRipup );
      }
      _fsm.addAction( _segment, SegmentAction::SelfLock );

      cdebug_tabw(159,-1);
      return true;
    }
#endif

    for ( ; begin < end ; ++begin ) {
      TrackElement* conflict = track->getSegment(begin);
      if (conflict->getCanonicalInterval().intersect( _segment->getCanonicalInterval() )) break; 
    }

    if (begin == end) {
      axisHint = termConstraints.getCenter();
      cdebug_log(159,0) << "No conflict under canonical interval (?) use terminal center." << endl;
      cdebug_log(159,0) << "term: " << termConstraints << " center:" << DbU::getValueString(axisHint) << endl;
    } else {
      Interval previousFree = track->getPreviousFree( begin, _segment->getNet() );
      if (previousFree.intersect(termConstraints))
        axisHint = previousFree.getCenter();
      else {
        Interval nextFree = track->getNextFree( end, _segment->getNet() );
        if (nextFree.intersect(termConstraints)) {
          axisHint = nextFree.getCenter();
        } else {
          cdebug_log(159,0) << "Neither previous free nor next free can be used." << endl;
          cdebug_log(159,0) << "| previous:" << previousFree << endl;
          cdebug_log(159,0) << "| next:    " << nextFree << endl;
          cdebug_tabw(159,-1);
          return false;
        }
      }
    }

    cdebug_log(159,0) << "Axis Hint: " << DbU::getValueString(axisHint) << endl;

    const vector<TrackElement*>& perpandiculars = _event->getPerpandiculars();
    for ( size_t i=0 ; i<perpandiculars.size() ; i++ ) {
      DataNegociate* data2 = perpandiculars[i]->getDataNegociate();
      if (not data2) continue;

      cdebug_log(159,0) << "  | " << perpandiculars[i] << endl;

      RoutingEvent* event2 = data2->getRoutingEvent();
      if (not event2) continue;

      _fsm.addAction( perpandiculars[i], SegmentAction::SelfRipupPerpandWithAxisHint, axisHint );
    }

  //_fsm.addAction( _segment, SegmentAction::SelfRipup|SegmentAction::EventLevel3 );

    _event->setMinimized();

    cdebug_tabw(159,-1);
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

      if ( moveLeft ) {
        if ( perpandicular->getTargetAxis() == _event->getAxisHistory() )
          _fsm.addAction ( perpandicular, SegmentAction::OtherRipupPerpandAndPacking );
      } else {
        if ( perpandicular->getSourceAxis() == _event->getAxisHistory() )
          _fsm.addAction ( perpandicular, SegmentAction::OtherRipupPerpandAndPacking );
      }
    }
  }


  void  Manipulator::repackPerpandiculars ()
  {
    cdebug_log(159,0) << "Manipulator::repackPerpandiculars()" << endl;

    const vector<TrackElement*>& perpandiculars = _event->getPerpandiculars();
    for ( size_t iperpand=0 ; iperpand<perpandiculars.size() ; iperpand++ ) {
      TrackElement*  perpandicular = perpandiculars[iperpand];
      DataNegociate* data          = perpandicular->getDataNegociate();

      if (perpandicular->isFixed ()) continue;
      if (perpandicular->isGlobal()) continue;
      if (not data) continue;
      if (data->getState() >= DataNegociate::RepairFailed) continue;

      if (RoutingEvent::getStage() == RoutingEvent::Repair) {
        data->setState( DataNegociate::Repair );
        if (data->getStateCount() > 1) data->resetStateCount();
      }
      _fsm.addAction( perpandicular, SegmentAction::SelfRipupPerpand );
    }
    _fsm.addAction( _segment, SegmentAction::SelfRipup|SegmentAction::EventLevel4 );
  }


}  // Katana namespace.
