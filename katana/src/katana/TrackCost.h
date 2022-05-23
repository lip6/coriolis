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
// |  C++ Header  :  "./katana/TrackCost.h"                          |
// +-----------------------------------------------------------------+


#pragma  once
#include <string>
#include <tuple>
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
  class TrackElement;


// -------------------------------------------------------------------
// Class  :  "TrackCost".
 
  class TrackCost {
    public:
      enum Flags { NoFlags            =  0
                 , IgnoreAxisWeight   = (1 <<  0)
                 , DiscardGlobals     = (1 <<  1)
                 , IgnoreSharedLength = (1 <<  2)
                 , LocalAndTopDepth   = (1 <<  3)
                 , ZeroCost           = (1 <<  4)
                 , Analog             = (1 <<  5)
                 , ShortNet           = (1 <<  6)
                 , Symmetric          = (1 <<  7)
                 , ForGlobal          = (1 <<  8)
                 , Blockage           = (1 <<  9)
                 , Fixed              = (1 << 10)
                 , Infinite           = (1 << 11)
                 , HardOverlap        = (1 << 12)
                 , Overlap            = (1 << 13)
                 , LeftOverlap        = (1 << 14)
                 , RightOverlap       = (1 << 15)
                 , OverlapGlobal      = (1 << 16)
                 , GlobalEnclosed     = (1 << 17)
                 , AtRipupLimit       = (1 << 18)
                 , IgnoreTerminals    = (1 << 19)
                 , IgnoreShort        = (1 << 20)
                 , Blacklisted        = (1 << 21)
                 , MergeMask          = ForGlobal     |Blockage|Fixed       |Infinite
                                       |HardOverlap   |Overlap |RightOverlap|LeftOverlap|OverlapGlobal
                                       |GlobalEnclosed         |AtRipupLimit
                 };

    public:
    // Sub-Class: "CompareByDelta()".
      class CompareByDelta {
        public:
          bool  operator() ( const TrackCost* lhs, const TrackCost* rhs );
      };
      class Compare {
        public:
          inline       Compare    ( uint32_t flags=0 );
                 bool  operator() ( const TrackCost* lhs, const TrackCost* rhs );
        private:
          uint32_t _flags;
      };

    public:
                                 TrackCost           ( TrackElement* refSegment
                                                     , TrackElement* symSegment
                                                     , Track*        refTrack
                                                     , Track*        symTrack
                                                     , DbU::Unit     refCandidateAxis
                                                     , DbU::Unit     symCandidateAxis
                                                     );
                                ~TrackCost           ();
      inline       bool          isForGlobal         () const;
      inline       bool          isBlockage          () const;
      inline       bool          isAnalog            () const;
      inline       bool          isShortNet          () const;
      inline       bool          isFixed             () const;
      inline       bool          isInfinite          () const;
      inline       bool          isOverlap           () const;
      inline       bool          isLeftOverlap       () const;
      inline       bool          isRightOverlap      () const;
      inline       bool          isHardOverlap       () const;
      inline       bool          isOverlapGlobal     () const;
      inline       bool          isGlobalEnclosed    () const;
      inline       bool          isAtRipupLimit      () const;
      inline       bool          isBlacklisted       () const;
                   bool          isFree              () const;
      inline       bool          isSymmetric         () const;
      inline       bool          isWide              () const;
      inline       bool          doIgnoreShort       () const;
      inline       uint32_t      getFlags            () const;
      inline       size_t        getSpan             () const;
      inline       Net*          getNet              () const;
                   Net*          getNet1             () const;
                   Net*          getNet2             () const;
      inline       TrackElement* getRefElement       () const;
      inline       TrackElement* getSymElement       () const;
      inline       DbU::Unit     getRefCandidateAxis () const;
      inline       DbU::Unit     getSymCandidateAxis () const;
      inline       Track*        getTrack            () const;
      inline       size_t        getBegin            () const;
      inline       size_t        getEnd              () const;
      inline       Track*        getTrack            ( size_t i ) const;
      inline       size_t        getBegin            ( size_t i ) const;
      inline       size_t        getEnd              ( size_t i ) const;
      inline       Track*        getTrack            ( size_t i, uint32_t flags ) const;
                   size_t        getBegin            ( size_t i, uint32_t flags ) const;
                   size_t        getEnd              ( size_t i, uint32_t flags ) const;
      inline const Interval&     getInterval         () const;
      inline const Interval&     getInterval1        () const;
      inline const Interval&     getInterval2        () const;
      inline       uint32_t      getTerminals        () const;
      inline       DbU::Unit     getDelta            () const;
      inline       DbU::Unit     getDeltaPerpand     () const;
      inline       DbU::Unit     getLongestOverlap   () const;
      inline       DbU::Unit     getAxisWeight       () const;
      inline       DbU::Unit     getFreeLength       () const;
      inline       int           getRipupCount       () const;
      inline       uint32_t      getDataState        () const;
      inline       uint32_t      setFlags            ( uint32_t );
      inline       void          setTrack            ( Track*, size_t begin, size_t end );
      inline       void          setForGlobal        ();
      inline       void          setBlockage         ();
      inline       void          setFixed            ();
      inline       void          setInfinite         ();
      inline       void          setOverlap          ();
      inline       void          setLeftOverlap      ();
      inline       void          setRightOverlap     ();
      inline       void          setHardOverlap      ();
      inline       void          setOverlapGlobal    ();
      inline       void          setGlobalEnclosed   ();
      inline       void          setAtRipupLimit     ();
      inline       void          setBlacklisted      ();
      inline       void          incTerminals        ( uint32_t );
      inline       void          incDelta            ( DbU::Unit );
      inline       void          incDeltaPerpand     ( DbU::Unit );
      inline       void          incDeltaShared      ( DbU::Unit );
      inline       void          incAxisWeight       ( DbU::Unit );
      inline       void          setLonguestOverlap  ( DbU::Unit );
      inline       void          setFreeLength       ( DbU::Unit );
      inline       void          mergeRipupCount     ( int );
      inline       void          mergeDataState      ( uint32_t );
      inline       bool          selectNextTrack     ();
      inline       bool          select              ( size_t index, uint32_t flags );
                   void          consolidate         ();
                   void          setDistanceToFixed  ();
                   Record*       _getRecord          () const;
                   string        _getString          () const;
      inline       string        _getTypeName        () const;
    private:                                         
                                 TrackCost           ( const TrackCost& ) = delete;
                   TrackCost&    operator=           ( const TrackCost& ) = delete;
    // Attributes.
    private:
      uint32_t      _flags;
      size_t        _span;
      DbU::Unit     _refCandidateAxis;
      DbU::Unit     _symCandidateAxis;
      std::vector< std::tuple<Track*,size_t,size_t> >
                    _tracks;
      TrackElement* _segment1;
      TrackElement* _segment2;
      Interval      _interval1;
      Interval      _interval2;
      uint32_t      _terminals;
      DbU::Unit     _delta;
      DbU::Unit     _deltaShared;
      DbU::Unit     _deltaPerpand;
      DbU::Unit     _axisWeight;
      DbU::Unit     _distanceToFixed;
      DbU::Unit     _longuestOverlap;
      DbU::Unit     _freeLength;
      uint32_t      _dataState;
      int           _ripupCount;
      uint32_t      _selectFlags;
      size_t        _selectIndex;
  };


// Inline Functions.
  inline       bool          TrackCost::isForGlobal         () const { return _flags & ForGlobal; }
  inline       bool          TrackCost::isBlockage          () const { return _flags & Blockage; }
  inline       bool          TrackCost::isAnalog            () const { return _flags & Analog; }
  inline       bool          TrackCost::isShortNet          () const { return _flags & ShortNet; }
  inline       bool          TrackCost::isFixed             () const { return _flags & Fixed; }
  inline       bool          TrackCost::isInfinite          () const { return _flags & Infinite; }
  inline       bool          TrackCost::isOverlap           () const { return _flags & Overlap; }
  inline       bool          TrackCost::isLeftOverlap       () const { return _flags & LeftOverlap; }
  inline       bool          TrackCost::isRightOverlap      () const { return _flags & RightOverlap; }
  inline       bool          TrackCost::isHardOverlap       () const { return _flags & HardOverlap; }
  inline       bool          TrackCost::isOverlapGlobal     () const { return _flags & OverlapGlobal; }
  inline       bool          TrackCost::isGlobalEnclosed    () const { return _flags & GlobalEnclosed; }
  inline       bool          TrackCost::isAtRipupLimit      () const { return _flags & AtRipupLimit; }
  inline       bool          TrackCost::isBlacklisted       () const { return _flags & Blacklisted; }
  inline       bool          TrackCost::isSymmetric         () const { return _flags & Symmetric; }
  inline       bool          TrackCost::isWide              () const { return (_span > 1); }
  inline       bool          TrackCost::doIgnoreShort       () const { return _flags & IgnoreShort; }
  inline       uint32_t      TrackCost::getFlags            () const { return _flags; }
  inline       size_t        TrackCost::getSpan             () const { return _span; }
  inline       Net*          TrackCost::getNet              () const { return (_selectFlags & Symmetric) ? getNet2() : getNet1(); }
  inline       TrackElement* TrackCost::getRefElement       () const { return _segment1; }
  inline       TrackElement* TrackCost::getSymElement       () const { return _segment2; }
  inline       DbU::Unit     TrackCost::getRefCandidateAxis () const { return _refCandidateAxis; }
  inline       DbU::Unit     TrackCost::getSymCandidateAxis () const { return _symCandidateAxis; }
  inline       Track*        TrackCost::getTrack            ( size_t i ) const { return getTrack(i,NoFlags); }
  inline       size_t        TrackCost::getBegin            () const { return getBegin(_selectIndex,_selectFlags); }
  inline       size_t        TrackCost::getBegin            ( size_t i ) const { return getBegin(i,NoFlags); }
  inline       size_t        TrackCost::getEnd              () const { return getEnd  (_selectIndex,_selectFlags); }
  inline       size_t        TrackCost::getEnd              ( size_t i ) const { return getEnd  (i,NoFlags); }
  inline const Interval&     TrackCost::getInterval         () const { return (_selectFlags & Symmetric) ? getInterval2() : getInterval1(); }
  inline const Interval&     TrackCost::getInterval1        () const { return _interval1; }
  inline const Interval&     TrackCost::getInterval2        () const { return _interval2; }
  inline       uint32_t      TrackCost::getTerminals        () const { return _terminals; }
  inline       DbU::Unit     TrackCost::getLongestOverlap   () const { return _longuestOverlap; }
  inline       DbU::Unit     TrackCost::getFreeLength       () const { return _freeLength; }
  inline       DbU::Unit     TrackCost::getDelta            () const { return _delta; }
  inline       DbU::Unit     TrackCost::getAxisWeight       () const { return _axisWeight; }
  inline       int           TrackCost::getRipupCount       () const { return _ripupCount; }
  inline       uint32_t      TrackCost::getDataState        () const { return _dataState; }
  inline       uint32_t      TrackCost::setFlags            ( uint32_t mask ) { _flags |= mask; return _flags; }
  inline       void          TrackCost::setForGlobal        () { _flags |= ForGlobal; }
  inline       void          TrackCost::setBlockage         () { _flags |= Blockage; }
  inline       void          TrackCost::setFixed            () { _flags |= Fixed; }
  inline       void          TrackCost::setInfinite         () { _flags |= Infinite; }
  inline       void          TrackCost::setOverlap          () { _flags |= Overlap; }
  inline       void          TrackCost::setLeftOverlap      () { _flags |= LeftOverlap; }
  inline       void          TrackCost::setRightOverlap     () { _flags |= RightOverlap; }
  inline       void          TrackCost::setHardOverlap      () { _flags |= HardOverlap; }
  inline       void          TrackCost::setOverlapGlobal    () { _flags |= OverlapGlobal; }
  inline       void          TrackCost::setGlobalEnclosed   () { _flags |= GlobalEnclosed; }
  inline       void          TrackCost::setAtRipupLimit     () { _flags |= AtRipupLimit; }
  inline       void          TrackCost::setBlacklisted      () { _flags |= Blacklisted; }
  inline       void          TrackCost::incTerminals        ( uint32_t terminals    ) { _terminals    += terminals; }
  inline       void          TrackCost::incDelta            ( DbU::Unit delta )       { _delta        += delta; }
  inline       void          TrackCost::incDeltaPerpand     ( DbU::Unit delta )       { _deltaPerpand += delta; }
  inline       void          TrackCost::incDeltaShared      ( DbU::Unit delta )       { _deltaShared  += delta; }
  inline       void          TrackCost::incAxisWeight       ( DbU::Unit weight )      { _axisWeight   += weight; }
  inline       void          TrackCost::setLonguestOverlap  ( DbU::Unit overlap )     { _longuestOverlap = std::max( overlap, _longuestOverlap ); }
  inline       void          TrackCost::setFreeLength       ( DbU::Unit length )      { _freeLength      = std::min( length , _freeLength      ); }
  inline       void          TrackCost::mergeRipupCount     ( int count )             { _ripupCount      = std::max( count  , _ripupCount      ); }
  inline       void          TrackCost::mergeDataState      ( uint32_t state )        { _dataState       = std::max( state  , _dataState       ); }
  inline       string        TrackCost::_getTypeName        () const { return "TrackCost"; }


  inline  TrackCost::Compare::Compare ( uint32_t flags ) : _flags(flags) { }


  inline  Track* TrackCost::getTrack () const
  {
    // cdebug_log( 55,0) << "TrackCost::getTrack() _index:" << _selectIndex
    //                   << " flags:" << _selectFlags << std::endl;
    return getTrack(_selectIndex,_selectFlags);
  }


  inline  bool  TrackCost::selectNextTrack ()
  {
    if (_selectIndex+1 < _span) {
      ++_selectIndex;
      cdebug_log( 55,0) << "TrackCost::selectNextTrack() _index:" << _selectIndex
                        << " flags:" << _selectFlags << std::endl;
      return true;
    }
    cdebug_log( 55,0) << "TrackCost::selectNextTrack() over span:" << _span << std::endl;
    return false;
  }


  inline  bool  TrackCost::select ( size_t index, uint32_t flags )
  {
    if ( (index >= _span) or ((flags & Symmetric) and not (_flags & Symmetric)) ) {
      _selectIndex = 0;
      _selectFlags = NoFlags;
      return false;
    }

    _selectIndex = index;
    _selectFlags = flags;

    cdebug_log( 55,0) << "TrackCost::select() _index:" << _selectIndex
                      << " flags:" << _selectFlags << std::endl;
    return true;
  }


  inline  Track*  TrackCost::getTrack ( size_t i, uint32_t flags ) const
  {
    if (i >= _span) return NULL;

    // cdebug_log( 55,0) << "TrackCost::getTrack() i:" << i
    //                   << " flags:" << flags
    //                   << " index:" << (i + ((flags & Symmetric) ? _span : 0)) << std::endl;

    return std::get<0>( _tracks[i + ((flags & Symmetric) ? _span : 0)] );
  }


  inline  void  TrackCost::setTrack ( Track* track, size_t begin, size_t end )
  {
    cdebug_log( 55,0) << "TrackCost::setTrack() sindex:" << _selectIndex 
                      << " sflags:" << _selectFlags
                      << " index:" << (_selectIndex + ((_selectFlags & Symmetric) ? _span : 0))
                      << " " << track << std::endl;

    auto& entry = _tracks[_selectIndex + ((_selectFlags & Symmetric) ? _span : 0)];
    std::get<0>( entry ) = track;
    std::get<1>( entry ) = begin;
    std::get<2>( entry ) = end;
  }


} // Katana namespace.


INSPECTOR_P_SUPPORT(Katana::TrackCost);
