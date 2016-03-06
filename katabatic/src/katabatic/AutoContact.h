// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katabatic/AutoContact.h"                     |
// +-----------------------------------------------------------------+


#ifndef  KATABATIC_AUTOCONTACT_H
#define  KATABATIC_AUTOCONTACT_H

#include <vector>
#include <map>
#include "hurricane/Contact.h"
#include "hurricane/ExtensionGo.h"
#include "katabatic/Constants.h"
#include "katabatic/AutoSegment.h"
#include "katabatic/GCell.h"


namespace Katabatic {


  using std::cerr;
  using std::endl;
  using Hurricane::tab;
  using Hurricane::inltrace;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;
  using Hurricane::Name;
  using Hurricane::Net;
  using Hurricane::Component;
  using Hurricane::Components;
  using Hurricane::Layer;
  using Hurricane::Contact;
  using Hurricane::ExtensionGo;

  class GCell;
  class KatabaticEngine;
  class AutoHorizontal;
  class AutoVertical;
  class AutoContact;


  typedef  std::map<Contact*,AutoContact*>  AutoContactLut;


// -------------------------------------------------------------------
// Class  :  "Katabatic::AutoContact".

  enum AutoContactFlag { CntFixed               = 0x00000001
                       , CntTerminal            = 0x00000002
                       , CntTurn                = 0x00000004
                       , CntHTee                = 0x00000008
                       , CntVTee                = 0x00000010
                       , CntInvalidated         = 0x00000020
                       , CntInvalidatedCache    = 0x00000040
                       , CntInCreationStage     = 0x00000080
                       , CntBadTopology         = 0x00000100
                       , CntIgnoreAnchor        = 0x00000200
                       , CntWeakTerminal        = 0x00000400
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
      inline  void             setPosition                ( DbU::Unit width, DbU::Unit height );
      inline  void             setPosition                ( const Point& );
      inline  void             setDx                      ( DbU::Unit );
      inline  void             setDy                      ( DbU::Unit );
      inline  void             setOffset                  ( DbU::Unit dx, DbU::Unit dy );
      virtual void             translate                  ( const DbU::Unit& tx, const DbU::Unit& ty );
    // Predicates.
      inline  bool             isInCreationStage          () const;
      inline  bool             isInvalidated              () const;
      inline  bool             isInvalidatedCache         () const;
      inline  bool             isTerminal                 () const;
      inline  bool             isTurn                     () const;
              bool             isTee                      ( unsigned int direction ) const;
      inline  bool             isHTee                     () const;
      inline  bool             isVTee                     () const;
      inline  bool             isFixed                    () const;
      inline  bool             hasBadTopology             () const;
              bool             canDestroy                 ( unsigned int flags=0 ) const;
              bool             canMoveUp                  ( const AutoSegment* moved ) const;
    // Accessors.                                        
      inline  Contact*         base                       () const;
      static  size_t           getAllocateds              ();
      static  const Name&      getStaticName              ();
      virtual const Name&      getName                    () const;
      inline  size_t           getId                      () const;
      virtual Box              getBoundingBox             () const;
      inline  GCell*           getGCell                   () const;
      virtual AutoSegment*     getOpposite                ( const AutoSegment* ) const = 0;
      virtual AutoSegment*     getPerpandicular           ( const AutoSegment* ) const = 0;
      virtual AutoSegment*     getSegment                 ( unsigned int ) const = 0;
              unsigned int     getMinDepth                () const;
              unsigned int     getMaxDepth                () const;
              void             getLengths                 ( DbU::Unit* lengths, AutoSegment::DepthLengthSet& );
      virtual Box              getNativeConstraintBox     () const;
              Interval         getNativeUConstraints      ( unsigned int direction ) const;
              Interval         getUConstraints            ( unsigned int direction ) const;
      inline  DbU::Unit        getCBXMin                  () const;
      inline  DbU::Unit        getCBXMax                  () const;
      inline  DbU::Unit        getCBYMin                  () const;
      inline  DbU::Unit        getCBYMax                  () const;
      inline  Box              getConstraintBox           () const;
              Box&             intersectConstraintBox     ( Box& box ) const;
    // Collections.
              AutoSegments     getAutoSegments            ();
    // Modifiers.              
              void             invalidate                 ( unsigned int flags=0 );
      virtual void             cacheDetach                ( AutoSegment* ) = 0;
      virtual void             cacheAttach                ( AutoSegment* ) = 0;
      virtual void             updateCache                () = 0;
      virtual void             updateGeometry             () = 0;
      virtual void             updateTopology             () = 0;
              void             showTopologyError          ( const std::string&, unsigned int flags=0 );
      virtual void             checkTopology              ();
      inline  void             setFlags                   ( unsigned int );
      inline  void             unsetFlags                 ( unsigned int );
              void             setGCell                   ( GCell* );
      inline  void             setCBXMin                  ( DbU::Unit xMin );
      inline  void             setCBXMax                  ( DbU::Unit xMax );
      inline  void             setCBYMin                  ( DbU::Unit yMin );
      inline  void             setCBYMax                  ( DbU::Unit yMax );
              void             setConstraintBox           ( const Box& box );
              bool             restrictConstraintBox      ( DbU::Unit constraintMin
                                                          , DbU::Unit constraintMax
                                                          , unsigned int flags=KbWarnOnError );
              void             restoreNativeConstraintBox ();
              void             migrateConstraintBox       ( AutoContact* other );
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
              unsigned int     _flags;
              int              _dxMin:8;
              int              _dxMax:8;
              int              _dyMin:8;
              int              _dyMax:8;

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
      inline  int              _getDeltaMin ( DbU::Unit x, DbU::Unit xMin );
      inline  int              _getDeltaMax ( DbU::Unit x, DbU::Unit xMin, DbU::Unit xMax );
      static  void             _getTopology ( Contact*, Component*& anchor, Horizontal**&, Vertical**&, size_t );
      virtual void             _invalidate  ( unsigned int flags ) = 0;
  };


// Wrapped Contact Inline Functions.
  inline Hook*          AutoContact::getBodyHook           () { return _contact->getBodyHook(); }
  inline Hook*          AutoContact::getAnchorHook         () { return _contact->getAnchorHook(); }
  inline Component*     AutoContact::getAnchor             () const { return _contact->getAnchor(); }
  inline Net*           AutoContact::getNet                () const { return _contact->getNet(); }
  inline const Layer*   AutoContact::getLayer              () const { return _contact->getLayer(); }
  inline DbU::Unit      AutoContact::getX                  () const { return _contact->getX(); }
  inline DbU::Unit      AutoContact::getY                  () const { return _contact->getY(); }
  inline DbU::Unit      AutoContact::getDx                 () const { return _contact->getDx(); }
  inline DbU::Unit      AutoContact::getDy                 () const { return _contact->getDy(); }
  inline Point          AutoContact::getCenter             () const { return _contact->getCenter(); }
  inline Point          AutoContact::getPosition           () const { return _contact->getPosition(); }
  inline DbU::Unit      AutoContact::getWidth              () const { return _contact->getWidth(); }
  inline DbU::Unit      AutoContact::getHalfWidth          () const { return _contact->getHalfWidth(); }
  inline DbU::Unit      AutoContact::getHeight             () const { return _contact->getHeight(); }
  inline DbU::Unit      AutoContact::getHalfHeight         () const { return _contact->getHalfHeight(); }
  inline Components     AutoContact::getSlaveComponents    () const { return _contact->getSlaveComponents(); }
  inline void           AutoContact::setLayer              ( const Layer* layer ) { return _contact->setLayer(layer); }
  inline void           AutoContact::setWidth              ( DbU::Unit w ) { return _contact->setWidth(w); }
  inline void           AutoContact::setHeight             ( DbU::Unit h ) { return _contact->setHeight(h); }
  inline void           AutoContact::setSizes              ( DbU::Unit w, DbU::Unit h ) { return _contact->setSizes(w,h); }
  inline void           AutoContact::setX                  ( DbU::Unit x ) { return _contact->setX(x); }
  inline void           AutoContact::setY                  ( DbU::Unit y ) { return _contact->setY(y); }
  inline void           AutoContact::setPosition           ( DbU::Unit x, DbU::Unit y ) { return _contact->setPosition(x,y); }
  inline void           AutoContact::setPosition           ( const Point& p ) { return _contact->setPosition(p); }
  inline void           AutoContact::setDx                 ( DbU::Unit dx ) { return _contact->setDx(dx); }
  inline void           AutoContact::setDy                 ( DbU::Unit dy ) { return _contact->setDy(dy); }
  inline void           AutoContact::setOffset             ( DbU::Unit dx, DbU::Unit dy ) { return _contact->setOffset(dx,dy); }
// AutoContact Inline Functions.                                       
  inline bool          AutoContact::isInCreationStage      () const { return _flags&CntInCreationStage; }
  inline bool          AutoContact::isInvalidated          () const { return _flags&CntInvalidated; }
  inline bool          AutoContact::isInvalidatedCache     () const { return _flags&CntInvalidatedCache; }
  inline bool          AutoContact::isTurn                 () const { return _flags&CntTurn; }
  inline bool          AutoContact::isFixed                () const { return _flags&CntFixed; }
  inline bool          AutoContact::isTerminal             () const { return _flags&CntTerminal; }
  inline bool          AutoContact::isHTee                 () const { return _flags&CntHTee; }
  inline bool          AutoContact::isVTee                 () const { return _flags&CntVTee; }
  inline bool          AutoContact::hasBadTopology         () const { return _flags&CntBadTopology; }
  inline size_t        AutoContact::getId                  () const { return _id; }
  inline Contact*      AutoContact::base                   () const { return _contact; }
  inline GCell*        AutoContact::getGCell               () const { return _gcell; }
  inline Box           AutoContact::getConstraintBox       () const { return Box(getCBXMin(),getCBYMin(),getCBXMax(),getCBYMax()); }
  inline void          AutoContact::setCBXMin              ( DbU::Unit xMin ) { _dxMin = _getDeltaMin(xMin,_gcell->getX()); }
  inline void          AutoContact::setCBXMax              ( DbU::Unit xMax ) { _dxMax = _getDeltaMax(xMax,_gcell->getX(),_gcell->getXMax()); }
  inline void          AutoContact::setCBYMin              ( DbU::Unit yMin ) { _dyMin = _getDeltaMin(yMin,_gcell->getY()); }
  inline void          AutoContact::setCBYMax              ( DbU::Unit yMax ) { _dyMax = _getDeltaMax(yMax,_gcell->getY(),_gcell->getYMax()); }
  inline void          AutoContact::setFlags               ( unsigned int flags ) { _flags|= flags; }
  inline void          AutoContact::unsetFlags             ( unsigned int flags ) { _flags&=~flags; }
  inline int           AutoContact::_getDeltaMin           ( DbU::Unit x, DbU::Unit xMin ) { if (x<xMin) return 0; return (int)DbU::toLambda(x-xMin); }
  inline int           AutoContact::_getDeltaMax           ( DbU::Unit x, DbU::Unit xMin, DbU::Unit xMax ) { if (x>xMax) x=xMax; return (int)DbU::toLambda(x-xMin); }

  inline DbU::Unit AutoContact::getCBXMin () const
  { return isFixed() ? _contact->getX() : DbU::fromLambda(_dxMin) + _gcell->getX(); }

  inline DbU::Unit AutoContact::getCBXMax () const
  { return isFixed() ? _contact->getX() : DbU::fromLambda(_dxMax) + _gcell->getX(); }

  inline DbU::Unit AutoContact::getCBYMin () const
  { return isFixed() ? _contact->getY() : DbU::fromLambda(_dyMin) + _gcell->getY(); }

  inline DbU::Unit AutoContact::getCBYMax () const
  { return isFixed() ? _contact->getY() : DbU::fromLambda(_dyMax) + _gcell->getY(); }

// -------------------------------------------------------------------
// Class  :  "Katabatic::LocatorHelper".

  class LocatorHelper {
    public:
      inline                LocatorHelper ( AutoContact*, unsigned int flags=0 );
      inline  bool          isValid       () const;
      inline  AutoSegment*  getSegment    () const;
      inline  void          progress      ();
    private:
      inline  unsigned int  _min          () const;
      inline  unsigned int  _max          () const;
    private:
      unsigned int  _flags;
      unsigned int  _index;
      AutoContact*  _contact;
  };


  inline LocatorHelper::LocatorHelper ( AutoContact* contact, unsigned int flags )
    : _flags(flags), _index(_min()), _contact(contact)
  {
    ltracein(80);
    ltrace(80) << "CTOR LocatorHelper " << contact->_getString() << endl;
    ltrace(80) << "+ _min():" << _min() << endl;
    ltrace(80) << "+ _max():" << _max() << endl;
    ltrace(80) << "+ getSegment(_min()):" << _contact->getSegment(_min()) << endl;
    if (not _contact->getSegment(_index)) progress();
    ltraceout(80);
  }

  inline bool  LocatorHelper::isValid () const
  { return _index < _max(); }

  inline unsigned int  LocatorHelper::_min () const
  { return (_flags & (KbHorizontal|KbWithPerpands)) ? 0 : 2; }

  inline unsigned int  LocatorHelper::_max () const
  { return ((_flags & KbHorizontal) and not (_flags & KbWithPerpands)) ? 2 : 4; }

  inline AutoSegment* LocatorHelper::getSegment () const
  {
    ltrace(80) << "LocatorHelper::getSegment(" << _index << ") - " << _contact->getSegment(_index) << endl;
    return (_index < _max()) ? _contact->getSegment(_index) : NULL;
  }

  inline void  LocatorHelper::progress ()
  {
    ltracein(80);
    ++_index;
    ltrace(80) << "LocatorHelper::progress() [" << _index << "] " << _contact->getSegment(_index) << endl;
    while ((_index < _max()) and (_contact->getSegment(_index) == NULL)) {
      ++_index;
      ltrace(80) << "LocatorHelper::progress() [" << _index << "] " << _contact->getSegment(_index) << endl;
    }
    ltraceout(80);
  }


// -------------------------------------------------------------------
// Helper Functions.


  template<typename Type>inline void  order ( Type& a, Type& b ) { if (a>b) std::swap(a,b); }

  inline DbU::Unit  setInBound ( DbU::Unit lower, DbU::Unit upper, DbU::Unit& value )
  {
    if ( lower > value ) value = lower;
    if ( upper < value ) value = upper;

    return value;
  }

  inline size_t abssub ( size_t a, size_t b ) { return (a>b) ? a-b : b-a; }
  

} // Katabatic namespace.


INSPECTOR_P_SUPPORT(Katabatic::AutoContact);


#endif  // KATABATIC_AUTOCONTACT_H
