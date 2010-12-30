
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./RoutingEvent.h"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __KITE_ROUTING_EVENT__
#define  __KITE_ROUTING_EVENT__

#include  <vector>
#include  <cstdlib>

#include "hurricane/Interval.h"
namespace Hurricane {
  class Net;
}

#include "kite/TrackCost.h"
#include "kite/TrackElement.h"
#include "kite/Session.h"


namespace Kite {

  using std::vector;
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
          struct Compare {
              bool  operator() ( const Key& lhs, const Key& rhs ) const;
          };
        public:
                Key    ( const RoutingEvent* );
          void  update ( const RoutingEvent* );
        private:
          bool           _slackenStrap;
          bool           _isHorizontal;
          bool           _canRipple;
          unsigned int   _tracksNb:6;
          float          _priority;
          unsigned int   _eventLevel;
          DbU::Unit      _length;
          DbU::Unit      _axis;
          DbU::Unit      _sourceU;
          Net*           _net;
          unsigned long  _id;
        friend class Compare;
      };

    public:
    // Sub-Class:  "Compare".
      struct Compare {
          bool  operator() ( const RoutingEvent* lhs, const RoutingEvent* rhs ) const;
      };
    friend class Compare;

    public:
      enum Mode { Negociate=1, Pack=2, PostPack=3 };

    public:
      static  size_t                       getAllocateds     ();
      static  size_t                       getProcesseds     ();
      static  size_t                       getCloneds        ();
      static  void                         resetProcesseds   ();
    public:                                                  
      static  RoutingEvent*                create            ( TrackElement*, unsigned int mode=Negociate );
              RoutingEvent*                clone             () const;
              void                         destroy           ();
      inline  bool                         isCloned          () const;
      inline  bool                         isValid           () const;
              bool                         isUnimplemented   () const;
      inline  bool                         isProcessed       () const;
      inline  bool                         isDisabled        () const;
      inline  bool                         isForcedToHint    () const;
      inline  bool                         isSheared         () const;
      inline  bool                         isRipedByLocal    () const;
      inline  bool                         getMode           () const;
      inline  bool                         canMinimize       () const;
              unsigned int                 getState          () const;
      inline  const Key&                   getKey            () const;
      inline  TrackElement*                getSegment        () const;
      inline  const vector<TrackElement*>& getPerpandiculars () const;
      inline  DbU::Unit                    getAxisHint       () const;
      inline  DbU::Unit                    getAxisHistory    () const;
      inline  long                         getAxisWeight     ( DbU::Unit ) const;
      inline  const Interval&              getOptimalAxis    () const;
      inline  const Interval&              getConstraints    () const;
      inline  const Interval&              getOptimal        () const;
      inline  const Interval&              getPerpandicular  () const;
      inline  Katabatic::GCell*            getShearGCell     () const;
      inline  float                        getPriority       () const;
      inline  unsigned int                 getTracksNb       () const;
      inline  unsigned int                 getTracksFree     () const;
      inline  unsigned int                 getInsertState    () const;
      inline  unsigned int                 getEventLevel     () const;
      inline  void                         invalidate        ( bool withPerpandiculars=false, bool withConstraints=false );
              void                         revalidate        ( bool force=false );
      inline  void                         updateKey         ();
              void                         process           ( RoutingEventQueue&
                                                             , RoutingEventHistory&
                                                             , RoutingEventLoop&
                                                             );
              void                         setSegment        ( TrackElement* );
              RoutingEvent*                reschedule        ( RoutingEventQueue&, unsigned int eventLevel );
      inline  void                         setMode           ( unsigned int );
              void                         setState          ( unsigned int );
      inline  void                         setProcessed      ( bool state=true );
      inline  void                         setDisabled       ( bool state=true );
      inline  void                         setMinimized      ( bool state=true );
      inline  void                         setRipedByLocal   ( bool state=true );
              void                         setEventLevel     ( unsigned int );
      inline  void                         setTracksFree     ( unsigned int );
      inline  void                         setForcedToHint   ( bool state = true );
              void                         setAxisHint       ( DbU::Unit );
              void                         cacheAxisHint     ();
      inline  void                         setOptimalAxis    ( Interval );
      inline  void                         incInsertState    ();
      inline  void                         resetInsertState  ();
              void                         _processNegociate ( RoutingEventQueue&, RoutingEventHistory& );
              void                         _processPacking   ( RoutingEventQueue&, RoutingEventHistory& );
              Record*                      _getRecord        () const;
              string                       _getString        () const;
              string                       _getTypeName      () const;
    private:                                                 
                                           RoutingEvent      ( TrackElement*, unsigned int mode );
                                          ~RoutingEvent      ();

    protected:
    // Attributes.
      static size_t         _allocateds;
      static size_t         _processeds;
      static size_t         _cloneds;
      mutable bool          _cloned;
      bool                  _processed;
      bool                  _disabled;
      bool                  _valid;
      bool                  _validConstraints;
      bool                  _validPerpandiculars;
      bool                  _canHandleConstraints;
      bool                  _minimized;
      bool                  _forceToHint;
      bool                  _ripedByLocal;
      TrackElement*         _segment;
      DbU::Unit             _axisHistory;
      DbU::Unit             _axisHint;
      Interval              _optimalAxis;
      Interval              _constraints;
      Interval              _optimal;
      Interval              _perpandicular;
      Katabatic::GCell*     _shearGCell;
      unsigned int          _tracksNb        : 6;
      unsigned int          _tracksFree      : 4;
      unsigned int          _insertState     : 6;
      unsigned int          _mode            : 4;
      unsigned int          _rippleState     : 4;
      unsigned int          _eventLevel;
      float                 _priority;
      vector<TrackElement*> _perpandiculars;
      Key                   _key;
  };


// Inline Functions.
  inline bool                          RoutingEvent::isCloned                () const { return _cloned; }
  inline bool                          RoutingEvent::isValid                 () const { return _valid; }
  inline bool                          RoutingEvent::isProcessed             () const { return _processed; }
  inline bool                          RoutingEvent::isDisabled              () const { return _disabled; }
  inline bool                          RoutingEvent::isForcedToHint          () const { return _forceToHint; }
  inline bool                          RoutingEvent::isSheared               () const { return (_shearGCell != NULL); }
  inline bool                          RoutingEvent::isRipedByLocal          () const { return _ripedByLocal; }
  inline bool                          RoutingEvent::getMode                 () const { return _mode; }
  inline bool                          RoutingEvent::canMinimize             () const { return !_minimized; }
  inline const RoutingEvent::Key&      RoutingEvent::getKey                  () const { return _key; }
  inline TrackElement*                 RoutingEvent::getSegment              () const { return _segment; }
  inline const vector<TrackElement*>&  RoutingEvent::getPerpandiculars       () const { return _perpandiculars; }
  inline DbU::Unit                     RoutingEvent::getAxisHistory          () const { return _axisHistory; }
  inline DbU::Unit                     RoutingEvent::getAxisHint             () const { return _axisHint; }
  inline long                          RoutingEvent::getAxisWeight           ( DbU::Unit axis ) const { return abs(axis - getAxisHint()); }
  inline const Interval&               RoutingEvent::getOptimalAxis          () const { return _optimalAxis; }
  inline const Interval&               RoutingEvent::getConstraints          () const { return _constraints; }
  inline const Interval&               RoutingEvent::getOptimal              () const { return _optimal; }
  inline const Interval&               RoutingEvent::getPerpandicular        () const { return _perpandicular; }
  inline float                         RoutingEvent::getPriority             () const { return _priority; }
  inline unsigned int                  RoutingEvent::getEventLevel           () const { return _eventLevel; }
  inline unsigned int                  RoutingEvent::getTracksNb             () const { return _tracksNb; }
  inline unsigned int                  RoutingEvent::getTracksFree           () const { return _tracksFree; }
  inline unsigned int                  RoutingEvent::getInsertState          () const { return _insertState; }
  inline Katabatic::GCell*             RoutingEvent::getShearGCell           () const { return _shearGCell; }
  inline void                          RoutingEvent::setMode                 ( unsigned int mode ) { _mode = mode; }
  inline void                          RoutingEvent::setProcessed            ( bool state ) { _processed  = state; }
  inline void                          RoutingEvent::setDisabled             ( bool state ) { _disabled = state; }
  inline void                          RoutingEvent::setMinimized            ( bool state ) { _minimized = state; }
  inline void                          RoutingEvent::setRipedByLocal         ( bool state ) { _ripedByLocal = state; }
  inline void                          RoutingEvent::setTracksFree           ( unsigned int nb ) { _tracksFree = nb; }
  inline void                          RoutingEvent::setForcedToHint         ( bool state ) { _forceToHint = state; }
  inline void                          RoutingEvent::setOptimalAxis          ( Interval i ) { _optimalAxis = i; }
  inline void                          RoutingEvent::incInsertState          () { _insertState++; }
  inline void                          RoutingEvent::resetInsertState        () { _insertState = 0; }
  inline void                          RoutingEvent::updateKey               () { revalidate(); _key.update(this); }

  inline void  RoutingEvent::invalidate ( bool withPerpandiculars, bool withConstraints )
  {
    _valid = false;
    _validPerpandiculars = _validPerpandiculars and !withPerpandiculars;
    _validConstraints    = _validConstraints    and !withConstraints;
  }


// -------------------------------------------------------------------
// Macros.


# if !defined(NDEBUG)
#   define _preCheck(segment)                                               \
  DbU::Unit  beforeMin = segment->base()->getSourcePosition();              \
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


} // End of Kite namespace.


INSPECTOR_P_SUPPORT(Kite::RoutingEvent);


# endif
