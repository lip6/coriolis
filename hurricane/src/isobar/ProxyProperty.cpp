
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Authors     :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./ProxyProperty.cpp"                           |
// +-----------------------------------------------------------------+


#if not defined(__PYTHON_MODULE__)

#include <iostream>
#include <sstream>
#include "hurricane/isobar/ProxyProperty.h"
#include "hurricane/Error.h"
#include "hurricane/Warning.h"

using namespace Hurricane;

namespace Isobar {

namespace {

  using namespace std;


  // ---------------------------------------------------------------
  // Local Variables.


  const char* twiceSetOffset =
    "ProxyProperty::setOffset () :\n\n"
    "    Attempt to sets the _shadow member offset twice.\n";


}


// x-----------------------------------------------------------------x
// |              "::ProxyProperty" Global Variables                 |
// x-----------------------------------------------------------------x


 Name  ProxyProperty::_name   = "Isobar::ProxyProperty";
 int   ProxyProperty::_offset = -1;




// x-----------------------------------------------------------------x
// |              "::ProxyProperty" Class Definitions                |
// x-----------------------------------------------------------------x


ProxyProperty::ProxyProperty ( void* shadow )
  : Property()
  , _owner  (NULL)
  , _shadow (shadow)
{ }


ProxyProperty* ProxyProperty::create ( void* shadow ) {
  ProxyProperty* property = new ProxyProperty ( shadow );

  if ( property == NULL )
    throw Error ( "ProxyProperty::create()" );

  return property;
}


void  ProxyProperty::_preDestroy () {
  if ( _owner ) _owner->_onDestroyed ( this );

  cdebug_log(20,0) << "ProxyProperty::_owner := " << hex << (void*)_owner << endl;

  if ( _offset > 0 ) {
    void** shadowMember = ( (void**)( (unsigned long)_shadow + _offset ) );

    cdebug_log(20,0) << "ProxyProperty::_shadowMember := " << hex << *shadowMember << endl;

    *shadowMember = NULL;
  }
}


void  ProxyProperty::onCapturedBy ( DBo* owner ) {
  if ( ( _owner != NULL ) && ( _owner != owner ) )
    throw Error ( getString(this).c_str() );

  _owner = owner;
}


void  ProxyProperty::onReleasedBy ( DBo* owner ) {
  if ( _owner == owner ) onNotOwned ();
}


void  ProxyProperty::onNotOwned () {
  destroy ();
}


void  ProxyProperty::setOffset ( int offset ) {
  if ( _offset >= 0 ) throw Error(twiceSetOffset);

  _offset = offset;
}


string  ProxyProperty::_getString () const {
  ostringstream  os;

  os << "<" << _getTypeName() << " ";
  if ( _owner ) os << hex << (void*)_owner << " ";
  else          os << "[not owned] ";
  os << _shadow << ">";

  return ( os.str() );
}


Record* ProxyProperty::_getRecord () const
{
  Record* record = Property::_getRecord ();

  if ( record != NULL ) {
    record->add ( getSlot ( "_owner" ,  _owner  ) );
    record->add ( getSlot ( "_shadow",  _shadow ) );
  }

  return ( record );
}


} // Isobar namespace.

#endif
