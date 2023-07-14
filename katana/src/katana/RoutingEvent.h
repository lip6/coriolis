// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2008-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :   "./katana/RoutingEvent.h"                      |
// +-----------------------------------------------------------------+


#pragma  once
#include <set>
#include <vector>
#include <cstdlib>
#include <functional>

#include "hurricane/Interval.h"
namespace Hurricane {
  class Net;
}

#include "katana/TrackCost.h"
#include "katana/TrackElement.h"
#include "katana/DataNegociate.h"
#include "katana/Session.h"


namespace Katana {

  using std::set;
  using std::vector;
  using std::binary_function;
  using std::labs;
  using Hurricane::DbU;
  using Hurricane::Interval;
  using Hurricane::Net;
  class TrackElement;
  class Track;
  class RoutingEventHistory;
  class RoutingEventQueue;
  class RoutingEventLoop;


// -------------------------------------------------------------------
// Class  :  "RoutingEvent".

  class RoutingEvent {

    private:
      class Key {
        public:
          class Compare : public binary_function<const Key&,const Key&,bool> {
            public:
              bool  operator() ( const Key& lhs, const Key& rhs ) const;
          };

        public:
                Key    ( const RoutingEvent* );
          void  update ( const RoutingEvent* );
        private:
          unsigned int  _tracksNb  :16;
          unsigned int  _rpDistance: 4;
          float         _priority;
          uint32_t      _eventLevel;
          uint32_t      _segFlags;
          uint32_t      _layerDepth;
          DbU::Unit     _length;
          DbU::Unit     _axis;
          DbU::Unit     _sourceU;
          Net*          _net;
          uint64_t      _id;
       friend class Compare;
      };

    public:
    // Sub-Class: "Compare".
      class Compare : public binary_function<const RoutingEvent*,const RoutingEvent*,bool> {
        public:
          bool  operator() ( const RoutingEvent* lhs, const RoutingEvent* rhs ) const;
      };
    // Sub-Class: "CompareById".
      class CompareById : public binary_function<const RoutingEvent*,const RoutingEvent*,bool> {
        public:
          inline bool  operator() ( const RoutingEvent* lhs, const RoutingEvent* rhs ) const;
      };
    friend class Compare;

    public:
      static  uint32_t                     getStage              ();
      static  uint32_t                     getAllocateds         ();
      static  uint32_t                     getProcesseds         ();
      static  uint32_t                     getCloneds            ();
      static  void                         resetProcesseds       ();
    public:                                                      
      static  RoutingEvent*                create                ( TrackElement* );
              RoutingEvent*                clone                 () const;
              void                         destroy               ();
      inline  bool                         isCloned              () const;
      inline  bool                         isValid               () const;
              bool                         isUnimplemented       () const;
      inline  bool                         isProcessed           () const;
      inline  bool                         isDisabled            () const;
      inline  bool                         isForcedToHint        () const;
      inline  bool                         isSheared             () const;
      inline  bool                         isRipedByLocal        () const;
      inline  bool                         isOverConstrained     () const;
      inline  uint32_t                     getId                 () const;
      inline  uint32_t                     getTimeStamp          () const;
      inline  bool                         getMode               () const;
      inline  bool                         canMinimize           () const;
              uint32_t                     getState              () const;
      inline  const Key&                   getKey                () const;
      inline  TrackElement*                getSegment            () const;
      inline  const vector<TrackElement*>& getPerpandiculars     () const;
      inline  DbU::Unit                    getAxisHint           () const;
      inline  DbU::Unit                    getAxisHistory        () const;
      inline  DbU::Unit                    getAxisWeight         ( DbU::Unit ) const;
      inline  const Interval&              getConstraints        () const;
      inline  const Interval&              getOptimal            () const;
      inline  const Interval&              getPerpandicularFree  () const;
      inline  float                        getPriority           () const;
      inline  uint32_t                     getTracksNb           () const;
      inline  uint32_t                     getTracksFree         () const;
      inline  uint32_t                     getInsertState        () const;
      inline  uint32_t                     getEventLevel         () const;
              void                         revalidate            ();
              void                         _revalidateNonPref    ();
      inline  void                         updateKey             ();
              void                         process               ( RoutingEventQueue&
                                                                 , RoutingEventHistory&
                                                                 , RoutingEventLoop&
                                                                 );
              void                         setSegment            ( TrackElement* );
              RoutingEvent*                reschedule            ( RoutingEventQueue&, uint32_t eventLevel );
              void                         setState              ( uint32_t );
      inline  void                         setTimeStamp          ( uint32_t );
      inline  void                         setProcessed          ( bool state=true );
      inline  void                         setDisabled           ( bool state=true );
      inline  void                         setMinimized          ( bool state=true );
      inline  void                         setRipedByLocal       ( bool state=true );
      inline  void                         setTracksFree         ( uint32_t );
      inline  void                         setForcedToHint       ( bool state = true );
              void                         setAxisHint           ( DbU::Unit );
              void                         setAxisHintFromParent ();
      inline  void                         updateAxisHistory     ();
      inline  void                         setInsertState        ( uint32_t );
      inline  void                         incInsertState        ();
      inline  void                         resetInsertState      ();
      inline  void                         setEventLevel         ( uint32_t );
              void                         _processNegociate     ( RoutingEventQueue&, RoutingEventHistory& );
              void                         _processPack          ( RoutingEventQueue&, RoutingEventHistory& );
              void                         _processRepair        ( RoutingEventQueue&, RoutingEventHistory& );
              void                         _processRealign       ( RoutingEventQueue&, RoutingEventHistory& );
              Record*                      _getRecord            () const;
              string                       _getString            () const;
              string                       _getTypeName          () const;
    private:                                                 
                                           RoutingEvent          ( TrackElement* );
                                          ~RoutingEvent          ();

    protected:
    // Attributes.
      static uint32_t       _idCounter;
      static uint32_t       _stage;
      static uint32_t       _allocateds;
      static uint32_t       _processeds;
      static uint32_t       _cloneds;
      mutable bool          _cloned;
      bool                  _processed;
      bool                  _disabled;
      bool                  _overConstrained;
      bool                  _minimized;
      bool                  _forceToHint;
      bool                  _ripedByLocal;
      uint32_t              _id;
      uint32_t              _timeStamp;
      TrackElement*         _segment;
      DataNegociate*        _dataNegociate;
      DbU::Unit             _axisHistory;
      DbU::Unit             _axisHint;
      Interval              _constraints;
      Interval              _optimal;
    //Interval              _perpandicular;
      unsigned int          _tracksNb        :16;
      unsigned int          _tracksFree      :16;
      unsigned int          _insertState     : 6;
      unsigned int          _rippleState     : 4;
      uint32_t              _eventLevel;
    //vector<TrackElement*> _perpandiculars;
      Key                   _key;
  };


// Inline Functions.
  inline bool                          RoutingEvent::isCloned                () const { return _cloned; }
  inline bool                          RoutingEvent::isProcessed             () const { return _processed; }
  inline bool                          RoutingEvent::isDisabled              () const { return _disabled; }
  inline bool                          RoutingEvent::isForcedToHint          () const { return _forceToHint; }
  inline bool                          RoutingEvent::isRipedByLocal          () const { return _ripedByLocal; }
  inline bool                          RoutingEvent::isOverConstrained       () const { return _overConstrained; }
  inline uint32_t                      RoutingEvent::getId                   () const { return _id; }
  inline uint32_t                      RoutingEvent::getTimeStamp            () const { return _timeStamp; }
  inline bool                          RoutingEvent::canMinimize             () const { return not _minimized; }
  inline const RoutingEvent::Key&      RoutingEvent::getKey                  () const { return _key; }
  inline TrackElement*                 RoutingEvent::getSegment              () const { return _segment; }
  inline const vector<TrackElement*>&  RoutingEvent::getPerpandiculars       () const { return _dataNegociate->getPerpandiculars(); }
//inline const vector<TrackElement*>&  RoutingEvent::getPerpandiculars       () const { return _perpandiculars; }
  inline DbU::Unit                     RoutingEvent::getAxisHistory          () const { return _axisHistory; }
  inline DbU::Unit                     RoutingEvent::getAxisHint             () const { return _axisHint; }
  inline DbU::Unit                     RoutingEvent::getAxisWeight           ( DbU::Unit axis ) const { return labs(axis - getAxisHint()); }
  inline const Interval&               RoutingEvent::getConstraints          () const { return _constraints; }
  inline const Interval&               RoutingEvent::getOptimal              () const { return _optimal; }
  inline const Interval&               RoutingEvent::getPerpandicularFree    () const { return _dataNegociate->getPerpandicularFree(); }
//inline const Interval&               RoutingEvent::getPerpandicular        () const { return _perpandicular; }
  inline float                         RoutingEvent::getPriority             () const { return _segment->getPriority(); }
  inline uint32_t                      RoutingEvent::getEventLevel           () const { return _eventLevel; }
  inline uint32_t                      RoutingEvent::getTracksNb             () const { return _tracksNb; }
  inline uint32_t                      RoutingEvent::getTracksFree           () const { return _tracksFree; }
  inline uint32_t                      RoutingEvent::getInsertState          () const { return _insertState; }
  inline void                          RoutingEvent::setTimeStamp            ( uint32_t stamp ) { _timeStamp = stamp; }
  inline void                          RoutingEvent::setProcessed            ( bool state ) { _processed  = state; }
  inline void                          RoutingEvent::setDisabled             ( bool state ) { _disabled = state; }
  inline void                          RoutingEvent::setMinimized            ( bool state ) { _minimized = state; }
  inline void                          RoutingEvent::setRipedByLocal         ( bool state ) { _ripedByLocal = state; }
  inline void                          RoutingEvent::setTracksFree           ( uint32_t nb ) { _tracksFree = nb; }
  inline void                          RoutingEvent::setForcedToHint         ( bool state ) { _forceToHint = state; }
  inline void                          RoutingEvent::updateAxisHistory       () { _axisHistory = _segment->getAxis(); }
  inline void                          RoutingEvent::setInsertState          ( uint32_t state ) { _insertState = state; }
  inline void                          RoutingEvent::incInsertState          () { _insertState++; }
  inline void                          RoutingEvent::resetInsertState        () { _insertState = 0; }
  inline void                          RoutingEvent::setEventLevel           ( uint32_t level ) { _eventLevel = level; }
  inline void                          RoutingEvent::updateKey               () { revalidate(); _key.update(this); }

  inline bool  RoutingEvent::CompareById::operator() ( const RoutingEvent* lhs, const RoutingEvent* rhs ) const
  { return lhs->getId() < rhs->getId(); }


  typedef  set<RoutingEvent*,RoutingEvent::CompareById>  RoutingEventSet;


// -------------------------------------------------------------------
// Macros.


# if !defined(NDEBUG)
#   define _preCheck(segment)                                               \
           DbU::Unit  beforeMin = segment->base()->getSourcePosition();     \
           DbU::Unit  beforeMax = segment->base()->getTargetPosition();     \
           segment->base()->checkPositions ();                              \
           if ( Session::getSegmentStackSize() )                            \
             cerr << "[ERROR] Session Segment Stack is not empty ("         \
                  << Session::getSegmentStackSize() << ")." << endl;

#   define _postCheck(segment)                                                              \
           cdebug_log(159,0) << "Bounds := ["                                                     \
                       << DbU::getValueString(segment->base()->getSourcePosition()) << ":"  \
                       << DbU::getValueString(segment->base()->getTargetPosition()) << "] " \
                       << DbU::getValueString(segment->getAxis()) << " "                    \
                       << segment->getTrack() << endl;                                      \
           if ( beforeMin != segment->base()->getSourcePosition() )                         \
             cerr << "[ERROR] " << segment                                                  \
                  << " origin was " << DbU::getValueString(beforeMin) << endl;              \
           if ( beforeMax != segment->base()->getTargetPosition() )                         \
             cerr << "[ERROR] " << segment                                                  \
                  << " extremity was " << DbU::getValueString(beforeMax) << endl;
# else
#   define _preCheck(segment)   ;
#   define _postCheck(segment)  ;
# endif


}  // Katana namespace.


INSPECTOR_P_SUPPORT(Katana::RoutingEvent);
