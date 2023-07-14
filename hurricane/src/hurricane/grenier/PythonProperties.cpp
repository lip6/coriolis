// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PythonProperties.cpp"                        |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PythonProperties.h"
#include <iomanip>
#include <ctime>
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/Cell.h"


namespace Isobar {

  using namespace std;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Property;


#ifdef PROPERTY_MAP_IMPLEMENTATION

// -------------------------------------------------------------------
// Class  :  "::PyObjectsMap".

  ptrdiff_t  PyObjectsMap::_offset = 0;

  
  string  PyObjectsMap::toPythonName ( std::string name )
  {
    if (name.substr(0,7) != "Python.") {
      cerr << Error( "PyObjectsMap::toPythonName(): Python properties *must* start with \"Python.\"\n"
                     "        (name \"%s\")"
                   , name.c_str() ) << endl;
      return string();
    }
    return name.substr( 7 );
  }


  PyObjectsMap::PyObjectsMap ( PyObjectsProperty* property )
    : _pyObjects()
  {
    if (not _offset) {
      _offset = (ptrdiff_t)this - (ptrdiff_t)property;
    }
  }


  PyObjectsMap::~PyObjectsMap ()
  {
    for ( auto item : _pyObjects ) {
      Py_DECREF( item.second );
    }
  }

  PyObjectsProperty* PyObjectsMap::getProperty  () const
  { return reinterpret_cast<PyObjectsProperty*>( (ptrdiff_t)this - _offset ); }

  
  DBo* PyObjectsMap::getOwner () const
  { return getProperty()->getOwner(); }

  
  void  PyObjectsMap::add ( std::string name, PyObject* object )
  {
    string pyPropName = toPythonName( name );
    if (pyPropName.empty()) return;

    auto imap = _pyObjects.find( name );
    if (imap != _pyObjects.end()) {
      cerr << Error( "PyObjectsMap::add(): Overwritting Python property \"%s\"\n"
                     "        (name \"%s\")"
                   , name.c_str() ) << endl;
      Py_DECREF( imap->second );
      _pyObjects.erase( imap );
    }

    Py_INCREF( object );
    _pyObjects.insert( make_pair(name,object) );
  }

  
  PyObject* PyObjectsMap::get ( std::string name ) const
  {
    string pyPropName = toPythonName( name );
    if (pyPropName.empty()) return NULL;

    auto imap = _pyObjects.find( name );
    if (imap == _pyObjects.end()) return NULL;
    return imap->second;
  }

  
  void  PyObjectsMap::remove ( std::string name )
  {
    string pyPropName = toPythonName( name );
    if (pyPropName.empty()) return;

    auto imap = _pyObjects.find( name );
    if (imap != _pyObjects.end()) {
      Py_DECREF( imap->second );
      _pyObjects.erase( imap );
    }
  }


  string  PyObjectsMap::_getString () const
  {
    string s = "<PyObjectsMap " + getString(getOwner()) + ">";
    return s;
  }


  Record* PyObjectsMap::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    if (record != NULL) {
      record->add( getSlot("_pyObjects", &_pyObjects) );
    }
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "PyObjectsProperty"

  Name  PyObjectsProperty::_name = "Isobar::PyObjectsProperty";


  PyObjectsProperty* PyObjectsProperty::create ( DBo* owner )
  {
    PyObjectsProperty *property = new PyObjectsProperty();

    property->_postCreate ();
    return property;
  }


  void  PyObjectsProperty::onReleasedBy ( DBo* owner )
  { PrivateProperty::onReleasedBy( owner ); }


  Name  PyObjectsProperty::getPropertyName ()
  { return _name; }


  Name  PyObjectsProperty::getName () const
  { return getPropertyName(); }


  string  PyObjectsProperty::_getTypeName () const
  { return "Isobar::PyObjectsProperty"; }


  string  PyObjectsProperty::_getString () const
  {
    string s = PrivateProperty::_getString ();
    s.insert ( s.length() - 1 , " " + getString(&_pyObjectsMap) );

    return s;
  }


  Record* PyObjectsProperty::_getRecord () const
  {
    Record* record = PrivateProperty::_getRecord();
    if ( record ) {
      record->add( getSlot( "_name"        ,  _name         ) );
      record->add( getSlot( "_pyObjectsMap", &_pyObjectsMap ) );
    }
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "PythonProperties"

  std::vector<PyObjectsProperty*>  PythonProperties::_allocateds;


  PyObjectsMap* PythonProperties::getMap ( const DBo* dbo )
  {
    Property* property = dbo->getProperty( PyObjectsProperty::getPropertyName() );
    if (property)
      return static_cast<PyObjectsProperty*>( property )->getMap();
    return NULL;
  }


  PyObjectsMap* PythonProperties::create ( DBo* dbo )
  {
    PyObjectsProperty* property = static_cast<PyObjectsProperty*>
      ( dbo->getProperty( PyObjectsProperty::getPropertyName() ));
    if (not property) {
      property = PyObjectsProperty::create( dbo );
      dbo->put( property );
      _allocateds.push_back( property );
    }
    return property->getMap();
  }


  void  PythonProperties::destroy ( DBo* dbo )
  {
    Property* property = dbo->getProperty( PyObjectsProperty::getPropertyName() );
    if (property) {
      dbo->remove( property );
      for ( size_t i=0 ; i<_allocateds.size(); ++i ) {
        if (_allocateds[i] == property) {
          _allocateds[i] = _allocateds[ _allocateds.size() - 1 ];
          _allocateds.resize( _allocateds.size() - 1 );
          break;
        }
      }
    }
  }


  void  PythonProperties::removeAll ( std::string name )
  {
    if (_allocateds.empty()) return;
    
    size_t removeds = 0;
    if (not name.empty()) {
      for ( size_t i=0 ; i+removeds<_allocateds.size() ; ) {
        PyObjectsMap* pyMap = _allocateds[i]->getMap();
        pyMap->remove( name );
        if (pyMap->empty()) {
          _allocateds[i]->getOwner()->remove( _allocateds[i] );
          ++removeds;
          _allocateds[i] = _allocateds[ _allocateds.size()-removeds ];
        } else
          ++i;
      }
      if (removeds)
        _allocateds.resize( _allocateds.size() - removeds );
    } else {
      for ( size_t i=0 ; i<_allocateds.size() ; ++i ) {
        _allocateds[i]->getOwner()->remove( _allocateds[i] );
      }
      _allocateds.clear();
    }
  }

#else


// -------------------------------------------------------------------
// Class  :  "PyObjectsProperty"

  Name  PyObjectsProperty::_name = "Isobar::PyObjectsProperty";


  PyObjectsProperty* PyObjectsProperty::create ( DBo* owner )
  {
    PyObjectsProperty *property = new PyObjectsProperty();

    property->_postCreate ();
    return property;
  }

  
  void  PyObjectsProperty::_preDestroy ()
  {
    Py_DECREF( _pyObject );
    Super::_preDestroy();
  }


  void  PyObjectsProperty::onReleasedBy ( DBo* owner )
  { PrivateProperty::onReleasedBy( owner ); }


  Name  PyObjectsProperty::getPropertyName ()
  { return _name; }


  Name  PyObjectsProperty::getName () const
  { return getPropertyName(); }


  string  PyObjectsProperty::_getTypeName () const
  { return "Isobar::PyObjectsProperty"; }


  string  PyObjectsProperty::_getString () const
  {
    string s = PrivateProperty::_getString ();
    s.insert ( s.length() - 1 , " " + getString(_pyObject) );

    return s;
  }


  Record* PyObjectsProperty::_getRecord () const
  {
    Record* record = PrivateProperty::_getRecord();
    if ( record ) {
      record->add( getSlot( "_name"    , _name     ) );
      record->add( getSlot( "_pyObject", _pyObject ) );
    }
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "PythonProperties"

  std::vector<PyObjectsProperty*>  PythonProperties::_allocateds;


  PyObject* PythonProperties::get ( const DBo* dbo )
  {
    Property* property = dbo->getProperty( PyObjectsProperty::getPropertyName() );
    if (property)
      return property->getHolder();
    return NULL;
  }


  PyObject* PythonProperties::create ( DBo* dbo )
  {
    PyObjectsProperty* property = static_cast<PyObjectsProperty*>
      ( dbo->getProperty( PyObjectsProperty::getPropertyName() ));
    if (not property) {
      property = PyObjectsProperty::create( dbo );
      dbo->put( property );
      _allocateds.push_back( property );
    }
    return property->getPyObject();
  }


  void  PythonProperties::destroy ( DBo* dbo )
  {
    Property* property = dbo->getProperty( PyObjectsProperty::getPropertyName() );
    if (property) {
      dbo->remove( property );
      for ( size_t i=0 ; i<_allocateds.size(); ++i ) {
        if (_allocateds[i] == property) {
          _allocateds[i] = _allocateds[ _allocateds.size() - 1 ];
          _allocateds.resize( _allocateds.size() - 1 );
          break;
        }
      }
    }
  }


  void  PythonProperties::removeAll ( std::string name )
  {
    if (_allocateds.empty()) return;
    
    size_t removeds = 0;
    if (not name.empty()) {
      for ( size_t i=0 ; i+removeds<_allocateds.size() ; ) {
        // PyObject* pyObject = _allocateds[i]->getPyObject();
        // pyMap->remove( name );
        // if (pyMap->empty()) {
        //   _allocateds[i]->getOwner()->remove( _allocateds[i] );
        //   ++removeds;
        //   _allocateds[i] = _allocateds[ _allocateds.size()-removeds ];
        // } else
          ++i;
      }
      if (removeds)
        _allocateds.resize( _allocateds.size() - removeds );
    } else {
      for ( size_t i=0 ; i<_allocateds.size() ; ++i ) {
        _allocateds[i]->getOwner()->remove( _allocateds[i] );
      }
      _allocateds.clear();
    }
  }


#endif


}  // Isobar namespace.
