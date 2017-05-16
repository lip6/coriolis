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
// |  C++ Header  :  "./katana/TrackElement.h"                       |
// +-----------------------------------------------------------------+


#ifndef  KATANA_TRACK_ELEMENT_H
#define  KATANA_TRACK_ELEMENT_H

#include  <string>
#include  <map>

#include  "hurricane/Interval.h"
#include  "hurricane/Observer.h"
namespace Hurricane {
  class Record;
  class Net;
  class Layer;
}

#include  "anabatic/AutoSegment.h"
#include  "katana/Constants.h"
#include  "katana/Session.h"
#include  "katana/TrackElements.h"


namespace Katana {

  using std::string;
  using std::map;
  using Hurricane::Observer;
  using Hurricane::Record;
  using Hurricane::Interval;
  using Hurricane::DbU;
  using Hurricane::Box;
  using Hurricane::Net;
  using Hurricane::Layer;
  using Anabatic::GCell;
  using Anabatic::AutoSegment;

  class DataNegociate;
  class Track;
  class TrackCost;


  typedef  map<Segment*,TrackElement*>  TrackElementLut;
  typedef  void  (SegmentOverlapCostCB)( const TrackElement*, TrackCost& );


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
      static  SegmentOverlapCostCB*   setOverlapCostCB     ( SegmentOverlapCostCB* );
      static  void                    notify               ( TrackElement*, unsigned int flags );
    public:                                                
              void                    destroy              ();
      virtual AutoSegment*            base                 () const;
      virtual Segment*                getSegment           () const = 0;
    // Wrapped AutoSegment Functions (when applicable).
      virtual bool                    isFixed              () const;
      virtual bool                    isHorizontal         () const = 0;
      virtual bool                    isVertical           () const = 0;
      virtual bool                    isLocal              () const;
      virtual bool                    isGlobal             () const;
      virtual bool                    isBipoint            () const;
      virtual bool                    isTerminal           () const;
      virtual bool                    isStrongTerminal     ( Flags flags=Flags::NoFlags ) const;
      virtual bool                    isStrap              () const;
      virtual bool                    isSlackened          () const;
      virtual bool                    isDogleg             () const;
      virtual bool                    isReduced            () const;
      virtual bool                    isUTurn              () const;
      virtual bool                    isUserDefined        () const;
    // Predicates.                    
      inline  bool                    isCreated            () const;
      inline  bool                    isInvalidated        () const;
      inline  bool                    isBlockage           () const;
      inline  bool                    isLocked             () const;
      inline  bool                    isRouted             () const;
      virtual bool                    hasSymmetric         () const;
      inline  bool                    hasSourceDogleg      () const;
      inline  bool                    hasTargetDogleg      () const;
      inline  bool                    canRipple            () const;
      virtual bool                    canSlacken           () const;
      virtual bool                    canPivotUp           ( float reserve, Flags flags ) const;
      virtual bool                    canPivotDown         ( float reserve, Flags flags ) const;
      virtual bool                    canMoveUp            ( float reserve, Flags flags=Flags::WithPerpands ) const;
      virtual bool                    canDogleg            ();
      virtual bool                    canDogleg            ( Interval );
      virtual bool                    canDogleg            ( Anabatic::GCell*, Flags flags=0 );
    // Accessors
      inline  Observer<TrackElement>* getObserver          ();
      virtual unsigned long           getId                () const;
      virtual Flags                   getDirection         () const = 0;
      virtual Net*                    getNet               () const = 0;
      virtual const Layer*            getLayer             () const = 0;
      virtual DbU::Unit               getPitch             () const;
      virtual DbU::Unit               getPPitch            () const;
      inline  Track*                  getTrack             () const;
      inline  size_t                  getIndex             () const;
      virtual unsigned long           getFreedomDegree     () const;
      virtual float                   getMaxUnderDensity   ( Flags flags=Flags::NoFlags ) const;
      inline  Box                     getBoundingBox       () const;
      virtual TrackElement*           getNext              () const;
      virtual TrackElement*           getPrevious          () const;
      virtual DbU::Unit               getAxis              () const = 0;
      inline  DbU::Unit               getSourceU           () const;
      inline  DbU::Unit               getTargetU           () const;
      inline  DbU::Unit               getLength            () const;
      inline  Interval                getCanonicalInterval () const;
      virtual Interval                getFreeInterval      () const;
      virtual Interval                getSourceConstraints () const;
      virtual Interval                getTargetConstraints () const;
      virtual DataNegociate*          getDataNegociate     ( Flags flags=Flags::DataSelf ) const;
      virtual TrackElement*           getCanonical         ( Interval& );
      virtual size_t                  getGCells            ( vector<GCell*>& ) const;
      virtual TrackElement*           getParent            () const;
      virtual uint32_t                getDoglegLevel       () const;
      virtual TrackElement*           getSourceDogleg      ();
      virtual TrackElement*           getTargetDogleg      ();
      virtual TrackElement*           getSymmetric         ();
      virtual TrackElements           getPerpandiculars    ();
    // Mutators.                       
      inline  void                    setFlags             ( uint32_t );
      inline  void                    unsetFlags           ( uint32_t );
      inline  void                    setRouted            ();
      virtual void                    setTrack             ( Track* );
      inline  void                    setIndex             ( size_t );
      virtual void                    setSymmetric         ( TrackElement* );
      virtual void                    updateFreedomDegree  ();
      virtual void                    setDoglegLevel       ( uint32_t );
      virtual void                    swapTrack            ( TrackElement* );
      virtual void                    reschedule           ( uint32_t level );
      virtual void                    detach               ();
      virtual void                    invalidate           ();
      virtual void                    revalidate           ();
      virtual void                    updatePPitch         ();
      virtual void                    incOverlapCost       ( Net*, TrackCost& ) const;
      virtual void                    setAxis              ( DbU::Unit, uint32_t flags=Anabatic::SegAxisSet );
      virtual TrackElement*           makeDogleg           ();
      inline  bool                    makeDogleg           ( Anabatic::GCell* );
      virtual TrackElement*           makeDogleg           ( Anabatic::GCell*, TrackElement*& perpandicular, TrackElement*& parallel );
      virtual TrackElement*           makeDogleg           ( Interval, Flags& flags );
      virtual void                    _postDoglegs         ( TrackElement*& perpandicular, TrackElement*& parallel );
      virtual bool                    moveAside            ( Flags flags );
      virtual bool                    slacken              ( Flags flags=Flags::NoFlags );
      virtual bool                    moveUp               ( Flags flags );
      virtual bool                    moveDown             ( Flags flags );
#if THIS_IS_DISABLED                  
      virtual void                    desalignate          ();
#endif                                
      virtual bool                    _check               () const;
      virtual Record*                 _getRecord           () const;
      virtual string                  _getString           () const;
      virtual string                  _getTypeName         () const;

    protected:
    // Static Attributes.
      static SegmentOverlapCostCB*    _overlapCostCallback;
    // Attributes.                   
             uint32_t                _flags;
             Track*                  _track;
             size_t                  _index;
             DbU::Unit               _sourceU;
             DbU::Unit               _targetU;
             Observer<TrackElement>  _observer;

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
  inline Observer<TrackElement>* TrackElement::getObserver          () { return &_observer; }
  inline void                    TrackElement::setFlags             ( uint32_t flags ) { _flags|= flags; }
  inline void                    TrackElement::unsetFlags           ( uint32_t flags ) { _flags&=~flags; }
  inline bool                    TrackElement::isCreated            () const { return _flags & TElemCreated; }
  inline bool                    TrackElement::isInvalidated        () const { return _flags & TElemInvalidated; }
  inline bool                    TrackElement::isBlockage           () const { return _flags & TElemBlockage; }
  inline bool                    TrackElement::isLocked             () const { return _flags & TElemLocked; }
  inline bool                    TrackElement::isRouted             () const { return _flags & TElemRouted; }
  inline bool                    TrackElement::hasSourceDogleg      () const { return _flags & TElemSourceDogleg; }
  inline bool                    TrackElement::hasTargetDogleg      () const { return _flags & TElemTargetDogleg; }
  inline bool                    TrackElement::canRipple            () const { return _flags & TElemRipple; }
  inline Track*                  TrackElement::getTrack             () const { return _track; }
  inline size_t                  TrackElement::getIndex             () const { return _index; }
  inline DbU::Unit               TrackElement::getLength            () const { return getTargetU() - getSourceU(); }
  inline DbU::Unit               TrackElement::getSourceU           () const { return _sourceU; }
  inline DbU::Unit               TrackElement::getTargetU           () const { return _targetU; }
  inline Interval                TrackElement::getCanonicalInterval () const { return Interval(getSourceU(),getTargetU()); }
  inline void                    TrackElement::setIndex             ( size_t index ) { _index=index; }

  inline void  TrackElement::setRouted()
  {
    _flags |= TElemRouted;
    if (base()) base()->setFlags( Anabatic::SegFixed );
  }

  inline Box  TrackElement::getBoundingBox () const
  {
    if (getDirection() & Flags::Horizontal)
      return Box( getSourceU(), getAxis()-DbU::lambda(1.0), getTargetU(), getAxis()+DbU::lambda(1.0) );
    return Box( getAxis()-DbU::lambda(1.0), getSourceU(), getAxis()+DbU::lambda(1.0), getTargetU() );
  }

  inline  bool  TrackElement::makeDogleg ( Anabatic::GCell* gcell )
  {
    TrackElement* perpandicular = NULL;
    TrackElement* parallel      = NULL;

    makeDogleg( gcell, perpandicular, parallel );

    return (perpandicular != NULL);
  }


}  // Katana namespace.


INSPECTOR_P_SUPPORT(Katana::TrackElement);

#endif
