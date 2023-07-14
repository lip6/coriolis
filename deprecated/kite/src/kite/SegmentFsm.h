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
// |  C++ Header  :   "./kite/SegmentFsm.h"                          |
// +-----------------------------------------------------------------+


#ifndef  KITE_SEGMENT_FSM_H
#define  KITE_SEGMENT_FSM_H

#include "kite/TrackCost.h"

namespace Kite {

  class TrackElement;
  class DataNegociate;
  class RoutingEvent;
  class RoutingEventQueue;
  class RoutingEventHistory;


// -------------------------------------------------------------------
// Class  :  "SegmentAction".

  class SegmentAction {
    public:
      enum Type { Self                          = (1<< 0)
                , Other                         = (1<< 1)
                , Perpandicular                 = (1<< 2)
                , Insert                        = (1<< 3)
                , Ripup                         = (1<< 4)
                , RipedByLocal                  = (1<< 5)
                , ResetRipup                    = (1<< 6)
                , ToRipupLimit                  = (1<< 7)
                , MoveToAxis                    = (1<< 8)
                , AxisHint                      = (1<< 9)
                , PackingMode                   = (1<<10)
                , ToState                       = (1<<11)
                , EventLevel1                   = (1<<12)
                , EventLevel2                   = (1<<13)
                , EventLevel3                   = (1<<14)
                , EventLevel4                   = (1<<15)
                , EventLevel5                   = (1<<16)
                , SelfInsert                    = Self |Insert
                , SelfRipup                     = Self |Ripup
                , SelfRipupPerpand              = Self |Ripup|Perpandicular
                , SelfRipupPerpandWithAxisHint  = Self |Ripup|Perpandicular|EventLevel4|AxisHint
                , OtherRipup                    = Other|Ripup
                , OtherRipupPerpandAndPushAside = Other|Ripup|Perpandicular|EventLevel3|AxisHint
                , OtherRipupPerpandAndPacking   = Other|Ripup|Perpandicular|EventLevel4|PackingMode
                };
    public:
                           SegmentAction ( TrackElement*
                                         , unsigned int  type
                                         , DbU::Unit     axisHint=0
                                         , unsigned int  toState =0
                                         );
      inline TrackElement* getSegment    () const;
      inline unsigned int  getType       () const;
      inline void          setAxisHint   ( DbU::Unit );
      inline unsigned int  setFlag       ( unsigned int );
             bool          doAction      ( RoutingEventQueue& );
    private:
      TrackElement* _segment;
      unsigned int  _type;
      DbU::Unit     _axisHint;
      unsigned int  _toState;
  };


  inline TrackElement* SegmentAction::getSegment  () const { return _segment; }
  inline unsigned int  SegmentAction::getType     () const { return _type; }
  inline void          SegmentAction::setAxisHint ( DbU::Unit axis ) { _axisHint = axis; }
  inline unsigned int  SegmentAction::setFlag     ( unsigned int flag ) { _type |= flag; return _type; }


// -------------------------------------------------------------------
// Class  :  "SegmentFsm".

  class SegmentFsm {

    public:
      enum State        { MissingData              = (1<<0)
                        , EmptyTrackList           = (1<<1)
                        , Inserted                 = (1<<2)
                        , Self                     = (1<<3)
                        , Other                    = (1<<4)
                        , Ripup                    = (1<<5)
                        , MaximumSlack             = (1<<6)
                        , SelfInserted             = Self  | Inserted
                        , OtherRipup               = Other | Ripup
                        , SelfMaximumSlack         = Self  | MaximumSlack
                        };
      enum SlackenFlags { NoRecursive  = (1<<0)
                        , NoTransition = (1<<1)
                        };

    public:
                                    SegmentFsm             ( RoutingEvent*
                                                           , RoutingEventQueue&
                                                           , RoutingEventHistory&
                                                           );
      inline bool                   isFullBlocked          () const;
      inline RoutingEvent*          getEvent               () const;
      inline RoutingEventQueue&     getQueue               () const;
      inline RoutingEventHistory&   getHistory             () const;
      inline unsigned int           getState               () const;
      inline DataNegociate*         getData                ();
      inline Interval&              getConstraint          ();
      inline Interval&              getOptimal             ();
      inline vector<TrackCost>&     getCosts               ();
      inline TrackCost&             getCost                ( size_t );
      inline Track*                 getTrack               ( size_t );
      inline size_t                 getBegin               ( size_t );
      inline size_t                 getEnd                 ( size_t );
      inline vector<SegmentAction>& getActions             ();
      inline void                   setState               ( unsigned int );
             void                   addAction              ( TrackElement*
                                                           , unsigned int type
                                                           , DbU::Unit    axisHint=0
                                                           , unsigned int toState =0
                                                           );
             void                   doActions              ();
      inline void                   clearActions           ();
             bool                   insertInTrack          ( size_t );
             bool                   conflictSolveByHistory ();
             bool                   conflictSolveByPlaceds ();
             bool                   solveTerminalVsGlobal  ();
             bool                   desaturate             ();
             bool                   slackenTopology        ( unsigned int flags=0 );
             bool                   solveFullBlockages     ();
    private:                                               
             bool                   _slackenStrap          ( TrackElement*& 
                                                           , DataNegociate*&
                                                           , unsigned int    flags );
             bool                   _slackenLocal          ( TrackElement*& 
                                                           , DataNegociate*&
                                                           , unsigned int    flags  );
             bool                   _slackenGlobal         ( TrackElement*& 
                                                           , DataNegociate*&
                                                           , unsigned int    flags  );
    private:
      RoutingEvent*          _event;
      RoutingEventQueue&     _queue;
      RoutingEventHistory&   _history;
      unsigned int           _state;
      DataNegociate*         _data;
      Interval               _constraint;
      Interval               _optimal;
      vector<TrackCost>      _costs;
      vector<SegmentAction>  _actions;
      bool                   _fullBlocked;
  };


  inline bool                   SegmentFsm::isFullBlocked () const { return _fullBlocked and _costs.size(); }
  inline RoutingEvent*          SegmentFsm::getEvent      () const { return _event; }
  inline RoutingEventQueue&     SegmentFsm::getQueue      () const { return _queue; }
  inline RoutingEventHistory&   SegmentFsm::getHistory    () const { return _history; }
  inline unsigned int           SegmentFsm::getState      () const { return _state; }
  inline DataNegociate*         SegmentFsm::getData       () { return _data; }
  inline Interval&              SegmentFsm::getConstraint () { return _constraint; }
  inline Interval&              SegmentFsm::getOptimal    () { return _optimal; }
  inline vector<TrackCost>&     SegmentFsm::getCosts      () { return _costs; }
  inline TrackCost&             SegmentFsm::getCost       ( size_t i ) { return _costs[i]; }
  inline Track*                 SegmentFsm::getTrack      ( size_t i ) { return _costs[i].getTrack(); }
  inline size_t                 SegmentFsm::getBegin      ( size_t i ) { return _costs[i].getBegin(); }
  inline size_t                 SegmentFsm::getEnd        ( size_t i ) { return _costs[i].getEnd(); }
  inline vector<SegmentAction>& SegmentFsm::getActions    () { return _actions; }
  inline void                   SegmentFsm::setState      ( unsigned int state ) { _state = state; }
  inline void                   SegmentFsm::clearActions  () { _actions.clear(); }


}  // Kite namespace.

#endif  // KITE_SEGMENT_FSM_H
