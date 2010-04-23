

// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
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
// |  C++ Header  :       "./TrackSegment.h"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#ifndef  __KITE_TRACK_SEGMENT__
#define  __KITE_TRACK_SEGMENT__


#include  "kite/TrackElement.h"


namespace Kite {


  using std::string;
  using std::map;
  using Hurricane::Record;
  using Hurricane::Interval;
  using Hurricane::DbU;
  using Hurricane::Net;
  using Hurricane::Layer;
  using Katabatic::AutoSegment;

  class DataNegociate;
  class Track;
  class TrackCost;
  class GCell;


// -------------------------------------------------------------------
// Class  :  "TrackSegment".
 

  class TrackSegment : public TrackElement {
    public:
      static  TrackElement*         create                     ( AutoSegment*, Track*, bool& created );
    public:                                                    
      virtual AutoSegment*          base                       () const;
      virtual bool                  isCreated                  () const;
      virtual bool                  isFixed                    () const;
      virtual bool                  isStrap                    () const;
      virtual bool                  isSlackenStrap             () const;
      virtual bool                  isLocal                    () const;
      virtual bool                  isGlobal                   () const;
      virtual bool                  isLocked                   () const;
      virtual bool                  isTerminal                 () const;
      virtual bool                  isRevalidated              () const;
      virtual bool                  isSlackened                () const;
      virtual bool                  isSlackenDogLeg            () const;
      virtual bool                  isHorizontal               () const;
      virtual bool                  isVertical                 () const;
      virtual bool                  isRouted                   () const;
      virtual bool                  allowOutsideGCell          () const;
      virtual bool                  canDesalignate             () const;
      virtual bool                  canGoOutsideGCell          () const;
      virtual bool                  canSlacken                 () const;
      virtual bool                  canPivotUp                 ( float reserve ) const;
      virtual bool                  canMoveUp                  ( float reserve ) const;
      virtual bool                  canRipple                  () const;
      virtual bool                  hasSourceDogLeg            () const;
      virtual bool                  hasTargetDogLeg            () const;
      virtual bool                  canDogLeg                  ();
      virtual bool                  canDogLeg                  ( Interval );
      virtual bool                  canDogLegAt                ( GCell*, bool allowReuse=false );
      virtual unsigned long         getId                      () const;
      virtual unsigned int          getDirection               () const;
      virtual Net*                  getNet                     () const;
      virtual const Layer*          getLayer                   () const;
      virtual unsigned long         getArea                    () const;
      virtual unsigned int          getDogLegLevel             () const;
      virtual unsigned int          getDogLegOrder             () const;
      virtual TrackElement*         getNext                    () const;
      virtual TrackElement*         getPrevious                () const;
      virtual DbU::Unit             getAxis                    () const;
      virtual Interval              getFreeInterval            ( bool useOrder=false ) const;
      virtual Interval              getSourceConstraints       () const;
      virtual Interval              getTargetConstraints       () const;
      virtual DataNegociate*        getDataNegociate           () const;
      virtual TrackElement*         getCanonical               ( Interval& );
      virtual GCell*                getGCell                   () const;
      virtual size_t                getGCells                  ( vector<GCell*>& ) const;
      virtual TrackElement*         getSourceDogLeg            ();
      virtual TrackElement*         getTargetDogLeg            ();
      virtual TrackElements         getCollapsedPerpandiculars ();
      virtual size_t                getPerpandicularsBound     ( set<TrackElement*>& );
      virtual unsigned int          getOrder                   () const;
      virtual void                  updateGCellsStiffness      ( unsigned int );
      virtual void                  dataInvalidate             ();
      virtual void                  eventInvalidate            ();
      virtual void                  setAllowOutsideGCell       ( bool );
      virtual void                  setRevalidated             ( bool );
      virtual void                  setCanRipple               ( bool );
      virtual void                  setSourceDogLeg            ( bool state=true );
      virtual void                  setTargetDogLeg            ( bool state=true );
      virtual void                  setLock                    ( bool );
      virtual void                  setRouted                  ( bool );
      virtual void                  setTrack                   ( Track* );
      virtual void                  setGCell                   ( GCell* );
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
      virtual bool                  moveUp                     ();
      virtual bool                  moveAside                  ( bool onLeft );
      virtual TrackElement*         makeDogLeg                 ();
      virtual TrackElement*         makeDogLeg                 ( Interval, bool& leftDogleg );
      virtual TrackElement*         makeDogLeg                 ( GCell* );
      virtual TrackElement*         _postDogLeg                ( GCell* );
      virtual void                  _postModify                ();
      virtual void                  desalignate                ();
      virtual bool                  _check                     () const;
      virtual Record*               _getRecord                 () const;
      virtual string                _getString                 () const;
      virtual string                _getTypeName               () const;

    protected:
    // Attributes.
             AutoSegment*          _base;
             GCell*                _gcell;
             bool                  _created;
             bool                  _lock;
             bool                  _revalidated;
             bool                  _sourceDogLeg;
             bool                  _targetDogLeg;
             bool                  _canRipple;
             bool                  _routed;
             unsigned long         _area;
             DataNegociate*        _data;
             unsigned int          _dogLegLevel:4;
             unsigned int          _dogLegOrder:16;

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


} // End of Kite namespace.


INSPECTOR_P_SUPPORT(Kite::TrackSegment);


# endif
