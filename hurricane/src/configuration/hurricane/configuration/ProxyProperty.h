
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


#pragma  once
#include "hurricane/DBo.h"
#include "hurricane/Property.h"


namespace  Isobar3 {

using namespace Hurricane;


// -------------------------------------------------------------------
// Class  :  "::ProxyProperty"

  class ProxyProperty : public Property {
    public:
      static  const Name&    getPropertyName () { return _name; }
      static  size_t         getOffset       () { return _offset; };
      static  void           setOffset       ( size_t offset );
      static  ProxyProperty* create          ( void* shadow=NULL );
    public:   
              DBo*           getOwner        () const { return _owner; };
              void*          getShadow       () const { return _shadow; };
              void*          getShadowMember () const { return (void*)((size_t)_shadow+_offset); };
              template<typename DBoT>
              DBoT*          getAsType       () { return dynamic_cast<DBoT*>(_owner); };
    public:
      virtual Name           getName         () const { return getPropertyName(); }
      virtual void           onCapturedBy    ( DBo* owner );
      virtual void           onReleasedBy    ( DBo* owner );
      virtual void           onNotOwned      ();
    public:
      virtual string         _getString      () const;
      virtual Record*        _getRecord      () const;
      virtual string         _getTypeName    () const { return _TName("Isobar3::ProxyProperty"); };

    protected:
      static  Name    _name;
      static  size_t  _offset;
              DBo*    _owner;
              void*   _shadow;
    protected:       
                     ProxyProperty ( void* _shadow );
      virtual void   _preDestroy  ();
    private:
                     ProxyProperty ( const ProxyProperty& );
      ProxyProperty& operator=     ( const ProxyProperty& );
  };


}  // Isobar3 namespace.
