
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
// |  C++ Module  :       "./TrackCost.cpp"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include  <cstdlib>
#include  <sstream>
#include  <iostream>

#include  "kite/Track.h"
#include  "kite/TrackCost.h"
#include  "kite/Session.h"




namespace Kite {


  using std::cerr;
  using std::endl;


// -------------------------------------------------------------------
// Class  :  "TrackCost".


  TrackCost::TrackCost (       Track*        track
                       , const Interval&     interval
                       ,       size_t        begin
                       ,       size_t        end
                       ,       Net*          net
                       ,       unsigned int  flags
                       )
    : _flags          (flags)
    , _track          (track)
    , _begin          (begin)
    , _end            (end)
    , _interval       (interval)
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
    , _distanceToFixed(DbU::lambda(100.0))
    , _longuestOverlap(0)
    , _dataState      (0)
    , _ripupCount     (0)
  {
    TrackElement* neighbor;
    if ( _begin != Track::NPOS ) {
      neighbor = _track->getSegment(_begin);
      if ( neighbor and (neighbor->getNet() != net) ) {
        DbU::Unit distance = interval.getVMin() - neighbor->getTargetU();
        if ( distance < DbU::lambda(50.0) )
          _distanceToFixed = distance;
      }
      // if ( neighbor and neighbor->isFixed() ) {
      //   if ( _distanceToFixed == DbU::Max ) _distanceToFixed = 0;
      //   _distanceToFixed += interval.getVMin() - neighbor->getTargetU();
      // }
    }
    if ( _end != Track::NPOS ) {
      neighbor = _track->getSegment(_end);
      if ( neighbor and (neighbor->getNet() != net) ) {
        DbU::Unit distance = neighbor->getSourceU() - interval.getVMax();
        if ( _distanceToFixed == DbU::lambda(100.0) ) _distanceToFixed = 0;
        if ( distance < DbU::lambda(50.0) )
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

    if ( not (_flags & TrackCost::IgnoreSharedLength)
       or (lhs._delta > 0) or (rhs._delta > 0) ) {
      if ( lhs._delta < rhs._delta ) return true;
      if ( lhs._delta > rhs._delta ) return false;
    }

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
    //_deltaPerpand += - (_deltaShared << 1);
      _delta += - _deltaShared;
    }
  }


  string  TrackCost::_getString () const
  {
    string s = "<" + _getTypeName();

    s += " " + getString(_track);
    s += " " + getString(_dataState);
    s += "+" + getString(_ripupCount);
    s += ":" + getString((_dataState<<2)+_ripupCount);
    s += " " + string ( (_blockage      )?"b":"-" );
    s +=       string ( (_hardOverlap   )?"h":"-" );
    s +=       string ( (_overlap       )?"o":"-" );
    s +=       string ( (_overlapGlobal )?"g":"-" );
    s +=       string ( (_globalEnclosed)?"e":"-" );
    s += " " + getString(_terminals);
    s += "/" + DbU::getValueString(_delta);
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
    record->add ( getSlot ( "_track"    ,  _track     ) );
    record->add ( getSlot ( "_begin"    , &_begin     ) );
    record->add ( getSlot ( "_end"      , &_end       ) );
    record->add ( getSlot ( "_interval" , &_interval  ) );
    record->add ( getSlot ( "_infinite" ,  _infinite  ) );
    record->add ( getSlot ( "_overlap"  ,  _overlap   ) );
    record->add ( getSlot ( "_terminals",  _terminals ) );
    record->add ( getSlot ( "_delta"    , &_delta     ) );
                                     
    return record;
  }


} // End of Kite namespace.
