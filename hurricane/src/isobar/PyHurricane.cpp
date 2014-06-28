// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyHurricane.cpp"                             |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/isobar/PyBreakpoint.h"
#include "hurricane/isobar/PyUpdateSession.h"
#include "hurricane/isobar/PyDbU.h"
#include "hurricane/isobar/PyPoint.h"
#include "hurricane/isobar/PyBox.h"
#include "hurricane/isobar/PyTransformation.h"
#include "hurricane/isobar/PyOrientation.h"
#include "hurricane/isobar/PyDataBase.h"
#include "hurricane/isobar/PyLibrary.h"
#include "hurricane/isobar/PyEntity.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PyCellCollection.h"
#include "hurricane/isobar/PyLayer.h"
#include "hurricane/isobar/PyLayerMask.h"
#include "hurricane/isobar/PyMaterial.h"
#include "hurricane/isobar/PyBasicLayer.h"
#include "hurricane/isobar/PyRegularLayer.h"
#include "hurricane/isobar/PyContactLayer.h"
#include "hurricane/isobar/PyDiffusionLayer.h"
#include "hurricane/isobar/PyTransistorLayer.h"
#include "hurricane/isobar/PyViaLayer.h"
#include "hurricane/isobar/PyLayerCollection.h"
#include "hurricane/isobar/PyBasicLayerCollection.h"
#include "hurricane/isobar/PyRegularLayerCollection.h"
#include "hurricane/isobar/PyViaLayerCollection.h"
#include "hurricane/isobar/PyPin.h"
#include "hurricane/isobar/PyPinPlacementStatus.h"
#include "hurricane/isobar/PyPinDirection.h"
#include "hurricane/isobar/PyPinCollection.h"
#include "hurricane/isobar/PyPlacementStatus.h"
#include "hurricane/isobar/PyInstance.h"
#include "hurricane/isobar/PyInstanceCollection.h"
#include "hurricane/isobar/PyReference.h"
#include "hurricane/isobar/PyReferenceCollection.h"
#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyNetType.h"
#include "hurricane/isobar/PyNetDirection.h"
#include "hurricane/isobar/PyNetCollection.h"
#include "hurricane/isobar/PyNetExternalComponents.h"
#include "hurricane/isobar/PyHyperNet.h"
#include "hurricane/isobar/PyHook.h"
#include "hurricane/isobar/PyHookCollection.h"
#include "hurricane/isobar/PyComponent.h"
#include "hurricane/isobar/PyComponentCollection.h"
#include "hurricane/isobar/PyPlug.h"
#include "hurricane/isobar/PyPlugCollection.h"
#include "hurricane/isobar/PyRoutingPad.h"
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
#include "hurricane/isobar/PyQueryMask.h"
#include "hurricane/isobar/PyQuery.h"
#include "hurricane/NetExternalComponents.h"
#include <stddef.h>


namespace  Isobar {


#if !defined(__PYTHON_MODULE__)

// +=================================================================+
// |             "PyHurricane" Shared Library Code Part              |
// +=================================================================+


// +-----------------------------------------------------------------+
// |               "PyHurricane" C++ Local Variables                 |
// +-----------------------------------------------------------------+


namespace {

  using namespace std;

  // ---------------------------------------------------------------
  // Local Variables.

  const char* objectTypeRedefinition =
    "ConverterState::AddType () :\n\n"
    "    Attempt to add twice a type with identifier \n";


} // Anonymous namespace.


using namespace Hurricane;


// +-----------------------------------------------------------------+
// |              "PyHurricane" C++ Global Variables                 |
// +-----------------------------------------------------------------+


  const char*            ConverterState::ObjectType::_inheritStop
                                        = "comp";
  ConverterState         __cs           = ConverterState ();
  int                    __objectOffset = offsetof ( PyPoint, _object );


// +-----------------------------------------------------------------+
// |                 "PyHurricane" C++ Functions                     |
// +-----------------------------------------------------------------+


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
        //throw Error ( objectTypeRedefinition ); // 04.09.2009 d2 modification so Pharos can run several scripts during one execution
        trace << objectTypeRedefinition << endl; 
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
    if (not PyArg_ParseTuple(args,"O&",Converter,arg)) {
      ostringstream m;
      m << function << ": Take exactly one argument.";
      PyErr_SetString(ConstructorError, m.str().c_str());
      return false;
    }

    string firstArgType    = ConverterState::getObjectType ( __cs.getObjectIds(), 0 );
    string firstArgTypeExp = ConverterState::getObjectType ( format             , 0 );

    if ( firstArgType != firstArgTypeExp ) {
      ostringstream m;
      m << function << ": Argument type mismatch, should be:<" << firstArgTypeExp
        << "> not <" << firstArgType << ">";
      PyErr_SetString(ConstructorError, m.str().c_str());
      return false;
    }

    return true;
  }


// -------------------------------------------------------------------
// Function  : "ParseTwoArg ()"

  bool  ParseTwoArg  ( const char* function, PyObject* args, string format, PyObject** arg0, PyObject** arg1 ) {
    ostringstream  message;
    __cs.init ( function );
    if (not PyArg_ParseTuple(args,"O&O&",Converter,arg0,Converter,arg1)) {
      ostringstream m;
      m << function << ": Take exactly two argument.";
      PyErr_SetString(ConstructorError, m.str().c_str());
      return false;
    }

    string firstArgType    = ConverterState::getObjectType ( __cs.getObjectIds(), 0 );
    string firstArgTypeExp = ConverterState::getObjectType ( format             , 0 );

    if ( firstArgType != firstArgTypeExp ) {
      ostringstream m;
      m << function << ": First argument type mismatch, should be:<" << firstArgTypeExp
        << "> not <" << firstArgType << ">";
      PyErr_SetString(ConstructorError, m.str().c_str());
      return false;
    }

    string secondArgType    = ConverterState::getObjectType ( __cs.getObjectIds(), 1 );
    string secondArgTypeExp = ConverterState::getObjectType ( format             , 1 );

    if ( secondArgType != secondArgTypeExp ) {
      ostringstream m;
      m << function << ": Second argument type mismatch, should be:<" << secondArgTypeExp
        << "> not <" << secondArgType << ">";
      PyErr_SetString(ConstructorError, m.str().c_str());
      return false;
    }

    return true;
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


// +-----------------------------------------------------------------+
// |                "PyHurricane" C Global Variables                 |
// +-----------------------------------------------------------------+


extern "C" {


  PyObject* ConstructorError = NULL;
  PyObject* ProxyError       = NULL;
  PyObject* HurricaneError   = NULL;
  PyObject* CoriolisError    = NULL;
  PyObject* HurricaneWarning = NULL;


}

# else // End of PyHurricane Shared Library Code Part.

// +=================================================================+
// |              "PyHurricane" Python Module Code Part              |
// +=================================================================+


extern "C" {

  // +-------------------------------------------------------------+
  // |               "PyHurricane" Module Methods                  |
  // +-------------------------------------------------------------+

  static PyObject* PyCommons_trace ( PyObject* self, PyObject* args )
  {
    HTRY
    PyObject* state = NULL;
    if (PyArg_ParseTuple(args , "O:Hurricane.trace()", &state)) {
      if (PyObject_IsTrue(state)) trace_on ();
      else                  trace_off();
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to BasicLayer.create()." );
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }

  static PyMethodDef PyHurricane_Methods[] =
    { { "trace"                , PyCommons_trace        , METH_VARARGS, "Switch on/off the trace mode (for debugging)." }
    , { "DbU_db"               , PyDbU_fromDb           , METH_VARARGS, "Converts an integer to DbU::Unit (no scale factor)." }
    , { "DbU_grid"             , PyDbU_fromGrid         , METH_VARARGS, "Converts a founder grid to DbU::Unit." }
    , { "DbU_lambda"           , PyDbU_fromLambda       , METH_VARARGS, "Converts a symbolic (lambda) to DbU::Unit." }
    , { "DbU_getDb"            , PyDbU_toDb             , METH_VARARGS, "Converts a DbU::Unit to an integer value (no scale factor)." }
    , { "DbU_getGrid"          , PyDbU_toGrid           , METH_VARARGS, "Converts a DbU::Unit to a to grid founder." }
    , { "DbU_getLambda"        , PyDbU_toLambda         , METH_VARARGS, "Converts a DbU::Unit to a symbolic value (to lambda)." }
    , { "DbU_getPhysical"      , PyDbU_toPhysical       , METH_VARARGS, "Converts a DbU::Unit to a physical value." }
    , { "DbU_getOnPhysicalGrid", PyDbU_getOnPhysicalGrid, METH_VARARGS, "Adjusts a DbU::Unit to physical grid." }
    , {NULL, NULL, 0, NULL}    /* sentinel */
    };


  // ---------------------------------------------------------------
  // Module Initialization  :  "initHurricane ()"

  DL_EXPORT(void) initHurricane () {
    trace << "initHurricane()" << endl;

    PyUpdateSession_LinkPyType ();
    PyDbU_LinkPyType ();
    PyPoint_LinkPyType ();
    PyBox_LinkPyType ();
    PyTransformation_LinkPyType ();
    PyOrientation_LinkPyType ();
    PyDataBase_LinkPyType ();
    PyTechnology_LinkPyType ();
    PyLibrary_LinkPyType ();
    PyEntity_LinkPyType ();
    PyLayer_LinkPyType ();
    PyLayerMask_LinkPyType ();
    PyBasicLayer_LinkPyType ();
    PyRegularLayer_LinkPyType ();
    PyDiffusionLayer_LinkPyType ();
    PyTransistorLayer_LinkPyType ();
    PyContactLayer_LinkPyType ();
    PyViaLayer_LinkPyType ();
    PyLayerCollection_LinkPyType ();
    PyBasicLayerCollection_LinkPyType ();
    PyRegularLayerCollection_LinkPyType ();
    PyViaLayerCollection_LinkPyType ();
    PyMaterial_LinkPyType ();
    PyPath_LinkPyType ();
    PyOccurrence_LinkPyType ();
    PyInstanceCollection_LinkPyType ();
    PyPlugCollection_LinkPyType ();
    PyNetCollection_LinkPyType ();
    PyNetExternalComponents_LinkPyType ();
    PyCellCollection_LinkPyType ();
    PyPinPlacementStatus_LinkPyType ();
    PyPinDirection_LinkPyType ();
    PyPinCollection_LinkPyType ();
    PySegmentCollection_LinkPyType ();
    PyOccurrenceCollection_LinkPyType ();
    PyComponentCollection_LinkPyType ();
    PyReferenceCollection_LinkPyType ();
    PyCell_LinkPyType ();
    PyInstance_LinkPyType ();
    PyPlacementStatus_LinkPyType ();
    PyReference_LinkPyType ();
    PyNet_LinkPyType ();
    PyNetType_LinkPyType ();
    PyNetDirection_LinkPyType ();
    PyHyperNet_LinkPyType ();
    PyHook_LinkPyType ();
    PyHookCollection_LinkPyType ();
    PyComponent_LinkPyType ();
    PySegment_LinkPyType ();
    PyPad_LinkPyType ();
    PyRoutingPad_LinkPyType ();
    PyVertical_LinkPyType ();
    PyHorizontal_LinkPyType ();
    PyContact_LinkPyType ();
    PyPin_LinkPyType ();
    PyPlug_LinkPyType ();
    PyBreakpoint_LinkPyType ();
    PyQuery_LinkPyType ();
    PyQueryMask_LinkPyType ();

    PYTYPE_READY ( UpdateSession                 )
    PYTYPE_READY ( Point                         )
    PYTYPE_READY ( DbU                           )
    PYTYPE_READY ( Box                           )
    PYTYPE_READY ( Transformation                )
    PYTYPE_READY ( Orientation                   )
    PYTYPE_READY ( DataBase                      )
    PYTYPE_READY ( Technology                    )
    PYTYPE_READY ( Library                       )
    PYTYPE_READY ( Entity                        )
    PYTYPE_READY ( Hook                          )
    PYTYPE_READY ( HookCollection                )
    PYTYPE_READY ( Material                      )
    PYTYPE_READY ( Layer                         )
    PYTYPE_READY ( LayerMask                     )
    PYTYPE_READY ( BasicLayerCollection          )
    PYTYPE_READY ( BasicLayerCollectionLocator   )
    PYTYPE_READY ( LayerCollection               )
    PYTYPE_READY ( LayerCollectionLocator        )
    PYTYPE_READY ( RegularLayerCollection        )
    PYTYPE_READY ( RegularLayerCollectionLocator )
    PYTYPE_READY ( ViaLayerCollection            )
    PYTYPE_READY ( ViaLayerCollectionLocator     )
    PYTYPE_READY ( Path                          )
    PYTYPE_READY ( Occurrence                    )
    PYTYPE_READY ( PlacementStatus               )
    PYTYPE_READY ( InstanceCollection            )
    PYTYPE_READY ( InstanceCollectionLocator     )
    PYTYPE_READY ( PlugCollection                )
    PYTYPE_READY ( PlugCollectionLocator         )
    PYTYPE_READY ( NetType                       )
    PYTYPE_READY ( NetDirection                  )
    PYTYPE_READY ( NetCollection                 )
    PYTYPE_READY ( NetCollectionLocator          )
    PYTYPE_READY ( CellCollection                )
    PYTYPE_READY ( CellCollectionLocator         )
    PYTYPE_READY ( PinPlacementStatus            )
    PYTYPE_READY ( PinDirection                  )
    PYTYPE_READY ( PinCollection                 )
    PYTYPE_READY ( PinCollectionLocator          )
    PYTYPE_READY ( SegmentCollection             )
    PYTYPE_READY ( SegmentCollectionLocator      )
    PYTYPE_READY ( ComponentCollection           ) 
    PYTYPE_READY ( ComponentCollectionLocator    )
    PYTYPE_READY ( OccurrenceCollection          )
    PYTYPE_READY ( OccurrenceCollectionLocator   )
    PYTYPE_READY ( ReferenceCollection           )
    PYTYPE_READY ( ReferenceCollectionLocator    )
    PYTYPE_READY ( HyperNet                      )
    PYTYPE_READY ( NetExternalComponents         )
    PYTYPE_READY ( Breakpoint                    )
    PYTYPE_READY ( Query                         )
    PYTYPE_READY ( QueryMask                     )

    PYTYPE_READY_SUB ( BasicLayer     , Layer    )
    PYTYPE_READY_SUB ( RegularLayer   , Layer    )
    PYTYPE_READY_SUB ( ContactLayer   , Layer    )
    PYTYPE_READY_SUB ( DiffusionLayer , Layer    )
    PYTYPE_READY_SUB ( TransistorLayer, Layer    )
    PYTYPE_READY_SUB ( ViaLayer       , Layer    )
    PYTYPE_READY_SUB ( Cell           , Entity   )
    PYTYPE_READY_SUB ( Instance       , Entity   )
    PYTYPE_READY_SUB ( Reference      , Entity   )
    PYTYPE_READY_SUB ( Net            , Entity   )
    PYTYPE_READY_SUB ( Component      , Entity   )
    PYTYPE_READY_SUB ( RoutingPad     , Component)
    PYTYPE_READY_SUB ( Segment        , Component)
    PYTYPE_READY_SUB ( Horizontal     , Segment  )
    PYTYPE_READY_SUB ( Vertical       , Segment  )

    PYTYPE_READY_SUB ( Contact        , Component)
    PYTYPE_READY_SUB ( Pin            , Contact  )
    PYTYPE_READY_SUB ( Plug           , Component)
    PYTYPE_READY_SUB ( Pad            , Component)

    // Identifier string can take up to 10 characters !
    __cs.addType ( "box"        , &PyTypeBox                   , "<Box>"                   , false );
    __cs.addType ( "ent"        , &PyTypeEntity                , "<Entity>"                , false );
    __cs.addType ( "cell"       , &PyTypeCell                  , "<Cell>"                  , false, "ent" );
    __cs.addType ( "cellCol"    , &PyTypeCellCollection        , "<CellCollection>"        , false );
    __cs.addType ( "hook"       , &PyTypeHook                  , "<Hook>"                  , false );
    __cs.addType ( "hookColl"   , &PyTypeHookCollection        , "<HookCollection>"        , false );
    __cs.addType ( "comp"       , &PyTypeComponent             , "<Component>"             , false, "ent" );
    __cs.addType ( "compCol"    , &PyTypeComponentCollection   , "<ComponentCollection>"   , false );
    __cs.addType ( "contact"    , &PyTypeContact               , "<Contact>"               , false, "comp" );
    // Do not change the "none" string. It's hardwired to the None object.             
    __cs.addType ( "none"       ,  Py_None->ob_type            , "<None>"                  , true  );
    __cs.addType ( "float"      , &PyFloat_Type                , "<Float>"                 , true  );
    __cs.addType ( "int"        , &PyInt_Type                  , "<Int>"                   , true  );
    __cs.addType ( "bool"       , &PyBool_Type                 , "<Bool>"                  , true  );
    __cs.addType ( "string"     , &PyString_Type               , "<String>"                , true  );
    __cs.addType ( "list"       , &PyList_Type                 , "<List>"                  , true  );
    // Do not change the "function" string. It's hardwired to callable (function) objects.
    __cs.addType ( "function"   , NULL                         , "<Function>"              , true  );
    __cs.addType ( "horiz"      , &PyTypeHorizontal            , "<Horizontal>"            , false, "segment" );
    __cs.addType ( "inst"       , &PyTypeInstance              , "<Instance>"              , false, "ent" );
    __cs.addType ( "instCol"    , &PyTypeInstanceCollection    , "<InstanceCollection>"    , false );
    __cs.addType ( "mat"        , &PyTypeMaterial              , "<Material>"              , false );
    __cs.addType ( "basicLayer" , &PyTypeBasicLayer            , "<BasicLayer>"            , false, "layer" );
    __cs.addType ( "regLayer"   , &PyTypeRegularLayer          , "<RegularLayer>"          , false, "layer" );
    __cs.addType ( "contLayer"  , &PyTypeContactLayer          , "<ContactLayer>"          , false, "layer" );
    __cs.addType ( "diffLayer"  , &PyTypeDiffusionLayer        , "<DiffusionLayer>"        , false, "layer" );
    __cs.addType ( "tranLayer"  , &PyTypeTransistorLayer       , "<TransistorLayer>"       , false, "layer" );
    __cs.addType ( "viaLayer"   , &PyTypeViaLayer              , "<ViaLayer>"              , false, "layer" );
    __cs.addType ( "layerColl"  , &PyTypeLayerCollection       , "<LayerCollection>"       , false );
    __cs.addType ( "blayerColl" , &PyTypeBasicLayerCollection  , "<BasicLayerCollection>"  , false );
    __cs.addType ( "rlayerColl" , &PyTypeRegularLayerCollection, "<RegularLayerCollection>", false );
    __cs.addType ( "vlayerColl" , &PyTypeViaLayerCollection    , "<ViaLayerCollection>"    , false );
    __cs.addType ( "layer"      , &PyTypeLayer                 , "<Layer>"                 , false );
    __cs.addType ( "lmask"      , &PyTypeLayerMask             , "<Layer::Mask>"           , false );
    __cs.addType ( "library"    , &PyTypeLibrary               , "<Library>"               , false );
    __cs.addType ( "ref"        , &PyTypeReference             , "<Reference>"             , false, "ent" );
    __cs.addType ( "refCol"     , &PyTypeReferenceCollection   , "<ReferenceCollection>"   , false );
    __cs.addType ( "net"        , &PyTypeNet                   , "<Net>"                   , false, "ent" );
    __cs.addType ( "netCol"     , &PyTypeNetCollection         , "<NetCollection>"         , false );
    __cs.addType ( "hyperNet"   , &PyTypeHyperNet              , "<HyperNet>"              , false );
    __cs.addType ( "pin"        , &PyTypePin                   , "<Pin>"                   , false, "contact" );
    __cs.addType ( "pinCol"     , &PyTypePinCollection         , "<PinCollection>"         , false );
    __cs.addType ( "plug"       , &PyTypePlug                  , "<Plug>"                  , false, "comp" );
    __cs.addType ( "plugCol"    , &PyTypePlugCollection        , "<PlugCollection>"        , false );
    __cs.addType ( "point"      , &PyTypePoint                 , "<Point>"                 , false );
    __cs.addType ( "rp"         , &PyTypeRoutingPad            , "<RoutingPad>"            , false, "comp" );
    __cs.addType ( "segment"    , &PyTypeSegment               , "<Segment>"               , false, "comp" );
    __cs.addType ( "pad    "    , &PyTypePad                   , "<Pad>"                   , false, "comp" );
    __cs.addType ( "segmentCol" , &PyTypeSegmentCollection     , "<SegmentCollection>"     , false );
    __cs.addType ( "db"         , &PyTypeDataBase              , "<DataBase>"              , false );
    __cs.addType ( "techno"     , &PyTypeTechnology            , "<Technology>"            , false );
    __cs.addType ( "transfo"    , &PyTypeTransformation        , "<Transformation>"        , false );
    __cs.addType ( "orient"     , &PyTypeOrientation           , "<Orientation>"           , false );
    __cs.addType ( "vert"       , &PyTypeVertical              , "<Vertical>"              , false, "segment" );
    __cs.addType ( "path"       , &PyTypePath                  , "<Path>"                  , false );
    __cs.addType ( "occur"      , &PyTypeOccurrence            , "<Occurrence>"            , false );
    __cs.addType ( "occurCol"   , &PyTypeOccurrenceCollection  , "<OccurrenceCollection>"  , false );
    __cs.addType ( "query"      , &PyTypeQuery                 , "<Query>"                 , false );
    __cs.addType ( "qmask"      , &PyTypeQueryMask             , "<Query::Mask>"           , false );


    PyObject* module = Py_InitModule ( "Hurricane", PyHurricane_Methods );
    if ( module == NULL ) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Hurricane module." << endl;
      return;
    }

    Py_INCREF ( &PyTypeDbU );
    PyModule_AddObject ( module, "DbU"                  , (PyObject*)&PyTypeDbU );
    Py_INCREF ( &PyTypePoint );
    PyModule_AddObject ( module, "Box"                  , (PyObject*)&PyTypeBox );
    Py_INCREF ( &PyTypePoint );
    PyModule_AddObject ( module, "Point"                , (PyObject*)&PyTypePoint );
    Py_INCREF ( &PyTypeTransformation );
    PyModule_AddObject ( module, "Transformation"       , (PyObject*)&PyTypeTransformation );
    Py_INCREF ( &PyTypePath );
    PyModule_AddObject ( module, "Path"                 , (PyObject*)&PyTypePath );
    Py_INCREF ( &PyTypeOccurrence );
    PyModule_AddObject ( module, "Occurrence"           , (PyObject*)&PyTypeOccurrence );

    Py_INCREF ( &PyTypeDataBase );
    PyModule_AddObject ( module, "DataBase"             , (PyObject*)&PyTypeDataBase );
    Py_INCREF ( &PyTypeLibrary );
    PyModule_AddObject ( module, "Library"              , (PyObject*)&PyTypeLibrary );
    Py_INCREF ( &PyTypeNet );
    PyModule_AddObject ( module, "Net"                  , (PyObject*)&PyTypeNet );
    Py_INCREF ( &PyTypeHyperNet );
    PyModule_AddObject ( module, "HyperNet"             , (PyObject*)&PyTypeHyperNet );
    Py_INCREF ( &PyTypeInstance );
    PyModule_AddObject ( module, "Cell"                 , (PyObject*)&PyTypeCell );
    Py_INCREF ( &PyTypeInstance );
    PyModule_AddObject ( module, "Instance"             , (PyObject*)&PyTypeInstance );
    Py_INCREF ( &PyTypeTechnology );
    PyModule_AddObject ( module, "Technology"           , (PyObject*)&PyTypeTechnology );
    Py_INCREF ( &PyTypeLayer );
    PyModule_AddObject ( module, "Layer"                , (PyObject*)&PyTypeLayer );
    Py_INCREF ( &PyTypeBasicLayer );
    PyModule_AddObject ( module, "BasicLayer"           , (PyObject*)&PyTypeBasicLayer );
    Py_INCREF ( &PyTypeRegularLayer );
    PyModule_AddObject ( module, "RegularLayer"         , (PyObject*)&PyTypeRegularLayer );
    Py_INCREF ( &PyTypeContactLayer );
    PyModule_AddObject ( module, "ContactLayer"         , (PyObject*)&PyTypeContactLayer );
    Py_INCREF ( &PyTypeDiffusionLayer );
    PyModule_AddObject ( module, "DiffusionLayer"       , (PyObject*)&PyTypeDiffusionLayer );
    Py_INCREF ( &PyTypeTransistorLayer );
    PyModule_AddObject ( module, "TransistorLayer"      , (PyObject*)&PyTypeTransistorLayer );
    Py_INCREF ( &PyTypeViaLayer );
    PyModule_AddObject ( module, "ViaLayer"             , (PyObject*)&PyTypeViaLayer );
    Py_INCREF ( &PyTypeContactLayer );
    PyModule_AddObject ( module, "ContactLayer"         , (PyObject*)&PyTypeContactLayer );
    Py_INCREF ( &PyTypeNetExternalComponents );
    PyModule_AddObject ( module, "NetExternalComponents", (PyObject*)&PyTypeNetExternalComponents );
    Py_INCREF ( &PyTypeUpdateSession );
    PyModule_AddObject ( module, "UpdateSession"        , (PyObject*)&PyTypeUpdateSession );
    Py_INCREF ( &PyTypeBreakpoint );
    PyModule_AddObject ( module, "Breakpoint"           , (PyObject*)&PyTypeBreakpoint );
    Py_INCREF ( &PyTypeQuery );
    PyModule_AddObject ( module, "Query"                , (PyObject*)&PyTypeQuery );
    Py_INCREF ( &PyTypeReference );
    PyModule_AddObject ( module, "Reference"            , (PyObject*)&PyTypeReference );

    Py_INCREF ( &PyTypeHook );
    PyModule_AddObject ( module, "Hook"                 , (PyObject*)&PyTypeHook );
    Py_INCREF ( &PyTypeHookCollection );
    PyModule_AddObject ( module, "HookCollection"       , (PyObject*)&PyTypeHookCollection );
    Py_INCREF ( &PyTypePlug );
    PyModule_AddObject ( module, "PyPlug"               , (PyObject*)&PyTypePlug );
    Py_INCREF ( &PyTypeRoutingPad );
    PyModule_AddObject ( module, "RoutingPad"           , (PyObject*)&PyTypeRoutingPad );
    Py_INCREF ( &PyTypeVertical );
    PyModule_AddObject ( module, "Vertical"             , (PyObject*)&PyTypeVertical );
    Py_INCREF ( &PyTypeHorizontal );
    PyModule_AddObject ( module, "Horizontal"           , (PyObject*)&PyTypeHorizontal );
    Py_INCREF ( &PyTypeContact );
    PyModule_AddObject ( module, "Contact"              , (PyObject*)&PyTypeContact );
    Py_INCREF ( &PyTypePin );
    PyModule_AddObject ( module, "Pin"                  , (PyObject*)&PyTypePin );
    Py_INCREF ( &PyTypePad );
    PyModule_AddObject ( module, "Pad"                  , (PyObject*)&PyTypePad );
    
    
    PyObject* dictionnary = PyModule_GetDict ( module );

    ConstructorError = PyErr_NewException ( "hurricane.ConstructorError", NULL, NULL );
    ProxyError       = PyErr_NewException ( "hurricane.ProxyError"      , NULL, NULL );
    HurricaneError   = PyErr_NewException ( "hurricane.HurricaneError"  , NULL, NULL );
    HurricaneWarning = PyErr_NewException ( "hurricane.HurricaneWarning", PyExc_Warning, NULL );

    PyDict_SetItemString ( dictionnary, "ConstructorError", ConstructorError );
    PyDict_SetItemString ( dictionnary, "ProxyError"      , ProxyError );
    PyDict_SetItemString ( dictionnary, "HurricaneError"  , HurricaneError );

    DbULoadConstants( dictionnary );

    PyDbU_postModuleInit();
    PyTransformation_postModuleInit();
    PyLayer_postModuleInit();
    PyBasicLayer_postModuleInit();
    PyPin_postModuleInit();
    PyRoutingPad_postModuleInit();
    PyNet_postModuleInit();
    PyInstance_postModuleInit();

    trace << "Hurricane.so loaded " << (void*)&typeid(string) << endl;
  }

  
} // extern "C".

#endif // End of Python Module Code Part.

}  // Isobar namespace.
