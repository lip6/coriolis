
// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
// ===================================================================
//
// $Id: PyHurricane.cpp,v 1.58 2008/02/07 19:09:58 xtof Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyHurricane.cpp"                        |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include "PyHurricane.h"
#include "PyUpdateSession.h"
#include "PyUnit.h"
#include "PyPoint.h"
#include "PyBox.h"
#include "PyTransformation.h"
#include "PyName.h"
#include "PyDataBase.h"
#include "PyLibrary.h"
#include "PyEntity.h"
#include "PyCell.h"
#include "PyLayer.h"
#include "PyPin.h"
#include "PyInstance.h"
#include "PyReference.h"
#include "PyNet.h"
#include "PyHyperNet.h"
#include "PyComponent.h"
#include "PyPlug.h"
#include "PySegment.h"
#include "PyContact.h"
#include "PyHorizontal.h"
#include "PyVertical.h"
#include "PyPath.h"
#include "PyOccurrence.h"
#include "PyInstanceLocator.h"
#include "PyPlugLocator.h"
#include "PyPinLocator.h"
#include "PySegmentLocator.h"
#include "PyComponentLocator.h"
#include "PyOccurrenceLocator.h"
#include "PyNetLocator.h"
#include "PyCellLocator.h"
#include "PyReferenceLocator.h"
#include "PyTechnology.h"
#include "NetExternalComponents.h"


namespace  Isobar {


#if !defined(__PYTHON_MODULE__)

// x=================================================================x
// |             "PyHurricane" Shared Library Code Part              |
// x=================================================================x


// x-----------------------------------------------------------------x
// |               "PyHurricane" C++ Local Variables                 |
// x-----------------------------------------------------------------x


namespace {


  using namespace std;

  // ---------------------------------------------------------------
  // Local Variables.

  const char* objectTypeRedefinition =
    "ConverterState::AddType () :\n\n"
    "    Attempt to add twice a type with identifier \n";


} // End of local namespace.


using namespace Hurricane;




// x-----------------------------------------------------------------x
// |              "PyHurricane" C++ Global Variables                 |
// x-----------------------------------------------------------------x


  char*                  ConverterState::ObjectType::_inheritStop
                                        = "comp";
  ConverterState         __cs           = ConverterState ();
  int                    __objectOffset = offsetof ( PyPoint, _object );
  map <void*,PyObject*>  H2PMap;




// x-----------------------------------------------------------------x
// |                 "PyHurricane" C++ Functions                     |
// x-----------------------------------------------------------------x


// -------------------------------------------------------------------
// Method  : "::ConverterState::ObjectType::PyEqual ()"

  bool  ConverterState::ObjectType::PyEqual ( PyTypeObject* pyType ) {
    if ( _pyType    == pyType ) return true;
    if ( _idBase[0] == '\0'   ) return false;
    return ( __cs.getObject(_idBase)->PyEqual(pyType) );
  }




// -------------------------------------------------------------------
// Method  : "::ConverterState::ObjectType::PyBase ()"

  ConverterState::ObjectType* ConverterState::ObjectType::PyBase ( PyTypeObject* pyType )
  {
    if ( _pyType != pyType ) return NULL;
    return ( PyBase() );
  }
  
  
  ConverterState::ObjectType* ConverterState::ObjectType::PyBase ()
  {
    if ( _idBase[0] == '\0'         ) return this;
    if ( ! strcmp(_id,_inheritStop) ) return this;
    return ( __cs.getObject(_idBase)->PyBase() );
  }




// -------------------------------------------------------------------
// Method  : "::ConverterState::ObjectType::PyBaseId ()"

  char*  ConverterState::ObjectType::PyBaseId ( PyTypeObject* pyType )
  {
    if ( _idBase[0] == '\0'         ) return _id;
    if ( ! strcmp(_id,_inheritStop) ) return _id;
    return ( __cs.getObject(_idBase)->PyBase(pyType)->_id );
  }




// -------------------------------------------------------------------
// Destructor  : "::ConverterState::~ConverterState ()"

  ConverterState::~ConverterState () {
    for ( unsigned i=0 ; i < _types.size() ; i++ )
      delete _types [ i ];
  }




// -------------------------------------------------------------------
// Method  : "::ConverterState::AddType ()"

  void  ConverterState::AddType ( char*         id
                                , PyTypeObject* pyType
                                , char*         name
                                , bool          isPythonType
                                , char*         idBase ) {
    for ( unsigned i=0 ; i < _types.size() ; i++ ) {
      if ( ! strcmp ( _types[i]->_id, id ) )
        throw Error ( objectTypeRedefinition );
    }
  
    assert ( strlen ( id ) < 11 );
  
    ObjectType* otype = new ObjectType;
    strcpy ( otype->_id, id );
    otype->_pyType       = pyType;
    otype->_name         = name;
    otype->_isPythonType = isPythonType;
    strcpy ( otype->_idBase, idBase );
    otype->_index        = _types.size();
  
    _types.push_back ( otype );
  }




// -------------------------------------------------------------------
// Method  : "::ConverterState::getObjectType ()"

  string ConverterState::getObjectType ( string objectsTypes, unsigned n ) {
    string  temp;
    size_t  end, head, tail, start;
  
    unsigned element = 0;
  
    end  = objectsTypes.size();
    if ( end > 0 ) {
      for ( head=0 ; head <= end ; head=tail ) {
        tail  = objectsTypes.find ( ':' , head+1 );
        start = head+1;
  
        temp = objectsTypes.substr ( start, tail-start );
  
        if ( n == element )
            return temp;
  
        element++;
      }
    }
  
    return "";
  }




// -------------------------------------------------------------------
// Method  : "::ConverterState::getObjectId ()"

  char* ConverterState::getObjectId ( PyObject* object ) {
    for ( unsigned i=0 ; i < _types.size() ; i++ ) {
      if ( ( ! strcmp ( _types[i]->_id, "function" ) ) && ( PyCallable_Check(object) ) )
        return ( _types[i]->_id );
        
      if ( ( ! strcmp ( _types[i]->_id, "none" ) ) && ( object == Py_None ) )
        return ( _types[i]->_id );
        
      if ( object->ob_type == _types[i]->_pyType ) return ( _types[i]->_id );
    }

    return ( "unknown" ); // return 'X'
  }


  

// -------------------------------------------------------------------
// Method  : "::ConverterState::getObject ()"

  ConverterState::ObjectType* ConverterState::getObject ( char* id ) {
    for ( unsigned i=0 ; i < _types.size() ; i++ ) {
      if ( ! strcmp ( id, _types[i]->_id ) ) return ( _types[i] );
    }

    return ( NULL );
  }

// -------------------------------------------------------------------
// Method  : "::ConverterState::getObjectName ()"

  char *ConverterState::getObjectName ( string id ) {
    for ( unsigned i=0 ; i < _types.size() ; i++ ) {
      if ( ! strcmp ( id.c_str(), _types[i]->_id ) ) return ( _types[i]->_name );
    }
    return ( "<Unknown>" );
  }


  

// -------------------------------------------------------------------
// Method  : "::ConverterState::Init ()"

   void  ConverterState::Init ( char* function, char* inheritStop ) {
     _objectIds = "";
     SetFunction ( function );
     ObjectType::_inheritStop = inheritStop;
   };

// -------------------------------------------------------------------
// Function  : "Converter ()"


  int  Converter ( PyObject* object, void** pArg ) {
    ostringstream               message;
    string                      unboundObject = "";
    ConverterState::ObjectType* baseType;

    for ( unsigned i=0 ; i < __cs.getTypes().size() ; i++ ) {
      baseType = __cs.getTypes()[i]->PyBase ( object->ob_type );
      if ( PyCallable_Check(object) || baseType ) {
        *pArg = object;
        i = baseType->_index;
        __cs.AddId ( baseType->_id );

        if ( ! __cs.getTypes()[i]->_isPythonType ) {
          void** member = ( (void**)( (unsigned long)object + __objectOffset ) );

          if ( *member == NULL ) {
            unboundObject = __cs.getTypes()[i]->_name;
            break;
          }
        }

        return ( 1 );
      }
    }

    if ( unboundObject.size() ) {
      message << "Attempt to call " << __cs.getFunction()
              << "() with an unbound " << unboundObject << " argument";

      PyErr_SetString ( ProxyError, message.str().c_str() );
      return ( 0 );
    }

    message << "Argument " << __cs.getSize() + 1
            << " of call to " << __cs.getFunction()
            << "() is of unmanaged type " << object->ob_type->tp_name;

    PyErr_SetString ( ProxyError, message.str().c_str() );
    return ( 0 );
  }
  


// -------------------------------------------------------------------
// Function  : "ParseOneArg ()"

  bool  ParseOneArg  ( char* function, PyObject* args, string format, PyObject** arg ) {
    ostringstream  message;

    __cs.Init ( function );
    if ( ! PyArg_ParseTuple(args,"O&",Converter,arg) ) return ( false );

    string firstArgType    = ConverterState::getObjectType ( __cs.getObjectIds(), 0 );
    string firstArgTypeExp = ConverterState::getObjectType ( format             , 0 );
    if ( firstArgType != firstArgTypeExp ) {
      message << "First argument of call to " << __cs.getFunction()
              << "() is of unexpected type " << (*arg)->ob_type->tp_name
              << ", must be of type " << __cs.getObjectName( firstArgTypeExp );

      PyErr_SetString ( ProxyError, message.str().c_str() );
      return ( false );
    }

    return ( true );
  }




// -------------------------------------------------------------------
// Function  : "ParseTwoArg ()"

  bool  ParseTwoArg  ( char* function, PyObject* args, string format, PyObject** arg0, PyObject** arg1 ) {
    ostringstream  message;

    __cs.Init ( function );
    if ( ! PyArg_ParseTuple(args,"O&O&",Converter,arg0,Converter,arg1) ) return ( false );

    string firstArgType    = ConverterState::getObjectType ( __cs.getObjectIds(), 0 );
    string firstArgTypeExp = ConverterState::getObjectType ( format             , 0 );
    if ( firstArgType != firstArgTypeExp ) {
      message << "First argument of call to " << __cs.getFunction()
              << "() is of unexpected type " << (*arg0)->ob_type->tp_name
              << ", must be of type " << __cs.getObjectName( firstArgTypeExp );
      PyErr_SetString ( ProxyError, message.str().c_str() );
      return ( false );
    }
    string secondArgType    = ConverterState::getObjectType ( __cs.getObjectIds(), 1 );
    string secondArgTypeExp = ConverterState::getObjectType ( format             , 1 );
    if ( secondArgType != secondArgTypeExp ) {
      message << "Second argument of call to " << __cs.getFunction()
              << "() is of unexpected type " << (*arg1)->ob_type->tp_name
              << ", must be of type " << __cs.getObjectName( secondArgTypeExp );
      PyErr_SetString ( ProxyError, message.str().c_str() );
      return ( false );
    }


    return ( true );
  }




// -------------------------------------------------------------------
// Function  : "ParseThreeArg ()"

  bool  ParseThreeArg  ( char* function, PyObject* args, string format, PyObject** arg0, PyObject** arg1, PyObject** arg2 ) {
    ostringstream  message;

    __cs.Init ( function );
    if ( ! PyArg_ParseTuple(args,"O&O&O&",Converter,arg0,Converter,arg1,Converter,arg2) ) return ( false );

    string firstArgType    = ConverterState::getObjectType ( __cs.getObjectIds(), 0 );
    string firstArgTypeExp = ConverterState::getObjectType ( format             , 0 );
    if ( firstArgType != firstArgTypeExp ) {
      message << "First argument of call to " << __cs.getFunction()
              << "() is of unexpected type " << (*arg0)->ob_type->tp_name
              << ", must be of type " << __cs.getObjectName( firstArgTypeExp );
      PyErr_SetString ( ProxyError, message.str().c_str() );
      return ( false );
    }
    string secondArgType    = ConverterState::getObjectType ( __cs.getObjectIds(), 1 );
    string secondArgTypeExp = ConverterState::getObjectType ( format             , 1 );
    if ( secondArgType != secondArgTypeExp ) {
      message << "Second argument of call to " << __cs.getFunction()
              << "() is of unexpected type " << (*arg1)->ob_type->tp_name
              << ", must be of type " << __cs.getObjectName( secondArgTypeExp );
      PyErr_SetString ( ProxyError, message.str().c_str() );
      return ( false );
    }
    string thirdArgType    = ConverterState::getObjectType ( __cs.getObjectIds(), 2 );
    string thirdArgTypeExp = ConverterState::getObjectType ( format             , 2 );
    if ( thirdArgType != thirdArgTypeExp ) {
      message << "Third argument of call to " << __cs.getFunction()
              << "() is of unexpected type " << (*arg2)->ob_type->tp_name
              << ", must be of type " << __cs.getObjectName( thirdArgTypeExp );
      PyErr_SetString ( ProxyError, message.str().c_str() );
      return ( false );
    }


    return ( true );
  }




// x-----------------------------------------------------------------x
// |                "PyHurricane" C Global Variables                 |
// x-----------------------------------------------------------------x


extern "C" {


  PyObject* ConstructorError = NULL;
  PyObject* ProxyError       = NULL;
  PyObject* HurricaneError   = NULL;
  PyObject* CoriolisError    = NULL;
  PyObject* HurricaneWarning = NULL;


}


# else // End of PyHurricane Shared Library Code Part.

// x=================================================================x
// |              "PyHurricane" Python Module Code Part              |
// x=================================================================x


extern "C" {

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyNetExternalComponents_getNetExternalComponents ()"

  PyObject* PyNetExternalComponents_getExternalComponents ( PyObject* module, PyObject* args )
  {
    trace << "PyNetExternalComponents_getExternalComponents()" << endl;

    PyObject* arg0;
    if ( ! ParseOneArg ( "getExternalComponents", args, ":ent", &arg0) ) return ( NULL );

    PyComponentLocator* pyComponentLocator = NULL;

    HTRY

    Components components = getExternalComponents ( PYNET_O ( arg0 ) );

    pyComponentLocator = PyObject_NEW ( PyComponentLocator, &PyTypeComponentLocator );
    if (pyComponentLocator == NULL) { return NULL; }

    pyComponentLocator->_object = components.getLocator ();

    HCATCH

    return ( (PyObject*)pyComponentLocator );
  }

  // x-------------------------------------------------------------x
  // |               "PyHurricane" Module Methods                  |
  // x-------------------------------------------------------------x

  static PyMethodDef PyHurricane_Methods[] =
    { { "getUnit"               ,              PyUnit_getUnit                    , METH_VARARGS, "Convert to Unit." }
    , { "getValue"              ,              PyUnit_getValue                   , METH_VARARGS, "Convert a Unit to a value." }
    , { "getDataBase"           , (PyCFunction)PyDataBase_getDataBase            , METH_NOARGS , "Get the current DataBase." }
    //, { "openUpdateSession"     , (PyCFunction)PyUpdateSession_openUpdateSession , METH_NOARGS , "Open an UpdateSession." }
    //, { "closeUpdateSession"    , (PyCFunction)PyUpdateSession_closeUpdateSession, METH_NOARGS , "Close an UpdateSession." }
    , { "getExternalComponents" , (PyCFunction)PyNetExternalComponents_getExternalComponents, METH_VARARGS, "Returns the components collection of an external net" }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // ---------------------------------------------------------------
  // Module Initialization  :  "initHurricane ()"

  DL_EXPORT(void) initHurricane () {
    trace << "initHurricane()" << endl;

    PyPoint_LinkPyType ();
    PyBox_LinkPyType ();
    PyTransformation_LinkPyType ();
    PyName_LinkPyType ();
    PyDataBase_LinkPyType ();
    PyTechnology_LinkPyType ();
    PyLibrary_LinkPyType ();
    PyEntity_LinkPyType ();
    PyLayer_LinkPyType ();
    PyPath_LinkPyType ();
    PyOccurrence_LinkPyType ();
    PyInstanceLocator_LinkPyType ();
    PyPlugLocator_LinkPyType ();
    PyNetLocator_LinkPyType ();
    PyCellLocator_LinkPyType ();
    PyPinLocator_LinkPyType ();
    PySegmentLocator_LinkPyType ();
    PyOccurrenceLocator_LinkPyType ();
    PyComponentLocator_LinkPyType ();
    PyReferenceLocator_LinkPyType ();
    PyCell_LinkPyType ();
    PyInstance_LinkPyType ();
    PyReference_LinkPyType ();
    PyNet_LinkPyType ();
    PyHyperNet_LinkPyType ();
    PyComponent_LinkPyType ();
    PySegment_LinkPyType ();
    PyVertical_LinkPyType ();
    PyHorizontal_LinkPyType ();
    PyContact_LinkPyType ();
    PyPin_LinkPyType ();
    PyPlug_LinkPyType ();

    //constructors
    PyPoint_Constructor();
    PyDataBase_Constructor();
    PyLibrary_Constructor();
    PyBox_Constructor();
    PyTransformation_Constructor();
    PyName_Constructor();
    PyReference_Constructor();
    PyCell_Constructor();
    PyInstance_Constructor();
    PyNet_Constructor();
    PyHyperNet_Constructor();
    PyHorizontal_Constructor();
    PyVertical_Constructor();
    PyContact_Constructor();
    PyPin_Constructor();
    PyPath_Constructor();
    PyOccurrence_Constructor();

    PYTYPE_READY ( Point            )
    PYTYPE_READY ( Box              )
    PYTYPE_READY ( Transformation   )
    PYTYPE_READY ( Name             )
    PYTYPE_READY ( DataBase         )
    PYTYPE_READY ( Technology       )
    PYTYPE_READY ( Library          )
    PYTYPE_READY ( Entity           )
    PYTYPE_READY ( Layer            )
    PYTYPE_READY ( Path             )
    PYTYPE_READY ( Occurrence       )
    PYTYPE_READY ( InstanceLocator  )
    PYTYPE_READY ( PlugLocator      )
    PYTYPE_READY ( NetLocator       )
    PYTYPE_READY ( CellLocator      )
    PYTYPE_READY ( PinLocator       )
    PYTYPE_READY ( SegmentLocator   )
    PYTYPE_READY ( ComponentLocator )
    PYTYPE_READY ( OccurrenceLocator)
    PYTYPE_READY ( ReferenceLocator )
    PYTYPE_READY ( HyperNet         )

    PYTYPE_READY_SUB ( Cell      , Entity   )
    PYTYPE_READY_SUB ( Instance  , Entity   )
    PYTYPE_READY_SUB ( Reference , Entity   )
    PYTYPE_READY_SUB ( Net       , Entity   )
    PYTYPE_READY_SUB ( Component , Entity   )
    PYTYPE_READY_SUB ( Segment   , Component)
    PYTYPE_READY_SUB ( Horizontal, Segment  )
    PYTYPE_READY_SUB ( Vertical  , Segment  )

    PYTYPE_READY_SUB ( Contact   , Component)
    PYTYPE_READY_SUB ( Pin       , Contact  )
    PYTYPE_READY_SUB ( Plug      , Component)

   
    // Identifier string can take up to 10 characters !
    __cs.AddType ( "box"        , &PyTypeBox              , "<Box>"              , false );
    __cs.AddType ( "ent"        , &PyTypeEntity           , "<Entity>"           , false );
    __cs.AddType ( "cell"       , &PyTypeCell             , "<Cell>"             , false, "ent" );
    __cs.AddType ( "cellLoc"    , &PyTypeCellLocator      , "<CellLocator>"      , false );
    __cs.AddType ( "comp"       , &PyTypeComponent        , "<Component>"        , false, "ent" );
    __cs.AddType ( "compLoc"    , &PyTypeComponentLocator , "<ComponentLocator>" , false );
    __cs.AddType ( "contact"    , &PyTypeContact          , "<Contact>"          , false, "comp" );
    // Do not change the "none" string. It's hardwired to the None object.
    __cs.AddType ( "none"       ,  Py_None->ob_type       , "<None>"             , true  );
    __cs.AddType ( "float"      , &PyFloat_Type           , "<Float>"            , true  );
    __cs.AddType ( "int"        , &PyInt_Type             , "<Int>"              , true  );
    __cs.AddType ( "bool"       , &PyBool_Type            , "<Bool>"             , true  );
    __cs.AddType ( "string"     , &PyString_Type          , "<String>"           , true  );
    __cs.AddType ( "list"       , &PyList_Type            , "<List>"             , true  );
    // Do not change the "function" string. It's hardwired to callable (function) objects.
    __cs.AddType ( "function"   , NULL                    , "<Function>"         , true  );
    __cs.AddType ( "horiz"      , &PyTypeHorizontal       , "<Horizontal>"       , false, "segment" );
    __cs.AddType ( "inst"       , &PyTypeInstance         , "<Instance>"         , false, "ent" );
    __cs.AddType ( "instLoc"    , &PyTypeInstanceLocator  , "<InstanceLocator>"  , false );
    __cs.AddType ( "layer"      , &PyTypeLayer            , "<Layer>"            , false );
    __cs.AddType ( "library"    , &PyTypeLibrary          , "<Library>"          , false );
    __cs.AddType ( "name"       , &PyTypeName             , "<Name>"             , false );
    __cs.AddType ( "ref"        , &PyTypeReference        , "<Reference>"        , false, "ent" );
    __cs.AddType ( "refLoc"     , &PyTypeReferenceLocator , "<ReferenceLocator>" , false );
    __cs.AddType ( "net"        , &PyTypeNet              , "<Net>"              , false, "ent" );
    __cs.AddType ( "netLoc"     , &PyTypeNetLocator       , "<NetLocator>"       , false );
    __cs.AddType ( "hyperNet"   , &PyTypeHyperNet         , "<HyperNet>"         , false );
    __cs.AddType ( "pin"        , &PyTypePin              , "<Pin>"              , false, "contact" );
    __cs.AddType ( "pinLoc"     , &PyTypePinLocator       , "<PinLocator>"       , false );
    __cs.AddType ( "plug"       , &PyTypePlug             , "<Plug>"             , false, "comp" );
    __cs.AddType ( "plugLoc"    , &PyTypePlugLocator      , "<PlugLocator>"      , false );
    __cs.AddType ( "point"      , &PyTypePoint            , "<Point>"            , false );
    __cs.AddType ( "segment"    , &PyTypeSegment          , "<Segment>"          , false, "comp" );
    __cs.AddType ( "segmentLoc" , &PyTypeSegmentLocator   , "<SegmentLocator>"   , false );
    __cs.AddType ( "db"         , &PyTypeDataBase         , "<DataBase>"         , false );
    __cs.AddType ( "techno"     , &PyTypeTechnology       , "<Technology>"       , false );
    __cs.AddType ( "transfo"    , &PyTypeTransformation   , "<Transformation>"   , false );
    __cs.AddType ( "vert"       , &PyTypeVertical         , "<Vertical>"         , false, "segment" );
    __cs.AddType ( "path"       , &PyTypePath             , "<Path>"             , false );
    __cs.AddType ( "occur"      , &PyTypeOccurrence       , "<Occurrence>"       , false );
    __cs.AddType ( "occurLoc"   , &PyTypeOccurrenceLocator, "<OccurrenceLocator>", false );


    PyObject* module = Py_InitModule ( "Hurricane", PyHurricane_Methods );
    if ( module == NULL ) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Hurricane module." << endl;
      return;
    }
    PyModule_AddObject(module, "Point"          , (PyObject*)&PyTypePoint);
    PyModule_AddObject(module, "DataBase"       , (PyObject*)&PyTypeDataBase);
    PyModule_AddObject(module, "Library"        , (PyObject*)&PyTypeLibrary);
    PyModule_AddObject(module, "Box"            , (PyObject*)&PyTypeBox);
    PyModule_AddObject(module, "Transformation" , (PyObject*)&PyTypeTransformation);
    PyModule_AddObject(module, "Name"           , (PyObject*)&PyTypeName);
    PyModule_AddObject(module, "Reference"      , (PyObject*)&PyTypeReference);
    PyModule_AddObject(module, "Cell"           , (PyObject*)&PyTypeCell);
    PyModule_AddObject(module, "Instance"       , (PyObject*)&PyTypeInstance);
    PyModule_AddObject(module, "Net"            , (PyObject*)&PyTypeNet);
    PyModule_AddObject(module, "HyperNet"       , (PyObject*)&PyTypeHyperNet);
    PyModule_AddObject(module, "Horizontal"     , (PyObject*)&PyTypeHorizontal);
    PyModule_AddObject(module, "Vertical"       , (PyObject*)&PyTypeVertical);
    PyModule_AddObject(module, "Contact"        , (PyObject*)&PyTypeContact);
    PyModule_AddObject(module, "Pin"            , (PyObject*)&PyTypePin);
    PyModule_AddObject(module, "Path"           , (PyObject*)&PyTypePath);
    PyModule_AddObject(module, "Occurrence"     , (PyObject*)&PyTypeOccurrence);
    
    PyObject* dictionnary = PyModule_GetDict ( module );

    ConstructorError = PyErr_NewException ( "hurricane.ConstructorError", NULL, NULL );
    ProxyError       = PyErr_NewException ( "hurricane.ProxyError"      , NULL, NULL );
    HurricaneError   = PyErr_NewException ( "hurricane.HurricaneError"  , NULL, NULL );
    HurricaneWarning = PyErr_NewException ( "hurricane.HurricaneWarning", PyExc_Warning, NULL );

    PyDict_SetItemString ( dictionnary, "ConstructorError", ConstructorError );
    PyDict_SetItemString ( dictionnary, "ProxyError"      , ProxyError );
    PyDict_SetItemString ( dictionnary, "HurricaneError"  , HurricaneError );

    TransformationLoadConstants ( dictionnary );
    NetLoadConstants            ( dictionnary );
    InstanceLoadConstants       ( dictionnary );
    PinLoadConstants            ( dictionnary );
  }

  
} // End of extern "C".


#endif // End of Python Module Code Part.




}  // End of Isobar namespace.
