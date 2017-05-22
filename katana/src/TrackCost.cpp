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
// |  C++ Module  :       "./TrackCost.cpp"                          |
// +-----------------------------------------------------------------+


#include <cstdlib>
#include <sstream>
#include <iostream>
#include "katana/Track.h"
#include "katana/TrackCost.h"
#include "katana/Session.h"


namespace Katana {

  using std::cerr;
  using std::endl;


// -------------------------------------------------------------------
// Class  :  "TrackCost".

  TrackCost::TrackCost ( Track* track )
    : _flags          (ZeroCost)
    , _track          (track)
    , _begin          (Track::npos)
    , _end            (Track::npos)
    , _interval       ()
    , _forGlobal      (false)
    , _blockage       (false)
    , _fixed          (false)
    , _infinite       (false)
    , _hardOverlap    (false)
    , _overlap        (false)
    , _leftOverlap    (false)
    , _rightOverlap   (false)
    , _overlapGlobal  (false)
    , _globalEnclosed (false)
    , _terminals      (0)
    , _delta          (0)
    , _deltaShared    (0)
    , _deltaPerpand   (0)
    , _axisWeight     (0)
    , _distanceToFixed(2*Session::getSliceHeight())
    , _longuestOverlap(0)
    , _dataState      (0)
    , _ripupCount     (0)
  { }


  TrackCost::TrackCost (       Track*    track
                       , const Interval& interval
                       ,       size_t    begin
                       ,       size_t    end
                       ,       Net*      net
                       ,       uint32_t  flags
                       )
    : _flags          (flags)
    , _track          (track)
    , _begin          (begin)
    , _end            (end)
    , _interval       (interval)
    , _forGlobal      (false)
    , _blockage       (false)
    , _fixed          (false)
    , _infinite       (false)
    , _hardOverlap    (false)
    , _overlap        (false)
    , _leftOverlap    (false)
    , _rightOverlap   (false)
    , _overlapGlobal  (false)
    , _globalEnclosed (false)
    , _terminals      (0)
    , _delta          (-interval.getSize())
    , _deltaShared    (0)
    , _deltaPerpand   (0)
    , _axisWeight     (0)
    , _distanceToFixed(2*Session::getSliceHeight())
    , _longuestOverlap(0)
    , _dataState      (0)
    , _ripupCount     (0)
  {
  // This is the GCell side (it is *one* cell height from the gauge).
    DbU::Unit  cellHeight = Session::getSliceHeight();

    TrackElement* neighbor;
    if ( _begin != Track::npos ) {
      neighbor = _track->getSegment(_begin);
      if ( neighbor and (neighbor->getNet() != net) ) {
        DbU::Unit distance = interval.getVMin() - neighbor->getTargetU();
        if ( distance < cellHeight )
          _distanceToFixed = distance;
      }
      // if ( neighbor and neighbor->isFixed() ) {
      //   if ( _distanceToFixed == DbU::Max ) _distanceToFixed = 0;
      //   _distanceToFixed += interval.getVMin() - neighbor->getTargetU();
      // }
    }
    if ( _end != Track::npos ) {
      neighbor = _track->getSegment(_end);
      if ( neighbor and (neighbor->getNet() != net) ) {
        DbU::Unit distance = neighbor->getSourceU() - interval.getVMax();
        if ( _distanceToFixed == 2*cellHeight ) _distanceToFixed = 0;
        if ( distance < cellHeight )
          _distanceToFixed += distance;
      }
      // if ( neighbor and neighbor->isFixed() ) {
      //   if ( _distanceToFixed == DbU::Max ) _distanceToFixed = 0;
      //   _distanceToFixed += neighbor->getSourceU() - interval.getVMax();
      // }
    }
  }

  TrackCost::~TrackCost ()
  { }


  bool  TrackCost::isFree () const
  {
    return /*(not _terminals) and*/ (not _overlap) and (not _infinite);
  }


  bool  TrackCost::Compare::operator() ( const TrackCost& lhs, const TrackCost& rhs )
  {
    if ( lhs._infinite xor rhs._infinite ) return rhs._infinite;

    if (   (_flags & TrackCost::DiscardGlobals)
       and (lhs._overlapGlobal xor rhs._overlapGlobal) )
      return rhs._overlapGlobal;

    if ( lhs._hardOverlap xor rhs._hardOverlap ) return rhs._hardOverlap;

    if ( lhs._ripupCount + (int)Session::getRipupCost() < rhs._ripupCount ) return true;
    if ( lhs._ripupCount > (int)Session::getRipupCost() + rhs._ripupCount ) return false;

  //int lhsRipupCost = (lhs._dataState<<2) + lhs._ripupCount;
  //int rhsRipupCost = (rhs._dataState<<2) + rhs._ripupCount;
  //if ( lhsRipupCost + (int)Session::getRipupCost() < rhsRipupCost ) return true;
  //if ( lhsRipupCost > (int)Session::getRipupCost() + rhsRipupCost ) return false;

  //if ( _flags & TrackCost::DiscardGlobals ) {
  //  if ( lhs._longuestOverlap < rhs._longuestOverlap ) return true;
  //  if ( lhs._longuestOverlap > rhs._longuestOverlap ) return false;
  //}

    if ( lhs._overlap xor rhs._overlap ) return rhs._overlap;

    if ( lhs._terminals < rhs._terminals ) return true;
    if ( lhs._terminals > rhs._terminals ) return false;

    if (lhs._delta != rhs._delta) {
    //cdebug_log(155,0) << "TrackCost::Compare() lhs._delta:" << lhs._delta << " rhs._delta:" << rhs._delta << endl;
      if ( not (_flags & TrackCost::IgnoreSharedLength) or (lhs._delta > 0) or (rhs._delta > 0) ) {
    //if ( (lhs._delta > 0) or (rhs._delta > 0) ) {
        if (lhs._delta < rhs._delta) return true;
        if (lhs._delta > rhs._delta) return false;
      }

      return lhs._delta < rhs._delta;
    }

#if 0
    DbU::Unit lhsMixedWeight = 0.5*lhs._deltaPerpand;
    DbU::Unit rhsMixedWeight = 0.5*rhs._deltaPerpand;

    if ( not (_flags & TrackCost::IgnoreAxisWeight) ) {
      lhsMixedWeight += lhsMixedWeight;
      rhsMixedWeight += rhsMixedWeight;
    }

    if (lhsMixedWeight < rhsMixedWeight) return true;
    if (lhsMixedWeight > rhsMixedWeight) return false;
#endif

    if ( not (_flags & TrackCost::IgnoreAxisWeight) ) {
      if ( lhs._axisWeight < rhs._axisWeight ) return true;
      if ( lhs._axisWeight > rhs._axisWeight ) return false;
    }

    if ( lhs._deltaPerpand < rhs._deltaPerpand ) return true;
    if ( lhs._deltaPerpand > rhs._deltaPerpand ) return false;

    if ( lhs._distanceToFixed > rhs._distanceToFixed ) return true;
    if ( lhs._distanceToFixed < rhs._distanceToFixed ) return false;

    return lhs.getTrack()->getAxis() < rhs.getTrack()->getAxis();
  }


  bool  TrackCost::CompareByDelta::operator() ( const TrackCost& lhs, const TrackCost& rhs )
  {
    return lhs.getDelta() < rhs.getDelta();
  }


  void  TrackCost::consolidate ()
  {
    if ( not _infinite and not _hardOverlap ) {
      cdebug_log(159,0) << "TrackCost::consolidate() " << _delta << " - " << _deltaShared << endl;
    //_deltaPerpand += - (_deltaShared << 1);
      _delta += - _deltaShared;
    //_delta += _deltaShared;
    }
  }


  void  TrackCost::merge ( const TrackCost& other )
  {
    _terminals       += other._terminals;
    _delta           += other._delta;
    _deltaShared     += other._deltaShared;
    _deltaPerpand    += other._deltaPerpand;
    _axisWeight      += other._axisWeight;
    _distanceToFixed  = std::min( _distanceToFixed, other._distanceToFixed );
    _longuestOverlap  = std::min( _longuestOverlap, other._longuestOverlap );
    _dataState        = std::max( _dataState, other._dataState );
  }


  string  TrackCost::_getString () const
  {
    string s = "<" + _getTypeName();

    s += " " + getString(_track);
    s += " " + getString(_dataState);
    s += "+" + getString(_ripupCount);
    s += ":" + getString((_dataState<<2)+_ripupCount);
    s += " " + string ( (_infinite      )?"I":"-" );
    s +=       string ( (_blockage      )?"b":"-" );
    s +=       string ( (_fixed         )?"f":"-" );
    s +=       string ( (_hardOverlap   )?"h":"-" );
    s +=       string ( (_overlap       )?"o":"-" );
    s +=       string ( (_overlapGlobal )?"g":"-" );
    s +=       string ( (_globalEnclosed)?"e":"-" );
    s += " " + getString(_terminals);
    s += "/" + /*DbU::getValueString(_delta)*/ getString(_delta);
    s += "-" + /*DbU::getValueString(_deltaShared)*/ getString(_deltaShared);
    s += "/" + DbU::getValueString(_axisWeight);
    s += "/" + DbU::getValueString(_deltaPerpand);
    s += "/" + DbU::getValueString(_distanceToFixed);
    s += "/" + DbU::getValueString(_longuestOverlap);
    s += " " + getString(_dataState);
    s += ">";

    return s;
  }


  Record* TrackCost::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add( getSlot          ( "_track"          ,  _track           ) );
    record->add( getSlot          ( "_begin"          , &_begin           ) );
    record->add( getSlot          ( "_end"            , &_end             ) );
    record->add( getSlot          ( "_interval"       , &_interval        ) );
    record->add( getSlot          ( "_infinite"       ,  _infinite        ) );
    record->add( getSlot          ( "_overlap"        ,  _overlap         ) );
    record->add( getSlot          ( "_terminals"      ,  _terminals       ) );
    record->add( DbU::getValueSlot( "_delta"          , &_delta           ) );
    record->add( DbU::getValueSlot( "_deltaShared"    , &_deltaShared     ) );
    record->add( DbU::getValueSlot( "_deltaPerpand"   , &_deltaPerpand    ) );
    record->add( DbU::getValueSlot( "_axisWeight"     , &_axisWeight      ) );
    record->add( DbU::getValueSlot( "_distanceToFixed", &_distanceToFixed ) );
    record->add( DbU::getValueSlot( "_longuestOverlap", &_longuestOverlap ) );
                                     
    return record;
  }


} // Katana namespace.
