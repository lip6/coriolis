// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./TrackSegmentCost.cpp"                   |
// +-----------------------------------------------------------------+


#include  <cstdlib>
#include  <iostream>
#include  <sstream>
#include  <map>

#include  "hurricane/Bug.h"
#include  "hurricane/DebugSession.h"
#include  "anabatic/AutoSegment.h"
#include  "katana/TrackElement.h"
#include  "katana/TrackSegmentCost.h"


namespace Katana {


  using std::cerr;
  using std::endl;
  using std::map;
  using std::multimap;
  using std::make_pair;
  using std::ostringstream;
  using Hurricane::Bug;
  using Hurricane::DebugSession;
  using Hurricane::tab;


// -------------------------------------------------------------------
// Class  :  "TrackSegmentCost".


  TrackSegmentCost::TrackSegmentCost ( TrackElement* trackSegment )
    : _terminals     (0)
    , _ripupCount    (0)
    , _leftMinExtend (DbU::Max)
    , _rightMinExtend(DbU::Min)
    , _net           (trackSegment->getNet())
    , _attractors    ()
  {
  //update ( trackSegment );
  }


  TrackSegmentCost::~TrackSegmentCost ()
  { }


  DbU::Unit  TrackSegmentCost::getWiringDelta ( DbU::Unit axis ) const
  {
    DbU::Unit  attraction = 0;
    for ( size_t i=0 ; i < _attractors.size() ; i++ ) {
      if ( _attractors[i] > axis ) attraction += _attractors[i] - axis;
      else                         attraction += axis - _attractors[i];
    }
    return attraction;
  }


  void  TrackSegmentCost::update ( TrackElement* trackSegment )
  {
    DebugSession::open ( trackSegment->getNet(), 156, 160 );

    cdebug_log(159,1) << "TrackSegmentCost::update() - " << trackSegment << endl;

    vector<AutoSegment*>  collapseds;
    vector<AutoSegment*>  perpandiculars;
    map<DbU::Unit,int>    attractorSpins;

    AutoSegment::getTopologicalInfos ( trackSegment->base()
                                     , collapseds
                                     , perpandiculars
                                     , _leftMinExtend
                                     , _rightMinExtend
                                     );

    _terminals = AutoSegment::getTerminalCount ( trackSegment->base(), collapseds );
    _attractors.clear ();

    for ( size_t i=0 ; i < perpandiculars.size() ; i++ ) {
      Interval      interval;
      TrackElement* perpandicular;

      if ( perpandiculars[i]->isCanonical() ) {
        perpandicular = Session::lookup ( perpandiculars[i]->base() );
        if ( perpandicular )
          perpandicular->getCanonical ( interval );
      } else {
        perpandicular = Session::lookup ( perpandiculars[i]->getCanonical(interval)->base() );
      }

      if ( not perpandicular ) {
        cerr << Bug("Not a TrackSegment: %s:%s\n      (perpandicular: %s:%s)"
                   ,getString((void*)perpandiculars[i]->getCanonical(interval)->base()).c_str()
                   ,getString(perpandiculars[i]->getCanonical(interval)).c_str()
                   ,getString((void*)perpandiculars[i]->base()).c_str()
                   ,getString(perpandiculars[i]).c_str()
                   ) << endl;
        continue;
      }
      interval.inflate ( DbU::lambda(-1.5) );

      cdebug_log(159,0)   << "| perpandicular: " << perpandiculars[i] << endl;
      cdebug_log(159,1) << "| canonical:     " << perpandicular << endl;
      cdebug_log(159,0)   << "interval: " << interval << endl;

      if ( interval.isPonctual() ) {
        cdebug_log(159,0) << "Punctual attractor @" << DbU::getValueString(interval.getVMin()) << endl;
        _attractors.push_back ( interval.getVMin() );
        cdebug_tabw(159,-1);
        continue;
      }

      if (  ( interval.getVMin() != trackSegment->getAxis() )
         or AutoSegment::isTopologicalBound(perpandiculars[i]
                                           ,false
                                           ,perpandicular->isHorizontal()
                                           ) ) {
        map<DbU::Unit,int>::iterator iattractor = attractorSpins.find ( interval.getVMin() );
        if ( iattractor == attractorSpins.end() ) {
          attractorSpins.insert ( make_pair(interval.getVMin(),-1) );
        } else {
          iattractor->second -= 1;
        }
        cdebug_log(159,0) << "Left attractor @" << DbU::getValueString(interval.getVMin()) << endl;
      }

      if (  ( interval.getVMax() != trackSegment->getAxis() )
         or AutoSegment::isTopologicalBound(perpandiculars[i]
                                           ,true
                                           ,perpandicular->isHorizontal()
                                           ) ) {
        map<DbU::Unit,int>::iterator iattractor = attractorSpins.find ( interval.getVMax() );
        if ( iattractor == attractorSpins.end() ) {
          attractorSpins.insert ( make_pair(interval.getVMax(),1) );
        } else {
          iattractor->second += 1;
        }
        cdebug_log(159,0) << "Right attractor @" << DbU::getValueString(interval.getVMax()) << endl;
      }

      cdebug_tabw(159,-1);
    }

    map<DbU::Unit,int>::iterator iattractor = attractorSpins.begin();
    for ( ; iattractor != attractorSpins.end() ; iattractor++ ) {
      if ( iattractor->second != 0 )
        _attractors.push_back ( iattractor->first );
    }

    ostringstream s;
    s << "Attractors [";
    for ( size_t i=0 ; i<_attractors.size() ; i++ ) {
      if ( i ) s << ", ";
      s << DbU::getValueString(_attractors[i]);
    }
    s << "]";
    cdebug_log(159,0) << s.str() << endl;


    cdebug_tabw(159,-1);
    DebugSession::close ();
  }


  string  TrackSegmentCost::_getString () const
  {
    return "<" + _getTypeName() + " "
               + getString(_terminals)
               + " [" + DbU::getValueString(_leftMinExtend)
               +  ":" + DbU::getValueString(_rightMinExtend)
               +  "]>";
  }


  Record* TrackSegmentCost::_getRecord () const
  {
    Record* record = new Record ( getString(this) );
    record->add ( getSlot ( "_terminals"     ,  _terminals      ) );
    record->add ( getSlot ( "_ripupCount"    ,  _ripupCount     ) );
    record->add ( getSlot ( "_leftMinExtend" , &_leftMinExtend  ) );
    record->add ( getSlot ( "_rightMinExtend", &_rightMinExtend ) );
    record->add ( getSlot ( "_net"           ,  _net            ) );
                                     
    return record;
  }


} // End of Katana namespace.
