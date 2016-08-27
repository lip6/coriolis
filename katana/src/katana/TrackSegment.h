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
// |  C++ Header  :  "./katana/TrackSegment.h"                       |
// +-----------------------------------------------------------------+


#ifndef  KATANA_TRACK_SEGMENT_H
#define  KATANA_TRACK_SEGMENT_H

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

  class TrackSegment : public TrackElement {
    public:
      class CompareById : public binary_function<const TrackSegment*,const TrackSegment*,bool> {
        public:
          inline bool  operator() ( const TrackSegment* lhs, const TrackSegment* rhs );
      };

    public:
      static  TrackElement*         create                 ( AutoSegment*, Track*, bool& created );
      static  size_t                getAllocateds          ();
    public:                                                
    // Wrapped AutoSegment Functions (when applicable).
      virtual AutoSegment*          base                   () const;
      virtual Segment*              getSegment             () const;
      virtual bool                  isFixed                () const;
      virtual bool                  isHorizontal           () const;
      virtual bool                  isVertical             () const;
      virtual bool                  isLocal                () const;
      virtual bool                  isGlobal               () const;
      virtual bool                  isBipoint              () const;
      virtual bool                  isTerminal             () const;
      virtual bool                  isStrongTerminal       ( unsigned int flags=0 ) const;
      virtual bool                  isStrap                () const;
      virtual bool                  isSlackened            () const;
      virtual bool                  isDogleg               () const;
      virtual bool                  isReduced              () const;
      virtual bool                  isUTurn                () const;
      virtual bool                  isUserDefined          () const;
    // Predicates.
      virtual bool                  canDogleg              ();
      virtual bool                  canDogleg              ( Interval );
      virtual bool                  canDogleg              ( Anabatic::GCell*, unsigned int flags=0 );
      virtual bool                  canPivotUp             ( float reserve ) const;
      virtual bool                  canPivotDown           ( float reserve ) const;
      virtual bool                  canMoveUp              ( float reserve, unsigned int flags ) const;
      virtual bool                  canSlacken             () const;
      virtual float                 getMaxUnderDensity     ( unsigned int flags ) const;
      virtual unsigned long         getId                  () const;
      virtual Flags                 getDirection           () const;
      virtual Net*                  getNet                 () const;
      virtual const Layer*          getLayer               () const;
      virtual DbU::Unit             getPitch               () const;
      virtual DbU::Unit             getPPitch              () const;
      virtual unsigned long         getFreedomDegree       () const;
      virtual unsigned int          getDoglegLevel         () const;
      virtual TrackElement*         getNext                () const;
      virtual TrackElement*         getPrevious            () const;
      virtual TrackElement*         getParent              () const;
      virtual DbU::Unit             getAxis                () const;
      virtual Interval              getFreeInterval        () const;
      virtual Interval              getSourceConstraints   () const;
      virtual Interval              getTargetConstraints   () const;
      virtual DataNegociate*        getDataNegociate       ( unsigned int flags=Flags::DataSelf ) const;
      virtual TrackElement*         getCanonical           ( Interval& );
      virtual size_t                getGCells              ( vector<GCell*>& ) const;
      virtual TrackElement*         getSourceDogleg        ();
      virtual TrackElement*         getTargetDogleg        ();
      virtual TrackElements         getPerpandiculars      ();
      virtual size_t                getPerpandicularsBound ( set<TrackElement*>& );
    // Mutators.
      virtual void                  setTrack               ( Track* );
      virtual void                  updateFreedomDegree    ();
      virtual void                  setDoglegLevel         ( unsigned int );
      virtual void                  swapTrack              ( TrackElement* );
      virtual void                  reschedule             ( unsigned int level );
      virtual void                  detach                 ();
      virtual void                  invalidate             ();
      virtual void                  revalidate             ();
      virtual void                  updatePPitch           ();
      virtual void                  setAxis                ( DbU::Unit, unsigned int flags );
      virtual TrackElement*         makeDogleg             ();
      virtual TrackElement*         makeDogleg             ( Anabatic::GCell*, TrackElement*& perpandicular, TrackElement*& parallel );
      virtual TrackElement*         makeDogleg             ( Interval, unsigned int& flags );
      virtual void                  _postDoglegs           ( TrackElement*& perpandicular, TrackElement*& parallel );
      virtual bool                  moveAside              ( unsigned int flags );
      virtual bool                  slacken                ( unsigned int flags=Flags::NoFlags );
      virtual bool                  moveUp                 ( unsigned int flags );
      virtual bool                  moveDown               ( unsigned int flags );
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
             unsigned long  _freedomDegree;
             DbU::Unit      _ppitch;
             DataNegociate* _data;
             unsigned int   _dogLegLevel:4;

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


  inline bool  TrackSegment::CompareById::operator() ( const TrackSegment* lhs, const TrackSegment* rhs )
  { return lhs->getId() < rhs->getId(); }


  typedef  set<TrackSegment*,TrackSegment::CompareById>  TrackSegmentSet;


}  // Katana namespace.


INSPECTOR_P_SUPPORT(Katana::TrackSegment);

#endif  // KATANA_TRACK_SEGMENT_H
