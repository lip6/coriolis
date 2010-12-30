
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
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./AutoSegment.h"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __KATABATIC_AUTOSEGMENT__
#define  __KATABATIC_AUTOSEGMENT__

#include  <set>
#include  <iostream>
#include  <functional>
#include  <tr1/functional>

#include  "hurricane/Interval.h"
#include  "hurricane/Segment.h"
#include  "hurricane/Components.h"
#include  "hurricane/Contact.h"
namespace Hurricane {
  class Layer;
  class Horizontal;
  class Vertical;
  class Cell;
}

#include  "katabatic/GCell.h"
#include  "katabatic/AutoSegments.h"
#include  "katabatic/Session.h"


namespace Katabatic {


  using std::set;
  using std::cerr;
  using std::endl;
  using std::binary_function;
  using Hurricane::tab;
  using Hurricane::inltrace;
  using Hurricane::Interval;
  using Hurricane::Layer;
  using Hurricane::Components;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Hurricane::Cell;


  enum AutoSegmentFlags { Realignate=0x1, AxisSet=0x2 };


// -------------------------------------------------------------------
// Class  :  "AutoSegment".
 

  class AutoSegment {

    public:
    // Types.
      typedef  std::tr1::function< void(AutoSegment*) >  RevalidateCb_t;
    // Enumerations.
      enum Type               { Global = 1
                              , Local  = 2
                              , Guess  = 3
                              };
      enum PerpandicularState { PerpandicularAny       = (1<<0)
                              , PerpandicularIndirect  = (1<<1)
                              , ParallelOrExpanded     = (1<<2)
                              , ParallelAndLayerChange = (1<<3)
                              };
      enum Flags              { Propagate    =0x1
                              , AllowLocal   =0x2
                              , AllowTerminal=0x4
                              , IgnoreContact=0x8
                              };


    public:
      struct CompareId : public binary_function<AutoSegment*,AutoSegment*,bool> {
          inline bool  operator() ( const AutoSegment* lhs, const AutoSegment* rhs ) const;
      };
    public:
      struct CompareByDepthLength : public binary_function<AutoSegment*,AutoSegment*,bool> {
          bool operator() ( AutoSegment* lhs, AutoSegment* rhs ) const;
      };

    public:
    // Utilities.
      static        bool          isTopologicalBound         ( AutoSegment*  seed
                                                             , bool          superior
                                                             , bool          isHorizontal
                                                             );
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
    // Constructors & Destructor.                            
      static  AutoSegment*        create                     ( AutoContact*  source
                                                             , AutoContact*  target
                                                             , Segment*      hurricaneSegment
                                                             );
      static  AutoSegment*        create                     ( AutoContact*  source
                                                             , AutoContact*  target
                                                             , unsigned int  dir
                                                             , int           type
                                                             , bool          terminal=false
                                                             , bool          collapsed=false
                                                             );
              void                destroy                    ();
    // Wrapped Segment Functions.                            
      virtual Segment*            base                       () = 0;
      virtual Segment*            base                       () const = 0;
      virtual Segment*            getSegment                 () = 0;
      virtual Segment*            getSegment                 () const = 0;
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
    // Predicates.                                           
      inline  bool                isHorizontal               () const;
      inline  bool                isVertical                 () const;
      inline  bool                isInvalidated              () const;
      inline  bool                isGlobal                   () const;
      inline  bool                isLocal                    () const;
      inline  bool                isCanonicalLocal           () const;
      inline  bool                isTerminal                 () const;
      inline  bool                isCollapsed                () const;
      inline  bool                isCanonical                () const;
      inline  bool                isFixed                    () const;
      inline  bool                isStrap                    () const;
              bool                isCanonicalStrap           () const;
      inline  bool                isLayerChange              () const;
      inline  bool                isAccountable              () const;
      inline  bool                isUnsetAxis                () const;
      inline  bool                isSlackened                () const;
      inline  bool                isSlackenStrap             () const;
      inline  bool                allowOutsideGCell          () const;
              bool                canDesalignate             ();
      virtual bool                canDesalignate             ( AutoContact* ) const = 0;
              bool                canMoveUp                  ( float reserve=0.0, unsigned int flags=0 );
            //bool                canPivotUp                 ( bool propagate=false, float reserve=0.0 );
              bool                canPivotUp                 ( float reserve=0.0, unsigned int flags=0 );
              bool                canPivotDown               ( bool propagate=false, float reserve=0.0 );
              bool                canSlacken                 ( bool propagate=false );
      virtual bool                _canSlacken                () const = 0;
              bool                canGoOutsideGCell          () const;
    // Accessors.                                            
      inline  unsigned long       getId                      () const;
      virtual unsigned int        getDirection               () const = 0;
      inline  GCell*              getGCell                   () const;
      virtual size_t              getGCells                  ( vector<GCell*>& ) const = 0;
      inline  AutoContact*        getAutoSource              () const;
      inline  AutoContact*        getAutoTarget              () const;
              AutoContact*        getOppositeAnchor          ( AutoContact* ) const;
              size_t              getAlignedContacts         ( map<AutoContact*,int>& );
              size_t              getPerpandicularsBound     ( set<AutoSegment*>& );
      inline  AutoSegment*        getParent                  () const;
      inline  DbU::Unit           getAxis                    () const;
      virtual DbU::Unit           getSourceU                 () const = 0;
      virtual DbU::Unit           getTargetU                 () const = 0;
      virtual DbU::Unit           getDuSource                () const = 0;
      virtual DbU::Unit           getDuTarget                () const = 0;
      inline  DbU::Unit           getOrigin                  () const;
      inline  DbU::Unit           getExtremity               () const;
      virtual Interval            getSpanU                   () const = 0;
              Interval            getMinSpanU                ();
      virtual Interval            getSourceConstraints       ( bool native=false ) const = 0;
      virtual Interval            getTargetConstraints       ( bool native=false ) const = 0;
      virtual bool                getConstraints             ( DbU::Unit& min, DbU::Unit& max ) const = 0;
      inline  bool                getConstraints             ( Interval& i ) const;
      inline  const Interval&     getUserConstraints         () const;
      virtual DbU::Unit           getSlack                   () const;
      inline  DbU::Unit           getOptimalMin              () const;
      inline  DbU::Unit           getOptimalMax              () const;
              Interval&           getOptimal                 ( Interval& i ) const;
      virtual DbU::Unit           getCost                    ( DbU::Unit axis ) const;
      virtual AutoSegment*        getCanonical               ( DbU::Unit& min , DbU::Unit& max );
      inline  AutoSegment*        getCanonical               ( Interval& i );
    // Collections & Filters.                                
              AutoSegments        getOnSourceContact         ( unsigned int direction );
              AutoSegments        getOnTargetContact         ( unsigned int direction );
              AutoSegments        getCollapseds              ( bool withPerpand=false );
              AutoSegments        getCollapsedPerpandiculars ();
      static  AutoSegmentFilter   getIsAccountable           () { return new AutoSegments_IsAccountable(); };
    // Static Modifiers.                                     
      static  void                setDestroyMode             ( bool );
    // Modifiers.                                            
      inline  void                setGlobal                  ( bool );
      inline  void                setCanonicalLocal          ( bool );
      inline  void                setCanonical               ( bool );
      inline  void                setTerminal                ( bool );
      inline  void                setFixed                   ( bool );
      inline  void                setStrap                   ( bool );
      inline  void                setLayerChange             ( bool );
      inline  void                setSlackened               ( bool );
      inline  void                setSlackenStrap            ( bool );
              void                setAllowOutsideGCell       ( bool state, bool propagate=false );
              void                _setAllowOutsideGCell      ( bool );
      inline  void                setLayer                   ( const Layer* layer );
              void                setAxis                    ( DbU::Unit          axis
                                                             , unsigned int       flags=AxisSet 
                                                             , set<AutoSegment*>* processeds=NULL );
      virtual void                setDuSource                ( DbU::Unit du ) = 0;
      virtual void                setDuTarget                ( DbU::Unit du ) = 0;
      virtual void                orient                     () = 0;
      virtual void                setPositions               () = 0;
      virtual bool                checkPositions             () const = 0;
      virtual bool                checkConstraints           () const = 0;
      inline  void                mergeUserConstraints       ( const Interval& );
      inline  void                resetUserConstraints       ();
      virtual void                invalidate                 ();
              void                revalidate                 ();
              bool                collapse                   ();
              bool                expand                     ();
              bool                toConstraintAxis           ( set<AutoSegment*>* processeds=NULL );
              bool                toOptimalAxis              ( set<AutoSegment*>* processeds=NULL );
      virtual void                alignate                   ( DbU::Unit axis ) = 0;
      inline  void                setOptimalMin              ( DbU::Unit min );
      inline  void                setOptimalMax              ( DbU::Unit max );
              void                computeOptimal             ( set<AutoSegment*>* processeds=NULL );
              void                _computeTerminal           ( Segment* );
      virtual void                _computeTerminal           () = 0;
      virtual bool                checkInvalidated           () const;
      inline  void                setParent                  ( AutoSegment* );
              AutoSegment*        canonize                   ();
              void                changeDepth                ( unsigned int depth
                                                             , bool         propagate =false
                                                             , bool         standAlone=true
                                                             );
              void                _changeDepth               ( unsigned int depth, bool withNeighbors );
              bool                moveUp                     ( unsigned int flags=0 );
              bool                moveDown                   ( unsigned int flags=0 );
      virtual void                moveULeft                  () = 0;
      virtual void                moveURight                 () = 0;
              void                slacken                    ( bool propagate=false );
      virtual void                _slacken                   () = 0;
              bool                canDogLeg                  ( Interval );
              void                makeDogLeg                 ( Interval, bool upLayer, bool& leftDogleg );
              void                makeDogLeg                 ( GCell*, bool upLayer );
      virtual void                _makeDogLeg                ( GCell*, bool upLayer ) = 0;
      virtual void                desalignate                ( AutoContact* ) = 0;
              void                desalignate                ();
              bool                shearUp                    ( GCell*
                                                             , AutoSegment*& movedUp
                                                             , float         reserve
                                                             , unsigned int  flags );
              bool                _check                     () const;
    // Inspector Management.                                 
      virtual Record*             _getRecord                 () const = 0;
      virtual string              _getString                 () const = 0;
      virtual string              _getTypeName               () const = 0;
                                  
    protected:                    
    // Internal: Static Attributes.
      static size_t               _allocateds;
      static size_t               _globalsCount;
      static bool                 _destroyBase;
      static bool                 _destroyTool;
      static unsigned long        _maxId;
    // Internal: Attributes.      
             GCell*               _gcell;
             bool                 _isUnsetAxis;
             bool                 _invalidated;
             bool                 _isHorizontal;
             bool                 _isGlobal;
             bool                 _isCanonicalLocal;
             bool                 _isTerminal;
             bool                 _isCollapsed;
             bool                 _isCanonical;
             bool                 _isFixed;
             bool                 _strap;
             bool                 _layerChange;
             bool                 _slackened;
             bool                 _slackenStrap;
             bool                 _allowOutsideGCell;
      const  unsigned long        _id;
             unsigned int         _optimalMin : 8;
             unsigned int         _optimalMax : 8;
             DbU::Unit            _sourcePosition;
             DbU::Unit            _targetPosition;
             Interval             _userConstraints;
             AutoSegment*         _parent;
                                  
    // Internal: Constructors & Destructors.
    protected:                    
                                  AutoSegment        ( Segment* segment
                                                     , bool     isHorizontal
                                                     , int      type
                                                     , bool     terminal
                                                     , bool     collapsed
                                                     );
      virtual                    ~AutoSegment        ();
      static  void                _preCreate         ( Component*   source, Component*   target );
      static  void                _preCreate         ( AutoContact* source, AutoContact* target );
      virtual void                _postCreate        ();
      virtual void                _preDestroy        ();
    private:                                         
                                  AutoSegment        ( const AutoSegment& );
              AutoSegment&        operator=          ( const AutoSegment& );
    private:
      inline  void                setInvalidated     ( bool state );
              void                _invalidate        ();

  };


// Inline Functions.
  inline  unsigned long   AutoSegment::getId                () const { return _id; }
  inline  Cell*           AutoSegment::getCell              () const { return getSegment()->getCell(); }
  inline  Net*            AutoSegment::getNet               () const { return getSegment()->getNet(); }
  inline  const Layer*    AutoSegment::getLayer             () const { return getSegment()->getLayer(); }
  inline  Box             AutoSegment::getBoundingBox       () const { return getSegment()->getBoundingBox(); }
  inline  Hook*           AutoSegment::getSourceHook        () { return getSegment()->getSourceHook(); }
  inline  Hook*           AutoSegment::getTargetHook        () { return getSegment()->getTargetHook(); }
  inline  Contact*        AutoSegment::getSource            () const { return static_cast<Contact*>(getSegment()->getSource()); }
  inline  Contact*        AutoSegment::getTarget            () const { return static_cast<Contact*>(getSegment()->getTarget()); }
  inline  Component*      AutoSegment::getOppositeAnchor    ( Component* anchor ) const { return getSegment()->getOppositeAnchor(anchor); };
  inline  AutoSegment*    AutoSegment::getParent            () const { return _parent; }
  inline  DbU::Unit       AutoSegment::getSourcePosition    () const { return _sourcePosition; }
  inline  DbU::Unit       AutoSegment::getTargetPosition    () const { return _targetPosition; }
  inline  DbU::Unit       AutoSegment::getSourceX           () const { return getSegment()->getSourceX(); }
  inline  DbU::Unit       AutoSegment::getSourceY           () const { return getSegment()->getSourceY(); }
  inline  DbU::Unit       AutoSegment::getTargetX           () const { return getSegment()->getTargetX(); }
  inline  DbU::Unit       AutoSegment::getTargetY           () const { return getSegment()->getTargetY(); }
  inline  DbU::Unit       AutoSegment::getWidth             () const { return getSegment()->getWidth(); }
  inline  DbU::Unit       AutoSegment::getLength            () const { return getSegment()->getLength(); }
  inline  void            AutoSegment::invert               () { getSegment()->invert(); }
  inline  GCell*          AutoSegment::getGCell             () const { return _gcell; }
  inline  AutoContact*    AutoSegment::getAutoSource        () const { return Session::lookup(getSource()); }
  inline  AutoContact*    AutoSegment::getAutoTarget        () const { return Session::lookup(getTarget()); }
  inline  bool            AutoSegment::getConstraints       ( Interval& i ) const { return getConstraints(i.getVMin(),i.getVMax()); }
  inline  AutoSegment*    AutoSegment::getCanonical         ( Interval& i ) { return getCanonical(i.getVMin(),i.getVMax()); }
  inline  DbU::Unit       AutoSegment::getAxis              () const { return _isHorizontal?getSegment()->getY():getSegment()->getX(); }
  inline  DbU::Unit       AutoSegment::getOrigin            () const { return _isHorizontal?_gcell->getY():_gcell->getX(); }
  inline  DbU::Unit       AutoSegment::getExtremity         () const { return _isHorizontal?_gcell->getYMax():_gcell->getXMax(); }
  inline  DbU::Unit       AutoSegment::getOptimalMin        () const { return DbU::lambda(_optimalMin) + getOrigin(); }
  inline  DbU::Unit       AutoSegment::getOptimalMax        () const { return DbU::lambda(_optimalMax) + getOrigin(); }
  inline  const Interval& AutoSegment::getUserConstraints   () const { return _userConstraints; }
                                                           
  inline  bool            AutoSegment::isInvalidated        () const { return  _invalidated; }
  inline  bool            AutoSegment::isHorizontal         () const { return  _isHorizontal; }
  inline  bool            AutoSegment::isVertical           () const { return !_isHorizontal; }
  inline  bool            AutoSegment::isGlobal             () const { return  _isGlobal; }
  inline  bool            AutoSegment::isCanonicalLocal     () const { return  _isCanonicalLocal; }
  inline  bool            AutoSegment::isLocal              () const { return !_isGlobal; }
  inline  bool            AutoSegment::isTerminal           () const { return  _isTerminal; }
  inline  bool            AutoSegment::isCollapsed          () const { return  _isCollapsed; }
  inline  bool            AutoSegment::isCanonical          () const { return  _isCanonical; }
  inline  bool            AutoSegment::isFixed              () const { return  _isFixed; }
  inline  bool            AutoSegment::isStrap              () const { return  _strap; }
  inline  bool            AutoSegment::isLayerChange        () const { return  _layerChange; }
  inline  bool            AutoSegment::isAccountable        () const { return  _isCanonical && !_isCollapsed; }
  inline  bool            AutoSegment::isUnsetAxis          () const { return  _isUnsetAxis; }
  inline  bool            AutoSegment::isSlackened          () const { return  _slackened; }
  inline  bool            AutoSegment::isSlackenStrap       () const { return  _slackenStrap; }
  inline  bool            AutoSegment::allowOutsideGCell    () const { return  _allowOutsideGCell; }
                                                            
  inline  void            AutoSegment::setLayer             ( const Layer* layer ) { invalidate(); getSegment()->setLayer(layer); }
  inline  void            AutoSegment::setInvalidated       ( bool state ) { _invalidated = state; }
  inline  void            AutoSegment::setGlobal            ( bool state ) { _isGlobal = state; }
  inline  void            AutoSegment::setCanonicalLocal    ( bool state ) { _isCanonicalLocal = state; }
  inline  void            AutoSegment::setTerminal          ( bool state ) { _isTerminal = state; }
  inline  void            AutoSegment::setFixed             ( bool state ) { _isFixed = state; }
  inline  void            AutoSegment::setStrap             ( bool state ) { _strap = state; }
  inline  void            AutoSegment::setLayerChange       ( bool state ) { _layerChange = state; }
  inline  void            AutoSegment::setSlackened         ( bool state ) { _slackened = state; }
  inline  void            AutoSegment::setSlackenStrap      ( bool state ) { _slackenStrap = state; }
  inline  void            AutoSegment::setOptimalMin        ( DbU::Unit min ) { _optimalMin = (unsigned int)DbU::getLambda(min-getOrigin()); }
  inline  void            AutoSegment::setOptimalMax        ( DbU::Unit max ) { _optimalMax = (unsigned int)DbU::getLambda(max-getOrigin()); }
  inline  void            AutoSegment::mergeUserConstraints ( const Interval& constraints ) { _userConstraints.intersection(constraints); }
  inline  void            AutoSegment::resetUserConstraints () { _userConstraints = Interval(false); }

  inline  void  AutoSegment::setParent ( AutoSegment* parent )
  {
    if ( parent == this ) {
      cerr << "Parentage Looping: " << parent->_getString() << endl;
    }
    _parent = parent;
  }


  inline bool  AutoSegment::CompareId::operator() ( const AutoSegment* lhs, const AutoSegment* rhs ) const
  { return lhs->getId() < rhs->getId(); }

  inline  void  AutoSegment::setCanonical ( bool state )
  {
    if ( _isCanonical != state ) {
    //ltrace(159) << "canonical:" << state << " " << (void*)this << " " << _getString() << endl;
      _isCanonical = state;
    }
  }
  
  inline unsigned long AutoSegment::getMaxId ()
  { return _maxId; }

  inline bool  AutoSegment::arePerpandiculars ( AutoSegment* a, AutoSegment* b )
  { return a->isHorizontal() != b->isHorizontal(); }

  inline bool  AutoSegment::arePerpandiculars ( bool isHorizontalA, AutoSegment* b )
  { return isHorizontalA != b->isHorizontal(); }

  inline bool  AutoSegment::areAligneds ( AutoSegment* a, AutoSegment* b )
  { return a->isHorizontal() == b->isHorizontal(); }

  inline unsigned int  AutoSegment::getPerpandicularState ( AutoContact* contact
                                                          , AutoSegment* source
                                                          , AutoSegment* current
                                                          , AutoSegment* master  )
  {
    return getPerpandicularState ( contact, source, current, master->isHorizontal(), master->getLayer() );
  }


  inline int  AutoSegment::getTerminalCount ( AutoSegment* seed )
  {
    ltrace(80) << "getTerminalCount() - " << seed << endl;

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


} // End of Katabatic namespace.


INSPECTOR_P_SUPPORT(Katabatic::AutoSegment);


# endif  // __KATABATIC_AUTOSEGMENT__
