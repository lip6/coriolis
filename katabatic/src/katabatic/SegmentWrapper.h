
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
// |  C++ Header  :       "./SegmentWrapper.h"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __KATABATIC_SEGMENT_WRAPPER__
#define  __KATABATIC_SEGMENT_WRAPPER__


#include  "hurricane/Segment.h"


namespace Katabatic {

  using  Hurricane::Hook;
  using  Hurricane::Component;
  using  Hurricane::DbU;
  using  Hurricane::Point;
  using  Hurricane::Layer;
  using  Hurricane::Segment;


// -------------------------------------------------------------------
// Class  :  "Katabatic::SegmentWrapper".
 
  class SegmentWrapper {
    public:
    // Accessors.
      virtual Segment*        getBase           () = 0;
      inline  Hook*           getSourceHook     ();
      inline  Hook*           getTargetHook     ();
      inline  Hook*           getOppositeHook   ( const Hook* );
      inline  Component*      getSource         () const;
      inline  Component*      getTarget         () const;
      inline  Component*      getOppositeAnchor ( const Component* );
      inline  Components      getAnchors        () const;
      virtual DbU::Unit       getX              () const = 0;
      virtual DbU::Unit       getY              () const = 0;
      inline  DbU::Unit       getWidth          () const;
      inline  DbU::Unit       getHalfWidth      () const;
      inline  DbU::Unit       getLength         () const;
      virtual DbU::Unit       getSourceX        () const = 0;
      virtual DbU::Unit       getSourceY        () const = 0;
      virtual Point           getSourcePosition () const = 0;
      virtual DbU::Unit       getTargetX        () const = 0;
      virtual DbU::Unit       getTargetY        () const = 0;
      virtual Point           getTargetPosition () const = 0;
    public:                   
    // Modifiers.             
      inline  void            setLayer          ( const Layer* );
      inline  void            setWidth          ( DbU::Unit );
      inline  void            invert            ();

    protected:
    // Constructor & Destructors.
      inline                  SegmentWrapper    ( Segment* );
    private:
              SegmentWrapper& SegmentWrapper    ( const SegmentWrapper& );
              SegmentWrapper& operator=         ( const SegmentWrapper& );
  };


// Inline Functions.
  inline Hook*      SegmentWrapper::getBodyHook    () { return getBase()->getBodyHook(); }
  inline Hook*      SegmentWrapper::getAnchorHook  () { return getBase()->getAnchorHook(); }
  inline Component* SegmentWrapper::getAnchor      () const { return getBase()->getAnchor(); }
  inline DbU::Unit  SegmentWrapper::getDx          () const { return getBase()->getDx(); }
  inline DbU::Unit  SegmentWrapper::getDy          () const { return getBase()->getDy(); }
  inline DbU::Unit  SegmentWrapper::getWidth       () const { return getBase()->getWidth(); }
  inline DbU::Unit  SegmentWrapper::getHalfWidth   () const { return getBase()->getHalfWidth(); }
  inline DbU::Unit  SegmentWrapper::getHeight      () const { return getBase()->getHeight(); }
  inline DbU::Unit  SegmentWrapper::getHalfHeight  () const { return getBase()->getHalfHeight(); }
  inline void       SegmentWrapper::setLayer       ( const Layer* layer ) { return getBase()->setLayer(layer); }
  inline void       SegmentWrapper::setWidth       ( DbU::Unit w ) { return getBase()->setWidth(w); }
  inline void       SegmentWrapper::setHeight      ( DbU::Unit h ) { return getBase()->setHeight(h); }
  inline void       SegmentWrapper::setSizes       ( DbU::Unit w, Dbu::Unit h ) { return getBase()->setSizes(w,h); }
  inline void       SegmentWrapper::setX           ( DbU::Unit x ) { return getBase()->setX(x); }
  inline void       SegmentWrapper::setY           ( DbU::Unit y ) { return getBase()->setY(y); }
  inline void       SegmentWrapper::setPosition    ( DbU::Unit x, Dbu::Unit y ) { return getBase()->setPosition(x,y); }
  inline void       SegmentWrapper::setPosition    ( const Point& p ) { return getBase()->setPosition(p); }
  inline void       SegmentWrapper::setDx          ( DbU::Unit dx ) { return getBase()->setDx(dx); }
  inline void       SegmentWrapper::setDy          ( DbU::Unit dy ) { return getBase()->setDy(dy); }
  inline void       SegmentWrapper::setOffset      ( DbU::Unit dx, Dbu::Unit dy ) { return getBase()->setOffset(dx,dy); }
  inline            SegmentWrapper::SegmentWrapper ( BaseSegment* contact ) : getBase()(contact) { }


} // End of Katabatic namespace.


#endif  // __KATABATIC_SEGMENT_WRAPPER__
