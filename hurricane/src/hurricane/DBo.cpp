
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
// |  C++ Module  :       "./DBo.cpp"                                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include "hurricane/DBo.h"
#include "hurricane/Property.h"
#include "hurricane/Quark.h"
#include "hurricane/Error.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::DBo".


  DBo::DBo (): _propertySet()
  { }


  DBo::~DBo ()
  { }


  void  DBo::_postCreate ()
  { }


  void  DBo::_preDestroy ()
  {
    clearProperties ();
  }


  void DBo::destroy ()
  {
    _preDestroy();
    
    delete this;
  }


  Property* DBo::getProperty ( const Name& name ) const
  {
    PropertySet::const_iterator iterator = _propertySet.begin();
    while ( iterator != _propertySet.end() ) {
      Property* property = *iterator;
      if (property->getName() == name) return property;
      ++iterator;
    }
    return NULL;
  }


  Properties  DBo::getProperties () const
  {
    return getCollection(_propertySet);
  }


  void  DBo::put ( Property* property )
  {
    if ( !property )
      throw Error("DBo::put(): Can't put property : NULL property.");

    Property* oldProperty = getProperty ( property->getName() );
    if ( property != oldProperty ) {
      if ( oldProperty ) {
        _propertySet.erase ( oldProperty );
        oldProperty->onReleasedBy ( this );
      }
      _propertySet.insert ( property );
      property->onCapturedBy ( this );
    }
  }


  void  DBo::remove ( Property* property )
  {
    if ( !property )
      throw Error("DBo::remove(): Can't remove property : NULL property.");

    if ( _propertySet.find(property) != _propertySet.end() ) {
      _propertySet.erase ( property );
      property->onReleasedBy ( this );
      if ( dynamic_cast<Quark*>(this) && _propertySet.empty() )
        destroy();
    }
  }


  void  DBo::removeProperty ( const Name& name )
  {
    Property* property = getProperty ( name );
    if ( property ) {
      _propertySet.erase ( property );
      property->onReleasedBy ( this );
      if ( dynamic_cast<Quark*>(this) && _propertySet.empty() )
        destroy();
    }
  }


  void  DBo::_onDestroyed ( Property* property )
  {
    if ( property && ( _propertySet.find(property) != _propertySet.end() ) ) {
      _propertySet.erase ( property );
      if ( dynamic_cast<Quark*>(this) && _propertySet.empty() )
        destroy();
    }
  }


  void  DBo::clearProperties ()
  {
    while ( !_propertySet.empty() ) {
      Property* property = *_propertySet.begin();
      _propertySet.erase ( property );
      property->onReleasedBy ( this );
    }
  }


  string  DBo::_getTypeName () const
  {
    return "DBo";
  }


  string  DBo::_getString () const
  {
    return "<" + _getTypeName() + ">";
  }


  Record* DBo::_getRecord () const
  {
    Record* record = new Record ( getString(this) );
    record->add ( getSlot("Properties", &_propertySet) );
    return record;
  }


} // End of Hurricane namespace.
