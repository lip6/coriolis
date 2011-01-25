

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
// |  C++ Header  :       "./TrackElement.h"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#ifndef  __KITE_TRACK_ELEMENT__
#define  __KITE_TRACK_ELEMENT__


#include  <string>
#include  <map>

#include  "hurricane/Interval.h"
namespace Hurricane {
  class Record;
  class Net;
  class Layer;
}

#include  "katabatic/AutoSegment.h"
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

  class DataNegociate;
  class Track;
  class TrackCost;


  typedef  map<Segment*,TrackElement*>  TrackElementLut;
  typedef  void  (SegmentOverlapCostCB)( const TrackElement*, TrackCost& );


// -------------------------------------------------------------------
// Class  :  "TrackElement".
 

  class TrackElement {

    public:
      enum Flags { AddToGCells     =0x01
                 , RemoveFromGCells=0x02
                 , UnRouted        =0x04
                 , Routed          =0x08
                 , AllowDoglegReuse=0x10
                 , DataSelf        =0x01
                 , DataParent      =0x02
                 };

    public:
    // Sub-Class:  "Compare()".
      class Compare {
        public:
          bool  operator() ( TrackElement* lhs, TrackElement* rhs );
      };
    public:
    // Sub-Class:  "CompareByPosition()".
      struct CompareByPosition {
          bool  operator() ( const TrackElement* lhs, const TrackElement* rhs ) const;
      };

    friend class Compare;


    public:
      static  SegmentOverlapCostCB* setOverlapCostCB           ( SegmentOverlapCostCB* );
    public:                                                    
              void                  destroy                    ();
      virtual AutoSegment*          base                       () const = 0;
      virtual bool                  isCreated                  () const;
      virtual bool                  isBlockage                 () const;
      virtual bool                  isFixed                    () const;
      virtual bool                  isStrap                    () const;
      virtual bool                  isSlackenStrap             () const;
      virtual bool                  isLocal                    () const;
      virtual bool                  isGlobal                   () const;
      virtual bool                  isLocked                   () const;
      virtual bool                  isTerminal                 () const;
      virtual bool                  isDogleg                   () const;
      virtual bool                  isRevalidated              () const;
      virtual bool                  isRouted                   () const;
      virtual bool                  isSlackened                () const;
      virtual bool                  isSlackenDogLeg            () const;
      virtual bool                  isHorizontal               () const = 0;
      virtual bool                  isVertical                 () const = 0;
      virtual bool                  isBipoint                  () const;
      virtual bool                  allowOutsideGCell          () const;
      virtual bool                  canDesalignate             () const;
      virtual bool                  canGoOutsideGCell          () const;
      virtual bool                  canSlacken                 () const;
      virtual bool                  canPivotUp                 ( float reserve ) const;
      virtual bool                  canPivotDown               ( float reserve ) const;
      virtual bool                  canMoveUp                  ( float reserve, unsigned int flags=AutoSegment::Propagate|AutoSegment::PerpandicularFrag ) const;
      virtual float                 getMaxUnderDensity         ( unsigned int flags=AutoSegment::Propagate ) const;
      virtual bool                  canRipple                  () const;
      virtual bool                  hasSourceDogLeg            () const;
      virtual bool                  hasTargetDogLeg            () const;
      virtual bool                  canDogLeg                  ();
      virtual bool                  canDogLeg                  ( Interval );
      virtual bool                  canDogLegAt                ( Katabatic::GCell*, unsigned int flags=0 );
      virtual unsigned long         getId                      () const;
      virtual unsigned int          getDirection               () const = 0;
      virtual Net*                  getNet                     () const = 0;
      virtual const Layer*          getLayer                   () const = 0;
      inline  Track*                getTrack                   () const;
      inline  size_t                getIndex                   () const;
      virtual unsigned long         getArea                    () const;
      inline  Box                   getBoundingBox             () const;
      virtual unsigned int          getDogLegLevel             () const;
      virtual unsigned int          getDogLegOrder             () const;
      virtual TrackElement*         getNext                    () const;
      virtual TrackElement*         getPrevious                () const;
      virtual TrackElement*         getParent                  () const;
      virtual DbU::Unit             getAxis                    () const = 0;
      inline  DbU::Unit             getSourceU                 () const;
      inline  DbU::Unit             getTargetU                 () const;
      inline  DbU::Unit             getLength                  () const;
      virtual Interval              getFreeInterval            () const;
      inline  Interval              getCanonicalInterval       () const;
      virtual Interval              getSourceConstraints       () const;
      virtual Interval              getTargetConstraints       () const;
      virtual DataNegociate*        getDataNegociate           ( unsigned int flags=DataSelf ) const;
      virtual TrackElement*         getCanonical               ( Interval& );
      virtual size_t                getGCells                  ( Katabatic::GCellVector& ) const;
      virtual TrackElement*         getSourceDogLeg            ();
      virtual TrackElement*         getTargetDogLeg            ();
      virtual TrackElements         getCollapsedPerpandiculars ();
      virtual size_t                getPerpandicularsBound     ( set<TrackElement*>& );
      virtual unsigned int          getOrder                   () const;
      virtual void                  incOverlapCost             ( Net*, TrackCost& ) const;
      virtual void                  dataInvalidate             ();
      virtual void                  eventInvalidate            ();
      inline  void                  setSlackened               ( bool );
      virtual void                  setAllowOutsideGCell       ( bool );
      virtual void                  setRevalidated             ( bool );
      virtual void                  setCanRipple               ( bool );
      virtual void                  setSourceDogLeg            ( bool state=true );
      virtual void                  setTargetDogLeg            ( bool state=true );
      virtual void                  setLock                    ( bool );
      virtual void                  setRouted                  ( bool );
      virtual void                  setTrack                   ( Track* );
      inline  void                  setIndex                   ( size_t );
      virtual void                  setArea                    ();
      virtual void                  setDogLegLevel             ( unsigned int );
      virtual void                  setDogLegOrder             ( unsigned int );
      virtual void                  swapTrack                  ( TrackElement* );
      virtual void                  reschedule                 ( unsigned int level );
      virtual void                  detach                     ();
      virtual void                  revalidate                 ( bool invalidEvent=false );
      virtual void                  invalidate                 ();
      virtual void                  setAxis                    ( DbU::Unit, unsigned int flags=Katabatic::AxisSet );
      virtual void                  slacken                    ();
      virtual bool                  moveUp                     ( unsigned int flags=AutoSegment::Propagate );
      virtual bool                  moveDown                   ( unsigned int flags=AutoSegment::Propagate );
      virtual bool                  moveAside                  ( bool onLeft );
      virtual TrackElement*         makeDogLeg                 ();
      virtual TrackElement*         makeDogLeg                 ( Interval, bool& leftDogleg );
      virtual TrackElement*         makeDogLeg                 ( Katabatic::GCell* );
      virtual TrackElement*         _postDogLeg                ( Katabatic::GCell* );
      virtual void                  _postModify                ();
      virtual void                  desalignate                ();
      virtual bool                  _check                     () const;
      virtual Record*               _getRecord                 () const;
      virtual string                _getString                 () const;
      virtual string                _getTypeName               () const;

    protected:
    // Static Attributes.
      static SegmentOverlapCostCB* _overlapCostCallback;
    // Attributes.
             Track*                _track;
             size_t                _index;
             DbU::Unit             _sourceU;
             DbU::Unit             _targetU;

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
  inline Track*     TrackElement::getTrack             () const { return _track; }
  inline size_t     TrackElement::getIndex             () const { return _index; }
  inline DbU::Unit  TrackElement::getLength            () const { return getTargetU() - getSourceU(); }
  inline DbU::Unit  TrackElement::getSourceU           () const { return _sourceU; }
  inline DbU::Unit  TrackElement::getTargetU           () const { return _targetU; }
  inline Interval   TrackElement::getCanonicalInterval () const { return Interval(getSourceU(),getTargetU()); }
  inline void       TrackElement::setIndex             ( size_t index ) { _index = index; }
  inline void       TrackElement::setSlackened         ( bool state ) { if (base()) base()->setSlackened(state); };

  inline Box  TrackElement::getBoundingBox () const
  {
    if ( getDirection() == Constant::Horizontal )
      return Box ( getSourceU(), getAxis()-DbU::lambda(1.0), getTargetU(), getAxis()+DbU::lambda(1.0) );

    return Box ( getAxis()-DbU::lambda(1.0), getSourceU(), getAxis()+DbU::lambda(1.0), getTargetU() );
  }


} // End of Kite namespace.


INSPECTOR_P_SUPPORT(Kite::TrackElement);


# endif
