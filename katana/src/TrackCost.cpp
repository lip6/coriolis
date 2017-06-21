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
#include "katana/TrackElement.h"
#include "katana/Session.h"


namespace Katana {

  using std::cerr;
  using std::endl;
  using Hurricane::Error;


// -------------------------------------------------------------------
// Class  :  "TrackCost".

  TrackCost::TrackCost ( TrackElement* refSegment
                       , TrackElement* symSegment
                       , Track*        refTrack
                       , Track*        symTrack
                       )
    : _flags          ((symSegment) ? Symmetric : NoFlags)
    , _span           (1)
    , _tracks         ( _span * ((symSegment) ? 2 : 1)
                      , std::tuple<Track*,size_t,size_t>(NULL,Track::npos,Track::npos) )
    , _segment1       (refSegment)
    , _segment2       (symSegment)
    , _interval1      (refSegment->getCanonicalInterval())
    , _interval2      ((symSegment) ? symSegment->getCanonicalInterval() : Interval())
    , _terminals      (0)
    , _delta          (-_interval1.getSize() -_interval2.getSize())
    , _deltaShared    (0)
    , _deltaPerpand   (0)
    , _axisWeight     (0)
    , _distanceToFixed(2*Session::getSliceHeight())
    , _longuestOverlap(0)
    , _dataState      (0)
    , _ripupCount     (0)
    , _selectFlags    (NoFlags)
    , _selectIndex    (0)
  {
    std::get<0>( _tracks[0] ) = refTrack;
    _segment1->addOverlapCost( *this );

    if (symTrack) {
      std::get<0>( _tracks[_span] ) = symTrack;
      select( 0, Symmetric );
      _segment2->addOverlapCost( *this );
    }

    consolidate();
  }


  TrackCost::~TrackCost ()
  { }


  bool  TrackCost::isFree () const
  {
    return /*(not _terminals) and*/ (not isOverlap()) and (not isInfinite());
  }


  bool  TrackCost::Compare::operator() ( const TrackCost* lhs, const TrackCost* rhs )
  {
    if ( lhs->isInfinite() xor rhs->isInfinite() ) return rhs->isInfinite();

    if (   (_flags & TrackCost::DiscardGlobals)
       and (lhs->isOverlapGlobal() xor rhs->isOverlapGlobal()) )
      return rhs->isOverlapGlobal();

    if ( lhs->isHardOverlap() xor rhs->isHardOverlap() ) return rhs->isHardOverlap();

    if ( lhs->_ripupCount + (int)Session::getRipupCost() < rhs->_ripupCount ) return true;
    if ( lhs->_ripupCount > (int)Session::getRipupCost() + rhs->_ripupCount ) return false;

  //int lhsRipupCost = (lhs->_dataState<<2) + lhs->_ripupCount;
  //int rhsRipupCost = (rhs->_dataState<<2) + rhs->_ripupCount;
  //if ( lhsRipupCost + (int)Session::getRipupCost() < rhsRipupCost ) return true;
  //if ( lhsRipupCost > (int)Session::getRipupCost() + rhsRipupCost ) return false;

  //if ( _flags & TrackCost::DiscardGlobals ) {
  //  if ( lhs->_longuestOverlap < rhs->_longuestOverlap ) return true;
  //  if ( lhs->_longuestOverlap > rhs->_longuestOverlap ) return false;
  //}

    if ( lhs->isOverlap() xor rhs->isOverlap() ) return rhs->isOverlap();

    if ( lhs->_terminals < rhs->_terminals ) return true;
    if ( lhs->_terminals > rhs->_terminals ) return false;

    if (lhs->_delta != rhs->_delta) {
    //cdebug_log(155,0) << "TrackCost::Compare() lhs->_delta:" << lhs->_delta << " rhs->_delta:" << rhs->_delta << endl;
    //if ( not (_flags & TrackCost::IgnoreSharedLength) or (lhs->_delta > 0) or (rhs->_delta > 0) ) {
    //if ( (lhs->_delta > 0) or (rhs->_delta > 0) ) {
        if (lhs->_delta < rhs->_delta) return true;
        if (lhs->_delta > rhs->_delta) return false;
    //}

    // Both delta should be negative, chose the least one.
    //return lhs->_delta > rhs->_delta;
      return lhs->_delta < rhs->_delta;
    }

#if 0
    DbU::Unit lhsMixedWeight = 0.5*lhs->_deltaPerpand;
    DbU::Unit rhsMixedWeight = 0.5*rhs->_deltaPerpand;

    if ( not (_flags & TrackCost::IgnoreAxisWeight) ) {
      lhsMixedWeight += lhsMixedWeight;
      rhsMixedWeight += rhsMixedWeight;
    }

    if (lhsMixedWeight < rhsMixedWeight) return true;
    if (lhsMixedWeight > rhsMixedWeight) return false;
#endif

    if ( not (_flags & TrackCost::IgnoreAxisWeight) ) {
      if ( lhs->_axisWeight < rhs->_axisWeight ) return true;
      if ( lhs->_axisWeight > rhs->_axisWeight ) return false;
    }

    if ( lhs->_deltaPerpand < rhs->_deltaPerpand ) return true;
    if ( lhs->_deltaPerpand > rhs->_deltaPerpand ) return false;

    if ( lhs->_distanceToFixed > rhs->_distanceToFixed ) return true;
    if ( lhs->_distanceToFixed < rhs->_distanceToFixed ) return false;

    return lhs->getTrack(0)->getAxis() < rhs->getTrack(0)->getAxis();
  }


  bool  TrackCost::CompareByDelta::operator() ( const TrackCost* lhs, const TrackCost* rhs )
  {
    return lhs->getDelta() < rhs->getDelta();
  }


  Net* TrackCost::getNet1 () const { return (_segment1) ? _segment1->getNet() : NULL; }
  Net* TrackCost::getNet2 () const { return (_segment2) ? _segment2->getNet() : NULL; }


  size_t  TrackCost::getBegin ( size_t i, uint32_t flags ) const
  {
    if (i >= _span) return Track::npos;
    return std::get<1>( _tracks[i + ((flags & Symmetric) ? _span : 0)] );
  }

  size_t  TrackCost::getEnd ( size_t i, uint32_t flags ) const
  {
    if (i >= _span) return Track::npos;
    return std::get<2>( _tracks[i + ((flags & Symmetric) ? _span : 0)] );
  }


  void  TrackCost::consolidate ()
  {
    if ( not isInfinite() and not isHardOverlap() ) {
      cdebug_log(159,0) << "TrackCost::consolidate() " << _delta << " - " << _deltaShared << endl;
    //_deltaPerpand += - (_deltaShared << 1);
      _delta -= _deltaShared;
    //if (_delta > 0) _delta -= _deltaShared;
    //else            _delta += _deltaShared;
    }
  }


  void  TrackCost::setDistanceToFixed ()
  {
    if (_flags & Analog) return;

    // This is the GCell side (it is *one* cell height from the gauge).
    DbU::Unit  cellHeight = Session::getSliceHeight();

    cdebug_log(159,0) << "TrackCost::setDistanceToFixed() begin:" << getBegin(0) << endl;

    TrackElement* neighbor;
    if (getBegin(0) != Track::npos) {
      neighbor = getTrack(0)->getSegment(getBegin(0));
      if ( neighbor and (neighbor->getNet() != getNet()) ) {
        DbU::Unit distance = getInterval().getVMin() - neighbor->getTargetU();
        if ( distance < cellHeight )
          _distanceToFixed = distance;
      }
    // if ( neighbor and neighbor->isFixed() ) {
    //   if ( _distanceToFixed == DbU::Max ) _distanceToFixed = 0;
    //   _distanceToFixed += _interval.getVMin() - neighbor->getTargetU();
    // }
    }
    if (getEnd(0) != Track::npos) {
      neighbor = getTrack(0)->getSegment(getEnd(0));
      if ( neighbor and (neighbor->getNet() != getNet()) ) {
        DbU::Unit distance = neighbor->getSourceU() - getInterval().getVMax();
        if ( _distanceToFixed == 2*cellHeight ) _distanceToFixed = 0;
        if ( distance < cellHeight )
          _distanceToFixed += distance;
      }
    // if ( neighbor and neighbor->isFixed() ) {
    //   if ( _distanceToFixed == DbU::Max ) _distanceToFixed = 0;
    //   _distanceToFixed += neighbor->getSourceU() - _interval.getVMax();
    // }
    }
  }


  string  TrackCost::_getString () const
  {
    string s = "<" + _getTypeName();

    s += " "   + getString(getTrack(0));
    s += " "   + getString(_dataState);
    s += "+"   + getString(_ripupCount);
    s += ":"   + getString((_dataState<<2)+_ripupCount);
    s += " "   + string ( (isInfinite()      )?"I":"-" );
    s +=         string ( (isBlockage()      )?"b":"-" );
    s +=         string ( (isFixed()         )?"f":"-" );
    s +=         string ( (isHardOverlap()   )?"h":"-" );
    s +=         string ( (isOverlap()       )?"o":"-" );
    s +=         string ( (isOverlapGlobal() )?"g":"-" );
    s +=         string ( (isGlobalEnclosed())?"e":"-" );
    s += " "   + getString(_terminals);
    s += "/"   + /*DbU::getValueString(_delta)*/       getString(_delta);
    s += "-"   + /*DbU::getValueString(_deltaShared)*/ getString(_deltaShared);
    s += "/"   + DbU::getValueString(_axisWeight);
    s += "/"   + DbU::getValueString(_deltaPerpand);
    s += "/f:" + DbU::getValueString(_distanceToFixed);
    s += "/"   + DbU::getValueString(_longuestOverlap);
    s += " "   + getString(_dataState);
    s += ">";

    return s;
  }


  Record* TrackCost::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add( getSlot          ( "_flags"          ,  _flags           ) );
    record->add( getSlot          ( "_tracks"         ,  _tracks          ) );
    record->add( getSlot          ( "_interval1"      , &_interval1       ) );
    record->add( getSlot          ( "_interval2"      , &_interval2       ) );
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
