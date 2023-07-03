// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
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


#pragma  once
#include <tuple>
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

  using std::tuple;
  using std::array;
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

  class AutoSegment {
      friend class AutoHorizontal;
      friend class AutoVertical;

    public:
      static const uint64_t  SegNoFlags           =  0L;
      static const uint64_t  SegHorizontal        = (1L<< 0);
      static const uint64_t  SegFixed             = (1L<< 1);
      static const uint64_t  SegFixedAxis         = (1L<< 2);
      static const uint64_t  SegGlobal            = (1L<< 3);
      static const uint64_t  SegWeakGlobal        = (1L<< 4);
      static const uint64_t  SegLongLocal         = (1L<< 5);
      static const uint64_t  SegCanonical         = (1L<< 6);
      static const uint64_t  SegBipoint           = (1L<< 7);
      static const uint64_t  SegDogleg            = (1L<< 8);
      static const uint64_t  SegStrap             = (1L<< 9);
      static const uint64_t  SegSourceTop         = (1L<<10);
      static const uint64_t  SegSourceBottom      = (1L<<11);
      static const uint64_t  SegTargetTop         = (1L<<12);
      static const uint64_t  SegTargetBottom      = (1L<<13);
      static const uint64_t  SegIsReduced         = (1L<<14);
      static const uint64_t  SegDrag              = (1L<<15);
      static const uint64_t  SegLayerChange       = (1L<<16);
      static const uint64_t  SegSourceTerminal    = (1L<<17);  // Replace Terminal.
      static const uint64_t  SegTargetTerminal    = (1L<<18);  // Replace Terminal.
      static const uint64_t  SegStrongTerminal    = SegSourceTerminal|SegTargetTerminal;
      static const uint64_t  SegWeakTerminal1     = (1L<<19);  // Replace TopologicalEnd.
      static const uint64_t  SegWeakTerminal2     = (1L<<20);  // Replace TopologicalEnd.
      static const uint64_t  SegNotSourceAligned  = (1L<<21);
      static const uint64_t  SegNotTargetAligned  = (1L<<22);
      static const uint64_t  SegUnbound           = (1L<<23);
      static const uint64_t  SegHalfSlackened     = (1L<<24);
      static const uint64_t  SegSlackened         = (1L<<25);
      static const uint64_t  SegAxisSet           = (1L<<26);
      static const uint64_t  SegInvalidated       = (1L<<27);
      static const uint64_t  SegInvalidatedSource = (1L<<28);
      static const uint64_t  SegInvalidatedTarget = (1L<<29);
      static const uint64_t  SegInvalidatedLayer  = (1L<<30);
      static const uint64_t  SegCreated           = (1L<<31);
      static const uint64_t  SegUserDefined       = (1L<<32);
      static const uint64_t  SegAnalog            = (1L<<33);
      static const uint64_t  SegWide              = (1L<<34);
      static const uint64_t  SegShortNet          = (1L<<35);
      static const uint64_t  SegUnbreakable       = (1L<<36);
      static const uint64_t  SegNonPref           = (1L<<37);
      static const uint64_t  SegAtMinArea         = (1L<<38);
      static const uint64_t  SegNoMoveUp          = (1L<<39);
      static const uint64_t  SegOnVSmall          = (1L<<40);
    // Masks.
      static const uint64_t  SegWeakTerminal      = SegStrongTerminal|SegWeakTerminal1|SegWeakTerminal2;
      static const uint64_t  SegNotAligned        = SegNotSourceAligned|SegNotTargetAligned;
      static const uint64_t  SegSpinTop           = SegSourceTop    |SegTargetTop;
      static const uint64_t  SegSpinBottom        = SegSourceBottom |SegTargetBottom;
      static const uint64_t  SegDepthSpin         = SegSpinTop      |SegSpinBottom;

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
      enum ObserverFlag { Create           = (1 <<  0)
                        , Destroy          = (1 <<  1)
                        , Invalidate       = (1 <<  2)
                        , Revalidate       = (1 <<  3)
                        , RevalidatePPitch = (1 <<  4)
                        };
    public:
      typedef  std::function< void(AutoSegment*) >  RevalidateCb_t;
    public:
             static  void                setAnalogMode              ( bool );
             static  bool                getAnalogMode              ();
             static  void                setShortNetMode            ( bool );
      inline static  DbU::Unit           getViaToTopCap             ( size_t depth );
      inline static  DbU::Unit           getViaToBottomCap          ( size_t depth );
      inline static  DbU::Unit           getViaToSameCap            ( size_t depth );
      inline static  DbU::Unit           getMinimalLength           ( size_t depth );
             static  AutoSegment*        create                     ( AutoContact*  source
                                                                    , AutoContact*  target
                                                                    , Segment*      hurricaneSegment
                                                                    );
             static  AutoSegment*        create                     ( AutoContact*  source
                                                                    , AutoContact*  target
                                                                    , Flags         dir
                                                                    , size_t        depth=RoutingGauge::nlayerdepth
                                                                    );
                     void                destroy                    ();
    // Wrapped Segment Functions.                            
             virtual Segment*            base                       () const = 0;
             virtual Segment*            base                       () = 0;
             virtual Horizontal*         getHorizontal              () { return NULL; };
             virtual Vertical*           getVertical                () { return NULL; };
      inline         Cell*               getCell                    () const;
      inline         Net*                getNet                     () const;
      inline         const Layer*        getLayer                   () const;
      inline         Box                 getBoundingBox             () const;
      inline         Hook*               getSourceHook              ();
      inline         Hook*               getTargetHook              ();
      inline         Contact*            getSource                  () const;
      inline         Contact*            getTarget                  () const;
      inline         Component*          getOppositeAnchor          ( Component* ) const;
      inline         Components          getAnchors                 () const;
             virtual DbU::Unit           getX                       () const;
             virtual DbU::Unit           getY                       () const;
      inline         DbU::Unit           getWidth                   () const;
      inline         DbU::Unit           getContactWidth            () const;
      inline         DbU::Unit           getLength                  () const;
      inline         DbU::Unit           getSpanLength              () const;
      inline         DbU::Unit           getAnchoredLength          () const;
      inline         DbU::Unit           getSourcePosition          () const;
      inline         DbU::Unit           getTargetPosition          () const;
      inline         DbU::Unit           getSourceX                 () const;
      inline         DbU::Unit           getSourceY                 () const;
      inline         DbU::Unit           getTargetX                 () const;
      inline         DbU::Unit           getTargetY                 () const;
      inline         void                invert                     ();
      inline         void                setLayer                   ( const Layer* );
      inline         void                setLayer                   ( size_t depth );
      inline         void                setWidth                   ( DbU::Unit );
    // Predicates.                                           
      inline         bool                isHorizontal               () const;
      inline         bool                isVertical                 () const;
      inline         bool                isGlobal                   () const;
      inline         bool                isWeakGlobal               () const;
      inline         bool                isLongLocal                () const;
      inline         bool                isLocal                    () const;
      inline         bool                isFixed                    () const;
      inline         bool                isFixedAxis                () const;
      inline         bool                isBipoint                  () const;
      inline         bool                isWeakTerminal             () const;
      inline         bool                isWeakTerminal1            () const;
      inline         bool                isWeakTerminal2            () const;
      inline         bool                isTerminal                 () const;
      inline         bool                isUnbreakable              () const;
      inline         bool                isNonPref                  () const;
      inline         bool                isNonPrefOnVSmall          () const;
      inline         bool                isDrag                     () const;
      inline         bool                isAtMinArea                () const;
      inline         bool                isNotSourceAligned         () const;
      inline         bool                isNotTargetAligned         () const;
      inline         bool                isNotAligned               () const;
                     bool                isStrongTerminal           ( Flags flags=Flags::NoFlags ) const;
      inline         bool                isSourceTerminal           () const;
      inline         bool                isTargetTerminal           () const;
      inline         bool                isLayerChange              () const;
      inline         bool                isStackedStrap             () const;
      inline         bool                isSpinTop                  () const;
      inline         bool                isSpinBottom               () const;
      inline         bool                isSpinTopOrBottom          () const;
      inline         bool                isReduced                  () const;
      inline         bool                isStrap                    () const;
      inline         bool                isDogleg                   () const;
      inline         bool                isUnbound                  () const;
      inline         bool                isInvalidated              () const;
      inline         bool                isInvalidatedLayer         () const;
      inline         bool                isCreated                  () const;
      inline         bool                isCanonical                () const;
      inline         bool                isUnsetAxis                () const;
      inline         bool                isSlackened                () const;
      inline         bool                isUserDefined              () const;
                     bool                isNearMinArea              () const;
                     bool                isReduceCandidate          () const;
                     bool                isUTurn                    () const;
      inline         bool                isAnalog                   () const;
      inline         bool                isWide                     () const;
      inline         bool                isShortNet                 () const;
      inline         bool                isNoMoveUp                 () const;
             virtual bool                _canSlacken                () const = 0;
                     bool                canReduce                  ( Flags flags=Flags::WithPerpands ) const;
                     bool                mustRaise                  () const;
                     Flags               canDogleg                  ( Interval );
             virtual bool                canMoveULeft               ( float reserve=0.0 ) const = 0;
             virtual bool                canMoveURight              ( float reserve=0.0 ) const = 0;
                     bool                canMoveUp                  ( float reserve=0.0, Flags flags=Flags::NoFlags ) const;
                     bool                canPivotUp                 ( float reserve=0.0, Flags flags=Flags::NoFlags ) const;
                     bool                canPivotDown               ( float reserve=0.0, Flags flags=Flags::NoFlags ) const;
                     bool                canSlacken                 ( Flags flags=Flags::NoFlags ) const;
             virtual bool                checkPositions             () const = 0;
             virtual bool                checkConstraints           () const = 0;
                     bool                checkDepthSpin             () const;
    // Accessors.                                            
      inline         unsigned long       getId                      () const;
      inline         uint64_t            getFlags                   () const;
             virtual Flags               getDirection               () const = 0;
      inline         GCell*              getGCell                   () const;
             virtual bool                getGCells                  ( vector<GCell*>& ) const = 0;
      inline         AutoContact*        getAutoSource              () const;
      inline         AutoContact*        getAutoTarget              () const;
                     AutoContact*        getOppositeAnchor          ( AutoContact* ) const;
                     size_t              getPerpandicularsBound     ( set<AutoSegment*>& );
      inline         AutoSegment*        getParent                  () const;
      inline         unsigned int        getRpDistance              () const;
      inline         unsigned int        getDepth                   () const;
      inline         DbU::Unit           getPitch                   () const;
                     DbU::Unit           getPPitch                  () const;
#if DISABLED
                     DbU::Unit           getExtensionCap            () const;
#endif
                     DbU::Unit           getExtensionCap            ( Flags ) const;
      inline         DbU::Unit           getAxis                    () const;
                     void                getEndAxes                 ( DbU::Unit& sourceAxis, DbU::Unit& targetAxis ) const;
             virtual DbU::Unit           getSourceU                 () const = 0;
             virtual DbU::Unit           getTargetU                 () const = 0;
             virtual DbU::Unit           getDuSource                () const = 0;
             virtual DbU::Unit           getDuTarget                () const = 0;
      inline         DbU::Unit           getOrigin                  () const;
      inline         DbU::Unit           getExtremity               () const;
             virtual Interval            getSpanU                   () const = 0;
                     Interval            getMinSpanU                () const;
             virtual Interval            getSourceConstraints       ( Flags flags=Flags::NoFlags ) const = 0;
             virtual Interval            getTargetConstraints       ( Flags flags=Flags::NoFlags ) const = 0;
             virtual bool                getConstraints             ( DbU::Unit& min, DbU::Unit& max ) const = 0;
      inline         bool                getConstraints             ( Interval& i ) const;
      inline         const Interval&     getUserConstraints         () const;
      inline         const Interval&     getNativeConstraints       () const;
             virtual DbU::Unit           getSlack                   () const;
      inline         DbU::Unit           getOptimalMin              () const;
      inline         DbU::Unit           getOptimalMax              () const;
      inline         DbU::Unit           getNativeMin               () const;
      inline         DbU::Unit           getNativeMax               () const;
                     Interval&           getOptimal                 ( Interval& i ) const;
             virtual DbU::Unit           getCost                    ( DbU::Unit axis ) const;
             virtual AutoSegment*        getCanonical               ( DbU::Unit& min , DbU::Unit& max );
      inline         AutoSegment*        getCanonical               ( Interval& i );
                     float               getMaxUnderDensity         ( Flags flags );
      inline         uint32_t            getReduceds                () const;
                     uint32_t            getNonReduceds             ( Flags flags=Flags::WithPerpands ) const;
    // Modifiers.                                            
      inline         void                unsetFlags                 ( uint64_t );
      inline         void                setFlags                   ( uint64_t );
                     void                setFlagsOnAligneds         ( uint64_t );
      inline         void                setRpDistance              ( unsigned int );
      inline         void                incReduceds                ();
      inline         void                decReduceds                ();
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
    //inline         void                mergeUserConstraints       ( const Interval& );
                     void                mergeUserConstraints       ( const Interval& );
      inline         void                resetUserConstraints       ();
      inline         void                setOptimalMin              ( DbU::Unit min );
      inline         void                setOptimalMax              ( DbU::Unit max );
      inline         void                mergeNativeMin             ( DbU::Unit min );
      inline         void                mergeNativeMax             ( DbU::Unit max );
      inline         void                resetNativeConstraints     ( DbU::Unit min, DbU::Unit max );
                     bool                checkNotInvalidated        () const;
      inline         void                setParent                  ( AutoSegment* );
                     void                revalidate                 ();
                     AutoSegment*        makeDogleg                 ( AutoContact* );
                     Flags               makeDogleg                 ( Interval, Flags flags=Flags::NoFlags );
                     Flags               makeDogleg                 ( GCell*  , Flags flags=Flags::NoFlags );
             virtual Flags               _makeDogleg                ( GCell*  , Flags flags ) = 0;
             virtual bool                moveULeft                  () = 0;
             virtual bool                moveURight                 () = 0;
                     bool                slacken                    ( Flags flags );
             virtual bool                _slacken                   ( Flags flags ) = 0;
                     void                _changeDepth               ( unsigned int depth, Flags flags );
                     void                changeDepth                ( unsigned int depth, Flags flags );
                     bool                moveUp                     ( Flags flags=Flags::NoFlags );
                     bool                moveDown                   ( Flags flags=Flags::NoFlags );
                     bool                reduceDoglegLayer          ();
                     bool                bloatStackedStrap          ();
                     bool                reduce                     ( Flags flags=Flags::WithPerpands );
                     bool                raise                      ();
                     void                expandToMinLength          ( Interval );
                     void                unexpandToMinLength        ();
    // Canonical Modifiers.                                            
                     AutoSegment*        canonize                   ( Flags flags=Flags::NoFlags );
             virtual void                invalidate                 ( Flags flags=Flags::Propagate );
                     void                invalidate                 ( AutoContact* );
                     void                computeOptimal             ( set<AutoSegment*>& processeds );
                     void                setAxis                    ( DbU::Unit, Flags flags=Flags::NoFlags );
                     bool                toConstraintAxis           ( Flags flags=Flags::Realignate );
                     bool                toOptimalAxis              ( Flags flags=Flags::Realignate );
    // Collections & Filters.                                
                     AutoSegments        getOnSourceContact         ( Flags direction );
                     AutoSegments        getOnTargetContact         ( Flags direction );
                     AutoSegments        getCachedOnSourceContact   ( Flags direction );
                     AutoSegments        getCachedOnTargetContact   ( Flags direction );
                     AutoSegments        getAligneds                ( Flags flags=Flags::NoFlags );
                     AutoSegments        getConnecteds              ( Flags flags=Flags::NoFlags );
                     AutoSegments        getPerpandiculars          ( Flags flags=Flags::NoFlags );
                     size_t              getAlignedContacts         ( map<AutoContact*,int>& ) const ;
    // Observers.                         
      template< typename OwnerT >
      inline         OwnerT*             getObserver                ( size_t slot );
      inline         void                setObserver                ( size_t slot, BaseObserver* );
      inline         void                notify                     ( unsigned int flags );
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
                                                                    , Flags         flags );
#endif
                                  
    protected:                    
    // Internal: Static Attributes.
      static size_t                        _allocateds;
      static size_t                        _globalsCount;
      static bool                          _analogMode;
      static bool                          _shortNetMode;
      static bool                          _initialized;
      static vector< array<DbU::Unit*,4> > _extensionCaps;
    // Internal: Attributes.      
      const  unsigned long                 _id;
             GCell*                        _gcell;
             uint64_t                      _flags;
             unsigned int                  _depth        : 8;
             unsigned int                  _optimalMin   :16;
             unsigned int                  _optimalMax   :16;
             unsigned int                  _reduceds     : 2;
             unsigned int                  _rpDistance   : 4;
             DbU::Unit                     _sourcePosition;
             DbU::Unit                     _targetPosition;
             Interval                      _userConstraints;
             Interval                      _nativeConstraints;
             AutoSegment*                  _parent;
             Observable                    _observers;
                                  
    // Internal: Constructors & Destructors.
    protected:                    
                                         AutoSegment     ( Segment* segment );
             virtual                    ~AutoSegment     ();
             static  void                _preCreate      ( AutoContact* source, AutoContact* target );
             virtual void                _postCreate     ();
             virtual void                _preDestroy     ();
             static  void                _initialize     ();
    private:                                      
                                         AutoSegment     ( const AutoSegment& );
                     AutoSegment&        operator=       ( const AutoSegment& );
    protected:                                    
                     void                _invalidate     ();
      inline         uint64_t            _getFlags       () const;
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
      struct CompareBySourceU : public binary_function<AutoSegment*,AutoSegment*,bool> {
          bool operator() ( AutoSegment* lhs, AutoSegment* rhs ) const;
      };
    public:
      struct CompareByRevalidate : public binary_function<AutoSegment*,AutoSegment*,bool> {
          bool operator() ( AutoSegment* lhs, AutoSegment* rhs ) const;
      };
      struct CompareByReduceds : public binary_function<AutoSegment*,AutoSegment*,bool> {
          bool operator() ( AutoSegment* lhs, AutoSegment* rhs ) const;
      };
    public:
      typedef std::set<AutoSegment*,CompareByDepthLength>  DepthLengthSet;
      typedef std::set<AutoSegment*,CompareId>             IdSet;

    // Static Utilities.
    public:
      static inline uint64_t      swapSourceTargetFlags      ( AutoSegment* );
      static inline bool          areAlignedsAndDiffLayer    ( AutoSegment*, AutoSegment* );
      static        AutoSegment*  getGlobalThroughDogleg     ( AutoSegment* dogleg, AutoContact* from );
      static        bool          isTopologicalBound         ( AutoSegment*  seed, Flags flags );
      static inline bool          arePerpandiculars          ( AutoSegment* a, AutoSegment* b );
      static inline bool          arePerpandiculars          ( bool isHorizontalA, AutoSegment* b );
      static inline bool          areAligneds                ( AutoSegment* a, AutoSegment* b );
      static        Flags         getPerpandicularState      ( AutoContact* contact
                                                             , AutoSegment* source
                                                             , AutoSegment* current
                                                             , bool         isHorizontalMaster
                                                             , const Layer* masterLayer=NULL
                                                             );
      static inline Flags         getPerpandicularState      ( AutoContact* contact
                                                             , AutoSegment* source
                                                             , AutoSegment* current
                                                             , AutoSegment* master
                                                             );
      static void                 getTopologicalInfos        ( AutoSegment*          seed
                                                             , vector<AutoSegment*>& collapseds
                                                             , vector< tuple<AutoSegment*,Flags> >&
                                                                                     perpandiculars
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
  inline  DbU::Unit       AutoSegment::getViaToTopCap         ( size_t depth ) { return (depth < _extensionCaps.size()) ? *(_extensionCaps[depth][0]) : 0; }
  inline  DbU::Unit       AutoSegment::getViaToBottomCap      ( size_t depth ) { return (depth < _extensionCaps.size()) ? *(_extensionCaps[depth][1]) : 0; }
  inline  DbU::Unit       AutoSegment::getViaToSameCap        ( size_t depth ) { return (depth < _extensionCaps.size()) ? *(_extensionCaps[depth][2]) : 0; }
  inline  DbU::Unit       AutoSegment::getMinimalLength       ( size_t depth ) { return (depth < _extensionCaps.size()) ? *(_extensionCaps[depth][3]) : 0; }
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
  inline  unsigned int    AutoSegment::getRpDistance          () const { return _rpDistance; }
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
  inline  uint32_t        AutoSegment::getReduceds            () const { return _reduceds; }
                                                              
  inline  bool            AutoSegment::isHorizontal           () const { return _flags & SegHorizontal; }
  inline  bool            AutoSegment::isVertical             () const { return not (_flags & SegHorizontal); }
  inline  bool            AutoSegment::isFixed                () const { return _flags & SegFixed; }
  inline  bool            AutoSegment::isFixedAxis            () const { return _flags & SegFixedAxis; }
  inline  bool            AutoSegment::isGlobal               () const { return _flags & SegGlobal; }
  inline  bool            AutoSegment::isWeakGlobal           () const { return _flags & SegWeakGlobal; }
  inline  bool            AutoSegment::isLongLocal            () const { return _flags & SegLongLocal; }
  inline  bool            AutoSegment::isLocal                () const { return not (_flags & SegGlobal); }
  inline  bool            AutoSegment::isUnbreakable          () const { return _flags & SegUnbreakable; }
  inline  bool            AutoSegment::isNonPref              () const { return _flags & SegNonPref; }
  inline  bool            AutoSegment::isNonPrefOnVSmall      () const { return (_flags & SegNonPref) and (_flags & SegOnVSmall); }
  inline  bool            AutoSegment::isBipoint              () const { return _flags & SegBipoint; }
  inline  bool            AutoSegment::isWeakTerminal         () const { return (_rpDistance < 2); }
  inline  bool            AutoSegment::isWeakTerminal1        () const { return (_rpDistance == 1); }
  inline  bool            AutoSegment::isWeakTerminal2        () const { return (_rpDistance == 2); }
  inline  bool            AutoSegment::isSourceTerminal       () const { return _flags & SegSourceTerminal; }
  inline  bool            AutoSegment::isTargetTerminal       () const { return _flags & SegTargetTerminal; }
  inline  bool            AutoSegment::isTerminal             () const { return (_rpDistance == 0); }
  inline  bool            AutoSegment::isDrag                 () const { return _flags & SegDrag; }
  inline  bool            AutoSegment::isAtMinArea            () const { return _flags & SegAtMinArea; }
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
  inline  bool            AutoSegment::isAnalog               () const { return _flags & SegAnalog; }
  inline  bool            AutoSegment::isWide                 () const { return _flags & SegWide; }
  inline  bool            AutoSegment::isShortNet             () const { return _flags & SegShortNet; }
  inline  bool            AutoSegment::isNoMoveUp             () const { return _flags & SegNoMoveUp; }
  inline  void            AutoSegment::setFlags               ( uint64_t flags ) { _flags |=  flags; }
  inline  void            AutoSegment::unsetFlags             ( uint64_t flags ) { _flags &= ~flags; }
                                                              
  inline  uint64_t        AutoSegment::getFlags               () const { return _flags; }
  inline  uint64_t        AutoSegment::_getFlags              () const { return _flags; }
  inline  void            AutoSegment::setRpDistance          ( unsigned int distance ) { _rpDistance=distance; }
  inline  void            AutoSegment::incReduceds            () { if (_reduceds<3) ++_reduceds; }
  inline  void            AutoSegment::decReduceds            () { if (_reduceds>0) --_reduceds; }
  inline  void            AutoSegment::setLayer               ( const Layer* layer ) { base()->setLayer(layer); _depth=Session::getLayerDepth(layer); _flags|=SegInvalidatedLayer; }
  inline  void            AutoSegment::setWidth               ( DbU::Unit width ) { base()->setWidth(width); }
  inline  void            AutoSegment::setOptimalMin          ( DbU::Unit min ) { _optimalMin = (unsigned int)DbU::getLambda(min-getOrigin()); }
  inline  void            AutoSegment::setOptimalMax          ( DbU::Unit max ) { _optimalMax = (unsigned int)DbU::getLambda(max-getOrigin()); }
  inline  void            AutoSegment::mergeNativeMin         ( DbU::Unit min ) { _nativeConstraints.getVMin() = std::max( min, _nativeConstraints.getVMin() ); }
  inline  void            AutoSegment::mergeNativeMax         ( DbU::Unit max ) { _nativeConstraints.getVMax() = std::min( max, _nativeConstraints.getVMax() ); }
  inline  void            AutoSegment::resetNativeConstraints ( DbU::Unit min, DbU::Unit max ) { _nativeConstraints = Interval( min, max ); }
//inline  void            AutoSegment::mergeUserConstraints   ( const Interval& constraints ) { _userConstraints.intersection(constraints); }
  inline  void            AutoSegment::resetUserConstraints   () { _userConstraints = Interval(false); }
  inline  DbU::Unit       AutoSegment::getAnchoredLength      () const { return std::abs(getTargetU() - getSourceU()); }

  inline  void  AutoSegment::setLayer ( size_t depth )
  {
    RoutingLayerGauge* layerGauge = Session::getLayerGauge( depth );
    base()->setLayer( layerGauge->getLayer    () );
    base()->setWidth( layerGauge->getWireWidth() );

    _depth = depth;
    _flags|=SegInvalidatedLayer;
  }

  inline  DbU::Unit  AutoSegment::getContactWidth () const
  { return getWidth() + Session::getViaWidth(getLayer()) - Session::getWireWidth(getLayer()); }

  inline  DbU::Unit  AutoSegment::getSpanLength () const
  { return getAnchoredLength() + getExtensionCap( Flags::Source|Flags::LayerCapOnly|Flags::NoMinLength )
                               + getExtensionCap( Flags::Target|Flags::LayerCapOnly|Flags::NoMinLength );
  }

  inline  void  AutoSegment::setParent ( AutoSegment* parent )
  {
    if ( parent == this ) {
      cerr << "Parentage Looping: " << parent->_getString() << endl;
    }
    _parent = parent;
  }

  inline bool  AutoSegment::CompareId::operator() ( const AutoSegment* lhs, const AutoSegment* rhs ) const
  { return lhs->getId() < rhs->getId(); }
 
  inline uint64_t  AutoSegment::swapSourceTargetFlags ( AutoSegment* segment )
  {
    uint64_t segFlags  = segment->getFlags();
    uint64_t swapFlags = segment->getFlags() & ~(SegSourceTop        |SegTargetTop
                                                |SegSourceBottom     |SegTargetBottom
                                                |SegSourceTerminal   |SegTargetTerminal
                                                |SegNotSourceAligned |SegNotTargetAligned
                                                |SegInvalidatedSource|SegInvalidatedTarget
                                                );

    swapFlags |= (segFlags & SegSourceTop        ) ? SegTargetTop         : SegNoFlags;
    swapFlags |= (segFlags & SegSourceBottom     ) ? SegTargetBottom      : SegNoFlags;
    swapFlags |= (segFlags & SegSourceTerminal   ) ? SegTargetTerminal    : SegNoFlags;
    swapFlags |= (segFlags & SegNotSourceAligned ) ? SegNotTargetAligned  : SegNoFlags;
    swapFlags |= (segFlags & SegInvalidatedSource) ? SegInvalidatedTarget : SegNoFlags;

    swapFlags |= (segFlags & SegTargetTop        ) ? SegSourceTop         : SegNoFlags;
    swapFlags |= (segFlags & SegTargetBottom     ) ? SegSourceBottom      : SegNoFlags;
    swapFlags |= (segFlags & SegTargetTerminal   ) ? SegSourceTerminal    : SegNoFlags;
    swapFlags |= (segFlags & SegNotTargetAligned ) ? SegNotSourceAligned  : SegNoFlags;
    swapFlags |= (segFlags & SegInvalidatedTarget) ? SegInvalidatedSource : SegNoFlags;
    return swapFlags;
  }

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

  inline Flags  AutoSegment::getPerpandicularState ( AutoContact* contact
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
    vector< tuple<AutoSegment*,Flags> >
                          perpandiculars;
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
