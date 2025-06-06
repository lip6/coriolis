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
// |  C++ Module  :       "./DataNegociate.cpp"                      |
// +-----------------------------------------------------------------+


#include <cstdlib>
#include <sstream>
#include "hurricane/Bug.h"
#include "hurricane/DebugSession.h"
#include "anabatic/AutoSegment.h"
#include "katana/DataNegociate.h"
#include "katana/RoutingPlane.h"
#include "katana/RoutingEvent.h"
#include "katana/KatanaEngine.h"


namespace Katana {

  using std::cerr;
  using std::endl;
  using std::tuple;
  using std::map;
  using std::multimap;
  using std::make_pair;
  using std::ostringstream;
  using Hurricane::Bug;
  using Hurricane::DebugSession;
  using Hurricane::tab;


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
    , _sameRipup        (0)
    , _leftMinExtend    (DbU::Max)
    , _rightMinExtend   (DbU::Min)
    , _attractors       ()
    , _perpandiculars   ()
    , _perpandicularFree(false)
    , _reduceRanges     { Interval(), Interval() }
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
    for ( size_t i=0 ; i<2 ; ++i ) {
      if (_reduceRanges[i].isEmpty()) continue;
      if (_reduceRanges[i].contains(axis)) attraction -= 2*_trackSegment->getPitch();
    }
    return attraction;
  }
  

  void  DataNegociate::update ()
  {
    DebugSession::open( _trackSegment->getNet(), 156, 160 );

  //cdebug_log(9000,0) << "Deter| DataNegociate::update() - " << _trackSegment << endl;
    cdebug_log(159,1) << "DataNegociate::update() - " << _trackSegment << endl;

#if 0
    size_t               reduceCandidates = 0;
    DbU::Unit            pitch            = _trackSegment->getPitch();
#endif
    vector<AutoSegment*> collapseds;
    vector< tuple<AutoSegment*,Anabatic::Flags> > perpandiculars;
    map<DbU::Unit,int>   attractorSpins;
    size_t               reducedPerpands = 0;

    _reduceRanges[0].makeEmpty();
    _reduceRanges[1].makeEmpty();
    _perpandiculars.clear();
    AutoSegment::getTopologicalInfos( _trackSegment->base()
                                    , collapseds
                                    , perpandiculars
                                    , _leftMinExtend
                                    , _rightMinExtend
                                    );

    _terminals = AutoSegment::getTerminalCount( _trackSegment->base(), collapseds );
  //cdebug_log(9000,0) << "Deter|    Terminals:" << _terminals << endl;
    _attractors.clear();
    _perpandiculars.clear();
    _perpandicularFree = Interval(false);

    cdebug_log(159,0) << "Extracting attractors from perpandiculars." << endl;
    for ( size_t i=0 ; i < perpandiculars.size() ; i++ ) {
      cdebug_log(159,0) << "Perpandicular Free (before): " << _perpandicularFree << endl;

      Interval      interval;
      AutoSegment*  basePerpand = std::get<0>( perpandiculars[i] );
      TrackElement* perpandicular;

      if (basePerpand->isCanonical()) {
        perpandicular = Session::lookup( basePerpand->base() );
        if (perpandicular) perpandicular->getCanonical( interval );
      } else {
        perpandicular = Session::lookup( basePerpand->getCanonical(interval)->base() );
      }
      if (not perpandicular) {
        if (not (Session::isChannelStyle()
                and (basePerpand->isReduced() or basePerpand->isNonPref())))
          cerr << Bug( "Not a TrackSegment: %s\n      (perpandicular: %s)"
                   //, getString((void*)basePerpand->getCanonical(interval)->base()).c_str()
                     , getString(basePerpand->getCanonical(interval)).c_str()
                   //, getString((void*)basePerpand->base()).c_str()
                     , getString(basePerpand).c_str()
                     ) << endl;
        else
          ++reducedPerpands;
        continue;
      }

      if (RoutingEvent::getStage() == Anabatic::StageRepair)
        perpandicular->base()->setFlagsOnAligneds( AutoSegment::SegUnbound );

    //cerr << "perpandicular:" << perpandicular << endl;
    //cerr << "  " << interval << endl;
    //interval.inflate( DbU::fromLambda(-0.5) );

      cdebug_log(159,0) << "| perpandicular: " << basePerpand << endl;
      cdebug_log(159,1) << "| canonical:     " << perpandicular << endl;
      cdebug_log(159,0) << "Canonical // interval: " << interval << endl;

      _perpandiculars.push_back( perpandicular );

      if (perpandicular->isNonPref()) {
        AutoContact* source    = perpandicular->base()->getAutoSource();
        AutoContact* target    = perpandicular->base()->getAutoTarget();
        DbU::Unit    pitch     = Session::getPitch    ( perpandicular->getLayer() );
        Flags        direction = Session::getDirection( perpandicular->getLayer() );
        Interval     trackFree ( false );

        if (source->canDrag()) {
          DbU::Unit slack = (source->isMetalOnly()) ? pitch : 0;
          if (direction & Flags::Horizontal)
            trackFree.intersection( source->getCBYMin()-slack, source->getCBYMax()+slack );
          else
            trackFree.intersection( source->getCBXMin()-slack, source->getCBXMax()+slack );
          cdebug_log(159,0) << "trackFree (source drag): " << trackFree << endl;
        }
        if (target->canDrag()) {
          DbU::Unit slack = (target->isMetalOnly()) ? pitch : 0;
          if (direction & Flags::Horizontal)
            trackFree.intersection( target->getCBYMin()-slack, target->getCBYMax()+slack );
          else
            trackFree.intersection( target->getCBXMin()-slack, target->getCBXMax()+slack );
          cdebug_log(159,0) << "trackFree (target drag): " << trackFree << endl;
        }

        if (not source->canDrag() and not target->canDrag()) {
          trackFree = Interval( perpandicular->base()->getNonPrefSourcePosition()
                              , perpandicular->base()->getNonPrefTargetPosition() ); 
          trackFree.inflate( pitch );
          cdebug_log(159,0) << "trackFree (no drag): " << trackFree << endl;
        }

        if (Session::getStage() < Anabatic::StagePack) {
          int pitchSlack = 1;
          if (Session::getConfiguration()->isVH() and (_trackSegment->getDepth() == 1))
            pitchSlack = 5;
          trackFree.inflate( pitchSlack*pitch, pitchSlack*pitch );
          cdebug_log(159,0) << "One pitch expand: " << trackFree << endl;
        }
        cdebug_log(159,0) << "Non-Pref Track Perpandicular Free: " << trackFree << endl;

        //_perpandicularFree.intersection
        //  ( trackFree.inflate ( pitch - perpandicular->getExtensionCap(Flags::Source)
        //                      , pitch - perpandicular->getExtensionCap(Flags::Target)) );
        _perpandicularFree.intersection( trackFree );
      } else if (perpandicular->getTrack()) {
        Interval  trackFree = perpandicular->getFreeInterval();
        cdebug_log(159,0) << "Track Perpandicular Free: " << trackFree << endl;

        DbU::Unit sourceCap = basePerpand->getExtensionCap( Flags::Source );
        DbU::Unit targetCap = basePerpand->getExtensionCap( Flags::Target );
        if (std::get<1>(perpandiculars[i]) & Flags::Source)
          targetCap = std::max( targetCap, sourceCap );
        else
          sourceCap = std::max( targetCap, sourceCap );
        _perpandicularFree.intersection( trackFree.inflate ( -sourceCap, -targetCap ) );
        cdebug_log(159,0) << "Source cap:"
                          << DbU::getValueString(perpandicular->getExtensionCap(Flags::Source)) << endl;
      } else if (       perpandicular->isFixedAxis()
                and not perpandicular->isReduced()
                /*or _trackSegment->isDogleg()*/) {
        RoutingPlane* plane = Session::getKatanaEngine()->getRoutingPlaneByLayer(perpandicular->getLayer());
        Track*        track = plane->getTrackByPosition( perpandicular->getAxis() );
        if (track and (perpandicular->getAxis() == track->getAxis())) {
          Interval  trackFree = track->getFreeInterval( perpandicular->getSourceU(), _trackSegment->getNet() );
          cdebug_log(159,0) << "SourceU: " << DbU::getValueString(perpandicular->getSourceU()) << endl;
          cdebug_log(159,0) << "Track Perpandicular Free (fixed axis, source): " << trackFree << endl;

          if (trackFree.isEmpty()) {
            trackFree = track->getFreeInterval( perpandicular->getTargetU(), _trackSegment->getNet() );
            cdebug_log(159,0) << "TargetU: " << DbU::getValueString(perpandicular->getTargetU()) << endl;
            cdebug_log(159,0) << "Track Perpandicular Free (fixed axis, target): " << trackFree << endl;
          }

          Flags isSource = std::get<1>( perpandiculars[i] );
          _perpandicularFree.intersection
            ( trackFree.inflate ( -perpandicular->getExtensionCap( isSource )
                                , -perpandicular->getExtensionCap( isSource ) ));
        }
      } else {
        DataNegociate* perpandData = perpandicular->getDataNegociate();
        DbU::Unit      pitch       = Session::getPitch( perpandicular->getLayer() );
        if (perpandData and perpandicular->isLocal()) {
          uint32_t limit = Session::getKatanaEngine()->getRipupLimit( _trackSegment );
          if ((perpandData->getRipupCount() > 8) and (perpandData->getRipupCount() < 14)) {
            cdebug_log(159,0) << "Highly riped-up local perpandicular" << endl;
            RoutingPlane* plane = Session::getKatanaEngine()->getRoutingPlaneByLayer(perpandicular->getLayer());
            Track*        track = plane->getTrackByPosition( perpandicular->getAxis() );
            Interval      trackFree;
            if (_trackSegment->getAxis() == perpandicular->getSourceAxis()) {
              trackFree = track->getFreeInterval( perpandicular->getTargetU(), _trackSegment->getNet() );
              cdebug_log(159,0) << "Track Perpandicular Free (source highly riped up): " << trackFree << endl;
            } else {
              trackFree = track->getFreeInterval( perpandicular->getSourceU(), _trackSegment->getNet() );
              cdebug_log(159,0) << "Track Perpandicular Free (target highly riped up): " << trackFree << endl;
            }

            if (trackFree.getSize() > 2*pitch) {
              Flags    isSource           = std::get<1>( perpandiculars[i] );
              Interval updatedPerpandFree = _perpandicularFree;
              updatedPerpandFree.intersection( trackFree.inflate( -perpandicular->getExtensionCap( isSource )
                                                                , -perpandicular->getExtensionCap( isSource ) ));
              if (updatedPerpandFree.getSize() > 2*pitch) {
                cdebug_log(159,0) << "Updated perpandicular Free: " << updatedPerpandFree << endl;
                _perpandicularFree = updatedPerpandFree;
              }
            } else {
              cdebug_log(159,0) << "Track Perpandicular Free empty, ignoring." << endl;
            }
          }
        } else {
          cdebug_log(159,0) << "Not in any track " << perpandicular << endl;
        }
      }

#if 0
      if (interval.isPonctual()) {
        cdebug_log(159,0) << "Punctual attractor @" << DbU::getValueString(interval.getVMin()) << endl;
        _attractors.push_back( interval.getVMin() );
        cdebug_tabw(159,-1);
        continue;
      }

      if (  (interval.getVMin() != _trackSegment->getAxis())
         or AutoSegment::isTopologicalBound(basePerpand
                                           ,perpandicular->isHorizontal() ? Flags::Horizontal : 0
                                           ) ) {
        map<DbU::Unit,int>::iterator iattractor = attractorSpins.find( interval.getVMin() );
        if (iattractor == attractorSpins.end()) {
          attractorSpins.insert( make_pair(interval.getVMin(),-1) );
        } else {
          iattractor->second -= 1;
        }
        cdebug_log(159,0) << "Left attractor @" << DbU::getValueString(interval.getVMin()) << endl;
      }

      if (  (interval.getVMax() != _trackSegment->getAxis())
         or AutoSegment::isTopologicalBound(basePerpand
                                           ,Flags::Propagate | (perpandicular->isHorizontal() ? Flags::Horizontal : 0)
                                           ) ) {
        map<DbU::Unit,int>::iterator iattractor = attractorSpins.find( interval.getVMax() );
        if (iattractor == attractorSpins.end()) {
          attractorSpins.insert( make_pair(interval.getVMax(),1) );
        } else {
          iattractor->second += 1;
        }
        cdebug_log(159,0) << "Right attractor @" << DbU::getValueString(interval.getVMax()) << endl;
      }

      if (perpandicular->base()->isReduceCandidate()) {
        if (reduceCandidates < 2) {
          if (interval.getVMin()+DbU::fromLambda(0.5) == _trackSegment->getAxis()) {
            _reduceRanges[reduceCandidates] = Interval( interval.getVMax()-pitch
                                                      , interval.getVMax()+pitch );
          } else if (interval.getVMax()-DbU::fromLambda(0.5) == _trackSegment->getAxis()) {
            _reduceRanges[reduceCandidates] = Interval( interval.getVMin()-pitch
                                                      , interval.getVMin()+pitch );
          }
          ++reduceCandidates;
        } 
      }
#endif

      cdebug_tabw(159,-1);
    }
    if ( not _trackSegment->isTerminal() and (_perpandiculars.size()+reducedPerpands < 2) )
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
    cdebug_log(159,0) << s.str() << endl;
    cdebug_log(159,0) << "Final Perpandicular Free: " << _perpandicularFree << endl;

    cdebug_log(159,0) << "Perpandiculars: " << endl;
    for ( TrackElement* perpand : _perpandiculars ) {
      cdebug_log(159,0) << "| " << perpand << endl;
    }

    cdebug_tabw(159,-1);
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


  string  DataNegociate::getStateString ( uint32_t state, unsigned int stateCount )
  {
    ostringstream s;
    switch ( state ) {
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
      case Repair:                 s << "Repair";                  break;
      case RepairFailed:           s << "RepairFailed";            break;
      default:
        s << "Unknown(" << state << ")"; break;
    }
    s << ":" << stateCount;
    return s.str();
  }


  string  DataNegociate::getStateString ( DataNegociate* data )
  {
    return getStateString( data->_state, data->_stateCount );
  }


} // Katana namespace.
