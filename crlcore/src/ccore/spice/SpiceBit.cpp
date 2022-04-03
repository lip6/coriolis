// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU 2021-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |         S P I C E  / Hurricane  Interface                       |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :   "./alliance/spice/SpiceBit.cpp"                |
// +-----------------------------------------------------------------+


#include "crlcore/SpiceBit.h"

#include <limits>

namespace Spice {

  using namespace std;
  using Hurricane::_TName;
  using Hurricane::Property;


  const size_t  Bit::nindex = std::numeric_limits<size_t>::max();


// -------------------------------------------------------------------
// Class  :  "::Bit" (implementation).

  ptrdiff_t  Bit::_offset = 0;


  Bit::Bit ( BitProperty* property, const Net* net, size_t index, string name )
    : _index(index)
    , _name (name)
  {
    if (not _offset) {
    //_offset = offsetof(ScalarSignal,_toNet);
      _offset = (ptrdiff_t)this - (ptrdiff_t)property;
    }
    if (_name.empty())
      _name = to_string(_index);

    // if (net->isPower ()) _name = getString(net->getName());
    // if (net->isGround()) _name = getString(net->getName());
  }


  Bit::Bit ( size_t index, string name )
    : _index(index)
    , _name (name)
  { }


  Bit::~Bit ()
  { }


  bool          Bit::isExternal () const { return getNet()->isExternal(); }
  size_t        Bit::getIndex   () const { return _index; }


  string  Bit::_getString () const
  {
    string s = "<" + getString(getNet()->getName());
    s += " order:";
    if (_index != nindex) s += getString(_index);
    else                  s += "nindex";
    s += " node:\"" + _name + "\"";
    s += ">";
    return s;
  }


  Record* Bit::_getRecord () const
  {
    Record* record = new Record ( "<Bit " + _getString() + " >" );
    if (record != NULL) {
      record->add( getSlot("_index" , _index ) );
    }
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "BitProperty"

  Name  BitProperty::_name = "Spice::Bit";


  BitProperty* BitProperty::create ( Net* owner, size_t index )
  {
    BitProperty *property = new BitProperty( owner, index );

    property->_postCreate ();
    return property;
  }


  void  BitProperty::onReleasedBy ( DBo* owner )
  {
    BitExtension::clearCache( static_cast<Net*>(owner) );
    PrivateProperty::onReleasedBy( owner );
  }


  Name  BitProperty::getPropertyName ()
  { return _name; }


  Name  BitProperty::getName () const
  { return getPropertyName(); }


  string  BitProperty::_getTypeName () const
  { return "Spice::BitProperty"; }


  string  BitProperty::_getString () const
  {
    string s = PrivateProperty::_getString ();
    s.insert ( s.length() - 1 , " " + getString(&_bit) );

    return s;
  }


  Record* BitProperty::_getRecord () const
  {
    Record* record = PrivateProperty::_getRecord();
    if ( record ) {
      record->add( getSlot( "_name",  _name ) );
      record->add( getSlot( "_bit" , &_bit  ) );
    }
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "BitExtension"


  const Net* BitExtension::_owner = NULL;
  Bit*       BitExtension::_cache = NULL;


  void  BitExtension::remove ( const Net* net )
  {
    Property* property = net->getProperty( BitProperty::getPropertyName() );
    if (property) {
      const_cast<Net*>( net )->remove( property );
    }
    if (net == _owner) _owner = NULL;
  }


  Bit* BitExtension::get ( const Net* net )
  {
    if (net == _owner) return _cache;
    _owner = net;

    Property* property = _owner->getProperty( BitProperty::getPropertyName() );
    if (property) _cache = static_cast<BitProperty*>(property)->getBit();
    else          _cache = NULL;

    return _cache;
  }


  Bit* BitExtension::create ( Net* net, size_t index )
  {
    get( net );
    if (_cache) return _cache;

    BitProperty* property = new BitProperty( net, index );
    net->put( property );

    _cache = property->getBit();
    return _cache;
  }


} // Spice namespace.
