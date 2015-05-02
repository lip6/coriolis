// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |        Alliance VHDL / Hurricane  Interface                     |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :   "./alliance/vst/VhdlNetProperty.cpp"           |
// +-----------------------------------------------------------------+


#include "crlcore/VhdlBit.h"
#include "crlcore/VhdlSignal.h"


namespace Vhdl {

  using namespace std;
  using Hurricane::_TName;
  using Hurricane::Property;


  const size_t  Bit::nindex = std::numeric_limits<size_t>::max();


// -------------------------------------------------------------------
// Class  :  "::Bit" (implementation).

  ptrdiff_t  Bit::_offset = 0;


  Bit::Bit ( BitProperty* property, const Net* net, Signal* signal, size_t index )
    : _signal(signal)
    , _index (index)
  {
    if (not _offset) {
    //_offset = offsetof(ScalarSignal,_toNet);
      _offset = (ptrdiff_t)this - (ptrdiff_t)property;
    }
  }


  Bit::Bit ( size_t index )
    : _signal(NULL)
    , _index (index)
  { }


  Bit::~Bit ()
  { }


  bool          Bit::isExternal () const { return getNet()->isExternal(); }
  bool          Bit::isElement  () const { return _index != nindex; }
  const Signal* Bit::getSignal  () const { return _signal; }
  size_t        Bit::getIndex   () const { return _index; }
  string        Bit::getName    () const { return _getString(); }


  string  Bit::_getString () const
  {
    string s = getSignal()->getName();

    if (_index != nindex) {
      if (getSignal()->isContiguous())
        s += "(" + getString(_index) + ")";
      else
        s += "_" + getString(_index);
    }

    return s;
  }


  Record* Bit::_getRecord () const
  {
    Record* record = new Record ( "<Bit " + _getString() + " >" );
    if (record != NULL) {
      record->add( getSlot("_signal", _signal) );
      record->add( getSlot("_index" , _index ) );
    }
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "BitProperty"

  Name  BitProperty::_name = "Vhdl::Bit";


  BitProperty* BitProperty::create ( Net* owner, Signal* signal, size_t index )
  {
    BitProperty *property = new BitProperty( owner, signal, index );

    property->_postCreate ();
    return property;
  }


  void  BitProperty::onReleasedBy ( DBo* owner )
  { PrivateProperty::onReleasedBy( owner ); }


  Name  BitProperty::getPropertyName ()
  { return _name; }


  Name  BitProperty::getName () const
  { return getPropertyName(); }


  string  BitProperty::_getTypeName () const
  { return _TName( "BitProperty" ); }


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


  Bit* BitExtension::get ( const Net* net )
  {
    if (net == _owner) return _cache;
    _owner = net;

    Property* property = _owner->getProperty( BitProperty::getPropertyName() );
    if (property) _cache = static_cast<BitProperty*>(property)->getBit();
    else          _cache = NULL;

    return _cache;
  }


  Bit* BitExtension::create ( Net* net, Signal* signal, size_t index )
  {
    get( net );
    if (_cache) return _cache;

    BitProperty* property = new BitProperty( net, signal, index );
    net->put( property );

    _cache = property->getBit();
    return _cache;
  }


} // Vhdl namespace.
