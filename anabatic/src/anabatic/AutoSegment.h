// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |         A n a b a t i c  -  Routing Toolbox                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./anabatic/AutoSegment.h"                      |
// +-----------------------------------------------------------------+


#ifndef  ANABATIC_AUTOSEGMENT_H
#define  ANABATIC_AUTOSEGMENT_H

#include <set>
#include <iostream>
#include <functional>
#include "hurricane/Interval.h"
#include "hurricane/Segment.h"
#include "hurricane/Components.h"
#include "hurricane/Contact.h"
namespace Hurricane {
  class Layer;
  class Horizontal;
  class Vertical;
  class Cell;
}
#include "crlcore/RoutingGauge.h"
#include "anabatic/Constants.h"
#include "anabatic/GCell.h"
#include "anabatic/AutoSegments.h"
#include "anabatic/Session.h"


namespace Anabatic {

  using std::set;
  using std::cerr;
  using std::endl;
  using std::binary_function;
  using Hurricane::StaticObservable;
  using Hurricane::BaseObserver;
  using Hurricane::tab;
  using Hurricane::Interval;
  using Hurricane::Layer;
  using Hurricane::Components;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Hurricane::Cell;
  using CRL::RoutingGauge;

  class AutoHorizontal;
  class AutoVertical;

// -------------------------------------------------------------------
// Class  :  "AutoSegment".

  enum AutoSegmentFlag { SegNoFlags           = 0x0
                       , SegHorizontal        = (1<< 0)
                       , SegFixed             = (1<< 1)
                       , SegGlobal            = (1<< 2)
                       , SegWeakGlobal        = (1<< 3)
                       , SegCanonical         = (1<< 4)
                       , SegBipoint           = (1<< 5)
                       , SegDogleg            = (1<< 6)
                       , SegStrap             = (1<< 7)
                       , SegSourceTop         = (1<< 8)
                       , SegSourceBottom      = (1<< 9)
                       , SegTargetTop         = (1<<10)
                       , SegTargetBottom      = (1<<11)
                       , SegIsReduced         = (1<<12)
                       , SegLayerChange       = (1<<13)
                       , SegSourceTerminal    = (1<<14)  // Replace Terminal.
                       , SegTargetTerminal    = (1<<15)  // Replace Terminal.
                       , SegStrongTerminal    = SegSourceTerminal|SegTargetTerminal
                       , SegWeakTerminal1     = (1<<16)  // Replace TopologicalEnd.
                       , SegWeakTerminal2     = (1<<17)  // Replace TopologicalEnd.
                       , SegNotSourceAligned  = (1<<18)
                       , SegNotTargetAligned  = (1<<19)
                       , SegUnbound           = (1<<20)
                       , SegHalfSlackened     = (1<<21)
                       , SegSlackened         = (1<<22)
                       , SegAxisSet           = (1<<23)
                       , SegInvalidated       = (1<<24)
                       , SegInvalidatedSource = (1<<25)
                       , SegInvalidatedTarget = (1<<26)
                       , SegInvalidatedLayer  = (1<<27)
                       , SegCreated           = (1<<28)
                       , SegUserDefined       = (1<<29)
                       // Masks.              
                       , SegWeakTerminal      = SegStrongTerminal|SegWeakTerminal1|SegWeakTerminal2
                       , SegNotAligned        = SegNotSourceAligned|SegNotTargetAligned
                       , SegSpinTop           = SegSourceTop    |SegTargetTop
                       , SegSpinBottom        = SegSourceBottom |SegTargetBottom
                       , SegDepthSpin         = SegSpinTop      |SegSpinBottom
                       };
 

  class AutoSegment {
      friend class AutoHorizontal;
      friend class AutoVertical;

    public:
      class Observable : public StaticObservable<1> {
        public:
          enum Indexes { TrackSegment = 0
                       };
        public:
          inline             Observable ();
        private:
                             Observable ( const StaticObservable& );
                 Observable& operator=  ( const StaticObservable& );
      };
    public:
      enum ObserverFlag { Create           = 0x000000001
                        , Destroy          = 0x000000002
                        , Invalidate       = 0x000000004
                        , Revalidate       = 0x000000008
                        , RevalidatePPitch = 0x000000010
                        };
    public:
      typedef  std::function< void(AutoSegment*) >  RevalidateCb_t;
    public:
      static  void                setDestroyMode             ( bool );
      static  AutoSegment*        create                     ( AutoContact*  source
                                                             , AutoContact*  target
                                                             , Segment*      hurricaneSegment
                                                             );
      static  AutoSegment*        create                     ( AutoContact*  source
                                                             , AutoContact*  target
                                                             , unsigned int  dir
                                                             , size_t        depth=RoutingGauge::nlayerdepth
                                                             );
              void                destroy                    ();
    // Wrapped Segment Functions.                            
      virtual Segment*            base                       () const = 0;
      virtual Segment*            base                       () = 0;
      virtual Horizontal*         getHorizontal              () { return NULL; };
      virtual Vertical*           getVertical                () { return NULL; };
      inline  Cell*               getCell                    () const;
      inline  Net*                getNet                     () const;
      inline  const Layer*        getLayer                   () const;
      inline  Box                 getBoundingBox             () const;
      inline  Hook*               getSourceHook              ();
      inline  Hook*               getTargetHook              ();
      inline  Contact*            getSource                  () const;
      inline  Contact*            getTarget                  () const;
      inline  Component*          getOppositeAnchor          ( Component* ) const;
      inline  Components          getAnchors                 () const;
      virtual DbU::Unit           getX                       () const;
      virtual DbU::Unit           getY                       () const;
      inline  DbU::Unit           getWidth                   () const;
      inline  DbU::Unit           getLength                  () const;
      inline  DbU::Unit           getSourcePosition          () const;
      inline  DbU::Unit           getTargetPosition          () const;
      inline  DbU::Unit           getSourceX                 () const;
      inline  DbU::Unit           getSourceY                 () const;
      inline  DbU::Unit           getTargetX                 () const;
      inline  DbU::Unit           getTargetY                 () const;
      inline  void                invert                     ();
      inline  void                setLayer                   ( const Layer* );
    // Predicates.                                           
      inline  bool                isHorizontal               () const;
      inline  bool                isVertical                 () const;
      inline  bool                isGlobal                   () const;
      inline  bool                isWeakGlobal               () const;
      inline  bool                isLocal                    () const;
      inline  bool                isFixed                    () const;
      inline  bool                isBipoint                  () const;
      inline  bool                isWeakTerminal             () const;
      inline  bool                isWeakTerminal1            () const;
      inline  bool                isWeakTerminal2            () const;
      inline  bool                isTerminal                 () const;
      inline  bool                isNotSourceAligned         () const;
      inline  bool                isNotTargetAligned         () const;
      inline  bool                isNotAligned               () const;
              bool                isStrongTerminal           ( unsigned int flags=0 ) const;
      inline  bool                isSourceTerminal           () const;
      inline  bool                isTargetTerminal           () const;
      inline  bool                isLayerChange              () const;
      inline  bool                isSpinTop                  () const;
      inline  bool                isSpinBottom               () const;
      inline  bool                isSpinTopOrBottom          () const;
      inline  bool                isReduced                  () const;
      inline  bool                isStrap                    () const;
      inline  bool                isDogleg                   () const;
      inline  bool                isUnbound                  () const;
      inline  bool                isInvalidated              () const;
      inline  bool                isInvalidatedLayer         () const;
      inline  bool                isCreated                  () const;
      inline  bool                isCanonical                () const;
      inline  bool                isUnsetAxis                () const;
      inline  bool                isSlackened                () const;
      inline  bool                isUserDefined              () const;
              bool                isReduceCandidate          () const;
              bool                isUTurn                    () const;
      virtual bool                _canSlacken                () const = 0;
              bool                canReduce                  () const;
              bool                mustRaise                  () const;
              unsigned int        canDogleg                  ( Interval );
      virtual bool                canMoveULeft               ( float reserve=0.0 ) const = 0;
      virtual bool                canMoveURight              ( float reserve=0.0 ) const = 0;
              bool                canMoveUp                  ( float reserve=0.0, unsigned int flags=0 ) const;
              bool                canPivotUp                 ( float reserve=0.0, unsigned int flags=0 ) const;
              bool                canPivotDown               ( float reserve=0.0, unsigned int flags=0 ) const;
              bool                canSlacken                 ( unsigned int flags=0 ) const;
      virtual bool                checkPositions             () const = 0;
      virtual bool                checkConstraints           () const = 0;
              bool                checkDepthSpin             () const;
    // Accessors.                                            
      inline  unsigned long       getId                      () const;
      inline  unsigned int        getFlags                   () const;
      virtual unsigned int        getDirection               () const = 0;
      inline  GCell*              getGCell                   () const;
      virtual size_t              getGCells                  ( vector<GCell*>& ) const = 0;
      inline  AutoContact*        getAutoSource              () const;
      inline  AutoContact*        getAutoTarget              () const;
              AutoContact*        getOppositeAnchor          ( AutoContact* ) const;
              size_t              getPerpandicularsBound     ( set<AutoSegment*>& );
      inline  AutoSegment*        getParent                  () const;
      inline  unsigned int        getDepth                   () const;
      inline  DbU::Unit           getPitch                   () const;
              DbU::Unit           getPPitch                  () const;
      inline  DbU::Unit           getAxis                    () const;
      virtual DbU::Unit           getSourceU                 () const = 0;
      virtual DbU::Unit           getTargetU                 () const = 0;
      virtual DbU::Unit           getDuSource                () const = 0;
      virtual DbU::Unit           getDuTarget                () const = 0;
      inline  DbU::Unit           getOrigin                  () const;
      inline  DbU::Unit           getExtremity               () const;
      virtual Interval            getSpanU                   () const = 0;
              Interval            getMinSpanU                () const;
      virtual Interval            getSourceConstraints       ( unsigned int flags=0 ) const = 0;
      virtual Interval            getTargetConstraints       ( unsigned int flags=0 ) const = 0;
      virtual bool                getConstraints             ( DbU::Unit& min, DbU::Unit& max ) const = 0;
      inline  bool                getConstraints             ( Interval& i ) const;
      inline  const Interval&     getUserConstraints         () const;
      inline  const Interval&     getNativeConstraints       () const;
      virtual DbU::Unit           getSlack                   () const;
      inline  DbU::Unit           getOptimalMin              () const;
      inline  DbU::Unit           getOptimalMax              () const;
      inline  DbU::Unit           getNativeMin               () const;
      inline  DbU::Unit           getNativeMax               () const;
              Interval&           getOptimal                 ( Interval& i ) const;
      virtual DbU::Unit           getCost                    ( DbU::Unit axis ) const;
      virtual AutoSegment*        getCanonical               ( DbU::Unit& min , DbU::Unit& max );
      inline  AutoSegment*        getCanonical               ( Interval& i );
              float               getMaxUnderDensity         ( unsigned int flags );
    // Modifiers.                                            
      inline  void                unsetFlags                 ( unsigned int );
      inline  void                setFlags                   ( unsigned int );
              void                setFlagsOnAligneds         ( unsigned int );
      inline  void                incReduceds                ();
      inline  void                decReduceds                ();
      virtual void                setDuSource                ( DbU::Unit du ) = 0;
      virtual void                setDuTarget                ( DbU::Unit du ) = 0;
              void                computeTerminal            ();
      virtual void                updateOrient               () = 0;
      virtual void                updatePositions            () = 0;
      virtual void                updateNativeConstraints    () = 0;
              void                updateSourceSpin           ();
              void                updateTargetSpin           ();
              void                sourceDetach               ();
              void                targetDetach               ();
              void                sourceAttach               ( AutoContact* );
              void                targetAttach               ( AutoContact* );
    //inline  void                mergeUserConstraints       ( const Interval& );
              void                mergeUserConstraints       ( const Interval& );
      inline  void                resetUserConstraints       ();
      inline  void                setOptimalMin              ( DbU::Unit min );
      inline  void                setOptimalMax              ( DbU::Unit max );
      inline  void                mergeNativeMin             ( DbU::Unit min );
      inline  void                mergeNativeMax             ( DbU::Unit max );
      inline  void                resetNativeConstraints     ( DbU::Unit min, DbU::Unit max );
              bool                checkNotInvalidated        () const;
      inline  void                setParent                  ( AutoSegment* );
              void                revalidate                 ();
              AutoSegment*        makeDogleg                 ( AutoContact* );
              unsigned int        makeDogleg                 ( Interval, unsigned int flags=Flags::NoFlags );
              unsigned int        makeDogleg                 ( GCell*, unsigned int flags=Flags::NoFlags );
      virtual unsigned int        _makeDogleg                ( GCell*, unsigned int flags ) = 0;
      virtual bool                moveULeft                  () = 0;
      virtual bool                moveURight                 () = 0;
              bool                slacken                    ( unsigned int flags );
      virtual bool                _slacken                   ( unsigned int flags ) = 0;
              void                _changeDepth               ( unsigned int depth, unsigned int flags );
              void                changeDepth                ( unsigned int depth, unsigned int flags );
              bool                moveUp                     ( unsigned int flags=Flags::NoFlags );
              bool                moveDown                   ( unsigned int flags=Flags::NoFlags );
              bool                reduceDoglegLayer          ();
              bool                reduce                     ();
              bool                raise                      ();
    // Canonical Modifiers.                                            
              AutoSegment*        canonize                   ( unsigned int flags=Flags::NoFlags );
      virtual void                invalidate                 ( unsigned int flags=Flags::Propagate );
              void                invalidate                 ( AutoContact* );
              void                computeOptimal             ( set<AutoSegment*>& processeds );
              void                setAxis                    ( DbU::Unit, unsigned int flags=Flags::NoFlags );
              bool                toConstraintAxis           ( unsigned int flags=Flags::Realignate );
              bool                toOptimalAxis              ( unsigned int flags=Flags::Realignate );
    // Collections & Filters.                                
              AutoSegments        getOnSourceContact         ( unsigned int direction );
              AutoSegments        getOnTargetContact         ( unsigned int direction );
              AutoSegments        getCachedOnSourceContact   ( unsigned int direction );
              AutoSegments        getCachedOnTargetContact   ( unsigned int direction );
              AutoSegments        getAligneds                ( unsigned int flags=Flags::NoFlags );
              AutoSegments        getPerpandiculars          ();
              size_t              getAlignedContacts         ( map<AutoContact*,int>& ) const ;
    // Observers.                         
      template< typename OwnerT >
      inline  OwnerT*             getObserver                ( size_t slot );
      inline  void                setObserver                ( size_t slot, BaseObserver* );
      inline  void                notify                     ( unsigned int flags );
    // Inspector Management.                                 
      virtual Record*             _getRecord                 () const = 0;
      virtual string              _getString                 () const = 0;
      virtual string              _getTypeName               () const = 0;
    // Non-reviewed atomic modifiers.
              bool                _check                     () const;
#if THIS_IS_DISABLED
      virtual void                desalignate                ( AutoContact* ) = 0;
              bool                shearUp                    ( GCell*
                                                             , AutoSegment*& movedUp
                                                             , float         reserve
                                                             , unsigned int  flags );
#endif
                                  
    protected:                    
    // Internal: Static Attributes.
      static size_t               _allocateds;
      static size_t               _globalsCount;
      static bool                 _destroyBase;
      static bool                 _destroyTool;
      static unsigned long        _maxId;
    // Internal: Attributes.      
      const  unsigned long        _id;
             GCell*               _gcell;
             unsigned int         _flags;
             unsigned int         _depth      : 8;
             unsigned int         _optimalMin : 8;
             unsigned int         _optimalMax : 8;
             unsigned int         _reduceds   : 2;
             DbU::Unit            _sourcePosition;
             DbU::Unit            _targetPosition;
             Interval             _userConstraints;
             Interval             _nativeConstraints;
             AutoSegment*         _parent;
             Observable           _observers;
                                  
    // Internal: Constructors & Destructors.
    protected:                    
                                  AutoSegment     ( Segment* segment );
      virtual                    ~AutoSegment     ();
      static  void                _preCreate      ( AutoContact* source, AutoContact* target );
      virtual void                _postCreate     ();
      virtual void                _preDestroy     ();
    private:                                      
                                  AutoSegment     ( const AutoSegment& );
              AutoSegment&        operator=       ( const AutoSegment& );
    protected:                                    
              void                _invalidate     ();
      inline  unsigned int        _getFlags       () const;
              std::string         _getStringFlags () const;
      virtual void                _setAxis        ( DbU::Unit ) = 0;

    public:
      struct CompareId : public binary_function<AutoSegment*,AutoSegment*,bool> {
          inline bool  operator() ( const AutoSegment* lhs, const AutoSegment* rhs ) const;
      };
    public:
      struct CompareByDepthLength : public binary_function<AutoSegment*,AutoSegment*,bool> {
          bool operator() ( AutoSegment* lhs, AutoSegment* rhs ) const;
      };
    public:
      struct CompareByDepthAxis : public binary_function<AutoSegment*,AutoSegment*,bool> {
          bool operator() ( AutoSegment* lhs, AutoSegment* rhs ) const;
      };
    public:
      typedef std::set<AutoSegment*,CompareByDepthLength>  DepthLengthSet;

    // Static Utilities.
    public:
      static inline bool          areAlignedsAndDiffLayer    ( AutoSegment*, AutoSegment* );
      static        bool          isTopologicalBound         ( AutoSegment*  seed, unsigned int flags );
      static inline bool          arePerpandiculars          ( AutoSegment* a, AutoSegment* b );
      static inline bool          arePerpandiculars          ( bool isHorizontalA, AutoSegment* b );
      static inline bool          areAligneds                ( AutoSegment* a, AutoSegment* b );
      static        unsigned int  getPerpandicularState      ( AutoContact* contact
                                                             , AutoSegment* source
                                                             , AutoSegment* current
                                                             , bool         isHorizontalMaster
                                                             , const Layer* masterLayer=NULL
                                                             );
      static inline unsigned int  getPerpandicularState      ( AutoContact* contact
                                                             , AutoSegment* source
                                                             , AutoSegment* current
                                                             , AutoSegment* master
                                                             );
      static void                 getTopologicalInfos        ( AutoSegment*          seed
                                                             , vector<AutoSegment*>& collapseds
                                                             , vector<AutoSegment*>& perpandiculars
                                                             , DbU::Unit&            leftBound
                                                             , DbU::Unit&            rightBound
                                                             );
      static int                  getTerminalCount           ( AutoSegment* seed
                                                             , vector<AutoSegment*>& collapseds
                                                             );
      static inline int           getTerminalCount           ( AutoSegment* seed );
      static inline size_t        getGlobalsCount            ();
      static inline size_t        getAllocateds              ();
      static inline unsigned long getMaxId                   ();
  };


// Inline Functions.
  inline  unsigned long   AutoSegment::getId                  () const { return _id; }
  inline  Cell*           AutoSegment::getCell                () const { return base()->getCell(); }
  inline  Net*            AutoSegment::getNet                 () const { return base()->getNet(); }
  inline  const Layer*    AutoSegment::getLayer               () const { return base()->getLayer(); }
  inline  Box             AutoSegment::getBoundingBox         () const { return base()->getBoundingBox(); }
  inline  Hook*           AutoSegment::getSourceHook          () { return base()->getSourceHook(); }
  inline  Hook*           AutoSegment::getTargetHook          () { return base()->getTargetHook(); }
  inline  Contact*        AutoSegment::getSource              () const { return static_cast<Contact*>(base()->getSource()); }
  inline  Contact*        AutoSegment::getTarget              () const { return static_cast<Contact*>(base()->getTarget()); }
  inline  Component*      AutoSegment::getOppositeAnchor      ( Component* anchor ) const { return base()->getOppositeAnchor(anchor); };
  inline  AutoSegment*    AutoSegment::getParent              () const { return _parent; }
  inline  DbU::Unit       AutoSegment::getSourcePosition      () const { return _sourcePosition; }
  inline  DbU::Unit       AutoSegment::getTargetPosition      () const { return _targetPosition; }
  inline  DbU::Unit       AutoSegment::getSourceX             () const { return base()->getSourceX(); }
  inline  DbU::Unit       AutoSegment::getSourceY             () const { return base()->getSourceY(); }
  inline  DbU::Unit       AutoSegment::getTargetX             () const { return base()->getTargetX(); }
  inline  DbU::Unit       AutoSegment::getTargetY             () const { return base()->getTargetY(); }
  inline  DbU::Unit       AutoSegment::getWidth               () const { return base()->getWidth(); }
  inline  DbU::Unit       AutoSegment::getLength              () const { return base()->getLength(); }
  inline  void            AutoSegment::invert                 () { base()->invert(); }
  inline  GCell*          AutoSegment::getGCell               () const { return _gcell; }
  inline  AutoContact*    AutoSegment::getAutoSource          () const { return Session::lookup(getSource()); }
  inline  AutoContact*    AutoSegment::getAutoTarget          () const { return Session::lookup(getTarget()); }
  inline  bool            AutoSegment::getConstraints         ( Interval& i ) const { return getConstraints(i.getVMin(),i.getVMax()); }
  inline  AutoSegment*    AutoSegment::getCanonical           ( Interval& i ) { return getCanonical(i.getVMin(),i.getVMax()); }
  inline  unsigned int    AutoSegment::getDepth               () const { return _depth; }
  inline  DbU::Unit       AutoSegment::getPitch               () const { return Session::getPitch(getDepth(),Flags::NoFlags); }
  inline  DbU::Unit       AutoSegment::getAxis                () const { return isHorizontal()?base()->getY():base()->getX(); }
  inline  DbU::Unit       AutoSegment::getOrigin              () const { return isHorizontal()?_gcell->getYMin():_gcell->getXMin(); }
  inline  DbU::Unit       AutoSegment::getExtremity           () const { return isHorizontal()?_gcell->getYMax():_gcell->getXMax(); }
  inline  DbU::Unit       AutoSegment::getOptimalMin          () const { return DbU::lambda(_optimalMin) + getOrigin(); }
  inline  DbU::Unit       AutoSegment::getOptimalMax          () const { return DbU::lambda(_optimalMax) + getOrigin(); }
  inline  DbU::Unit       AutoSegment::getNativeMin           () const { return _nativeConstraints.getVMin(); }
  inline  DbU::Unit       AutoSegment::getNativeMax           () const { return _nativeConstraints.getVMax(); }
  inline  const Interval& AutoSegment::getUserConstraints     () const { return _userConstraints; }
  inline  const Interval& AutoSegment::getNativeConstraints   () const { return _nativeConstraints; }
                                                              
  inline  bool            AutoSegment::isHorizontal           () const { return _flags & SegHorizontal; }
  inline  bool            AutoSegment::isVertical             () const { return not (_flags & SegHorizontal); }
  inline  bool            AutoSegment::isFixed                () const { return _flags & SegFixed; }
  inline  bool            AutoSegment::isGlobal               () const { return _flags & SegGlobal; }
  inline  bool            AutoSegment::isWeakGlobal           () const { return _flags & SegWeakGlobal; }
  inline  bool            AutoSegment::isLocal                () const { return not (_flags & SegGlobal); }
  inline  bool            AutoSegment::isBipoint              () const { return _flags & SegBipoint; }
  inline  bool            AutoSegment::isWeakTerminal         () const { return _flags & SegWeakTerminal; }
  inline  bool            AutoSegment::isWeakTerminal1        () const { return _flags & SegWeakTerminal1; }
  inline  bool            AutoSegment::isWeakTerminal2        () const { return _flags & SegWeakTerminal2; }
  inline  bool            AutoSegment::isSourceTerminal       () const { return _flags & SegSourceTerminal; }
  inline  bool            AutoSegment::isTargetTerminal       () const { return _flags & SegTargetTerminal; }
  inline  bool            AutoSegment::isTerminal             () const { return _flags & SegStrongTerminal; }
  inline  bool            AutoSegment::isNotSourceAligned     () const { return _flags & SegNotSourceAligned; }
  inline  bool            AutoSegment::isNotTargetAligned     () const { return _flags & SegNotTargetAligned; }
  inline  bool            AutoSegment::isNotAligned           () const { return (_flags & SegNotAligned) == SegNotAligned; }
  inline  bool            AutoSegment::isDogleg               () const { return _flags & SegDogleg  ; }
  inline  bool            AutoSegment::isUnbound              () const { return _flags & SegUnbound  ; }
  inline  bool            AutoSegment::isStrap                () const { return _flags & SegStrap; }
  inline  bool            AutoSegment::isLayerChange          () const { return _flags & SegLayerChange; }
  inline  bool            AutoSegment::isSpinTop              () const { return ((_flags & SegSpinTop   ) == SegSpinTop); }
  inline  bool            AutoSegment::isSpinBottom           () const { return ((_flags & SegSpinBottom) == SegSpinBottom); }
  inline  bool            AutoSegment::isSpinTopOrBottom      () const { return isSpinTop() or isSpinBottom(); }
  inline  bool            AutoSegment::isReduced              () const { return _flags & SegIsReduced; }
  inline  bool            AutoSegment::isSlackened            () const { return _flags & SegSlackened; }
  inline  bool            AutoSegment::isCanonical            () const { return _flags & SegCanonical; }
  inline  bool            AutoSegment::isUnsetAxis            () const { return not (_flags & SegAxisSet); }
  inline  bool            AutoSegment::isInvalidated          () const { return _flags & SegInvalidated; }
  inline  bool            AutoSegment::isInvalidatedLayer     () const { return _flags & SegInvalidatedLayer; }
  inline  bool            AutoSegment::isCreated              () const { return _flags & SegCreated; }
  inline  bool            AutoSegment::isUserDefined          () const { return _flags & SegUserDefined; }
  inline  void            AutoSegment::setFlags               ( unsigned int flags ) { _flags |=  flags; }
  inline  void            AutoSegment::unsetFlags             ( unsigned int flags ) { _flags &= ~flags; }
                                                              
  inline  unsigned int    AutoSegment::getFlags               () const { return _flags; }
  inline  unsigned int    AutoSegment::_getFlags              () const { return _flags; }
  inline  void            AutoSegment::incReduceds            () { if (_reduceds<3) ++_reduceds; }
  inline  void            AutoSegment::decReduceds            () { if (_reduceds>0) --_reduceds; }
  inline  void            AutoSegment::setLayer               ( const Layer* layer ) { base()->setLayer(layer); _depth=Session::getLayerDepth(layer); }
  inline  void            AutoSegment::setOptimalMin          ( DbU::Unit min ) { _optimalMin = (unsigned int)DbU::getLambda(min-getOrigin()); }
  inline  void            AutoSegment::setOptimalMax          ( DbU::Unit max ) { _optimalMax = (unsigned int)DbU::getLambda(max-getOrigin()); }
  inline  void            AutoSegment::mergeNativeMin         ( DbU::Unit min ) { _nativeConstraints.getVMin() = std::max( min, _nativeConstraints.getVMin() ); }
  inline  void            AutoSegment::mergeNativeMax         ( DbU::Unit max ) { _nativeConstraints.getVMax() = std::min( max, _nativeConstraints.getVMax() ); }
  inline  void            AutoSegment::resetNativeConstraints ( DbU::Unit min, DbU::Unit max ) { _nativeConstraints = Interval( min, max ); }
//inline  void            AutoSegment::mergeUserConstraints   ( const Interval& constraints ) { _userConstraints.intersection(constraints); }
  inline  void            AutoSegment::resetUserConstraints   () { _userConstraints = Interval(false); }

  inline  void  AutoSegment::setParent ( AutoSegment* parent )
  {
    if ( parent == this ) {
      cerr << "Parentage Looping: " << parent->_getString() << endl;
    }
    _parent = parent;
  }


  inline bool  AutoSegment::CompareId::operator() ( const AutoSegment* lhs, const AutoSegment* rhs ) const
  { return lhs->getId() < rhs->getId(); }
  
  inline unsigned long AutoSegment::getMaxId ()
  { return _maxId; }

  inline bool  AutoSegment::areAlignedsAndDiffLayer ( AutoSegment* s1, AutoSegment* s2 )
  { return s1 and s2
      and  (s1->isHorizontal() == s2->isHorizontal())
      and  (s1->getLayer() != s2->getLayer()); }

  inline bool  AutoSegment::arePerpandiculars ( AutoSegment* a, AutoSegment* b )
  { return a and b and (a->isHorizontal() != b->isHorizontal()); }

  inline bool  AutoSegment::arePerpandiculars ( bool isHorizontalA, AutoSegment* b )
  { return b and (isHorizontalA != b->isHorizontal()); }

  inline bool  AutoSegment::areAligneds ( AutoSegment* a, AutoSegment* b )
  { return a and b and (a->isHorizontal() == b->isHorizontal()); }

  inline unsigned int  AutoSegment::getPerpandicularState ( AutoContact* contact
                                                          , AutoSegment* source
                                                          , AutoSegment* current
                                                          , AutoSegment* master  )
  {
    return getPerpandicularState ( contact, source, current, master->isHorizontal(), master->getLayer() );
  }


  inline int  AutoSegment::getTerminalCount ( AutoSegment* seed )
  {
    cdebug_log(145,0) << "getTerminalCount() - " << seed << endl;

    vector<AutoSegment*>  collapseds;
    vector<AutoSegment*>  perpandiculars;
    DbU::Unit             leftBound;
    DbU::Unit             rightBound;

    getTopologicalInfos ( seed
                        , collapseds
                        , perpandiculars
                        , leftBound
                        , rightBound
                        );

    return getTerminalCount ( seed, collapseds );
  }


  inline size_t  AutoSegment::getGlobalsCount () { return _globalsCount; }
  inline size_t  AutoSegment::getAllocateds   () { return _allocateds; }


  inline void  AutoSegment::setObserver ( size_t slot, BaseObserver* observer )
  { _observers.setObserver( slot, observer ); }

  template<typename OwnerT>
  inline OwnerT* AutoSegment::getObserver ( size_t slot )
  { return _observers.getObserver<OwnerT>(slot); }

  inline void  AutoSegment::notify ( unsigned int flags )
  { _observers.notify( flags ); }

  inline AutoSegment::Observable::Observable () : StaticObservable<1>() { }


} // End of Anabatic namespace.


INSPECTOR_P_SUPPORT(Anabatic::AutoSegment);


# endif  // ANABATIC_AUTOSEGMENT_H
