// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2019-2019, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |   E t e s i a n  -  A n a l y t i c   P l a c e r               |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./BloatProperty.cpp"                           |
// +-----------------------------------------------------------------+


#include "hurricane/Initializer.h"
#include "hurricane/Error.h"
#include "hurricane/Cell.h"
#include "etesian/BloatProperty.h"


namespace Etesian {

  using namespace std;
  using Hurricane::Property;
  using Hurricane::Initializer;
  using Hurricane::JsonTypes;
  using Hurricane::Error;


// -------------------------------------------------------------------
// Class  :  "BloatState"

  string  BloatState::_getString () const
  {
    string  s;
    s += getString(_tracksCount);
    return s;
  }


  Record* BloatState::_getRecord () const
  {
    Record* record = new Record ( "<BloatState " + _getString() + " >" );
    if (record != NULL) {
      record->add( getSlot("_tracksCount", _tracksCount) );
    }
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "BloatProperty"

  Name  BloatProperty::_name = "Hurricane Bloat State";


  BloatProperty* BloatProperty::create ()
  {
    BloatProperty *property = new BloatProperty();

    property->_postCreate ();
    return property;
  }


  void  BloatProperty::onReleasedBy ( DBo* owner )
  {
    PrivateProperty::onReleasedBy( owner );
  }


  Name  BloatProperty::getPropertyName ()
  { return _name; }


  Name  BloatProperty::getName () const
  { return getPropertyName(); }


  string  BloatProperty::_getTypeName () const
  { return "BloatProperty"; }


  string  BloatProperty::_getString () const
  {
    string s = PrivateProperty::_getString ();
    s.insert ( s.length() - 1 , " " + getString(&_state) );

    return s;
  }


  Record* BloatProperty::_getRecord () const
  {
    Record* record = PrivateProperty::_getRecord();
    if ( record ) {
      record->add( getSlot("_name" , _name ) );
      record->add( getSlot("_state",&_state) );
    }
    return record;
  }


  bool  BloatProperty::hasJson () const
  { return true; }


  void  BloatProperty::toJson ( JsonWriter* w, const DBo* ) const
  {
    w->startObject();
    jsonWrite( w, "@typename", _getTypeName()      );
    jsonWrite( w, "_state"   , _state._getString() );
    w->endObject();
  }


// -------------------------------------------------------------------
// Class  :  "JsonBloatProperty"

  Initializer<JsonBloatProperty>  jsonBloatPropertyInit ( 20 );


  JsonBloatProperty::JsonBloatProperty ( unsigned long flags )
    : JsonObject(flags)
  { 
    add( "_state", typeid(string) );
  }


  string  JsonBloatProperty::getTypeName () const
  { return "BloatProperty"; }


  void  JsonBloatProperty::initialize ()
  { JsonTypes::registerType( new JsonBloatProperty (JsonWriter::RegisterMode) ); }


  JsonBloatProperty* JsonBloatProperty::clone ( unsigned long flags ) const
  { return new JsonBloatProperty ( flags ); }


  void JsonBloatProperty::toData ( JsonStack& stack )
  {
    check( stack, "JsonBloatProperty::toData" );

    uint32_t       tracksCount = get<uint32_t>( stack, "_state" );
    BloatProperty* property    = NULL;
    DBo*                dbo    = stack.back_dbo();

    if (dbo) {
      Cell* cell = dynamic_cast<Cell*>( dbo );
      if (cell) {
        property = dynamic_cast<BloatProperty*>( cell->getProperty( BloatProperty::getPropertyName() ) );
        if (property) {
          cerr << Error( "JsonBloatProperty::toData(): %s has already a BloatProperty (overwrite)."
                       , getString(cell).c_str()
                       )  << endl;
          BloatState* state = property->getState();
          state->setTracksCount( tracksCount );
        } else {
          property = BloatProperty::create();
          property->getState()->setTracksCount( tracksCount );
          cell->put( property );
        }
      } else {
        cerr << Error( "JsonBloatProperty::toData(): %s must be a Cell."
                     , getString(dbo).c_str()
                     ) << endl;
      }
    } else {
      cerr << Error( "JsonBloatProperty::toData(): No DBo in stack to attach to." ) << endl;
    }

    update( stack, property );
  }


// -------------------------------------------------------------------
// Class  :  "BloatExtension"


  Occurrence  BloatExtension::_owner;
  BloatState* BloatExtension::_cache = NULL;


  BloatState* BloatExtension::get ( Occurrence o )
  {
    if (o == _owner) return _cache;
    _owner = o;

    Property* property = _owner.getProperty( BloatProperty::getPropertyName() );
    if (property) _cache = static_cast<BloatProperty*>(property)->getState();
    else          _cache = NULL;

    return _cache;
  }


  BloatState* BloatExtension::create ( Occurrence o, size_t tracksCount )
  {
    get( o );
    if (not _cache) {
      BloatProperty* property = new BloatProperty();
      o.put( property );

      _cache = property->getState();
    }
    _cache->setTracksCount( tracksCount );
    return _cache;
  }


}  // Etesian namespace.
