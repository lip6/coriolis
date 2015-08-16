// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :   "./kite/RoutingEvent.h"                        |
// +-----------------------------------------------------------------+


#ifndef  KITE_ROUTING_EVENT_H
#define  KITE_ROUTING_EVENT_H

#include <set>
#include <vector>
#include <cstdlib>
#include <functional>

#include "hurricane/Interval.h"
namespace Hurricane {
  class Net;
}

#include "kite/TrackCost.h"
#include "kite/TrackElement.h"
#include "kite/DataNegociate.h"
#include "kite/Session.h"


namespace Kite {

  using std::set;
  using std::vector;
  using std::binary_function;
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
          unsigned int   _tracksNb:6;
          float          _priority;
          unsigned int   _eventLevel;
          unsigned int   _segFlags;
          unsigned int   _layerDepth;
          DbU::Unit      _length;
          DbU::Unit      _axis;
          DbU::Unit      _sourceU;
          Net*           _net;
          unsigned long  _id;
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
      enum Mode { Negociate=1, Pack=2, Repair=3 };

    public:
      static  unsigned int                 getStage              ();
      static  size_t                       getAllocateds         ();
      static  size_t                       getProcesseds         ();
      static  size_t                       getCloneds            ();
      static  void                         resetProcesseds       ();
      static  void                         setStage              ( unsigned int );
    public:                                                      
      static  RoutingEvent*                create                ( TrackElement*, unsigned int mode=Negociate );
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
      inline  unsigned int                 getId                 () const;
      inline  bool                         getMode               () const;
      inline  bool                         canMinimize           () const;
              unsigned int                 getState              () const;
      inline  const Key&                   getKey                () const;
      inline  TrackElement*                getSegment            () const;
      inline  const vector<TrackElement*>& getPerpandiculars     () const;
      inline  DbU::Unit                    getAxisHint           () const;
      inline  DbU::Unit                    getAxisHistory        () const;
      inline  long                         getAxisWeight         ( DbU::Unit ) const;
      inline  const Interval&              getConstraints        () const;
      inline  const Interval&              getOptimal            () const;
      inline  const Interval&              getPerpandicularFree  () const;
      inline  float                        getPriority           () const;
      inline  unsigned int                 getTracksNb           () const;
      inline  unsigned int                 getTracksFree         () const;
      inline  unsigned int                 getInsertState        () const;
      inline  unsigned int                 getEventLevel         () const;
              void                         revalidate            ();
      inline  void                         updateKey             ();
              void                         process               ( RoutingEventQueue&
                                                                 , RoutingEventHistory&
                                                                 , RoutingEventLoop&
                                                                 );
              void                         setSegment            ( TrackElement* );
              RoutingEvent*                reschedule            ( RoutingEventQueue&, unsigned int eventLevel );
              void                         setMode               ( unsigned int );
              void                         setState              ( unsigned int );
      inline  void                         setProcessed          ( bool state=true );
      inline  void                         setDisabled           ( bool state=true );
      inline  void                         setMinimized          ( bool state=true );
      inline  void                         setRipedByLocal       ( bool state=true );
      inline  void                         setTracksFree         ( unsigned int );
      inline  void                         setForcedToHint       ( bool state = true );
              void                         setAxisHint           ( DbU::Unit );
              void                         setAxisHintFromParent ();
      inline  void                         incInsertState        ();
      inline  void                         resetInsertState      ();
      inline  void                         setEventLevel         ( unsigned int );
              void                         _processNegociate     ( RoutingEventQueue&, RoutingEventHistory& );
              void                         _processPack          ( RoutingEventQueue&, RoutingEventHistory& );
              void                         _processRepair        ( RoutingEventQueue&, RoutingEventHistory& );
              Record*                      _getRecord            () const;
              string                       _getString            () const;
              string                       _getTypeName          () const;
    private:                                                 
                                           RoutingEvent      ( TrackElement*, unsigned int mode );
                                          ~RoutingEvent      ();

    protected:
    // Attributes.
      static unsigned int   _idCounter;
      static unsigned int   _stage;
      static size_t         _allocateds;
      static size_t         _processeds;
      static size_t         _cloneds;
      mutable bool          _cloned;
      bool                  _processed;
      bool                  _disabled;
      bool                  _overConstrained;
      bool                  _minimized;
      bool                  _forceToHint;
      bool                  _ripedByLocal;
      unsigned int          _id;
      TrackElement*         _segment;
      DataNegociate*        _dataNegociate;
      DbU::Unit             _axisHistory;
      DbU::Unit             _axisHint;
      Interval              _constraints;
      Interval              _optimal;
    //Interval              _perpandicular;
      unsigned int          _tracksNb        : 6;
      unsigned int          _tracksFree      : 4;
      unsigned int          _insertState     : 6;
      unsigned int          _mode            : 4;
      unsigned int          _rippleState     : 4;
      unsigned int          _eventLevel;
      float                 _priority;
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
  inline unsigned int                  RoutingEvent::getId                   () const { return _id; }
  inline bool                          RoutingEvent::getMode                 () const { return _mode; }
  inline bool                          RoutingEvent::canMinimize             () const { return !_minimized; }
  inline const RoutingEvent::Key&      RoutingEvent::getKey                  () const { return _key; }
  inline TrackElement*                 RoutingEvent::getSegment              () const { return _segment; }
  inline const vector<TrackElement*>&  RoutingEvent::getPerpandiculars       () const { return _dataNegociate->getPerpandiculars(); }
//inline const vector<TrackElement*>&  RoutingEvent::getPerpandiculars       () const { return _perpandiculars; }
  inline DbU::Unit                     RoutingEvent::getAxisHistory          () const { return _axisHistory; }
  inline DbU::Unit                     RoutingEvent::getAxisHint             () const { return _axisHint; }
  inline long                          RoutingEvent::getAxisWeight           ( DbU::Unit axis ) const { return abs(axis - getAxisHint()); }
  inline const Interval&               RoutingEvent::getConstraints          () const { return _constraints; }
  inline const Interval&               RoutingEvent::getOptimal              () const { return _optimal; }
  inline const Interval&               RoutingEvent::getPerpandicularFree    () const { return _dataNegociate->getPerpandicularFree(); }
//inline const Interval&               RoutingEvent::getPerpandicular        () const { return _perpandicular; }
  inline float                         RoutingEvent::getPriority             () const { return _priority; }
  inline unsigned int                  RoutingEvent::getEventLevel           () const { return _eventLevel; }
  inline unsigned int                  RoutingEvent::getTracksNb             () const { return _tracksNb; }
  inline unsigned int                  RoutingEvent::getTracksFree           () const { return _tracksFree; }
  inline unsigned int                  RoutingEvent::getInsertState          () const { return _insertState; }
  inline void                          RoutingEvent::setProcessed            ( bool state ) { _processed  = state; }
  inline void                          RoutingEvent::setDisabled             ( bool state ) { _disabled = state; }
  inline void                          RoutingEvent::setMinimized            ( bool state ) { _minimized = state; }
  inline void                          RoutingEvent::setRipedByLocal         ( bool state ) { _ripedByLocal = state; }
  inline void                          RoutingEvent::setTracksFree           ( unsigned int nb ) { _tracksFree = nb; }
  inline void                          RoutingEvent::setForcedToHint         ( bool state ) { _forceToHint = state; }
  inline void                          RoutingEvent::incInsertState          () { _insertState++; }
  inline void                          RoutingEvent::resetInsertState        () { _insertState = 0; }
  inline void                          RoutingEvent::setEventLevel           ( unsigned int level ) { _eventLevel = level; }
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
           ltrace(149) << "Bounds := ["                                                     \
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


}  // Kite namespace.


INSPECTOR_P_SUPPORT(Kite::RoutingEvent);


#endif  // KITE_ROUTING_EVENT_H
