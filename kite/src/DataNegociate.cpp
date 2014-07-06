// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./DataNegociate.cpp"                      |
// +-----------------------------------------------------------------+


#include <cstdlib>
#include <sstream>
#include "hurricane/Bug.h"
#include "hurricane/DebugSession.h"
#include "katabatic/AutoSegment.h"
#include "kite/DataNegociate.h"
#include "kite/RoutingEvent.h"


namespace Kite {

  using std::cerr;
  using std::endl;
  using std::map;
  using std::multimap;
  using std::make_pair;
  using std::ostringstream;
  using Hurricane::Bug;
  using Hurricane::DebugSession;
  using Hurricane::inltrace;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::tab;
  using Katabatic::KbHorizontal;
  using Katabatic::KbPropagate;


// -------------------------------------------------------------------
// Class  :  "DataNegociate".


  DataNegociate::DataNegociate ( TrackElement* trackSegment )
    : _trackSegment     (trackSegment)
    , _childSegment     (NULL)
    , _routingEvent     (NULL)
    , _net              (trackSegment->getNet())
    , _state            (RipupPerpandiculars)
    , _stateCount       (1)
    , _terminals        (0)
    , _ripupCount       (0)
    , _leftMinExtend    (DbU::Max)
    , _rightMinExtend   (DbU::Min)
    , _attractors       ()
    , _perpandiculars   ()
    , _perpandicularFree(false)
  { }


  DataNegociate::~DataNegociate ()
  { }


  DbU::Unit  DataNegociate::getWiringDelta ( DbU::Unit axis ) const
  {
    DbU::Unit  attraction = 0;
    for ( size_t i=0 ; i < _attractors.size() ; i++ ) {
      if ( _attractors[i] > axis ) attraction += _attractors[i] - axis;
      else                         attraction += axis - _attractors[i];
    }
    return attraction;
  }


  void  DataNegociate::update ()
  {
    DebugSession::open( _trackSegment->getNet(), 148 );

  //ltrace(500) << "Deter| DataNegociate::update() - " << _trackSegment << endl;
    ltrace(148) << "DataNegociate::update() - " << _trackSegment << endl;
    ltracein(148);

    vector<AutoSegment*> collapseds;
    vector<AutoSegment*> perpandiculars;
    map<DbU::Unit,int>   attractorSpins;

    _perpandiculars.clear();
    AutoSegment::getTopologicalInfos( _trackSegment->base()
                                    , collapseds
                                    , perpandiculars
                                    , _leftMinExtend
                                    , _rightMinExtend
                                    );

    _terminals = AutoSegment::getTerminalCount( _trackSegment->base(), collapseds );
  //ltrace(500) << "Deter|    Terminals:" << _terminals << endl;
    _attractors.clear();
    _perpandiculars.clear();
    _perpandicularFree = Interval(false);

    ltrace(148) << "Extracting attractors from perpandiculars." << endl;
    for ( size_t i=0 ; i < perpandiculars.size() ; i++ ) {
      Interval      interval;
      TrackElement* perpandicular;

      if (perpandiculars[i]->isCanonical()) {
        perpandicular = Session::lookup( perpandiculars[i]->base() );
        if (perpandicular) perpandicular->getCanonical( interval );
      } else {
        perpandicular = Session::lookup( perpandiculars[i]->getCanonical(interval)->base() );
      }
      if (not perpandicular) {
        cerr << Bug( "Not a TrackSegment: %s\n      (perpandicular: %s)"
                 //, getString((void*)perpandiculars[i]->getCanonical(interval)->base()).c_str()
                   , getString(perpandiculars[i]->getCanonical(interval)).c_str()
                 //, getString((void*)perpandiculars[i]->base()).c_str()
                   , getString(perpandiculars[i]).c_str()
                   ) << endl;
        continue;
      }

      if (RoutingEvent::getStage() == RoutingEvent::Repair)
        perpandicular->base()->setFlagsOnAligneds( Katabatic::SegUnbound );

      interval.inflate( DbU::fromLambda(-0.5) );

      ltrace(148) << "| perpandicular: " << perpandiculars[i] << endl;
      ltrace(148) << "| canonical:     " << perpandicular << endl;
      ltracein(148);
      ltrace(148) << "Canonical // interval: " << interval << endl;

      _perpandiculars.push_back( perpandicular );
      if (perpandicular->getTrack()) {
        Interval  trackFree = perpandicular->getFreeInterval();
        ltrace(148) << "Track Perpandicular Free: " << trackFree << endl;

        _perpandicularFree.intersection( trackFree );
      } else {
        ltrace(148) << "Not in any track " << perpandicular << endl;
      }

      if (interval.isPonctual()) {
        ltrace(148) << "Punctual attractor @" << DbU::getValueString(interval.getVMin()) << endl;
        _attractors.push_back( interval.getVMin() );
        ltraceout(148);
        continue;
      }

      if (  (interval.getVMin() != _trackSegment->getAxis())
         or AutoSegment::isTopologicalBound(perpandiculars[i]
                                           ,perpandicular->isHorizontal() ? KbHorizontal : 0
                                           ) ) {
        map<DbU::Unit,int>::iterator iattractor = attractorSpins.find( interval.getVMin() );
        if (iattractor == attractorSpins.end()) {
          attractorSpins.insert( make_pair(interval.getVMin(),-1) );
        } else {
          iattractor->second -= 1;
        }
        ltrace(148) << "Left attractor @" << DbU::getValueString(interval.getVMin()) << endl;
      }

      if (  (interval.getVMax() != _trackSegment->getAxis())
         or AutoSegment::isTopologicalBound(perpandiculars[i]
                                           ,KbPropagate | (perpandicular->isHorizontal() ? KbHorizontal : 0)
                                           ) ) {
        map<DbU::Unit,int>::iterator iattractor = attractorSpins.find( interval.getVMax() );
        if (iattractor == attractorSpins.end()) {
          attractorSpins.insert( make_pair(interval.getVMax(),1) );
        } else {
          iattractor->second += 1;
        }
        ltrace(148) << "Right attractor @" << DbU::getValueString(interval.getVMax()) << endl;
      }

      ltraceout(148);
    }
    if ( not _trackSegment->isTerminal() and (_perpandiculars.size() < 2) )
      cerr << Bug( "Less than two perpandiculars on %s.", getString(_trackSegment).c_str() ) << endl;

    map<DbU::Unit,int>::iterator iattractor = attractorSpins.begin();
    for ( ; iattractor != attractorSpins.end() ; iattractor++ ) {
      if (iattractor->second != 0)
        _attractors.push_back( iattractor->first );
    }

    ostringstream s;
    s << "Attractors [";
    for ( size_t i=0 ; i<_attractors.size() ; i++ ) {
      if ( i ) s << ", ";
      s << DbU::getValueString( _attractors[i] );
    }
    s << "]";
    ltrace(148) << s.str() << endl;
    ltrace(200) << "Perpandicular Free: " << _perpandicularFree << endl;


    ltraceout(148);
    DebugSession::close();
  }


  string  DataNegociate::_getString () const
  {
    return "<" + _getTypeName() + " "
               + getString(_trackSegment) + " "
               + getString(_terminals)
               + " [" + DbU::getValueString(_leftMinExtend)
               +  ":" + DbU::getValueString(_rightMinExtend)
               + "]>";
  }


  Record* DataNegociate::_getRecord () const
  {
    Record* record = new Record ( getString(this) );
    record->add( getSlot          ( "_routingEvent"  ,  _routingEvent   ) );
    record->add( getSlot          ( "_trackSegment"  ,  _trackSegment   ) );
    record->add( getSlot          ( "_childSegment"  ,  _childSegment   ) );
    record->add( getSlot          ( "_terminals"     ,  _terminals      ) );
    record->add( getSlot          ( "_ripupCount"    ,  _ripupCount     ) );
    record->add( DbU::getValueSlot( "_leftMinExtend" , &_leftMinExtend  ) );
    record->add( DbU::getValueSlot( "_rightMinExtend", &_rightMinExtend ) );
    record->add( getSlot          ( "_net"           ,  _net            ) );
                                     
    return record;
  }


  string  DataNegociate::getStateString ( DataNegociate* data )
  {
    ostringstream s;
    switch ( data->_state ) {
      case RipupPerpandiculars:    s << "RipupPerpandiculars";     break;
      case Minimize:               s << "Minimize";                break;
      case Dogleg:                 s << "Dogleg";                  break;
      case Slacken:                s << "Slacken";                 break;
      case ConflictSolveByHistory: s << "ConflictSolveByHistory1"; break;
      case ConflictSolveByPlaceds: s << "ConflictSolveByPlaceds";  break;
      case LocalVsGlobal:          s << "LocalVsGlobal";           break;
      case MoveUp:                 s << "MoveUp";                  break;
      case MaximumSlack:           s << "MaximumSlack";            break;
      case Unimplemented:          s << "Unimplemented";           break;
      case Repair:                 s << "REPAIR";                  break;
      default:
        s << "Unknown(" << data->_state << ")"; break;
    }
    s << ":" << data->_stateCount;
    return s.str();
  }


} // Kite namespace.
