
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
// |  C++ Header  :       "./ContactWrapper.h"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __KATABATIC_CONTACT_WRAPPER__
#define  __KATABATIC_CONTACT_WRAPPER__


#include  "hurricane/Contact.h"


namespace Katabatic {

  using  Hurricane::Hook;
  using  Hurricane::Component;
  using  Hurricane::DbU;
  using  Hurricane::Point;
  using  Hurricane::Layer;
  using  Hurricane::Contact;


// -------------------------------------------------------------------
// Template Class  :  "Katabatic::ContactWrapper".
 
  template<typename BaseContact>
  class ContactWrapper {
    public:
    // Accessors.
      inline BaseContact*    getBase        ();
      inline Hook*           getBodyHook    ();
      inline Hook*           getAnchorHook  ();
      inline Component*      getAnchor      () const;
      inline DbU::Unit       getDx          () const;
      inline DbU::Unit       getDy          () const;
      inline DbU::Unit       getWidth       () const;
      inline DbU::Unit       getHalfWidth   () const;
      inline DbU::Unit       getHeight      () const;
      inline DbU::Unit       getHalfHeight  () const;
    public:                 
    // Modifiers.           
      inline void            setLayer       ( const Layer* );
      inline void            setWidth       ( DbU::Unit );
      inline void            setHeight      ( DbU::Unit );
      inline void            setSizes       ( DbU::Unit width, Dbu::Unit height );
      inline void            setX           ( DbU::Unit );
      inline void            setY           ( DbU::Unit );
      inline void            setPosition    ( DbU::Unit width, Dbu::Unit height );
      inline void            setPosition    ( const Point& );
      inline void            setDx          ( DbU::Unit );
      inline void            setDy          ( DbU::Unit );
      inline void            setOffset      ( DbU::Unit dx, Dbu::Unit dy );
    protected:
    // Attribute.
             BaseContact*    _contact; 
    protected:
    // Constructor & Destructors.
      inline                 ContactWrapper ( BaseContact* );
    private:
             ContactWrapper& ContactWrapper ( const ContactWrapper& );
             ContactWrapper& operator=      ( const ContactWrapper& );
  };


// Inline Functions.
  inline BaseContact* ContactWrapper::getBase        () { return _contact; }
  inline Hook*        ContactWrapper::getBodyHook    () { return _contact->getBodyHook(); }
  inline Hook*        ContactWrapper::getAnchorHook  () { return _contact->getAnchorHook(); }
  inline Component*   ContactWrapper::getAnchor      () const { return _contact->getAnchor(); }
  inline DbU::Unit    ContactWrapper::getDx          () const { return _contact->getDx(); }
  inline DbU::Unit    ContactWrapper::getDy          () const { return _contact->getDy(); }
  inline DbU::Unit    ContactWrapper::getWidth       () const { return _contact->getWidth(); }
  inline DbU::Unit    ContactWrapper::getHalfWidth   () const { return _contact->getHalfWidth(); }
  inline DbU::Unit    ContactWrapper::getHeight      () const { return _contact->getHeight(); }
  inline DbU::Unit    ContactWrapper::getHalfHeight  () const { return _contact->getHalfHeight(); }
  inline void         ContactWrapper::setLayer       ( const Layer* layer ) { return _contact->setLayer(layer); }
  inline void         ContactWrapper::setWidth       ( DbU::Unit w ) { return _contact->setWidth(w); }
  inline void         ContactWrapper::setHeight      ( DbU::Unit h ) { return _contact->setHeight(h); }
  inline void         ContactWrapper::setSizes       ( DbU::Unit w, Dbu::Unit h ) { return _contact->setSizes(w,h); }
  inline void         ContactWrapper::setX           ( DbU::Unit x ) { return _contact->setX(x); }
  inline void         ContactWrapper::setY           ( DbU::Unit y ) { return _contact->setY(y); }
  inline void         ContactWrapper::setPosition    ( DbU::Unit x, Dbu::Unit y ) { return _contact->setPosition(x,y); }
  inline void         ContactWrapper::setPosition    ( const Point& p ) { return _contact->setPosition(p); }
  inline void         ContactWrapper::setDx          ( DbU::Unit dx ) { return _contact->setDx(dx); }
  inline void         ContactWrapper::setDy          ( DbU::Unit dy ) { return _contact->setDy(dy); }
  inline void         ContactWrapper::setOffset      ( DbU::Unit dx, Dbu::Unit dy ) { return _contact->setOffset(dx,dy); }
  inline              ContactWrapper::ContactWrapper ( BaseContact* contact ) : _contact(contact) { }


} // End of Katabatic namespace.


#endif  // __KATABATIC_CONTACT_WRAPPER__
