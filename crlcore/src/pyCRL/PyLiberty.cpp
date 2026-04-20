// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2013-2026, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                             Hippolyte MELICA     |
// |  E-mail      :  hippolyte.melica@etu.sorbonne-universite.fr     |
// | =============================================================== |
// |  C++ Module  :  "./PyLiberty.cpp"                               |
// +-----------------------------------------------------------------+


#include "crlcore/PyLiberty.h"
#include "hurricane/isobar/PyDataBase.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/DataBase.h"
#include "hurricane/Property.h"
#include "Library.h"
#include "Group.h"
#include "Attribute.h"
#include "Value.h"
#include "LibertyProperty.h"
#include <string>
#include <sstream>


namespace  CRL {

  using std::cerr;
  using std::endl;
  using std::hex;
  using std::string;
  using std::ostringstream;
  using Hurricane::tab;
  using Hurricane::Exception;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::DataBase;
  using Hurricane::Cell;
  using Isobar::PyTypeDataBase;
  using Isobar::PyDataBase;
  using Isobar::PyTypeCell;
  using Isobar::PyCell;
  using Isobar::ProxyProperty;
  using Isobar::ProxyError;
  using Isobar::ConstructorError;
  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;
  using Isobar::getPyHash;
  using Isobar::ParseOneArg;
  using Isobar::ParseTwoArg;
  using Isobar::__cs;

  typedef Liberty::Library   LibertyLibrary;
  typedef Liberty::Group     LibertyGroup;
  typedef Liberty::Attribute LibertyAttribute;
  typedef Liberty::Value     LibertyValue;


extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(LibertyLibrary,liberty,function)
#define METHOD_HEAD_GROUP(function) GENERIC_METHOD_HEAD(LibertyGroup,group,function)
#define METHOD_HEAD_ATTRIBUTE(function) GENERIC_METHOD_HEAD(LibertyAttribute,attribute,function)
#define METHOD_HEAD_VALUE(function) GENERIC_METHOD_HEAD(LibertyValue,value,function)


#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |          "PyLiberty" Python Module Code Part                    |
// +=================================================================+


  static PyObject* PyLibertyLibrary_NEW ( PyTypeObject* type, PyObject* args, PyObject* kwArgs )
  {
    cdebug_log(30,0) << "PyLibertyLibrary_NEW()" << endl;

    Liberty::Library* liberty = NULL;

    HTRY
      char* filepath = NULL;
      static char* keywords[] = { (char*)"filepath", NULL };

      if (not PyArg_ParseTupleAndKeywords( args
                                         , kwArgs
                                         , "s:LibertyLibrary.__new__"
                                         , keywords
                                         , &filepath )) {
        PyErr_SetString ( ConstructorError, "LibertyLibrary(): Invalid number or type of parameters." );
        return NULL;
      }

      liberty = new Liberty::Library( filepath );
    HCATCH

    return PyLibertyLibrary_Link( liberty );
  }


  static int PyLibertyLibrary_Init ( PyLibertyLibrary*, PyObject*, PyObject* )
  {
    cdebug_log(30,0) << "PyLibertyLibrary_Init()" << endl;
    return 0;
  }


  static PyObject* PyLibertyLibrary_getName ( PyLibertyLibrary* self )
  {
    cdebug_log(30,0) << "PyLibertyLibrary_getName()" << endl;
    HTRY
      METHOD_HEAD("LibertyLibrary.getName()")
      return PyString_FromString( liberty->getName().c_str() );
    HCATCH
    return NULL;
  }


  static PyObject* PyLibertyLibrary_getFilePath ( PyLibertyLibrary* self )
  {
    cdebug_log(30,0) << "PyLibertyLibrary_getFilePath()" << endl;
    HTRY
      METHOD_HEAD("LibertyLibrary.getFilePath()")
      return PyString_FromString( liberty->getFilePath().c_str() );
    HCATCH
    return NULL;
  }


  static PyObject* PyLibertyLibrary_getGroupIdentifier ( PyLibertyLibrary* self )
  {
    cdebug_log(30,0) << "PyLibertyLibrary_getGroupIdentifier()" << endl;
    HTRY
      METHOD_HEAD("LibertyLibrary.getGroupIdentifier()")
      return PyString_FromString( liberty->getGroupIdentifier().c_str() );
    HCATCH
    return NULL;
  }


  static PyObject* PyLibertyLibrary_setGroupIdentifier ( PyLibertyLibrary* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyLibertyLibrary_setGroupIdentifier()" << endl;

    HTRY
      METHOD_HEAD("LibertyLibrary.setGroupIdentifier()")
      char* groupId = NULL;
      if ( not PyArg_ParseTuple( args, "s:LibertyLibrary.setGroupIdentifier", &groupId ) ) {
        PyErr_SetString( ConstructorError, "LibertyLibrary.setGroupIdentifier(): Invalid number or type of parameters." );
        return NULL;
      }
      liberty->setGroupIdentifier( std::string(groupId) );
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyLibertyLibrary_load ( PyLibertyLibrary* self )
  {
    cdebug_log(30,0) << "PyLibertyLibrary_load()" << endl;

    bool loaded = false;
    HTRY
      METHOD_HEAD("LibertyLibrary.load()")
      loaded = liberty->load();
    HCATCH

    if (loaded) Py_RETURN_TRUE;
    Py_RETURN_FALSE;
  }


  static PyObject* PyLibertyLibrary_mapLibertyToDb ( PyLibertyLibrary* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyLibertyLibrary_mapLibertyToDb()" << endl;

    HTRY
      METHOD_HEAD("LibertyLibrary.mapLibertyToDb()")
      PyObject* pyDataBase = NULL;
      // Make db argument optional - if not provided, use DataBase::getDB()
      if ( not PyArg_ParseTuple( args, "|O:LibertyLibrary.mapLibertyToDb", &pyDataBase ) ) {
        PyErr_SetString ( ConstructorError, "LibertyLibrary.mapLibertyToDb(): Invalid arguments." );
        return NULL;
      }

      Hurricane::DataBase* db = NULL;
      if ( pyDataBase ) {
        // Use provided DataBase argument
        if ( not IsPyDataBase(pyDataBase) ) {
          PyErr_SetString ( ConstructorError, "LibertyLibrary.mapLibertyToDb(): Argument is not of type DataBase." );
          return NULL;
        }
        db = PYDATABASE_O(pyDataBase);
      } else {
        // Get current DataBase automatically
        db = DataBase::getDB();
        if ( not db ) {
          PyErr_SetString ( ConstructorError, "LibertyLibrary.mapLibertyToDb(): No DataBase created (call DataBase.getDB() first)." );
          return NULL;
        }
      }

      liberty->mapLibertyToDb( db );
      // Ownership is now held by the C++ DataBase side.
      // Detach from Python wrapper so tp_dealloc won't delete it.
      self->_object = NULL;
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyLibertyLibrary_getRootGroup ( PyLibertyLibrary* self )
  {
    cdebug_log(30,0) << "PyLibertyLibrary_getRootGroup()" << endl;
    HTRY
      METHOD_HEAD("LibertyLibrary.getRootGroup()")
      // Library extends SimpleGroup which extends Group, so we can cast it
      Liberty::Group* group = static_cast<Liberty::Group*>(liberty);
      return PyLibertyGroup_Link( group );
    HCATCH
    return NULL;
  }


  static PyObject* PyLibertyLibrary_getCellGroup ( PyLibertyLibrary* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyLibertyLibrary_getCellGroup()" << endl;
    HTRY
      METHOD_HEAD("LibertyLibrary.getCellGroup()")
      char* cellName = NULL;
      if ( not PyArg_ParseTuple( args, "s:LibertyLibrary.getCellGroup", &cellName ) ) {
        PyErr_SetString( ConstructorError, "LibertyLibrary.getCellGroup(): Invalid arguments." );
        return NULL;
      }
      Liberty::Group* group = liberty->getCellGroup( cellName );
      if ( group ) return PyLibertyGroup_Link( group );
      Py_RETURN_NONE;
    HCATCH
    return NULL;
  }


  PyMethodDef PyLibertyLibrary_Methods[] =
    { { "getName"           , (PyCFunction)PyLibertyLibrary_getName           , METH_NOARGS
                             , "Return the name of the Liberty library." }
    , { "getFilePath"       , (PyCFunction)PyLibertyLibrary_getFilePath       , METH_NOARGS
                             , "Return the Liberty file path." }
    , { "getGroupIdentifier" , (PyCFunction)PyLibertyLibrary_getGroupIdentifier, METH_NOARGS
                             , "Return the top-level group identifier." }
    , { "setGroupIdentifier" , (PyCFunction)PyLibertyLibrary_setGroupIdentifier, METH_VARARGS
                             , "Set the top-level group identifier." }
    , { "load"              , (PyCFunction)PyLibertyLibrary_load              , METH_NOARGS
                             , "Parse the Liberty file." }
    , { "mapLibertyToDb"       , (PyCFunction)PyLibertyLibrary_mapLibertyToDb       , METH_VARARGS
                             , "Map Liberty cells onto the Hurricane database (db=DataBase.getDB() by default). After success, ownership is transferred to C++." }
    , { "getRootGroup"      , (PyCFunction)PyLibertyLibrary_getRootGroup      , METH_NOARGS
                             , "Get the root Group of the library." }
    , { "getCellGroup"      , (PyCFunction)PyLibertyLibrary_getCellGroup      , METH_VARARGS
                             , "Get a cell group by name." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  DirectDeleteMethod(PyLibertyLibrary_DeAlloc,PyLibertyLibrary)
  PyTypeObjectLinkPyTypeNewInit(LibertyLibrary)


// +=================================================================+
// |          "PyLibertyGroup" Python Module Code Part              |
// +=================================================================+


  static PyObject* PyLibertyGroup_getName ( PyLibertyGroup* self )
  {
    cdebug_log(30,0) << "PyLibertyGroup_getName()" << endl;
    HTRY
      METHOD_HEAD_GROUP("LibertyGroup.getName()")
      return PyString_FromString( group->getName().c_str() );
    HCATCH
    return NULL;
  }


  static PyObject* PyLibertyGroup_getGroupName ( PyLibertyGroup* self )
  {
    cdebug_log(30,0) << "PyLibertyGroup_getGroupName()" << endl;
    HTRY
      METHOD_HEAD_GROUP("LibertyGroup.getGroupName()")
      return PyString_FromString( group->getGroupName().c_str() );
    HCATCH
    return NULL;
  }


  static PyObject* PyLibertyGroup_getStatements ( PyLibertyGroup* self )
  {
    cdebug_log(30,0) << "PyLibertyGroup_getStatements()" << endl;
    HTRY
      METHOD_HEAD_GROUP("LibertyGroup.getStatements()")
      const std::vector<Liberty::Statement*>& stmts = group->getStatements();
      PyObject* list = PyList_New( stmts.size() );
      for ( size_t i = 0; i < stmts.size(); ++i ) {
        Liberty::Statement* stmt = stmts[i];
        PyObject* pyStmt = NULL;

        if ( stmt->isGroup() ) {
          pyStmt = PyLibertyGroup_Link( stmt->getAsGroup() );
        } else if ( stmt->isAttribute() ) {
          pyStmt = PyLibertyAttribute_Link( stmt->getAsAttribute() );
        }

        if ( pyStmt ) {
          PyList_SetItem( list, i, pyStmt );
        }
      }
      return list;
    HCATCH
    return NULL;
  }


  static PyObject* PyLibertyGroup_getGroup ( PyLibertyGroup* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyLibertyGroup_getGroup()" << endl;
    HTRY
      METHOD_HEAD_GROUP("LibertyGroup.getGroup()")
      char* groupName = NULL;
      if ( not PyArg_ParseTuple( args, "s:LibertyGroup.getGroup", &groupName ) ) {
        PyErr_SetString( ConstructorError, "LibertyGroup.getGroup(): Invalid arguments." );
        return NULL;
      }
      Liberty::Group* subGroup = group->getGroup( groupName );
      if ( subGroup ) return PyLibertyGroup_Link( subGroup );
      Py_RETURN_NONE;
    HCATCH
    return NULL;
  }


  static PyObject* PyLibertyGroup_getGroups ( PyLibertyGroup* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyLibertyGroup_getGroups()" << endl;
    HTRY
      METHOD_HEAD_GROUP("LibertyGroup.getGroups()")
      char* groupNameRegex = NULL;
      if ( not PyArg_ParseTuple( args, "s:LibertyGroup.getGroups", &groupNameRegex ) ) {
        PyErr_SetString( ConstructorError, "LibertyGroup.getGroups(): Invalid arguments." );
        return NULL;
      }

      std::vector<Liberty::Group*> groups = group->getGroups( groupNameRegex );
      PyObject* list = PyList_New( groups.size() );
      for ( size_t i = 0; i < groups.size(); ++i ) {
        PyList_SetItem( list, i, PyLibertyGroup_Link(groups[i]) );
      }
      return list;
    HCATCH
    return NULL;
  }


  static PyObject* PyLibertyGroup_getAttribute ( PyLibertyGroup* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyLibertyGroup_getAttribute()" << endl;
    HTRY
      METHOD_HEAD_GROUP("LibertyGroup.getAttribute()")
      char* attrName = NULL;
      if ( not PyArg_ParseTuple( args, "s:LibertyGroup.getAttribute", &attrName ) ) {
        PyErr_SetString( ConstructorError, "LibertyGroup.getAttribute(): Invalid arguments." );
        return NULL;
      }
      Liberty::Attribute* attr = group->getAttribute( attrName );
      if ( attr ) return PyLibertyAttribute_Link( attr );
      Py_RETURN_NONE;
    HCATCH
    return NULL;
  }


  PyMethodDef PyLibertyGroup_Methods[] =
    { { "getName"       , (PyCFunction)PyLibertyGroup_getName       , METH_NOARGS
                        , "Return the name of the group." }
    , { "getGroupName"  , (PyCFunction)PyLibertyGroup_getGroupName  , METH_NOARGS
                        , "Return the group identifier (cell, pin, timing, ...)." }
    , { "getStatements" , (PyCFunction)PyLibertyGroup_getStatements , METH_NOARGS
                        , "Return list of statements (Groups and Attributes) in this group." }
    , { "getGroup"      , (PyCFunction)PyLibertyGroup_getGroup      , METH_VARARGS
                        , "Get a sub-group by name." }
    , { "getGroups"     , (PyCFunction)PyLibertyGroup_getGroups     , METH_VARARGS
              , "Get all sub-groups whose group name matches a regex." }
    , { "getAttribute"  , (PyCFunction)PyLibertyGroup_getAttribute  , METH_VARARGS
                        , "Get an attribute by name." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };

  PythonOnlyDeleteMethod(LibertyGroup)
  PyTypeObjectLinkPyType(LibertyGroup)


// +=================================================================+
// |        "PyLibertyAttribute" Python Module Code Part            |
// +=================================================================+


  static PyObject* PyLibertyAttribute_getName ( PyLibertyAttribute* self )
  {
    cdebug_log(30,0) << "PyLibertyAttribute_getName()" << endl;
    HTRY
      METHOD_HEAD_ATTRIBUTE("LibertyAttribute.getName()")
      return PyString_FromString( attribute->getName().c_str() );
    HCATCH
    return NULL;
  }


  static PyObject* PyLibertyAttribute_getValue ( PyLibertyAttribute* self )
  {
    cdebug_log(30,0) << "PyLibertyAttribute_getValue()" << endl;
    HTRY
      METHOD_HEAD_ATTRIBUTE("LibertyAttribute.getValue()")
      Liberty::Value* value = attribute->getValue();
      if ( value ) return PyString_FromString( value->getAsString().c_str() );
      Py_RETURN_NONE;
    HCATCH
    return NULL;
  }


  PyMethodDef PyLibertyAttribute_Methods[] =
    { { "getName"  , (PyCFunction)PyLibertyAttribute_getName  , METH_NOARGS
                   , "Return the name of the attribute." }
    , { "getValue" , (PyCFunction)PyLibertyAttribute_getValue , METH_NOARGS
                   , "Return the attribute value as a string." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };

  PythonOnlyDeleteMethod(LibertyAttribute)
  PyTypeObjectLinkPyType(LibertyAttribute)


// +=================================================================+
// |         "PyLibertyValue" Python Module Code Part              |
// +=================================================================+


  static PyObject* PyLibertyValue_getAsString ( PyLibertyValue* self )
  {
    cdebug_log(30,0) << "PyLibertyValue_getAsString()" << endl;
    HTRY
      METHOD_HEAD_VALUE("LibertyValue.getAsString()")
      return PyString_FromString( value->getAsString().c_str() );
    HCATCH
    return NULL;
  }


  PyMethodDef PyLibertyValue_Methods[] =
    { { "getAsString" , (PyCFunction)PyLibertyValue_getAsString , METH_NOARGS
                      , "Return the value as a string." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };

  PythonOnlyDeleteMethod(LibertyValue)
  PyTypeObjectLinkPyType(LibertyValue)


// +=================================================================+
// |         "PyLibertyHelper" Functions                            |
// +=================================================================+


  PyObject* PyCRL_getLibertyGroupFromCell ( PyObject* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyCRL_getLibertyGroupFromCell()" << endl;
    HTRY
      PyObject* pyCell = NULL;
      if ( not PyArg_ParseTuple( args, "O:CRL.getLibertyGroupFromCell", &pyCell ) ) {
        PyErr_SetString( ConstructorError, "getLibertyGroupFromCell(): Argument must be a Cell." );
        return NULL;
      }

      if ( not IsPyCell(pyCell) ) {
        PyErr_SetString( ConstructorError, "getLibertyGroupFromCell(): Argument is not of type Cell." );
        return NULL;
      }

      Cell* cell = PYCELL_O(pyCell);
      Hurricane::Property* property = cell->getProperty( Hurricane::Name("Liberty Cell Informations") );
      if ( not property ) {
        Py_RETURN_NONE;
      }

      // Avoid RTTI cross-DSO dependency on Liberty::LibertyProperty typeinfo.
      Liberty::LibertyProperty* prop = static_cast<Liberty::LibertyProperty*>(property);
      if (not prop) {
        Py_RETURN_NONE;
      }
      Liberty::Group* group = prop->getLibCell();
      if ( group ) return PyLibertyGroup_Link( group );
      Py_RETURN_NONE;
    HCATCH
    return NULL;
  }


#else  // End of Python Module Code Part.


// +=================================================================+
// |             "PyLiberty" Shared Library Code Part               |
// +=================================================================+


  PyObject* PyLibertyLibrary_Link ( Liberty::Library* object )
  {
    if ( object == NULL ) {
      Py_RETURN_NONE;
    }
    PyLibertyLibrary* pyObject = NULL;
    HTRY
      pyObject = PyObject_NEW( PyLibertyLibrary, &PyTypeLibertyLibrary );
      if ( pyObject == NULL ) return NULL;
      pyObject->ACCESS_OBJECT = object;
    HCATCH
    return (PyObject*)pyObject;
  }


  PyObject* PyLibertyGroup_Link ( Liberty::Group* object )
  {
    if ( object == NULL ) {
      Py_RETURN_NONE;
    }
    PyLibertyGroup* pyObject = NULL;
    HTRY
      pyObject = PyObject_NEW( PyLibertyGroup, &PyTypeLibertyGroup );
      if ( pyObject == NULL ) return NULL;
      pyObject->ACCESS_OBJECT = object;
    HCATCH
    return (PyObject*)pyObject;
  }


  PyObject* PyLibertyAttribute_Link ( Liberty::Attribute* object )
  {
    if ( object == NULL ) {
      Py_RETURN_NONE;
    }
    PyLibertyAttribute* pyObject = NULL;
    HTRY
      pyObject = PyObject_NEW( PyLibertyAttribute, &PyTypeLibertyAttribute );
      if ( pyObject == NULL ) return NULL;
      pyObject->ACCESS_OBJECT = object;
    HCATCH
    return (PyObject*)pyObject;
  }


  PyObject* PyLibertyValue_Link ( Liberty::Value* object )
  {
    if ( object == NULL ) {
      Py_RETURN_NONE;
    }
    PyLibertyValue* pyObject = NULL;
    HTRY
      pyObject = PyObject_NEW( PyLibertyValue, &PyTypeLibertyValue );
      if ( pyObject == NULL ) return NULL;
      pyObject->ACCESS_OBJECT = object;
    HCATCH
    return (PyObject*)pyObject;
  }

  PyTypeObjectDefinitionsOfModule(CRL,LibertyLibrary)
  PyTypeObjectDefinitionsOfModule(CRL,LibertyGroup)
  PyTypeObjectDefinitionsOfModule(CRL,LibertyAttribute)
  PyTypeObjectDefinitionsOfModule(CRL,LibertyValue)


#endif  // End of Shared Library Code Part.


}  // extern "C".

}  // CRL namespace.
