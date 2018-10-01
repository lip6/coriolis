// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2011-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                   Stephanie Youssef              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./BJT.cpp"                                     |
// +-----------------------------------------------------------------+

#include <bitset>
#include <string>
#include "hurricane/Library.h"
#include "hurricane/Name.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/analog/BJT.h"
#include "hurricane/analog/BJTArguments.h"


namespace Analog {
  
  using namespace Hurricane;


  const Name BJT::_bjt ( "BJT" );


  BJT::BJT ( Library* library, const Name& name, const Type& type )
    : Super(library,name,type)
    , _n            (NULL)
    , _m            (NULL)
    , _externalDummy(NULL)
  { }


  BJT* BJT::create ( Library* library, const Name& name, const Type& type )
  {
    preCreate( name );
    UpdateSession::open();
    BJT* bjt = new BJT( library, name, type );
    bjt->_postCreate( name );
    bjt->createConnections();
    UpdateSession::close();
    return bjt;
  }


  void BJT::_postCreate ( const Name& deviceName )
  {
    Super::_postCreate( deviceName );

    Choices externalDummy;
    externalDummy << "";
    _externalDummy = addMCheckBoxParameter( "External Dummy", externalDummy );

    _m = addFormFactorParameter( "M", 1, 10 );
    _n = addFormFactorParameter( "N", 1, 10 );
  }


  void BJT::createConnections ()
  {
    Net* e = Net::create( this, Name("E") );
    e->setExternal( true );

    Net* b = Net::create( this, Name("B") );
    b->setExternal( true );

    Net* c = Net::create( this, Name("C") );
    c->setExternal( true );
  }


  Name  BJT::getDeviceName () const
  { return _bjt; }


  Arguments* BJT::getArguments ()
  {
    bool isNPN           = (getType() == BJTFamily::NPN); 
    bool isExternalDummy = (_externalDummy->getValue() == 1);

    BJTArguments* bjtArgs = new BJTArguments( isNPN, isExternalDummy, _m->getValue(), _n->getValue() );
    return bjtArgs;
  }


  unsigned int  BJT::getM () const
  { return _m->getValue(); }


  unsigned int  BJT::getN () const
  { return _n->getValue(); }


  void BJT::setExternalDummy ( const bool externalDummy )
  {
    if (externalDummy) _externalDummy->setValue(1);
    else               _externalDummy->setValue(0);
  }


  bool BJT::getExternalDummy () const
  { return (_externalDummy->getValue() == 1); }


}  // Analog namespace.
