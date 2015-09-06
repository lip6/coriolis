
// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify
// it under the terms of the GNU  Lesser  General  Public  License  as
// published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will  be  useful,  but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHAN-
// TABILITY or FITNESS FOR A PARTICULAR PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser  GNU  General  Public
// License along with Hurricane. If not, see
//                                     <http://www.gnu.org/licenses/>.
//
// +-----------------------------------------------------------------+
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                       Remy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Property.cpp"                                |
// +-----------------------------------------------------------------+


#include  "hurricane/Property.h"
#include  "hurricane/DBo.h"
#include  "hurricane/Error.h"


namespace Hurricane {


// Error messages.
  const char* propertyTypeNameError =
    "Property<%s>::get():\n    Discrepency between type and property name on %s.\n";


// -------------------------------------------------------------------
// Class  :  "Hurricane::Property".


  Name  Property::_baseName = "Property";


  Name  Property::staticGetName ()
  {
    return _baseName;
  }


  Property::Property ()
  { }


  Property::~Property ()
  { }


  void  Property::destroy ()
  {
    _preDestroy();

    delete this;
  }


  string  Property::_getString () const
  {
    string s = "<" + _getTypeName() + ">";
    s.insert(s.length() - 1, " " + getString(getName()));
    return s;
  }


  Record*  Property::_getRecord () const
  {
    return new Record(getString(this));
  }


// -------------------------------------------------------------------
// Class  :  "Hurricane::PrivateProperty".


  PrivateProperty::PrivateProperty ()
     : Property()
     , _owner(NULL)
  { }


  void  PrivateProperty::_preDestroy ()
  {
    Property::_preDestroy();

    if ( _owner ) _owner->_onDestroyed(this);
  }


  void  PrivateProperty::onCapturedBy ( DBo* owner )
  {
    _owner = owner;
  }


  void  PrivateProperty::onReleasedBy ( DBo* owner )
  {
    if ( _owner == owner ) onNotOwned();
  }


  void  PrivateProperty::onNotOwned ()
  {
    destroy();
  }


  string  PrivateProperty::_getString () const
  {
    return Property::_getString ();
  }


  Record* PrivateProperty::_getRecord () const
  {
    Record* record = Property::_getRecord();
    if (record) {
      record->add(getSlot("Owner", _owner));
    }
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "Hurricane::SharedProperty".


  SharedProperty::SharedProperty ()
     : Property()
     , _ownerSet()
  { }


  void  SharedProperty::_preDestroy ()
  {
    Property::_preDestroy();

    while (!_ownerSet.empty()) {
      DBo* owner = *_ownerSet.begin();
      _ownerSet.erase(owner);
      owner->_onDestroyed(this);
    }
  }


  void  SharedProperty::onCapturedBy ( DBo* owner )
  {
    _ownerSet.insert(owner);
  }


  void  SharedProperty::onReleasedBy ( DBo* owner )
  {
    _ownerSet.erase(owner);

    if (_ownerSet.empty()) onNotOwned();
  }


  void SharedProperty::onNotOwned ()
  {
    destroy();
  }


  string  SharedProperty::_getString () const
  {
    return Property::_getString ();
  }


  Record* SharedProperty::_getRecord () const
  {
    Record* record = Property::_getRecord();
    if (record) {
      record->add(getSlot("Owners", &_ownerSet));
    }
    return record;
  }


} // End of Hurricane namespace.
