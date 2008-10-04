
// -*- C++ -*-
//
// This file is part of the Hurricane Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./Property.cpp"                           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


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
