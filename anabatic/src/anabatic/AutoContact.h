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
// |  C++ Header  :  "./anabatic/AutoContact.h"                      |
// +-----------------------------------------------------------------+


#ifndef  ANABATIC_AUTOCONTACT_H
#define  ANABATIC_AUTOCONTACT_H

#include <vector>
#include <map>
#include "hurricane/Contact.h"
#include "hurricane/ExtensionGo.h"
#include "anabatic/Constants.h"
#include "anabatic/AutoSegment.h"
#include "anabatic/GCell.h"


namespace Anabatic {


  using std::cerr;
  using std::endl;
  using Hurricane::order;
  using Hurricane::setInBound;
  using Hurricane::tab;
  using Hurricane::Name;
  using Hurricane::Entity;
  using Hurricane::Net;
  using Hurricane::Component;
  using Hurricane::Components;
  using Hurricane::Layer;
  using Hurricane::Contact;
  using Hurricane::ExtensionGo;

  class AnabaticEngine;
  class AutoHorizontal;
  class AutoVertical;
  class AutoContact;


  typedef  std::map<Contact*,AutoContact*,Entity::CompareById>  AutoContactLut;


// -------------------------------------------------------------------
// Class  :  "Anabatic::AutoContact".

  enum AutoContactFlag { CntFixed                 = (1 <<  0)
                       , CntTerminal              = (1 <<  1)
                       , CntTurn                  = (1 <<  2)
                       , CntHTee                  = (1 <<  3)
                       , CntVTee                  = (1 <<  4)
                       , CntInvalidated           = (1 <<  6)
                       , CntInvalidatedCache      = (1 <<  7)
                       , CntInvalidatedWidth      = (1 <<  8)
                       , CntInCreationStage       = (1 <<  9)
                       , CntBadTopology           = (1 << 10)
                       , CntIgnoreAnchor          = (1 << 11)
                       , CntWeakTerminal          = (1 << 12)
                       , CntUserNativeConstraints = (1 << 13)
                       , CntOnVertical            = (1 << 14)
                       , CntOnHorizontal          = (1 << 15)
                       , CntDrag                  = (1 << 16)
                       , CntHDogleg               = (1 << 17)
                       , CntVDogleg               = (1 << 18)
                       };

  class AutoContact {
    public:
      static  AutoContact*     createFrom                 ( Contact* );
    public:
    // Wrapped Contact Accessors.
      inline  Hook*            getBodyHook                ();
      inline  Hook*            getAnchorHook              ();
      inline  Component*       getAnchor                  () const;
      inline  Net*             getNet                     () const;
      inline  const Layer*     getLayer                   () const;
      inline  DbU::Unit        getX                       () const;
      inline  DbU::Unit        getY                       () const;
      inline  DbU::Unit        getDx                      () const;
      inline  DbU::Unit        getDy                      () const;
      inline  Point            getCenter                  () const;
      inline  Point            getPosition                () const;
      inline  DbU::Unit        getWidth                   () const;
      inline  DbU::Unit        getHalfWidth               () const;
      inline  DbU::Unit        getHeight                  () const;
      inline  DbU::Unit        getHalfHeight              () const;
      inline  Components       getSlaveComponents         () const;
    // Wrapped Contact Modifiers.                         
      inline  void             setLayer                   ( const Layer* );
      inline  void             setWidth                   ( DbU::Unit );
      inline  void             setHeight                  ( DbU::Unit );
      inline  void             setSizes                   ( DbU::Unit width, DbU::Unit height );
      inline  void             setX                       ( DbU::Unit );
      inline  void             setY                       ( DbU::Unit );
      inline  void             setPosition                ( DbU::Unit x, DbU::Unit y );
      inline  void             setPosition                ( const Point& );
      inline  void             setDx                      ( DbU::Unit );
      inline  void             setDy                      ( DbU::Unit );
      inline  void             setOffset                  ( DbU::Unit dx, DbU::Unit dy );
      virtual void             translate                  ( const DbU::Unit& tx, const DbU::Unit& ty );
    // Predicates.
      inline  bool             isInCreationStage          () const;
      inline  bool             isInvalidated              () const;
      inline  bool             isInvalidatedCache         () const;
      inline  bool             isInvalidatedWidth         () const;
      inline  bool             isTerminal                 () const;
      inline  bool             isTurn                     () const;
              bool             isTee                      ( Flags direction ) const;
      inline  bool             isHTee                     () const;
      inline  bool             isVTee                     () const;
      inline  bool             isFixed                    () const;
      inline  bool             isUserNativeConstraints    () const;
      inline  bool             isHDogleg                  () const;
      inline  bool             isVDogleg                  () const;
      virtual bool             isOnPin                    () const;
      inline  bool             hasBadTopology             () const;
              bool             canDestroy                 ( Flags flags=Flags::NoFlags ) const;
              bool             canMoveUp                  ( const AutoSegment* moved ) const;
      inline  bool             canDrag                    () const;
    // Accessors.                                        
      inline  Contact*         base                       () const;
      static  size_t           getAllocateds              ();
      static  const Name&      getStaticName              ();
      virtual const Name&      getName                    () const;
      inline  size_t           getId                      () const;
      inline  Flags            getFlags                   () const;
      virtual Box              getBoundingBox             () const;
      inline  GCell*           getGCell                   () const;
      virtual AutoSegment*     getOpposite                ( const AutoSegment* ) const = 0;
      virtual AutoSegment*     getPerpandicular           ( const AutoSegment* ) const = 0;
      virtual AutoSegment*     getSegment                 ( unsigned int ) const = 0;
      virtual AutoHorizontal*  getHorizontal1             () const;
      virtual AutoHorizontal*  getHorizontal2             () const;
      virtual AutoVertical*    getVertical1               () const;
      virtual AutoVertical*    getVertical2               () const;
              void             getDepthSpan               ( size_t& minDepth, size_t& maxDepth ) const;
      inline  unsigned int     getMinDepth                () const;
      inline  unsigned int     getMaxDepth                () const;
              void             getLengths                 ( DbU::Unit* lengths, AutoSegment::DepthLengthSet& );
      virtual Box              getNativeConstraintBox     () const;
              Interval         getNativeUConstraints      ( Flags direction ) const;
              Interval         getUConstraints            ( Flags direction ) const;
      inline  DbU::Unit        getCBXMin                  () const;
      inline  DbU::Unit        getCBXMax                  () const;
      inline  DbU::Unit        getCBYMin                  () const;
      inline  DbU::Unit        getCBYMax                  () const;
      inline  Box              getConstraintBox           () const;
              Box&             intersectConstraintBox     ( Box& box ) const;
    // Collections.
              AutoSegments     getAutoSegments            ();
    // Modifiers.              
              void             invalidate                 ( Flags flags=Flags::NoFlags );
      virtual void             cacheDetach                ( AutoSegment* ) = 0;
      virtual void             cacheAttach                ( AutoSegment* ) = 0;
      virtual void             updateCache                () = 0;
              void             updateLayer                ();
              void             updateSize                 ();
      virtual void             updateGeometry             () = 0;
      virtual void             updateTopology             () = 0;
              void             showTopologyError          ( const std::string&, Flags flags=Flags::NoFlags );
      virtual void             checkTopology              ();
      virtual void             forceOnGrid                ( Point );
      inline  void             setFlags                   ( Flags );
      inline  void             unsetFlags                 ( Flags );
              void             setGCell                   ( GCell* );
      inline  void             setCBXMin                  ( DbU::Unit xMin );
      inline  void             setCBXMax                  ( DbU::Unit xMax );
      inline  void             setCBYMin                  ( DbU::Unit yMin );
      inline  void             setCBYMax                  ( DbU::Unit yMax );
              void             setConstraintBox           ( const Box& box );
              bool             restrictConstraintBox      ( DbU::Unit constraintMin
                                                          , DbU::Unit constraintMax
                                                          , Flags flags=Flags::WarnOnError );
              void             restoreNativeConstraintBox ();
              void             migrateConstraintBox       ( AutoContact* other );
              void             setLayerAndWidth           ( size_t delta, size_t depth );
              void             destroy                    ();
    // Inspector Management.
              Record*          _getRecord                 () const;
      virtual string           _getString                 () const;
      virtual string           _getTypeName               () const;
                              
    private:                  
    // Internal: Attributes.
      static  size_t           _maxId;
      static  size_t           _allocateds;
      static  const Name       _goName;

    protected:                
              size_t           _id;
              Contact*         _contact;
              GCell*           _gcell;
              Flags            _flags;
              DbU::Unit        _xMin;
              DbU::Unit        _xMax;
              DbU::Unit        _yMin;
              DbU::Unit        _yMax;

    protected:
    // Constructors & Destructors.
                               AutoContact  ( GCell*, Contact* );
      virtual                 ~AutoContact  ();
      static  void             _preCreate   ( GCell*, Net*, const Layer* );
      virtual void             _postCreate  ();
      virtual void             _preDestroy  ();
    private:                               
                               AutoContact  ( const AutoContact& );
              AutoContact&     operator=    ( const AutoContact& );

    protected:
      inline  int              _boundX      ( DbU::Unit x ) const;
      inline  int              _boundY      ( DbU::Unit x ) const;
      static  void             _getTopology ( Contact*, Component*& anchor, Horizontal**&, Vertical**&, size_t );
      virtual void             _invalidate  ( Flags flags ) = 0;
  };


// Wrapped Contact Inline Functions.
  inline Hook*          AutoContact::getBodyHook            () { return _contact->getBodyHook(); }
  inline Hook*          AutoContact::getAnchorHook          () { return _contact->getAnchorHook(); }
  inline Component*     AutoContact::getAnchor              () const { return _contact->getAnchor(); }
  inline Net*           AutoContact::getNet                 () const { return _contact->getNet(); }
  inline const Layer*   AutoContact::getLayer               () const { return _contact->getLayer(); }
  inline DbU::Unit      AutoContact::getX                   () const { return _contact->getX(); }
  inline DbU::Unit      AutoContact::getY                   () const { return _contact->getY(); }
  inline DbU::Unit      AutoContact::getDx                  () const { return _contact->getDx(); }
  inline DbU::Unit      AutoContact::getDy                  () const { return _contact->getDy(); }
  inline Point          AutoContact::getCenter              () const { return _contact->getCenter(); }
  inline Point          AutoContact::getPosition            () const { return _contact->getPosition(); }
  inline DbU::Unit      AutoContact::getWidth               () const { return _contact->getWidth(); }
  inline DbU::Unit      AutoContact::getHalfWidth           () const { return _contact->getHalfWidth(); }
  inline DbU::Unit      AutoContact::getHeight              () const { return _contact->getHeight(); }
  inline DbU::Unit      AutoContact::getHalfHeight          () const { return _contact->getHalfHeight(); }
  inline Components     AutoContact::getSlaveComponents     () const { return _contact->getSlaveComponents(); }
  inline void           AutoContact::setLayer               ( const Layer* layer ) { _contact->setLayer(layer); }
  inline void           AutoContact::setWidth               ( DbU::Unit w ) { _contact->setWidth(w); }
  inline void           AutoContact::setHeight              ( DbU::Unit h ) { _contact->setHeight(h); }
  inline void           AutoContact::setSizes               ( DbU::Unit w, DbU::Unit h ) { _contact->setSizes(w,h); }
  inline void           AutoContact::setX                   ( DbU::Unit x ) { _contact->setX(x); }
  inline void           AutoContact::setY                   ( DbU::Unit y ) { _contact->setY(y); }
  inline void           AutoContact::setPosition            ( DbU::Unit x, DbU::Unit y ) { _contact->setPosition(x,y); }
  inline void           AutoContact::setPosition            ( const Point& p ) { _contact->setPosition(p); }
  inline void           AutoContact::setDx                  ( DbU::Unit dx ) { _contact->setDx(dx); }
  inline void           AutoContact::setDy                  ( DbU::Unit dy ) { _contact->setDy(dy); }
  inline void           AutoContact::setOffset              ( DbU::Unit dx, DbU::Unit dy ) { _contact->setOffset(dx,dy); }
// AutoContact Inline Functions.                                        
  inline bool          AutoContact::isInCreationStage       () const { return _flags&CntInCreationStage; }
  inline bool          AutoContact::isInvalidated           () const { return _flags&CntInvalidated; }
  inline bool          AutoContact::isInvalidatedCache      () const { return _flags&CntInvalidatedCache; }
  inline bool          AutoContact::isInvalidatedWidth      () const { return _flags&CntInvalidatedWidth; }
  inline bool          AutoContact::isTurn                  () const { return _flags&CntTurn; }
  inline bool          AutoContact::isFixed                 () const { return _flags&CntFixed; }
  inline bool          AutoContact::isUserNativeConstraints () const { return _flags&CntUserNativeConstraints; }
  inline bool          AutoContact::isTerminal              () const { return _flags&CntTerminal; }
  inline bool          AutoContact::isHTee                  () const { return _flags&CntHTee; }
  inline bool          AutoContact::isVTee                  () const { return _flags&CntVTee; }
  inline bool          AutoContact::isHDogleg               () const { return _flags&CntHDogleg; }
  inline bool          AutoContact::isVDogleg               () const { return _flags&CntVDogleg; }
  inline bool          AutoContact::hasBadTopology          () const { return _flags&CntBadTopology; }
  inline bool          AutoContact::canDrag                 () const { return _flags&CntDrag; }
  inline size_t        AutoContact::getId                   () const { return _id; }
  inline Flags         AutoContact::getFlags                () const { return _flags; }
  inline Contact*      AutoContact::base                    () const { return _contact; }
  inline GCell*        AutoContact::getGCell                () const { return _gcell; }
  inline Box           AutoContact::getConstraintBox        () const { return Box(getCBXMin(),getCBYMin(),getCBXMax(),getCBYMax()); }
  inline void          AutoContact::setCBXMin               ( DbU::Unit xMin ) { _xMin = _boundX(xMin); }
  inline void          AutoContact::setCBXMax               ( DbU::Unit xMax ) { _xMax = _boundX(xMax); }
  inline void          AutoContact::setCBYMin               ( DbU::Unit yMin ) { _yMin = _boundY(yMin); }
  inline void          AutoContact::setCBYMax               ( DbU::Unit yMax ) { _yMax = _boundY(yMax); }
  inline void          AutoContact::setFlags                ( Flags flags ) { _flags|= flags; }
  inline void          AutoContact::unsetFlags              ( Flags flags ) { _flags&=~flags; }
  inline DbU::Unit     AutoContact::getCBXMin               () const { return isFixed() ? _contact->getX() : _xMin; }
  inline DbU::Unit     AutoContact::getCBXMax               () const { return isFixed() ? _contact->getX() : _xMax; }
  inline DbU::Unit     AutoContact::getCBYMin               () const { return isFixed() ? _contact->getY() : _yMin; }
  inline DbU::Unit     AutoContact::getCBYMax               () const { return isFixed() ? _contact->getY() : _yMax; }

  inline unsigned int  AutoContact::getMinDepth () const
  { size_t minDepth, maxDepth; getDepthSpan(minDepth,maxDepth); return minDepth; }

  inline unsigned int  AutoContact::getMaxDepth () const
  { size_t minDepth, maxDepth; getDepthSpan(minDepth,maxDepth); return maxDepth; }

  inline int  AutoContact::_boundX ( DbU::Unit x ) const
  { return (x<_gcell->getXMin()) ? _gcell->getXMin()
                                 : ((x>_gcell->getConstraintXMax()) ? _gcell->getConstraintXMax() : x); }

  inline int  AutoContact::_boundY ( DbU::Unit y ) const
  { return (y<_gcell->getYMin()) ? _gcell->getYMin()
                                 : ((y>_gcell->getConstraintYMax()) ? _gcell->getConstraintYMax() : y); }

// -------------------------------------------------------------------
// Class  :  "Anabatic::LocatorHelper".

  class LocatorHelper {
    public:
      inline                LocatorHelper ( AutoContact*, Flags flags=Flags::NoFlags );
      inline  bool          isValid       () const;
      inline  AutoSegment*  getSegment    () const;
      inline  void          progress      ();
    private:
      inline  unsigned int  _min          () const;
      inline  unsigned int  _max          () const;
    private:
      Flags         _flags;
      unsigned int  _index;
      AutoContact*  _contact;
  };


  inline LocatorHelper::LocatorHelper ( AutoContact* contact, Flags flags )
    : _flags(flags), _index(_min()), _contact(contact)
  {
    cdebug_tabw(144,1);
    cdebug_log(144,0) << "CTOR LocatorHelper " << contact->_getString() << endl;
    cdebug_log(144,0) << "+ _min():" << _min() << endl;
    cdebug_log(144,0) << "+ _max():" << _max() << endl;
    cdebug_log(144,0) << "+ getSegment(_min()):" << _contact->getSegment(_min()) << endl;
    if (not _contact->getSegment(_index)) progress();
    cdebug_tabw(144,-1);
  }

  inline bool  LocatorHelper::isValid () const
  { return _index < _max(); }

  inline unsigned int  LocatorHelper::_min () const
  { return (_flags & (Flags::Horizontal|Flags::WithPerpands)) ? 0 : 2; }

  inline unsigned int  LocatorHelper::_max () const
  { return ((_flags & Flags::Horizontal) and not (_flags & Flags::WithPerpands)) ? 2 : 4; }

  inline AutoSegment* LocatorHelper::getSegment () const
  {
    cdebug_log(144,0) << "  LocatorHelper::getSegment(" << _index << ") - " << _contact->getSegment(_index) << endl;
    return (_index < _max()) ? _contact->getSegment(_index) : NULL;
  }

  inline void  LocatorHelper::progress ()
  {
    cdebug_tabw(144,1);
    ++_index;
    cdebug_log(144,0) << "LocatorHelper::progress() [" << _index << "] " << _contact->getSegment(_index) << endl;
    while ((_index < _max()) and (_contact->getSegment(_index) == NULL)) {
      ++_index;
      cdebug_log(144,0) << "LocatorHelper::progress() [" << _index << "] " << _contact->getSegment(_index) << endl;
    }
    cdebug_tabw(144,-1);
  }


// -------------------------------------------------------------------
// Helper Functions.

  inline size_t abssub ( size_t a, size_t b ) { return (a>b) ? a-b : b-a; }
  

} // Anabatic namespace.


INSPECTOR_P_SUPPORT(Anabatic::AutoContact);


#endif  // ANABATIC_AUTOCONTACT_H
