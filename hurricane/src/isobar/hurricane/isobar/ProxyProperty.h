
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Authors     :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/ProxyProperty.h"            |
// +-----------------------------------------------------------------+


#ifndef  ISOBAR_PROXY_PROPERTY_H
#define  ISOBAR_PROXY_PROPERTY_H

#include "hurricane/DBo.h"
#include "hurricane/Property.h"


namespace  Isobar {

using namespace Hurricane;


#define F_PY_OWNED    ( 1 << 0 )

#define CHECK_OFFSET(PY_STRUCT,TYPE)                                         \
  if ( ProxyProperty::getOffset() < 0 ) {                                    \
    ProxyProperty::setOffset(   (unsigned long)(&(PY_STRUCT->ACCESS_OBJECT)) \
                              - (unsigned long)PY_STRUCT);                   \
  }


// -------------------------------------------------------------------
// Class  :  "::ProxyProperty"

  class ProxyProperty : public Property {
    public:
      static  const Name&    getPropertyName () { return _name; }
      static  int            getOffset       () { return _offset; };
      static  void           setOffset       ( int offset );
      static  ProxyProperty* create          ( void* shadow=NULL );
    public:   
              DBo*           getOwner        () const { return _owner; };
              void*          getShadow       () const { return _shadow; };
              void*          getShadowMember () const { return (void*)((unsigned long)_shadow+_offset); };
              template<typename DBoType>
              DBoType*       getAsType       () { return dynamic_cast<DBoType*>(_owner); };
    public:
      virtual Name           getName         () const { return getPropertyName(); }
      virtual void           onCapturedBy    ( DBo* owner );
      virtual void           onReleasedBy    ( DBo* owner );
      virtual void           onNotOwned      ();
    public:
      virtual string         _getString      () const;
      virtual Record*        _getRecord      () const;
      virtual string         _getTypeName    () const { return _TName("ProxyProperty"); };

    protected:
      static  Name   _name;
      static  int    _offset;
              DBo*   _owner;
              void*  _shadow;
    protected:       
                     ProxyProperty ( void* _shadow );
      virtual void   _preDestroy  ();
    private:
                     ProxyProperty ( const ProxyProperty& );
      ProxyProperty& operator=     ( const ProxyProperty& );
  };


}  // Isobar namespace.

#endif
