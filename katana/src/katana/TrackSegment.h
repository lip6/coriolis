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
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katana/TrackSegment.h"                       |
// +-----------------------------------------------------------------+


#pragma  once
#include <set>
#include <functional>
#include "katana/TrackElement.h"


namespace Katana {

  using std::string;
  using std::map;
  using std::set;
  using std::binary_function;
  using Hurricane::Record;
  using Hurricane::Interval;
  using Hurricane::DbU;
  using Hurricane::Net;
  using Hurricane::Layer;
  using Anabatic::AutoSegment;

  class DataNegociate;
  class Track;
  class TrackCost;


// -------------------------------------------------------------------
// Class  :  "TrackSegment".

  enum TrackSegmentFlag { NoFlags        = 0
                        , PriorityLocked = (1 << 0)
                        };


  class TrackSegment : public TrackElement {
    public:
      class CompareById : public binary_function<const TrackSegment*,const TrackSegment*,bool> {
        public:
          inline bool  operator() ( const TrackSegment* lhs, const TrackSegment* rhs ) const;
      };

    public:
      static  TrackElement*         create                 ( AutoSegment*, Track*, bool& created );
      static  size_t                getAllocateds          ();
    public:                                                
    // Wrapped AutoSegment Functions (when applicable).
      virtual AutoSegment*          base                   () const;
      virtual Segment*              getSegment             () const;
      virtual bool                  isFixed                () const;
      virtual bool                  isFixedAxis            () const;
      virtual bool                  isHorizontal           () const;
      virtual bool                  isVertical             () const;
      virtual bool                  isLocal                () const;
      virtual bool                  isGlobal               () const;
      virtual bool                  isWeakGlobal           () const;
      virtual bool                  isBipoint              () const;
      virtual bool                  isTerminal             () const;
      virtual bool                  isDrag                 () const;
      virtual bool                  isStrongTerminal       ( Flags flags=Flags::NoFlags ) const;
      virtual bool                  isStrap                () const;
      virtual bool                  isUnbreakable          () const;
      virtual bool                  isSlackened            () const;
      virtual bool                  isDogleg               () const;
      virtual bool                  isShortDogleg          () const;
      virtual bool                  isReduced              () const;
      virtual bool                  isUTurn                () const;
      virtual bool                  isUserDefined          () const;
      virtual bool                  isAnalog               () const;
      virtual bool                  isWide                 () const;
      virtual bool                  isShortNet             () const;
      virtual bool                  isNonPrefOnVSmall      () const;
      virtual bool                  isPriorityLocked       () const;
    // Predicates.
      virtual bool                  hasSymmetric           () const;
      virtual bool                  canDogleg              ();
      virtual bool                  canDogleg              ( Interval );
      virtual bool                  canDogleg              ( Anabatic::GCell*, Flags flags=0 );
      virtual bool                  canPivotUp             ( float reserve, Flags ) const;
      virtual bool                  canPivotDown           ( float reserve, Flags ) const;
      virtual bool                  canMoveUp              ( float reserve, Flags ) const;
      virtual bool                  canSlacken             () const;
      virtual bool                  canRealign             () const;
      virtual float                 getMaxUnderDensity     ( Flags ) const;
      virtual unsigned long         getId                  () const;
      virtual Flags                 getDirection           () const;
      virtual Net*                  getNet                 () const;
      virtual DbU::Unit             getWidth               () const;
      virtual const Layer*          getLayer               () const;
      virtual unsigned int          getDepth               () const;
      virtual DbU::Unit             getPitch               () const;
      virtual DbU::Unit             getPPitch              () const;
      virtual DbU::Unit             getExtensionCap        ( Flags ) const;
      virtual unsigned long         getFreedomDegree       () const;
      virtual float                 getPriority            () const;
      virtual uint32_t              getDoglegLevel         () const;
      virtual TrackElement*         getNext                () const;
      virtual TrackElement*         getPrevious            () const;
      virtual TrackElement*         getParent              () const;
      virtual DbU::Unit             getAxis                () const;
      virtual DbU::Unit             getSourceAxis          () const;
      virtual DbU::Unit             getTargetAxis          () const;
      virtual Interval              getFreeInterval        () const;
      virtual Interval              getSourceConstraints   () const;
      virtual Interval              getTargetConstraints   () const;
      virtual DataNegociate*        getDataNegociate       ( Flags flags=Flags::DataSelf ) const;
      virtual size_t                getGCells              ( vector<GCell*>& ) const;
      virtual TrackElement*         getSourceDogleg        ();
      virtual TrackElement*         getTargetDogleg        ();
      virtual TrackElement*         getSymmetric           ();
      virtual TrackElements         getPerpandiculars      ();
      virtual size_t                getPerpandicularsBound ( set<TrackElement*>& );
    // Mutators.
      virtual void                  setTrack               ( Track* );
      virtual void                  setSymmetric           ( TrackElement* );
      virtual void                  setPriorityLock        ( bool state );
      virtual void                  forcePriority          ( float );
      virtual void                  computePriority        ();
      virtual void                  computeAlignedPriority ();
      virtual void                  updateFreedomDegree    ();
      virtual void                  setDoglegLevel         ( uint32_t );
      virtual void                  swapTrack              ( TrackElement* );
      virtual void                  reschedule             ( uint32_t level );
    //virtual void                  detach                 ();
      virtual void                  detach                 ( TrackSet& );
      virtual void                  invalidate             ();
      virtual void                  revalidate             ();
      virtual void                  updatePPitch           ();
      virtual void                  setAxis                ( DbU::Unit, uint32_t flags );
      virtual TrackElement*         makeDogleg             ();
      virtual TrackElement*         makeDogleg             ( Anabatic::GCell*, TrackElement*& perpandicular, TrackElement*& parallel );
      virtual Flags                 makeDogleg             ( Interval, TrackElement*& perpandicular, TrackElement*& parallel, Flags flags );
      virtual void                  _postDoglegs           ( TrackElement*& perpandicular, TrackElement*& parallel );
      virtual bool                  moveAside              ( Flags );
      virtual bool                  slacken                ( Flags flags=Flags::NoFlags );
      virtual bool                  moveUp                 ( Flags );
      virtual bool                  moveDown               ( Flags );
#if THIS_IS_DISABLED
      virtual void                  desalignate            ();
#endif
      virtual bool                  _check                 () const;
      virtual Record*               _getRecord             () const;
      virtual string                _getString             () const;
      virtual string                _getTypeName           () const;

    protected:
    // Attributes.
      static size_t         _allocateds;
             AutoSegment*   _base;
             TrackSegment*  _symmetric;
             unsigned long  _freedomDegree;
             DbU::Unit      _ppitch;
             DataNegociate* _data;
             float          _priority;
             unsigned int   _dogLegLevel:4;
             uint32_t       _flags; 

    protected:
    // Constructors & Destructors.
                            TrackSegment ( AutoSegment*, Track* ) ;
      virtual              ~TrackSegment ();
      virtual void          _postCreate  ();
      virtual void          _preDestroy  ();
    private:
                            TrackSegment ( const TrackSegment& );
              TrackSegment& operator=    ( const TrackSegment& );
      
  };


  inline bool  TrackSegment::CompareById::operator() ( const TrackSegment* lhs, const TrackSegment* rhs ) const
  { return lhs->getId() < rhs->getId(); }


  typedef  set<TrackSegment*,TrackSegment::CompareById>  TrackSegmentSet;


}  // Katana namespace.


INSPECTOR_P_SUPPORT(Katana::TrackSegment);
