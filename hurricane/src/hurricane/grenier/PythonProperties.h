// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :   "./hurricane/isobar/PythonProperties.h"        |
// +-----------------------------------------------------------------+


#pragma  once
#include <Python.h>
#include <string>
#include "hurricane/Name.h"
#include "hurricane/Property.h"
#include "hurricane/Slot.h"
#include "hurricane/DBo.h"
#include "hurricane/isobar/PyAttributesHolder.h"


namespace Isobar {

  using Hurricane::Record;
  using Hurricane::Name;
  using Hurricane::DBo;
  using Hurricane::PrivateProperty;
  class PyObjectsProperty;


#ifdef PROPERTY_MAP_IMPLEMENTATION

// -------------------------------------------------------------------
// Class  :  "Isobar::PyObjectsMap".

  class PyObjectsMap {
    public:           
      static std::string        toPythonName ( std::string );
    public:                    
                                PyObjectsMap ( PyObjectsProperty* );
                               ~PyObjectsMap ();
      inline bool               empty        () const;
             PyObjectsProperty* getProperty  () const;
             DBo*               getOwner     () const;
             void               add          ( std::string, PyObject* );
             PyObject*          get          ( std::string ) const;
             void               remove       ( std::string );
             std::string        _getString   () const;
             Record*            _getRecord   () const;
    private:
      static std::ptrdiff_t                   _offset;
             std::map<std::string,PyObject*>  _pyObjects;
  };

  
  inline bool  PyObjectsMap::empty () const { return _pyObjects.empty(); }


// -------------------------------------------------------------------
// Class  :  "Isobar::PyObjectsProperty".

  class PyObjectsProperty : public Hurricane::PrivateProperty {
      friend class PythonProperties;
    public:
      static  Name  _name;
    public:
      static  PyObjectsProperty* create            ( DBo* );
      static  Name               getPropertyName   ();
      virtual Name               getName           () const;
      inline  PyObjectsMap*      getMap            ();
      virtual void               onReleasedBy      ( DBo* owner );
      virtual std::string        _getTypeName      () const;
      virtual std::string        _getString        () const;
      virtual Record*            _getRecord        () const;
    protected:
    // Attributes.
      PyObjectsMap  _pyObjectsMap;
      
    protected:
    // Constructor.
      inline  PyObjectsProperty ();
  };


  inline  PyObjectsProperty::PyObjectsProperty ()
    : PrivateProperty(), _pyObjectsMap(this)
  { }


  inline PyObjectsMap* PyObjectsProperty::getMap () { return &_pyObjectsMap; }


// -------------------------------------------------------------------
// Class  :  "Isobar::PythonProperties".

  class PythonProperties {
    public:
      static        PyObjectsMap* getMap    ( const DBo* );
      static        PyObjectsMap* create    ( DBo* );
      static        void          destroy   ( DBo* );
      static inline PyObject*     get       ( const DBo*, std::string );
      static inline void          add       ( DBo*, std::string, PyObject* );
      static inline void          remove    ( DBo*, std::string );
      static        void          removeAll ( std::string );
    private:
      static std::vector<PyObjectsProperty*>  _allocateds;
  };


  inline PyObject* PythonProperties::get ( const DBo* dbo, std::string name )
  {
    PyObjectsMap* pyMap = getMap( dbo );
    return (pyMap == nullptr) ? nullptr : pyMap->get( name );
  }


  inline void  PythonProperties::add ( DBo* dbo, std::string name, PyObject* pyObject )
  {
    PyObjectsMap* pyMap = getMap( dbo );
    if (pyMap == nullptr)
      pyMap = create( dbo );
    pyMap->add( name, pyObject );
  }


  inline void  PythonProperties::remove ( DBo* dbo, std::string name )
  {
    PyObjectsMap* pyMap = getMap( dbo );
    if (pyMap == nullptr) return;
    pyMap->remove( name );
    if (pyMap->empty()) {
      destroy( dbo );
    }
  }

#else


// -------------------------------------------------------------------
// Class  :  "Isobar::PyObjectsProperty".

  class PyObjectsProperty : public Hurricane::PrivateProperty {
      friend class PythonProperties;
    public:
      typedef Hurricane::PrivateProperty  Super;
      static  Name  _name;
    public:
      static  PyObjectsProperty* create            ( DBo* );
      static  Name               getPropertyName   ();
      virtual Name               getName           () const;
      inline  PyObject*          getPyObject       ();
      virtual void               onReleasedBy      ( DBo* owner );
      virtual std::string        _getTypeName      () const;
      virtual std::string        _getString        () const;
      virtual Record*            _getRecord        () const;
    protected:
      virtual void               _preDestroy       ();
    protected:
    // Attributes.
      PyObject*  _pyObject;
      
    protected:
    // Constructor.
      inline  PyObjectsProperty ();
  };


  inline  PyObjectsProperty::PyObjectsProperty ()
    : PrivateProperty(), _pyObject(NULL)
  {
    _pyObject = (PyObject*)PyObject_NEW( PyAttributesHolder, &PyTypeAttributesHolder );
  }


  inline PyObject* PyObjectsProperty::getPyObject () { return _pyObject; }


// -------------------------------------------------------------------
// Class  :  "Isobar::PythonProperties".

  class PythonProperties {
    public:
      static        PyObject* get       ( const DBo* );
      static        PyObject* create    ( DBo* );
      static        void      destroy   ( DBo* );
      static        void      removeAll ( std::string );
    private:
      static std::vector<PyObjectsProperty*>  _allocateds;
  };

#endif


}  // Isobar Namespace.


#ifdef PROPERTY_MAP_IMPLEMENTATION
INSPECTOR_P_SUPPORT(Isobar::PyObjectsMap);
INSPECTOR_P_SUPPORT(Isobar::PyObjectsProperty);
#endif
