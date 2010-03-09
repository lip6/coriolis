
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
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
// |  C++ Header  :       "./AutoContact.h"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __KATABATIC_AUTOCONTACT__
#define  __KATABATIC_AUTOCONTACT__

#include  <vector>

#include  "hurricane/Contact.h"
#include  "hurricane/ExtensionGo.h"

namespace Hurricane {
  class RoutingPad;
}

#include  "katabatic/AutoContacts.h"
#include  "katabatic/GCell.h"


namespace Katabatic {


  using std::cerr;
  using std::endl;
  using Hurricane::tab;
  using Hurricane::inltrace;
  using Hurricane::Name;
  using Hurricane::Net;
  using Hurricane::Component;
  using Hurricane::Components;
  using Hurricane::Layer;
  using Hurricane::Contact;
  using Hurricane::RoutingPad;
  using Hurricane::ExtensionGo;

  class GCell;
  class KatabaticEngine;



// -------------------------------------------------------------------
// Class  :  "Katabatic::VirtualContacts".


  class VirtualContacts {
    public:
    // Methods.
      inline       VirtualContacts ();
      inline Box   getBoundingBox  () const;
             void  merge           ( const Point& , const Layer* );
      inline void  clear           ();

    public:
    // Sub-Class.
      class VC {
        public:
          inline              VC             ( const Point& , const Layer* );
          inline const Point& getPoint       () const;
          inline const Layer* getLayer       () const;
                 void         merge          ( const Layer* layer );
          friend bool         operator==     ( const VC& lhs, const VC& rhs );
        protected:
                 Point        _point;
                 const Layer* _layer;
      };

    protected:
    // Attributes.
      Box         _boundingBox;
      vector<VC>  _vcs;
  };


// Inline Functions.
  inline              VirtualContacts::VirtualContacts () : _boundingBox(), _vcs() {}
  inline Box          VirtualContacts::getBoundingBox  () const { return _boundingBox; }
  inline void         VirtualContacts::clear           () { _vcs.clear(); _boundingBox.makeEmpty(); }
  inline              VirtualContacts::VC::VC          ( const Point& p, const Layer* l ) : _point(p), _layer(l) {}
  inline const Point& VirtualContacts::VC::getPoint    () const { return _point; }
  inline const Layer* VirtualContacts::VC::getLayer    () const { return _layer; }


// -------------------------------------------------------------------
// Class  :  "Katabatic::AutoContact".
 

  class AutoContact : public ExtensionGo {

    public:
      static AutoContact*      fromRp                     ( GCell*       gcell
                                                          , RoutingPad*  routingPad
                                                          , const Layer* layer
                                                          , Point        point
                                                          , DbU::Unit    width
                                                          , DbU::Unit    height
                                                          , bool         fixed=false
                                                          );
    public:
    // Constructor & Destructor.
      static  AutoContact*     create                     (       GCell*      gcell
                                                          ,       Net*        net
                                                          , const Layer*      layer
                                                          ,       bool        hAlignate=false
                                                          ,       bool        vAlignate=false
                                                          );
      static  AutoContact*     create                     (       GCell*      gcell
                                                          ,       RoutingPad* rp
                                                          , const Layer*      layer
                                                          , const DbU::Unit   dx
                                                          , const DbU::Unit   dy
                                                          , const DbU::Unit   width
                                                          , const DbU::Unit   height
                                                          ,       bool        hAlignate=false
                                                          ,       bool        vAlignate=false
                                                          ,       bool        fixed=false
                                                          );
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
    // Accessors.                                        
      static  size_t           getSegmentEndAllocateds    ();
      static  size_t           getAllocateds              ();
      static  const Name&      getStaticName              ();
      virtual const Name&      getName                    () const;
      inline  size_t           getId                      () const;
      inline  Contact*         base                       () const;
      inline  Contact*         getContact                 () const;
      virtual Box              getBoundingBox             () const;
      inline  GCell*           getGCell                   () const;
              unsigned int     getMinDepth                () const;
              bool             canDestroy                 ( bool error=false ) const;
      inline  bool             isInvalidated              () const;
      inline  bool             isCorner                   () const;
      inline  bool             isFixed                    () const;
      inline  bool             isTerminal                 () const;
              bool             isAlignate                 ( unsigned int direction ) const;
      inline  bool             isHAlignate                () const;
      inline  bool             isVAlignate                () const;
              bool             isHExtended                ();
              bool             isVExtended                ();
              bool             canGoOutsideGCell          ( const AutoSegment* );
              bool             canHDesalignate            ();
              bool             canVDesalignate            ();
              bool             canMoveUp                  ( AutoSegment* moved ) const;
              void             getLengths                 ( DbU::Unit* lengths, set<AutoSegment*>& );
              Box              getNativeConstraintBox     () const;
              Interval         getUConstraints            ( unsigned int direction ) const;
      inline  DbU::Unit        getCBXMin                  () const;
      inline  DbU::Unit        getCBXMax                  () const;
      inline  DbU::Unit        getCBYMin                  () const;
      inline  DbU::Unit        getCBYMax                  () const;
      inline  Box              getConstraintBox           () const;
              Box&             intersectConstraintBox     ( Box& box ) const;
    // Modifiers.              
      inline  void             setInvalidated             ( bool state );
      inline  void             setCorner                  ( bool state );
      inline  void             setFixed                   ( bool state );
      inline  void             setTerminal                ( bool state );
      inline  void             setHAlignate               ( bool state );
      inline  void             setVAlignate               ( bool state );
              void             computeAlignate            ();
              void             invalidate                 ();
              void             revalidate                 ();
              void             updateGeometry             ();
      inline  void             setInvalidatedTopology     ( bool state );
              void             revalidateTopology         ();
              void             checkTopology              ();
              void             setGCell                   ( GCell* );
      inline  void             setCBXMin                  ( DbU::Unit xMin );
      inline  void             setCBXMax                  ( DbU::Unit xMax );
      inline  void             setCBYMin                  ( DbU::Unit yMin );
      inline  void             setCBYMax                  ( DbU::Unit yMax );
              void             setConstraintBox           ( const Box& box );
              void             restrictConstraintBox      ( DbU::Unit constraintMin
                                                          , DbU::Unit constraintMax
                                                          , unsigned int direction );
              void             restoreNativeConstraintBox ();
              void             breakUp                    ();
              void             split                      ();
              bool             hDesalignate               ();
              bool             vDesalignate               ();
              void             restoreHConnexity          ( DbU::Unit x, bool split=false );
              void             restoreVConnexity          ( DbU::Unit y, bool split=false );
    // Collections.            
              AutoContacts     getCollapseds              ( unsigned int direction );
    // Inspector Management.
              Record*          _getRecord                 () const;
      virtual string           _getString                 () const;
      virtual string           _getTypeName               () const { return "Katabatic::AutoContact"; };
                              
    private:                  
    // Internal: Attributes.
      static  size_t           _maxId;
      static  size_t           _allocateds;
      static  const Name       _goName;
    protected:                
              size_t           _id;
              Contact*         _contact;
              GCell*           _gcell;
              bool             _invalid;
              bool             _invalidTopology;
              bool             _isTerminal;
              bool             _fixed;
              bool             _hAlignate;
              bool             _vAlignate;
              bool             _isCorner;
              int              _dxMin        : 8;
              int              _dxMax        : 8;
              int              _dyMin        : 8;
              int              _dyMax        : 8;
              VirtualContacts  _subContacts;

    protected:
    // Constructors & Destructors.
                               AutoContact  ( GCell*   gcell
                                            , Contact* contact
                                            , bool     hAlignate=false
                                            , bool     vAlignate=false
                                            );
      virtual                 ~AutoContact  ();
      virtual void             _postCreate  ();
      virtual void             _preDestroy  ();
    private:                               
                               AutoContact  ( const AutoContact& );
              AutoContact&     operator=    ( const AutoContact& );

    protected:
      inline  int              _getDeltaMin ( DbU::Unit x, DbU::Unit xMin );
      inline  int              _getDeltaMax ( DbU::Unit x, DbU::Unit xMin, DbU::Unit xMax );
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
// Inline Functions.                                       
  inline size_t        AutoContact::getId                  () const { return _id; }
  inline Contact*      AutoContact::base                   () const { return _contact; }
  inline Contact*      AutoContact::getContact             () const { return _contact; }
  inline GCell*        AutoContact::getGCell               () const { return _gcell; }
  inline bool          AutoContact::isInvalidated          () const { return _invalid; }
  inline bool          AutoContact::isCorner               () const { return _isCorner; }
  inline bool          AutoContact::isFixed                () const { return _fixed; }
  inline bool          AutoContact::isTerminal             () const { return _isTerminal; }
  inline bool          AutoContact::isHAlignate            () const { return _hAlignate; }
  inline bool          AutoContact::isVAlignate            () const { return _vAlignate; }
  inline DbU::Unit     AutoContact::getCBXMin              () const { return DbU::lambda(_dxMin) + _gcell->getX(); }
  inline DbU::Unit     AutoContact::getCBXMax              () const { return DbU::lambda(_dxMax) + _gcell->getX(); }
  inline DbU::Unit     AutoContact::getCBYMin              () const { return DbU::lambda(_dyMin) + _gcell->getY(); }
  inline DbU::Unit     AutoContact::getCBYMax              () const { return DbU::lambda(_dyMax) + _gcell->getY(); }
  inline Box           AutoContact::getConstraintBox       () const { return Box(getCBXMin(),getCBYMin(),getCBXMax(),getCBYMax()); }
  inline void          AutoContact::setInvalidated         ( bool state ) { _invalid = state; }
  inline void          AutoContact::setInvalidatedTopology ( bool state ) { _invalidTopology = state; }
  inline void          AutoContact::setCorner              ( bool state ) { _isCorner = state; }
  inline void          AutoContact::setFixed               ( bool state ) { _fixed = state; }
  inline void          AutoContact::setTerminal            ( bool state ) { _isTerminal = state; }
  inline void          AutoContact::setHAlignate           ( bool state ) { _hAlignate = state; }
  inline void          AutoContact::setVAlignate           ( bool state ) { _vAlignate = state; }
  inline void          AutoContact::setCBXMin              ( DbU::Unit xMin ) { _dxMin = _getDeltaMin(xMin,_gcell->getX()); }
  inline void          AutoContact::setCBXMax              ( DbU::Unit xMax ) { _dxMax = _getDeltaMax(xMax,_gcell->getX(),_gcell->getXMax()); }
  inline void          AutoContact::setCBYMin              ( DbU::Unit yMin ) { _dyMin = _getDeltaMin(yMin,_gcell->getY()); }
  inline void          AutoContact::setCBYMax              ( DbU::Unit yMax ) { _dyMax = _getDeltaMax(yMax,_gcell->getY(),_gcell->getYMax()); }
  inline int           AutoContact::_getDeltaMin           ( DbU::Unit x, DbU::Unit xMin ) { if (x<xMin) return 0; return (int)DbU::getLambda(x-xMin); }
  inline int           AutoContact::_getDeltaMax           ( DbU::Unit x, DbU::Unit xMin, DbU::Unit xMax ) { if (x>xMax) x=xMax; return (int)DbU::getLambda(x-xMin); }

//template<typename Type>inline void  Swap  ( Type& a, Type& b ) { Type swp = a; a = b; b = swp; }
  template<typename Type>inline void  order ( Type& a, Type& b ) { if (a>b) swap(a,b); }

  inline DbU::Unit  setInBound ( DbU::Unit lower, DbU::Unit upper, DbU::Unit& value )
  {
    if ( lower > value ) value = lower;
    if ( upper < value ) value = upper;

    return value;
  }
  

} // End of Katabatic namespace.


INSPECTOR_P_SUPPORT(Katabatic::AutoContact);


#endif  // __KATABATIC_AUTOCONTACT__
