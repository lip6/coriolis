
// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
// ===================================================================
//
// $Id: ProxyProperty.cpp,v 1.13 2007/07/30 16:02:51 jpc Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./ProxyProperty.cpp"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




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
  // Miscellaneous.




  // ---------------------------------------------------------------
  // Local Variables.


  const char* twiceSetOffset =
    "FragmentProperty::SetOffest () :\n\n"
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


// -------------------------------------------------------------------
// Constructor  :  "ProxyProperty::ProxyProperty ()".

ProxyProperty::ProxyProperty ( void* shadow )
                             : Property()
                             , _owner(NULL)
                             , _shadow(shadow)
{ }




// -------------------------------------------------------------------
// Constructor  :  "ProxyProperty::create ()".

ProxyProperty* ProxyProperty::create ( void* shadow ) {
  ProxyProperty* property = new ProxyProperty ( shadow );

  if ( property == NULL )
    throw Error ( "ProxyProperty::create()" );

  return ( property );
}




// -------------------------------------------------------------------
// Destructor  :  "ProxyProperty::_preDestroy ()".

void  ProxyProperty::_preDestroy () {
  if ( _owner ) _owner->_onDestroyed ( this );

  trace << "ProxyProperty::_owner := " << hex << (void*)_owner << endl;

  if ( _offset > 0 ) {
    void** shadowMember = ( (void**)( (unsigned long)_shadow + _offset ) );

    trace << "ProxyProperty::_shadowMember := " << hex << *shadowMember << endl;

    *shadowMember = NULL;
  }
}




// -------------------------------------------------------------------
// Method  :  "ProxyProperty::onCapturedBy ()".

void  ProxyProperty::onCapturedBy ( DBo* owner ) {
  if ( ( _owner != NULL ) && ( _owner != owner ) )
    throw Error ( getString(this).c_str() );

  _owner = owner;
}




// -------------------------------------------------------------------
// Method  :  "ProxyProperty::onReleasedBy ()".

void  ProxyProperty::onReleasedBy ( DBo* owner ) {
  if ( _owner == owner ) onNotOwned ();
}




// -------------------------------------------------------------------
// Method  :  "ProxyProperty::onNotOwned ()".

void  ProxyProperty::onNotOwned () {
  destroy ();
}




// -------------------------------------------------------------------
// Method  :  "ProxyProperty::SetOffset ()".

void  ProxyProperty::SetOffset ( int offset ) {
  if ( _offset >= 0 ) throw Error ( twiceSetOffset );

  _offset = offset;
}




// -------------------------------------------------------------------
// Method  :  "ProxyProperty::_getString()".

string  ProxyProperty::_getString () const {
  ostringstream  os;

  os << "<" << _getTypeName() << " ";
  if ( _owner ) os << hex << (void*)_owner << " ";
  else          os << "[not owned] ";
  os << _shadow << ">";

  return ( os.str() );
}




// -------------------------------------------------------------------
// Method  :  "ProxyProperty::_getRecord()".

Record* ProxyProperty::_getRecord () const
{
  Record* record = Property::_getRecord ();

  if ( record != NULL ) {
    record->add ( getSlot ( "_owner" ,  _owner  ) );
    record->add ( getSlot ( "_shadow",  _shadow ) );
  }

  return ( record );
}




} // End of Isobar namespace.
