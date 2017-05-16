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
// |  C++ Header  :  "./katana/TrackCost.h"                          |
// +-----------------------------------------------------------------+


#ifndef  KATANA_TRACK_COST_H
#define  KATANA_TRACK_COST_H

#include <string>
#include "hurricane/Interval.h"
namespace Hurricane {
  class Net;
}


namespace Katana {

  using std::string;
  using Hurricane::Record;
  using Hurricane::DbU;
  using Hurricane::Interval;
  using Hurricane::Net;
  class Track;


// -------------------------------------------------------------------
// Class  :  "TrackCost".
 
  class TrackCost {
    public:
      enum Flags { IgnoreAxisWeight   = 0x0001
                 , DiscardGlobals     = 0x0002
                 , IgnoreSharedLength = 0x0004
                 , LocalAndTopDepth   = 0x0008
                 , ZeroCost           = 0x0010
                 };

    public:
    // Sub-Class: "CompareByDelta()".
      class CompareByDelta {
        public:
          bool  operator() ( const TrackCost& lhs, const TrackCost& rhs );
      };
      class Compare {
        public:
          inline       Compare    ( uint32_t flags=0 );
                 bool  operator() ( const TrackCost& lhs, const TrackCost& rhs );
        private:
          uint32_t _flags;
      };

    public:
                                 TrackCost          (       Track*    track );
                                 TrackCost          (       Track*    track
                                                    , const Interval& interval
                                                    ,       size_t    begin
                                                    ,       size_t    end
                                                    ,       Net*      net
                                                    ,       uint32_t  flags
                                                    );
                                ~TrackCost          ();
      inline       bool          isForGlobal        () const;
      inline       bool          isBlockage         () const;
      inline       bool          isFixed            () const;
      inline       bool          isInfinite         () const;
      inline       bool          isOverlap          () const;
      inline       bool          isLeftOverlap      () const;
      inline       bool          isRightOverlap     () const;
      inline       bool          isHardOverlap      () const;
      inline       bool          isOverlapGlobal    () const;
      inline       bool          isGlobalEnclosed   () const;
                   bool          isFree             () const;
      inline       uint32_t      getFlags           () const;
      inline       Track*        getTrack           () const;
      inline       size_t        getBegin           () const;
      inline       size_t        getEnd             () const;
      inline const Interval&     getInterval        () const;
      inline       uint32_t      getTerminals       () const;
      inline       DbU::Unit     getDelta           () const;
      inline       DbU::Unit     getDeltaPerpand    () const;
      inline       DbU::Unit     getLongestOverlap  () const;
      inline       long          getAxisWeight      () const;
      inline       int           getRipupCount      () const;
      inline       uint32_t      getDataState       () const;
      inline       void          setForGlobal       ();
      inline       void          setBlockage        ();
      inline       void          setFixed           ();
      inline       void          setInfinite        ();
      inline       void          setOverlap         ();
      inline       void          setLeftOverlap     ();
      inline       void          setRightOverlap    ();
      inline       void          setHardOverlap     ();
      inline       void          setOverlapGlobal   ();
      inline       void          setGlobalEnclosed  ();
      inline       void          incTerminals       ( uint32_t );
      inline       void          incDelta           ( DbU::Unit );
      inline       void          incDeltaPerpand    ( DbU::Unit );
      inline       void          incDeltaShared     ( DbU::Unit );
      inline       void          setAxisWeight      ( DbU::Unit );
      inline       void          setLonguestOverlap ( DbU::Unit );
      inline       void          mergeRipupCount    ( int );
      inline       void          mergeDataState     ( uint32_t );
                   void          merge              ( const TrackCost& );
                   void          consolidate        ();
                   Record*       _getRecord         () const;
                   string        _getString         () const;
      inline       string        _getTypeName       () const;
    // Operators.

    // Attributes.
    protected:
      uint32_t      _flags;
      Track*        _track;
      size_t        _begin;
      size_t        _end;
      Interval      _interval;
      bool          _forGlobal;
      bool          _blockage;
      bool          _fixed;
      bool          _infinite;
      bool          _hardOverlap;
      bool          _overlap;
      bool          _leftOverlap;
      bool          _rightOverlap;
      bool          _overlapGlobal;
      bool          _globalEnclosed;
      uint32_t      _terminals;
      DbU::Unit     _delta;
      DbU::Unit     _deltaShared;
      DbU::Unit     _deltaPerpand;
      DbU::Unit     _axisWeight;
      DbU::Unit     _distanceToFixed;
      DbU::Unit     _longuestOverlap;
      uint32_t      _dataState;
      int           _ripupCount;

  };


// Inline Functions.
  inline       bool          TrackCost::isForGlobal        () const { return _forGlobal; }
  inline       bool          TrackCost::isBlockage         () const { return _blockage; }
  inline       bool          TrackCost::isFixed            () const { return _fixed; }
  inline       bool          TrackCost::isInfinite         () const { return _infinite; }
  inline       bool          TrackCost::isOverlap          () const { return _overlap; }
  inline       bool          TrackCost::isLeftOverlap      () const { return _leftOverlap; }
  inline       bool          TrackCost::isRightOverlap     () const { return _rightOverlap; }
  inline       bool          TrackCost::isHardOverlap      () const { return _hardOverlap; }
  inline       bool          TrackCost::isOverlapGlobal    () const { return _overlapGlobal; }
  inline       bool          TrackCost::isGlobalEnclosed   () const { return _globalEnclosed; }
  inline       uint32_t      TrackCost::getFlags           () const { return _flags; }
  inline       Track*        TrackCost::getTrack           () const { return _track; }
  inline       size_t        TrackCost::getBegin           () const { return _begin; }
  inline       size_t        TrackCost::getEnd             () const { return _end; }
  inline const Interval&     TrackCost::getInterval        () const { return _interval; }
  inline       uint32_t      TrackCost::getTerminals       () const { return _terminals; }
  inline       DbU::Unit     TrackCost::getLongestOverlap  () const { return _longuestOverlap; }
  inline       DbU::Unit     TrackCost::getDelta           () const { return _delta; }
  inline       long          TrackCost::getAxisWeight      () const { return _axisWeight; }
  inline       int           TrackCost::getRipupCount      () const { return _ripupCount; }
  inline       uint32_t      TrackCost::getDataState       () const { return _dataState; }
  inline       void          TrackCost::setForGlobal       () { _forGlobal = true; }
  inline       void          TrackCost::setBlockage        () { _blockage = true; }
  inline       void          TrackCost::setFixed           () { _fixed = true; }
  inline       void          TrackCost::setInfinite        () { _infinite = true; }
  inline       void          TrackCost::setOverlap         () { _overlap = true; }
  inline       void          TrackCost::setLeftOverlap     () { _leftOverlap = true; }
  inline       void          TrackCost::setRightOverlap    () { _rightOverlap = true; }
  inline       void          TrackCost::setHardOverlap     () { _hardOverlap = true; }
  inline       void          TrackCost::setOverlapGlobal   () { _overlapGlobal = true; }
  inline       void          TrackCost::setGlobalEnclosed  () { _globalEnclosed = true; }
  inline       void          TrackCost::incTerminals       ( uint32_t terminals ) { _terminals += terminals; }
  inline       void          TrackCost::incDelta           ( DbU::Unit delta ) { _delta        += delta; }
  inline       void          TrackCost::incDeltaPerpand    ( DbU::Unit delta ) { _deltaPerpand += delta; }
  inline       void          TrackCost::incDeltaShared     ( DbU::Unit delta ) { _deltaShared  += delta; }
  inline       void          TrackCost::setAxisWeight      ( DbU::Unit weight ) { _axisWeight = weight; }
  inline       void          TrackCost::setLonguestOverlap ( DbU::Unit overlap ) { _longuestOverlap = (overlap > _longuestOverlap) ? overlap : _longuestOverlap; }
  inline       void          TrackCost::mergeRipupCount    ( int count ) { _ripupCount = (count>_ripupCount)?count:_ripupCount; }
  inline       void          TrackCost::mergeDataState     ( uint32_t state ) { _dataState = (state>_dataState)?state:_dataState; }
  inline       string        TrackCost::_getTypeName       () const { return "TrackCost"; }

  inline  TrackCost::Compare::Compare ( uint32_t flags ) : _flags(flags) { }


} // Katana namespace.


INSPECTOR_V_SUPPORT(Katana::TrackCost);


#endif  // KATANA_TRACK_COST_H
