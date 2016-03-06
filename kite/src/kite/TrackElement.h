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
// |  C++ Header  :  "./kite/TrackElement.h"                         |
// +-----------------------------------------------------------------+


#ifndef  KITE_TRACK_ELEMENT_H
#define  KITE_TRACK_ELEMENT_H

#include  <string>
#include  <map>

#include  "hurricane/Interval.h"
namespace Hurricane {
  class Record;
  class Net;
  class Layer;
}

#include  "katabatic/AutoSegment.h"
#include  "kite/Constants.h"
#include  "kite/Session.h"
#include  "kite/TrackElements.h"


namespace Kite {

  using std::string;
  using std::map;
  using Hurricane::Record;
  using Hurricane::Interval;
  using Hurricane::DbU;
  using Hurricane::Box;
  using Hurricane::Net;
  using Hurricane::Layer;
  using Katabatic::AutoSegment;
  using Katabatic::Observer;
  using Katabatic::KbNoFlags;

  class DataNegociate;
  class Track;
  class TrackCost;


  typedef  map<Segment*,TrackElement*>  TrackElementLut;
  typedef  void  (SegmentOverlapCostCB)( const TrackElement*, TrackCost& );


// -------------------------------------------------------------------
// Class  :  "SegmentObserver".

  class SegmentObserver : public Observer<TrackElement> {
    public:
      inline        SegmentObserver ( TrackElement* );
      virtual void  notify          ( unsigned int flags );
    private:
                    SegmentObserver ( const SegmentObserver& );
  };


  inline  SegmentObserver::SegmentObserver ( TrackElement* owner )
    : Observer<TrackElement>(owner)
  { }


// -------------------------------------------------------------------
// Class  :  "TrackElement".

  enum TrackElementFlags { TElemCreated     =0x00000001
                         , TElemBlockage    =0x00000002
                         , TElemFixed       =0x00000004
                         , TElemLocked      =0x00000008
                         , TElemRouted      =0x00000010
                         , TElemSourceDogleg=0x00000020
                         , TElemTargetDogleg=0x00000040
                         , TElemRipple      =0x00000080
                         , TElemInvalidated =0x00000100
                         };


  struct Compare {
      bool  operator() ( TrackElement* lhs, TrackElement* rhs );
  };

  struct CompareByPosition {
      bool  operator() ( const TrackElement* lhs, const TrackElement* rhs ) const;
  };


  class TrackElement {

    public:
      static  SegmentOverlapCostCB*  setOverlapCostCB     ( SegmentOverlapCostCB* );
    public:                                               
              void                   destroy              ();
      virtual AutoSegment*           base                 () const;
    // Wrapped AutoSegment Functions (when applicable).
      virtual bool                   isFixed              () const;
      virtual bool                   isHorizontal         () const = 0;
      virtual bool                   isVertical           () const = 0;
      virtual bool                   isLocal              () const;
      virtual bool                   isGlobal             () const;
      virtual bool                   isBipoint            () const;
      virtual bool                   isTerminal           () const;
      virtual bool                   isStrongTerminal     ( unsigned int flags=0 ) const;
      virtual bool                   isStrap              () const;
      virtual bool                   isSlackened          () const;
      virtual bool                   isDogleg             () const;
      virtual bool                   isReduced            () const;
      virtual bool                   isUTurn              () const;
      virtual bool                   isUserDefined        () const;
    // Predicates.                   
      inline  bool                   isCreated            () const;
      inline  bool                   isInvalidated        () const;
      inline  bool                   isBlockage           () const;
      inline  bool                   isLocked             () const;
      inline  bool                   isRouted             () const;
      inline  bool                   hasSourceDogleg      () const;
      inline  bool                   hasTargetDogleg      () const;
      inline  bool                   canRipple            () const;
      virtual bool                   canSlacken           () const;
      virtual bool                   canPivotUp           ( float reserve ) const;
      virtual bool                   canPivotDown         ( float reserve ) const;
      virtual bool                   canMoveUp            ( float reserve, unsigned int flags=KbWithPerpands ) const;
      virtual bool                   canDogleg            ();
      virtual bool                   canDogleg            ( Interval );
      virtual bool                   canDogleg            ( Katabatic::GCell*, unsigned int flags=0 );
    // Accessors
      inline  SegmentObserver*       getObserver          ();
      virtual unsigned long          getId                () const;
      virtual unsigned int           getDirection         () const = 0;
      virtual Net*                   getNet               () const = 0;
      virtual const Layer*           getLayer             () const = 0;
      virtual DbU::Unit              getPitch             () const;
      virtual DbU::Unit              getPPitch            () const;
      inline  Track*                 getTrack             () const;
      inline  size_t                 getIndex             () const;
      virtual unsigned long          getFreedomDegree     () const;
      virtual float                  getMaxUnderDensity   ( unsigned int flags=0 ) const;
      inline  Box                    getBoundingBox       () const;
      virtual TrackElement*          getNext              () const;
      virtual TrackElement*          getPrevious          () const;
      virtual DbU::Unit              getAxis              () const = 0;
      inline  DbU::Unit              getSourceU           () const;
      inline  DbU::Unit              getTargetU           () const;
      inline  DbU::Unit              getLength            () const;
      inline  Interval               getCanonicalInterval () const;
      virtual Interval               getFreeInterval      () const;
      virtual Interval               getSourceConstraints () const;
      virtual Interval               getTargetConstraints () const;
      virtual DataNegociate*         getDataNegociate     ( unsigned int flags=KtDataSelf ) const;
      virtual TrackElement*          getCanonical         ( Interval& );
      virtual size_t                 getGCells            ( Katabatic::GCellVector& ) const;
      virtual TrackElement*          getParent            () const;
      virtual unsigned int           getDoglegLevel       () const;
      virtual TrackElement*          getSourceDogleg      ();
      virtual TrackElement*          getTargetDogleg      ();
      virtual TrackElements          getPerpandiculars    ();
    // Mutators.                     
      inline  void                   setFlags             ( unsigned int );
      inline  void                   unsetFlags           ( unsigned int );
      inline  void                   setRouted            ();
      virtual void                   setTrack             ( Track* );
      inline  void                   setIndex             ( size_t );
      virtual void                   updateFreedomDegree  ();
      virtual void                   setDoglegLevel       ( unsigned int );
      virtual void                   swapTrack            ( TrackElement* );
      virtual void                   reschedule           ( unsigned int level );
      virtual void                   detach               ();
      virtual void                   invalidate           ();
      virtual void                   revalidate           ();
      virtual void                   updatePPitch         ();
      virtual void                   incOverlapCost       ( Net*, TrackCost& ) const;
      virtual void                   setAxis              ( DbU::Unit, unsigned int flags=Katabatic::SegAxisSet );
      virtual TrackElement*          makeDogleg           ();
      inline  bool                   makeDogleg           ( Katabatic::GCell* );
      virtual TrackElement*          makeDogleg           ( Katabatic::GCell*, TrackElement*& perpandicular, TrackElement*& parallel );
      virtual TrackElement*          makeDogleg           ( Interval, unsigned int& flags );
      virtual void                   _postDoglegs         ( TrackElement*& perpandicular, TrackElement*& parallel );
      virtual bool                   moveAside            ( unsigned int flags );
      virtual bool                   slacken              ( unsigned int flags=KbNoFlags );
      virtual bool                   moveUp               ( unsigned int flags );
      virtual bool                   moveDown             ( unsigned int flags );
#if THIS_IS_DISABLED                 
      virtual void                   desalignate          ();
#endif                               
      virtual bool                   _check               () const;
      virtual Record*                _getRecord           () const;
      virtual string                 _getString           () const;
      virtual string                 _getTypeName         () const;

    protected:
    // Static Attributes.
      static SegmentOverlapCostCB*   _overlapCostCallback;
    // Attributes.                   
             unsigned int     _flags;
             Track*           _track;
             size_t           _index;
             DbU::Unit        _sourceU;
             DbU::Unit        _targetU;
             SegmentObserver  _observer;

    protected:
    // Constructors & Destructors.
                            TrackElement ( Track* ) ;
      virtual              ~TrackElement ();
      virtual void          _postCreate  ();
      virtual void          _preDestroy  ();
    private:
                            TrackElement ( const TrackElement& );
              TrackElement& operator=    ( const TrackElement& );
      
  };


// Inline functions.
  inline SegmentObserver* TrackElement::getObserver          () { return &_observer; }
  inline void             TrackElement::setFlags             ( unsigned int flags ) { _flags|= flags; }
  inline void             TrackElement::unsetFlags           ( unsigned int flags ) { _flags&=~flags; }
  inline bool             TrackElement::isCreated            () const { return _flags & TElemCreated; }
  inline bool             TrackElement::isInvalidated        () const { return _flags & TElemInvalidated; }
  inline bool             TrackElement::isBlockage           () const { return _flags & TElemBlockage; }
  inline bool             TrackElement::isLocked             () const { return _flags & TElemLocked; }
  inline bool             TrackElement::isRouted             () const { return _flags & TElemRouted; }
  inline bool             TrackElement::hasSourceDogleg      () const { return _flags & TElemSourceDogleg; }
  inline bool             TrackElement::hasTargetDogleg      () const { return _flags & TElemTargetDogleg; }
  inline bool             TrackElement::canRipple            () const { return _flags & TElemRipple; }
  inline Track*           TrackElement::getTrack             () const { return _track; }
  inline size_t           TrackElement::getIndex             () const { return _index; }
  inline DbU::Unit        TrackElement::getLength            () const { return getTargetU() - getSourceU(); }
  inline DbU::Unit        TrackElement::getSourceU           () const { return _sourceU; }
  inline DbU::Unit        TrackElement::getTargetU           () const { return _targetU; }
  inline Interval         TrackElement::getCanonicalInterval () const { return Interval(getSourceU(),getTargetU()); }
  inline void             TrackElement::setIndex             ( size_t index ) { _index=index; }

  inline void  TrackElement::setRouted()
  {
    _flags |= TElemRouted;
    if (base()) base()->setFlags( Katabatic::SegFixed );
  }

  inline Box  TrackElement::getBoundingBox () const
  {
    if (getDirection() == KbHorizontal)
      return Box( getSourceU(), getAxis()-DbU::lambda(1.0), getTargetU(), getAxis()+DbU::lambda(1.0) );
    return Box( getAxis()-DbU::lambda(1.0), getSourceU(), getAxis()+DbU::lambda(1.0), getTargetU() );
  }

  inline  bool  TrackElement::makeDogleg ( Katabatic::GCell* gcell )
  {
    TrackElement* perpandicular = NULL;
    TrackElement* parallel      = NULL;

    makeDogleg( gcell, perpandicular, parallel );

    return (perpandicular != NULL);
  }


}  // Kite namespace.


INSPECTOR_P_SUPPORT(Kite::TrackElement);

#endif
