// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyInstance.cpp"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#include "hurricane/isobar/PyBox.h"
#include "hurricane/isobar/PyTransformation.h"
#include "hurricane/isobar/PyLibrary.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PyInstance.h"
#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyPlug.h"
#include "hurricane/isobar/PyPlugCollection.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#undef   ACCESS_OBJECT
#undef   ACCESS_CLASS
#define  ACCESS_OBJECT           _baseObject._object
#define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Instance,instance,function)

#define  LOAD_CONSTANT(CONSTANT_VALUE,CONSTANT_NAME)             \
  constant = PyInt_FromLong ( (long)CONSTANT_VALUE );             \
  PyDict_SetItemString ( dictionnary, CONSTANT_NAME, constant );  \
  Py_DECREF ( constant );


// x=================================================================x
// |              "PyInstance" Python Module Code Part               |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                  Global Constants Loading                   |
  // x-------------------------------------------------------------x


  extern void  InstanceLoadConstants ( PyObject* dictionnary )
  {
    PyObject* constant;
      
    LOAD_CONSTANT ( Instance::PlacementStatus::UNPLACED, "PlacementStatusUNPLACED" )
    LOAD_CONSTANT ( Instance::PlacementStatus::PLACED  , "PlacementStatusPLACED" )
    LOAD_CONSTANT ( Instance::PlacementStatus::FIXED   , "PlacementStatusFIXED" )
  }




  // x-------------------------------------------------------------x
  // |               "PyInstance" Local Functions                  |
  // x-------------------------------------------------------------x
      
  // ---------------------------------------------------------------
  // Local Function  :  "PyInt_AsType ()"

  static Instance::PlacementStatus  PyInt_AsPlacementStatus ( PyObject* object )
  {
    switch ( PyInt_AsLong(object) ) {
      case Instance::PlacementStatus::UNPLACED : return ( Instance::PlacementStatus(Instance::PlacementStatus::UNPLACED) );
      case Instance::PlacementStatus::PLACED   : return ( Instance::PlacementStatus(Instance::PlacementStatus::PLACED) );
      case Instance::PlacementStatus::FIXED    : return ( Instance::PlacementStatus(Instance::PlacementStatus::FIXED) );
    }
      
    return ( Instance::PlacementStatus(Instance::PlacementStatus::UNPLACED) );
  }
  



  // x-------------------------------------------------------------x
  // |              "PyInstance" Attribute Methods                 |
  // x-------------------------------------------------------------x


  // Standart Accessors (Attributes).


  // Standart destroy (Attribute).
  DBoDestroyAttribute(PyInstance_destroy,PyInstance)




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_getName ()"

  GetNameMethod(Instance, instance)

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_getMasterCell ()"

  static PyObject* PyInstance_getMasterCell ( PyInstance *self ) {
    trace << "PyInstance_getMasterCell ()" << endl;
    
    Cell* cell = NULL;

    HTRY
    METHOD_HEAD ( "Instance.getMasterCell()" )
    cell = instance->getMasterCell ();
    HCATCH

    return PyCell_Link ( cell );
  }

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_getPlacementStatus ()"

  static PyObject* PyInstance_getPlacementStatus ( PyInstance *self ) {
    trace << "PyInstance_getPlacementStatus ()" << endl;
    
    METHOD_HEAD ( "Instance.getPlacementStatus()" );

    PyObject* pyObject = NULL;
    HTRY
    pyObject = (PyObject*)Py_BuildValue("i",(long)instance->getPlacementStatus().getCode());
    HCATCH

    return pyObject;
  }

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_SetPlacementStatus ()"

  static PyObject* PyInstance_setPlacementStatus ( PyInstance *self, PyObject* args ) {
    trace << "PyInstance_setPlacementStatus()" << endl;
    METHOD_HEAD ( "Instance.setPlacementStatus()" )
      
    HTRY
    PyObject* arg0;
    if ( ! ParseOneArg ( "Instance.setPlacementStatus()", args, INT_ARG, (PyObject**)&arg0 ) ) return ( NULL );
      
    instance->setPlacementStatus ( PyInt_AsPlacementStatus(arg0) );
    HCATCH
      
    Py_RETURN_NONE;
  }

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_getTransformation ()"

  static PyObject* PyInstance_getTransformation ( PyInstance *self ) {
    trace << "PyInstance_getTransformation ()" << endl;
    METHOD_HEAD ( "Instance.getTransformation()" );

    PyTransformation* resultPyTransf = PyObject_NEW ( PyTransformation, &PyTypeTransformation );
    if ( resultPyTransf == NULL ) { return NULL; }

    HTRY
    resultPyTransf->_object = new Transformation ( instance->getTransformation () );
    HCATCH

    return ( (PyObject*)resultPyTransf );
  }

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_getPlug ()"

  static PyObject* PyInstance_getPlug ( PyInstance *self, PyObject* args ) {
    trace << "PyInstance_getPlug ()" << endl;

    Plug* plug = NULL;
    
    HTRY
    METHOD_HEAD ( "Instance.getPlug()" )

    PyNet* masterNet;
    if ( ! ParseOneArg ( "Instance.getPlug", args, NET_ARG, (PyObject**)&masterNet ) ) return ( NULL );
 
    plug = instance->getPlug( PYNET_O(masterNet) );
    HCATCH

    return PyPlug_Link ( plug );
  }

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_getPlugs()"

  static PyObject* PyInstance_getPlugs(PyInstance *self ) {
    trace << "PyInstance_getPlugs()" << endl;

    METHOD_HEAD ( "Instance.getPlugs()" )

    PyPlugCollection* pyPlugCollection = NULL;

    HTRY
    Plugs* plugs = new Plugs(instance->getPlugs());

    pyPlugCollection = PyObject_NEW(PyPlugCollection, &PyTypePlugCollection);
    if (pyPlugCollection == NULL) { 
        return NULL;
    }

    pyPlugCollection->_object = plugs;
    HCATCH
    
    return (PyObject*)pyPlugCollection;
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_getConnectedPlugs()"

  static PyObject* PyInstance_getConnectedPlugs(PyInstance *self) {
    trace << "PyInstance_getConnectedPlugs ()" << endl;

    METHOD_HEAD ( "Instance.getConnectedPlugs()")

    PyPlugCollection* pyPlugCollection = NULL;

    HTRY
    Plugs* plugs = new Plugs(instance->getConnectedPlugs());

    pyPlugCollection = PyObject_NEW(PyPlugCollection, &PyTypePlugCollection);
    if (pyPlugCollection == NULL) { 
        return NULL;
    }

    pyPlugCollection->_object = plugs;
    HCATCH
    
    return (PyObject*)pyPlugCollection;
  }

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_getUnconnectedPlugsLocator ()"

  static PyObject* PyInstance_getUnconnectedPlugs(PyInstance *self) {
    trace << "PyInstance_getUnconnectedPlugs ()" << endl;

    METHOD_HEAD ( "Instance.getUnconnectedPlugs()")

    PyPlugCollection* pyPlugCollection = NULL;

    HTRY
    Plugs* plugs = new Plugs(instance->getUnconnectedPlugs());

    pyPlugCollection = PyObject_NEW(PyPlugCollection, &PyTypePlugCollection);
    if (pyPlugCollection == NULL) { 
        return NULL;
    }

    pyPlugCollection->_object = plugs;
    HCATCH
    
    return (PyObject*)pyPlugCollection;
  }

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_getAbutmentBox ()"

  static PyObject* PyInstance_getAbutmentBox ( PyInstance *self ) {
    trace << "PyInstance_getAbutmentBox ()" << endl;
    METHOD_HEAD ( "Instance.getAbutmentBox()" )

    PyBox* pyBox = PyObject_NEW ( PyBox, &PyTypeBox );
    if (pyBox == NULL) { return NULL; }

    HTRY
    pyBox->_object = new Box ( instance->getAbutmentBox() );
    HCATCH

    return ( (PyObject*)pyBox );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_setName ()"

  SetNameMethod(Instance, instance)


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_SetTransformation ()"

  static PyObject* PyInstance_setTransformation ( PyInstance *self, PyObject* args ) {
    trace << "PyInstance_setTransformation()" << endl;
    METHOD_HEAD ( "Instance.setTransformation()" )

    HTRY
    PyTransformation* transformation;
    if ( ! ParseOneArg ( "Instance.setTransformation", args, TRANS_ARG, (PyObject**)&transformation ) ) return ( NULL );

    instance->setTransformation ( *PYTRANSFORMATION_O(transformation) );
    HCATCH

    Py_RETURN_NONE;
  }


  
  
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_SetMasterCell ()"

  static PyObject* PyInstance_setMasterCell ( PyInstance *self, PyObject* args ) {
    trace << "Instance.setMasterCell()" << endl;
    METHOD_HEAD ( "Instance.setMasterCell()" )

    HTRY
    PyCell* masterCell;
    if ( ! ParseOneArg ( "Instance.setMasterCell", args, CELL_ARG, (PyObject**)&masterCell ) ) return ( NULL );

    instance->setMasterCell ( PYCELL_O(masterCell) );
    HCATCH

    Py_RETURN_NONE;
  }

  // Standart Predicates (Attributes).
  DirectGetBoolAttribute(PyInstance_isTerminal ,isTerminal ,PyInstance,Instance)
  DirectGetBoolAttribute(PyInstance_isLeaf     ,isLeaf     ,PyInstance,Instance)

  GetBoundStateAttribute(PyInstance_isPyBound              ,PyInstance,Instance)
  

  
  // ---------------------------------------------------------------
  // PyInstance Attribute Method table.

  PyMethodDef PyInstance_Methods[] =
    { { "getName"                   , (PyCFunction)PyInstance_getName                   , METH_NOARGS , "Returns the instance name." }
    , { "getMasterCell"             , (PyCFunction)PyInstance_getMasterCell             , METH_NOARGS , "Returns the cell model referenced by the instance." }
    , { "getTransformation"         , (PyCFunction)PyInstance_getTransformation         , METH_NOARGS , "Returns the transformation associated to the instance." }
    , { "getPlacementStatus"        , (PyCFunction)PyInstance_getPlacementStatus        , METH_NOARGS , "Returns the placement status of the instance." }
    , { "getPlug"                   , (PyCFunction)PyInstance_getPlug                   , METH_VARARGS, "Returns the plug associated to the <masterNet> if it exists or else NULL (if the net is not external)." }
    , { "getPlugs"           , (PyCFunction)PyInstance_getPlugs           , METH_NOARGS , "Returns the collection of instance plugs." }
    , { "getConnectedPlugs"  , (PyCFunction)PyInstance_getConnectedPlugs  , METH_NOARGS , "Returns the collection of instance plugs which are effectively connected." }
    , { "getUnconnectedPlugs", (PyCFunction)PyInstance_getUnconnectedPlugs, METH_NOARGS , "Returns the collection of instance plugs which are not connected." }
    , { "getAbutmentBox"            , (PyCFunction)PyInstance_getAbutmentBox            , METH_NOARGS , "Returns the abutment box of the instance, that is the abutment box of the master cell to which has been applied the instance transformation." }
    , { "isTerminal"                , (PyCFunction)PyInstance_isTerminal                , METH_NOARGS , "Returns true if the instance is a terminal instance." }
    , { "isLeaf"                    , (PyCFunction)PyInstance_isLeaf                    , METH_NOARGS , "Returns true if the instance is a leaf instance." }
    , { "isBound"                   , (PyCFunction)PyInstance_isPyBound                 , METH_NOARGS, "Returns true if the instance is bounded to the hurricane instance" }
    , { "setName"                   , (PyCFunction)PyInstance_setName                   , METH_VARARGS, "Allows to change the instance name." }
    , { "setTransformation"         , (PyCFunction)PyInstance_setTransformation         , METH_VARARGS, "Allows to modify the instance transformation." }
    , { "setPlacementStatus"        , (PyCFunction)PyInstance_setPlacementStatus        , METH_VARARGS, "Allows to modify the instance placement status." }
    , { "setMasterCell"             , (PyCFunction)PyInstance_setMasterCell             , METH_VARARGS, "Allows to change the cell referenced by this instance." }
    , { "destroy"                   , (PyCFunction)PyInstance_destroy                   , METH_NOARGS
                                    , "Destroy associated hurricane object The python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |                "PyInstance" Object Methods                  |
  // x-------------------------------------------------------------x
  
  DBoDeleteMethod(Instance)
  PyTypeObjectLinkPyType(Instance)


#else  // End of Python Module Code Part.


// x=================================================================x
// |             "PyInstance" Shared Library Code Part               |
// x=================================================================x

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyInstance_new ()"

  PyObject* PyInstance_create ( PyObject *module, PyObject *args ) {
    trace << "PyInstance_create ()" << endl;
    
    Instance* instance = NULL;
    PyObject* arg0;
    PyObject* arg1;
    PyObject* arg2;
    PyObject* arg3;

    HTRY
    __cs.init ("Instance.create");
    if ( ! PyArg_ParseTuple(args,"O&O&O&|O&:Instance.new"
                           ,Converter,&arg0
                           ,Converter,&arg1
                           ,Converter,&arg2
                           ,Converter,&arg3
                           )) {
        PyErr_SetString ( ConstructorError, "invalid number of parameters for Instance constructor." );
        return NULL;
    }

    if      ( __cs.getObjectIds() == ":ent:string:ent") {
        instance = Instance::create(
                PYCELL_O(arg0),
                Name(PyString_AsString(arg1)),
                PYCELL_O(arg2) );
    } else if ( __cs.getObjectIds() == ":ent:string:ent:transfo") {
        instance = Instance::create(
                PYCELL_O(arg0),
                Name(PyString_AsString(arg1)),
                PYCELL_O(arg2),
                *PYTRANSFORMATION_O(arg3),
                Instance::PlacementStatus::PLACED);
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Instance constructor." );
      return NULL;
    }
    HCATCH

    return PyInstance_Link ( instance );
  }


  // Link/Creation Method.
  DBoLinkCreateMethod(Instance)


  // ---------------------------------------------------------------
  // PyInstance Object Definitions.

  PyTypeInheritedObjectDefinitions(Instance, Entity)


#endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
