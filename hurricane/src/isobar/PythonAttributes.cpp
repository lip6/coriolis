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
// |  C++ Module  :  "./PythonAttributes.cpp"                        |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PythonAttributes.h"
#include <iomanip>
#include <ctime>
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/Cell.h"


namespace Isobar {

  class PyHolderProperty;

}

namespace {

  std::vector<Isobar::PyHolderProperty*>  allPyProperties;

}  // Anonymous namespace.


namespace Isobar {

  using namespace std;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Property;


  extern "C" {

    static PyAttributesHolder* PyAttributesHolder_New ()
    {
      PyAttributesHolder* self = PyObject_NEW( PyAttributesHolder, &PyTypeAttributesHolder );
      self->dict = PyDict_New();
      return self;
    }


    static void  PyAttributesHolder_DeAlloc ( PyAttributesHolder* self )
    {
      Py_DECREF( self->dict );
    }

  
    static PyObject* PyAttributesHolder_getDictSize ( PyAttributesHolder* self, PyObject* )
    { return Py_BuildValue( "i", PyDict_Size( self->dict )); }


    PyMethodDef  PyAttributesHolder_Methods[] =
      { { "getDictSize"     , (PyCFunction)PyAttributesHolder_getDictSize, METH_NOARGS
                            , "Return the number of attributes in the dictionnary." }
      , {NULL, NULL, 0, NULL}  /* sentinel */
      };


#if __GNUC__ && (__GNUC__ > 8)
    PyTypeObject  PyTypeAttributesHolder = {
      PyVarObject_HEAD_INIT(NULL,0)
      .tp_name       = "Hurricane.AttributesHolder",
      .tp_basicsize  =  sizeof( PyAttributesHolder ),
      .tp_dealloc    =  (destructor)PyAttributesHolder_DeAlloc,
      .tp_getattro   =  PyObject_GenericGetAttr,
      .tp_setattro   =  PyObject_GenericSetAttr,
      .tp_flags      =  Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,
      .tp_doc        =  PyDoc_STR("Attributes holder for ancillary Python object (Property)."),
      .tp_methods    =  PyAttributesHolder_Methods,
      .tp_dictoffset =  offsetof( PyAttributesHolder, dict ),
    };
#else
    PyTypeObject  PyTypeAttributesHolder =
      { PyVarObject_HEAD_INIT(NULL,0)
        "Hurricane.AttributeHolder"             /* tp_name.          */
      , sizeof( PyAttributeHolder )             /* tp_basicsize.     */          
      , 0                                       /* tp_itemsize.      */          
      , (destructor)PyAttribute_DeAlloc         /* tp_dealloc.       */          
      , 0                                       /* tp_print.         */          
      , 0                                       /* tp_getattr.       */          
      , 0                                       /* tp_setattr.       */          
      , 0                                       /* tp_compare.       */          
      , 0                                       /* tp_repr.          */          
      , 0                                       /* tp_as_number.     */          
      , 0                                       /* tp_as_sequence.   */          
      , 0                                       /* tp_as_mapping.    */          
      , 0                                       /* tp_hash.          */          
      , 0                                       /* tp_call.          */          
      , 0                                       /* tp_str            */          
      , PyObject_GenericGetAttr                 /* tp_getattro.      */          
      , PyObject_GenericSetAttr                 /* tp_setattro.      */          
      , 0                                       /* tp_as_buffer.     */          
      , Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE  /* tp_flags          */          
      , PyDoc_STR("Attributes holder for ancillary Python object (Property).")
                                                /* tp_doc.           */          
      , 0                                       /* tp_traverse       */
      , 0                                       /* tp_clear          */
      , 0                                       /* tp_richcompare    */
      , 0                                       /* tp_weaklistoffset */
      , 0                                       /* tp_iter           */
      , 0                                       /* tp_iternext       */
      , PyAttributesHolder_Methods              /* tp_methods        */
      , 0                                       /* tp_members        */
      , 0                                       /* tp_getset         */
      , 0                                       /* tp_base           */
      , 0                                       /* tp_dict           */
      , 0                                       /* tp_decr_get       */
      , 0                                       /* tp_decr_set       */
      , offsetof( PyAttributesHolder, dict )    /* tp_dictoffset     */
      };

      PyVarObject_HEAD_INIT(NULL,0)
      .tp_name       = "Hurricane.AttributesHolder",
      .tp_basicsize  =  sizeof( PyAttributesHolder ),
      .tp_dealloc    =  (destructor)PyAttributesHolder_DeAlloc,
      .tp_getattro   =  PyObject_GenericGetAttr,
      .tp_setattro   =  PyObject_GenericSetAttr,
      .tp_flags      =  Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,
      .tp_doc        =  PyDoc_STR("Attributes holder for ancillary Python object (Property)."),
      .tp_methods    =  PyAttributesHolder_Methods,
      .tp_dictoffset =  offsetof( PyAttributesHolder, dict ),
    };
#endif

  }  // extern "C".


// -------------------------------------------------------------------
// Class  :  "PyHolderProperty"

  Name  PyHolderProperty::_name = "Isobar::PyHolderProperty";


  PyHolderProperty* PyHolderProperty::create ( DBo* owner )
  {
    PyHolderProperty *property = new PyHolderProperty();

    property->_holder = PyAttributesHolder_New();
    property->_postCreate ();
    return property;
  }

  
  void  PyHolderProperty::_preDestroy ()
  {
    cout.flush();
    cerr << "Removing PyHolderProperty on " << getOwner() << endl;
    Py_DECREF( _holder );
    Super::_preDestroy();
  }

  
  int32_t  PyHolderProperty::delattr ( string attrName )
  {
    PyObject* attr = PyDict_GetItemString( _holder->dict, attrName.c_str() );
    if (not attr) {
      cerr << Error( "PyHolderProperty::delattr(): %s has no attribute \"%\"."
                   , getString(getOwner()).c_str()
                   ) << endl;
      return -1;
    }
    PyDict_DelItemString( _holder->dict, attrName.c_str() );
    
    return PyDict_Size( _holder->dict );
  }


  void  PyHolderProperty::onReleasedBy ( DBo* owner )
  { PrivateProperty::onReleasedBy( owner ); }


  Name  PyHolderProperty::getPropertyName ()
  { return _name; }


  Name  PyHolderProperty::getName () const
  { return getPropertyName(); }


  string  PyHolderProperty::_getTypeName () const
  { return "Isobar::PyHolderProperty"; }


  string  PyHolderProperty::_getString () const
  {
    string s = PrivateProperty::_getString ();
    s.insert ( s.length() - 1 , " " + getString(_holder) );

    return s;
  }


  Record* PyHolderProperty::_getRecord () const
  {
    Record* record = PrivateProperty::_getRecord();
    if ( record ) {
      record->add( getSlot( "_name"  , _name   ) );
      record->add( getSlot( "_holder", _holder ) );
    }
    return record;
  }


// -------------------------------------------------------------------
// Class  :  "PythonAttributes"


  PyAttributesHolder* PythonAttributes::get ( const DBo* dbo )
  {
    Property* property = dbo->getProperty( PyHolderProperty::getPropertyName() );
    if (property)
      return dynamic_cast<PyHolderProperty*>( property )->getHolder();
    return NULL;
  }


  int32_t  PythonAttributes::delattr ( DBo* dbo, string attrName )
  {
    PyHolderProperty* property = static_cast<PyHolderProperty*>
      ( dbo->getProperty( PyHolderProperty::getPropertyName() ));
    if (not property) return -1;
    return property->delattr( attrName );
  }


  void  PythonAttributes::enable ( DBo* dbo )
  {
    PyHolderProperty* property = static_cast<PyHolderProperty*>
      ( dbo->getProperty( PyHolderProperty::getPropertyName() ));
    if (not property) {
      property = PyHolderProperty::create( dbo );
      dbo->put( property );
      allPyProperties.push_back( property );
    }
  }


  void  PythonAttributes::disable ( DBo* dbo )
  {
    Property* property = dbo->getProperty( PyHolderProperty::getPropertyName() );
    if (property) {
      dbo->remove( property );
      for ( size_t i=0 ; i<allPyProperties.size(); ++i ) {
        if (allPyProperties[i] == property) {
          allPyProperties[i] = allPyProperties[ allPyProperties.size() - 1 ];
          allPyProperties.resize( allPyProperties.size() - 1 );
          break;
        }
      }
    }
  }


  void  PythonAttributes::disableAll ()
  {
    for ( size_t i=0 ; i<allPyProperties.size() ; ++i ) {
      allPyProperties[i]->getOwner()->remove( allPyProperties[i] );
    }
    allPyProperties.clear();
  }


  void  PythonAttributes::disableAll ( std::string name )
  {
    cout.flush();
    cerr << "disableAll() name=" << name << endl;
    if (allPyProperties.empty()) return;
    
    size_t removeds = 0;
    for ( size_t i=0 ; i+removeds<allPyProperties.size() ; ) {
      cerr << "i=" << i << endl;
      if (not name.empty())
        allPyProperties[i]->delattr( name );
      if (name.empty() or (allPyProperties[i]->getDictSize() == 0)) {
        allPyProperties[i]->getOwner()->remove( allPyProperties[i] );
        ++removeds;
        allPyProperties[i] = allPyProperties[ allPyProperties.size()-removeds ];
      } else
        ++i;
    }
    if (removeds)
      allPyProperties.resize( allPyProperties.size() - removeds );
  }


  int32_t  PythonAttributes::count ()
  { return allPyProperties.size(); }


}  // Isobar namespace.
