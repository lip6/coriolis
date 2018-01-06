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
// |  C++ Header  :   "./kite/Manipulator.h"                         |
// +-----------------------------------------------------------------+


#ifndef  KITE_MANIPULATOR_H
#define  KITE_MANIPULATOR_H

#include "hurricane/DbU.h"
#include "katabatic/Constants.h"


namespace Kite {

  using Hurricane::DbU;
  using Katabatic::KbNoFlags;

  class TrackElement;
  class DataNegociate;
  class RoutingEvent;
  class SegmentFsm;


// -------------------------------------------------------------------
// Class Declaration  :  "::Manipulator".

  class Manipulator {
    public:
      enum FunctionFlag { ToRipupLimit        = 0x0001
                        , AllowExpand         = 0x0002
                        , NoExpand            = 0x0004
                        , PerpandicularsFirst = 0x0008
                        , ToMoveUp            = 0x0010
                        , AllowLocalMoveUp    = 0x0020
                        , AllowTerminalMoveUp = 0x0040
                        , AllowShortPivotUp   = 0x0080
                        , NoDoglegReuse       = 0x0100
                        , LeftAxisHint        = 0x0200
                        , RightAxisHint       = 0x0400 
                        , NotOnLastRipup      = 0x0800 
                        };
    public:
                                  Manipulator             ( TrackElement*, SegmentFsm& );
                                 ~Manipulator             ();
      inline TrackElement*        getSegment              () const;
      inline DataNegociate*       getData                 () const;
      inline RoutingEvent*        getEvent                () const;
      inline const Layer*         getLayer                () const;
      inline DbU::Unit            getPitch                () const;
      inline DbU::Unit            getPPitch               () const;
             bool                 canRipup                ( unsigned int flags=0 ) const;
             bool                 isCaged                 ( DbU::Unit ) const;
             bool                 ripup                   ( unsigned int type, DbU::Unit axisHint=0 );
             bool                 ripupPerpandiculars     ( unsigned int flags=0 );
             void                 repackPerpandiculars    ();
             void                 reprocessPerpandiculars ();
             bool                 ripple                  ();
             bool                 minimize                ();
             bool                 slacken                 ( unsigned int flags=KbNoFlags );
             bool                 pivotUp                 ();
             bool                 pivotDown               ();
             bool                 moveUp                  ( unsigned int flags=0 );
             bool                 makeDogleg              ();
             bool                 makeDogleg              ( DbU::Unit );
             bool                 makeDogleg              ( Interval );
             bool                 relax                   ( Interval, unsigned int flags=AllowExpand );
             bool                 insertInTrack           ( size_t );
             bool                 shrinkToTrack           ( size_t
                                                          , unsigned int flags=0
                                                          , DbU::Unit    leftAxisHint=0
                                                          , DbU::Unit    rightAxisHint=0
                                                          );
             bool                 forceToTrack            ( size_t );
             bool                 forceOverLocals         ();
    private:
      TrackElement*  _segment;
      DataNegociate* _data;
      RoutingEvent*  _event;
      SegmentFsm&    _fsm;
  };


  inline TrackElement*  Manipulator::getSegment () const { return _segment; }
  inline DataNegociate* Manipulator::getData    () const { return _data; }
  inline RoutingEvent*  Manipulator::getEvent   () const { return _event; }
  inline const Layer*   Manipulator::getLayer   () const { return _segment->getLayer(); }
  inline DbU::Unit      Manipulator::getPitch   () const { return _segment->getPitch(); }
  inline DbU::Unit      Manipulator::getPPitch  () const { return _segment->getPPitch(); }


}  // Kite namespace.

#endif  // KITE_MANIPULATOR_H
