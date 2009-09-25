
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




#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/isobar/PyUpdateSession.h"
#include "hurricane/isobar/PyDbU.h"
#include "hurricane/isobar/PyPoint.h"
#include "hurricane/isobar/PyBox.h"
#include "hurricane/isobar/PyTransformation.h"
#include "hurricane/isobar/PyDataBase.h"
#include "hurricane/isobar/PyLibrary.h"
#include "hurricane/isobar/PyEntity.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PyCellCollection.h"
#include "hurricane/isobar/PyLayer.h"
#include "hurricane/isobar/PyPin.h"
#include "hurricane/isobar/PyPinCollection.h"
#include "hurricane/isobar/PyInstance.h"
#include "hurricane/isobar/PyInstanceCollection.h"
#include "hurricane/isobar/PyReference.h"
#include "hurricane/isobar/PyReferenceCollection.h"
#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyNetCollection.h"
#include "hurricane/isobar/PyHyperNet.h"
#include "hurricane/isobar/PyComponent.h"
#include "hurricane/isobar/PyComponentCollection.h"
#include "hurricane/isobar/PyPlug.h"
#include "hurricane/isobar/PyPlugCollection.h"
#include "hurricane/isobar/PySegment.h"
#include "hurricane/isobar/PySegmentCollection.h"
#include "hurricane/isobar/PyContact.h"
#include "hurricane/isobar/PyHorizontal.h"
#include "hurricane/isobar/PyVertical.h"
#include "hurricane/isobar/PyPad.h"
#include "hurricane/isobar/PyPath.h"
#include "hurricane/isobar/PyOccurrence.h"
#include "hurricane/isobar/PyOccurrenceCollection.h"
#include "hurricane/isobar/PyTechnology.h"
#include "hurricane/NetExternalComponents.h"


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


  const char*            ConverterState::ObjectType::_inheritStop
                                        = "comp";
  ConverterState         __cs           = ConverterState ();
  int                    __objectOffset = offsetof ( PyPoint, _object );

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
// Method  : "::ConverterState::addType ()"

  void  ConverterState::addType ( const char*         id
                                , PyTypeObject* pyType
                                , const char*         name
                                , bool          isPythonType
                                , const char*         idBase ) {
    for ( unsigned i=0 ; i < _types.size() ; i++ ) {
      if ( ! strcmp ( _types[i]->_id, id ) ) {
        //throw Error ( objectTypeRedefinition ); // 04.09.2009 d2 modification
        cerr << objectTypeRedefinition << endl;
        return;
      }
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

  const char* ConverterState::getObjectId ( PyObject* object ) const {
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

  const char *ConverterState::getObjectName ( string id ) const {
    for ( unsigned i=0 ; i < _types.size() ; i++ ) {
      if ( ! strcmp ( id.c_str(), _types[i]->_id ) ) return ( _types[i]->_name );
    }
    return ( "<Unknown>" );
  }


  

// -------------------------------------------------------------------
// Method  : "::ConverterState::init ()"

   void  ConverterState::init ( const char* function, const char* inheritStop ) {
     _objectIds = "";
     setFunction ( function );
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
        __cs.addId ( baseType->_id );

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

  bool  ParseOneArg  ( const char* function, PyObject* args, string format, PyObject** arg ) {
    ostringstream  message;

    __cs.init ( function );
    if ( ! PyArg_ParseTuple(args,"O&",Converter,arg) ) return ( false );

    string firstArgType    = ConverterState::getObjectType ( __cs.getObjectIds(), 0 );
    string firstArgTypeExp = ConverterState::getObjectType ( format             , 0 );
    if ( firstArgType != firstArgTypeExp ) {
      return ( false );
    }

    return ( true );
  }




// -------------------------------------------------------------------
// Function  : "ParseTwoArg ()"

  bool  ParseTwoArg  ( const char* function, PyObject* args, string format, PyObject** arg0, PyObject** arg1 ) {
    ostringstream  message;
    __cs.init ( function );
    if ( ! PyArg_ParseTuple(args,"O&O&",Converter,arg0,Converter,arg1) ) return ( false );

    string firstArgType    = ConverterState::getObjectType ( __cs.getObjectIds(), 0 );
    string firstArgTypeExp = ConverterState::getObjectType ( format             , 0 );
    if ( firstArgType != firstArgTypeExp ) {
      return ( false );
    }
    string secondArgType    = ConverterState::getObjectType ( __cs.getObjectIds(), 1 );
    string secondArgTypeExp = ConverterState::getObjectType ( format             , 1 );
    if ( secondArgType != secondArgTypeExp ) {
      return ( false );
    }


    return ( true );
  }




// -------------------------------------------------------------------
// Function  : "ParseThreeArg ()"

  bool  ParseThreeArg  ( const char* function, PyObject* args, string format, PyObject** arg0, PyObject** arg1, PyObject** arg2 ) {
    ostringstream  message;
    __cs.init ( function );
    if ( ! PyArg_ParseTuple(args,"O&O&O&",Converter,arg0,Converter,arg1,Converter,arg2) ) return ( false );

    string firstArgType    = ConverterState::getObjectType ( __cs.getObjectIds(), 0 );
    string firstArgTypeExp = ConverterState::getObjectType ( format             , 0 );
    if ( firstArgType != firstArgTypeExp ) {
      return ( false );
    }
    string secondArgType    = ConverterState::getObjectType ( __cs.getObjectIds(), 1 );
    string secondArgTypeExp = ConverterState::getObjectType ( format             , 1 );
    if ( secondArgType != secondArgTypeExp ) {
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

    PyComponentCollection* pyComponentCollection = NULL;

    HTRY

    Components* components = new Components(NetExternalComponents::get(PYNET_O(arg0)));

    pyComponentCollection = PyObject_NEW(PyComponentCollection, &PyTypeComponentCollection);
    if (pyComponentCollection == NULL) { return NULL; }

    pyComponentCollection->_object = components;

    HCATCH

    return ((PyObject*)pyComponentCollection);
  }

  // x-------------------------------------------------------------x
  // |               "PyHurricane" Module Methods                  |
  // x-------------------------------------------------------------x

  static PyMethodDef PyHurricane_Methods[] =
    { { "DbU_db"                ,              PyDbU_db                          , METH_VARARGS, "Convert an integer to DbU::Unit (no scale factor)." }
    , { "DbU_grid"              ,              PyDbU_grid                        , METH_VARARGS, "Convert a founder grid to DbU::Unit." }
    , { "DbU_lambda"            ,              PyDbU_lambda                      , METH_VARARGS, "Convert a symbolic (lambda) to DbU::Unit." }
    , { "DbU_getDb"             ,              PyDbU_getDb                       , METH_VARARGS, "Convert a DbU::Unit to an integer value (no scale factor)." }
    , { "DbU_getGrid"           ,              PyDbU_getGrid                     , METH_VARARGS, "Convert a DbU::Unit to a to grid founder." }
    , { "DbU_getLambda"         ,              PyDbU_getLambda                   , METH_VARARGS, "Convert a DbU::Unit to a symbolic value (to lambda)." }
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
    PyDataBase_LinkPyType ();
    PyTechnology_LinkPyType ();
    PyLibrary_LinkPyType ();
    PyEntity_LinkPyType ();
    PyLayer_LinkPyType ();
    PyPath_LinkPyType ();
    PyOccurrence_LinkPyType ();
    PyInstanceCollection_LinkPyType ();
    PyPlugCollection_LinkPyType ();
    PyNetCollection_LinkPyType ();
    PyNetCollection_LinkPyType ();
    PyCellCollection_LinkPyType ();
    PyPinCollection_LinkPyType ();
    PySegmentCollection_LinkPyType ();
    PyOccurrenceCollection_LinkPyType ();
    PyComponentCollection_LinkPyType ();
    PyReferenceCollection_LinkPyType ();
    PyCell_LinkPyType ();
    PyInstance_LinkPyType ();
    PyReference_LinkPyType ();
    PyNet_LinkPyType ();
    PyHyperNet_LinkPyType ();
    PyComponent_LinkPyType ();
    PySegment_LinkPyType ();
    PyPad_LinkPyType ();
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
    PyReference_Constructor();
    PyCell_Constructor();
    PyInstance_Constructor();
    PyNet_Constructor();
    PyHyperNet_Constructor();
    PyHorizontal_Constructor();
    PyVertical_Constructor();
    PyContact_Constructor();
    PyPin_Constructor();
    PyPad_Constructor();
    PyPath_Constructor();
    PyOccurrence_Constructor();

    PYTYPE_READY ( Point                       )
    PYTYPE_READY ( Box                         )
    PYTYPE_READY ( Transformation              )
    PYTYPE_READY ( DataBase                    )
    PYTYPE_READY ( Technology                  )
    PYTYPE_READY ( Library                     )
    PYTYPE_READY ( Entity                      )
    PYTYPE_READY ( Layer                       )
    PYTYPE_READY ( Path                        )
    PYTYPE_READY ( Occurrence                  )
    PYTYPE_READY ( InstanceCollection          )
    PYTYPE_READY ( InstanceCollectionLocator   )
    PYTYPE_READY ( PlugCollection              )
    PYTYPE_READY ( PlugCollectionLocator       )
    PYTYPE_READY ( NetCollection               )
    PYTYPE_READY ( NetCollectionLocator        )
    PYTYPE_READY ( CellCollection              )
    PYTYPE_READY ( CellCollectionLocator       )
    PYTYPE_READY ( PinCollection               )
    PYTYPE_READY ( PinCollectionLocator        )
    PYTYPE_READY ( SegmentCollection           )
    PYTYPE_READY ( SegmentCollectionLocator    )
    PYTYPE_READY ( ComponentCollection         ) 
    PYTYPE_READY ( ComponentCollectionLocator  )
    PYTYPE_READY ( OccurrenceCollection        )
    PYTYPE_READY ( OccurrenceCollectionLocator )
    PYTYPE_READY ( ReferenceCollection         )
    PYTYPE_READY ( ReferenceCollectionLocator  )
    PYTYPE_READY ( HyperNet                    )

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
    PYTYPE_READY_SUB ( Pad       , Component)

   
    // Identifier string can take up to 10 characters !
    __cs.addType ( "box"        , &PyTypeBox              , "<Box>"              , false );
    __cs.addType ( "ent"        , &PyTypeEntity           , "<Entity>"           , false );
    __cs.addType ( "cell"       , &PyTypeCell             , "<Cell>"             , false, "ent" );
    __cs.addType ( "cellCol"    , &PyTypeCellCollection   , "<CellCollection>"   , false );
    __cs.addType ( "comp"       , &PyTypeComponent        , "<Component>"        , false, "ent" );
    __cs.addType ( "compCol"    , &PyTypeComponentCollection, "<ComponentCollection>" , false );
    __cs.addType ( "contact"    , &PyTypeContact          , "<Contact>"          , false, "comp" );
    // Do not change the "none" string. It's hardwired to the None object.
    __cs.addType ( "none"       ,  Py_None->ob_type       , "<None>"             , true  );
    __cs.addType ( "float"      , &PyFloat_Type           , "<Float>"            , true  );
    __cs.addType ( "int"        , &PyInt_Type             , "<Int>"              , true  );
    __cs.addType ( "bool"       , &PyBool_Type            , "<Bool>"             , true  );
    __cs.addType ( "string"     , &PyString_Type          , "<String>"           , true  );
    __cs.addType ( "list"       , &PyList_Type            , "<List>"             , true  );
    // Do not change the "function" string. It's hardwired to callable (function) objects.
    __cs.addType ( "function"   , NULL                    , "<Function>"         , true  );
    __cs.addType ( "horiz"      , &PyTypeHorizontal       , "<Horizontal>"       , false, "segment" );
    __cs.addType ( "inst"       , &PyTypeInstance         , "<Instance>"         , false, "ent" );
    __cs.addType ( "instCol"    , &PyTypeInstanceCollection, "<InstanceCollection>"  , false );
    __cs.addType ( "layer"      , &PyTypeLayer            , "<Layer>"            , false );
    __cs.addType ( "library"    , &PyTypeLibrary          , "<Library>"          , false );
    __cs.addType ( "ref"        , &PyTypeReference        , "<Reference>"        , false, "ent" );
    __cs.addType ( "refCol"     , &PyTypeReferenceCollection, "<ReferenceCollection>" , false );
    __cs.addType ( "net"        , &PyTypeNet              , "<Net>"              , false, "ent" );
    __cs.addType ( "netCol"     , &PyTypeNetCollection    , "<NetCollection>"    , false );
    __cs.addType ( "hyperNet"   , &PyTypeHyperNet         , "<HyperNet>"         , false );
    __cs.addType ( "pin"        , &PyTypePin              , "<Pin>"              , false, "contact" );
    __cs.addType ( "pinCol"     , &PyTypePinCollection    , "<PinCollection>"    , false );
    __cs.addType ( "plug"       , &PyTypePlug             , "<Plug>"             , false, "comp" );
    __cs.addType ( "plugCol"    , &PyTypePlugCollection   , "<PlugCollection>"   , false );
    __cs.addType ( "point"      , &PyTypePoint            , "<Point>"            , false );
    __cs.addType ( "segment"    , &PyTypeSegment          , "<Segment>"          , false, "comp" );
    __cs.addType ( "pad    "    , &PyTypePad              , "<Pad>"              , false, "comp" );
    __cs.addType ( "segmentCol" , &PyTypeSegmentCollection, "<SegmentCollection>", false );
    __cs.addType ( "db"         , &PyTypeDataBase         , "<DataBase>"         , false );
    __cs.addType ( "techno"     , &PyTypeTechnology       , "<Technology>"       , false );
    __cs.addType ( "transfo"    , &PyTypeTransformation   , "<Transformation>"   , false );
    __cs.addType ( "vert"       , &PyTypeVertical         , "<Vertical>"         , false, "segment" );
    __cs.addType ( "path"       , &PyTypePath             , "<Path>"             , false );
    __cs.addType ( "occur"      , &PyTypeOccurrence       , "<Occurrence>"       , false );
    __cs.addType ( "occurCol"   , &PyTypeOccurrenceCollection, "<OccurrenceCollection>", false );


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
    PyModule_AddObject(module, "Reference"      , (PyObject*)&PyTypeReference);
    PyModule_AddObject(module, "Cell"           , (PyObject*)&PyTypeCell);
    PyModule_AddObject(module, "Instance"       , (PyObject*)&PyTypeInstance);
    PyModule_AddObject(module, "Net"            , (PyObject*)&PyTypeNet);
    PyModule_AddObject(module, "HyperNet"       , (PyObject*)&PyTypeHyperNet);
    PyModule_AddObject(module, "Horizontal"     , (PyObject*)&PyTypeHorizontal);
    PyModule_AddObject(module, "Vertical"       , (PyObject*)&PyTypeVertical);
    PyModule_AddObject(module, "Pad"            , (PyObject*)&PyTypePad);
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
