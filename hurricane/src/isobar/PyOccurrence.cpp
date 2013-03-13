// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyOccurrence.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#include "hurricane/isobar/PyBox.h"
#include "hurricane/isobar/PyTransformation.h"
#include "hurricane/isobar/PyPath.h"
#include "hurricane/isobar/PyOccurrence.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PySegment.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Occurrence,occurrence,function)


// x=================================================================x
// |             "PyOccurrence" Python Module Code Part              |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |              "PyOccurrence" Attribute Methods               |
  // x-------------------------------------------------------------x



  // Standart Accessors (Attributes).


  // Standart Predicates (Attributes).
  DirectGetBoolAttribute  (PyOccurrence_isValid         ,isValid         ,PyOccurrence,Occurrence)
  DirectGetBoolAttribute  (PyOccurrence_hasProperty     ,hasProperty     ,PyOccurrence,Occurrence)
  DirectGetStringAttribute(PyOccurrence_getCompactString,getCompactString,PyOccurrence,Occurrence)


  // Standart destroy (Attribute).
  DirectDestroyAttribute(PyOccurrence_destroy, PyOccurrence)




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyOccurrence_getEntity ()"

  static PyObject* PyOccurrence_getEntity ( PyOccurrence *self ) {
    trace << "PyOccurrence_getEntity()" << endl;

    METHOD_HEAD ( "Occurrence.getEntity()" )

    PyObject* pyEntity = NULL;

    HTRY
    Entity* entity = occurrence->getEntity();
    if ( entity == NULL )
        Py_RETURN_NONE;

    pyEntity = PyEntity_NEW ( entity );
    HCATCH    

    return pyEntity;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyOccurrence_getOwnerCell ()"

  static PyObject* PyOccurrence_getOwnerCell ( PyOccurrence *self ) {
    trace << "PyOccurrence_getOwnerCell()" << endl;

    METHOD_HEAD ( "Occurernce.getOwnerCell()" )

    Cell* cell = NULL;

    HTRY
    cell = occurrence->getOwnerCell();
    HCATCH    

    return PyCell_Link ( cell );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyOccurrence_getMasterCell ()"

  static PyObject* PyOccurrence_getMasterCell ( PyOccurrence *self )
  {
    trace << "PyOccurrence_getMasterCell()" << endl;

    METHOD_HEAD ( "Occurrence.getMasterCell()" )

    Cell* cell = NULL;

    HTRY
    cell = occurrence->getMasterCell();
    HCATCH    

    return PyCell_Link ( cell );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyOccurrence_getPath ()"

  static PyObject* PyOccurrence_getPath ( PyOccurrence *self )
  {
    trace << "PyOccurrence_getPath ()" << endl;
    
    METHOD_HEAD ( "Occurrence.getPath()" )

    PyPath* pyPath = PyObject_NEW ( PyPath, &PyTypePath );
    if ( pyPath == NULL ) { return NULL; }

    HTRY
    pyPath->_object = new Path ( occurrence->getPath() );
    HCATCH
    
    return ( (PyObject*)pyPath );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyOccurrence_getName ()"

  GetNameMethod(Occurrence, occurrence)


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyOccurrence_getBoundingBox ()"

  static PyObject* PyOccurrence_getBoundingBox ( PyOccurrence *self )
  {
    trace << "PyOccurrence_getBoundingBox ()" << endl;
    
    METHOD_HEAD ( "Occurrence.getBoundingBox()" )

    PyBox* pyBox = PyObject_NEW ( PyBox, &PyTypeBox );
    if ( pyBox == NULL ) { return NULL; }

    HTRY
    pyBox->_object = new Box ( occurrence->getBoundingBox() );
    HCATCH
    
    return ( (PyObject*)pyBox );
  }




  // ---------------------------------------------------------------
  // PyOccurrence Attribute Method table.

  PyMethodDef PyOccurrence_Methods[] =
    { { "getEntity"       , (PyCFunction)PyOccurrence_getEntity       , METH_NOARGS, "Returns the referenced entity." }
    , { "getPath"         , (PyCFunction)PyOccurrence_getPath         , METH_NOARGS, "Returns the hierarchical instaciation path." }
    , { "getName"         , (PyCFunction)PyOccurrence_getName         , METH_NOARGS, "Returns the occurrence name (concatenation of path and entity name." }
    , { "getCompactString", (PyCFunction)PyOccurrence_getCompactString, METH_NOARGS, "Returns the occurrence name (compact format)." }
    , { "getOwnerCell"    , (PyCFunction)PyOccurrence_getOwnerCell    , METH_NOARGS, "Returns the occurrence owner cell." }
    , { "getMasterCell"   , (PyCFunction)PyOccurrence_getMasterCell   , METH_NOARGS, "Returns the cell owning the referenced entity." }
    , { "getBoundingBox"  , (PyCFunction)PyOccurrence_getBoundingBox  , METH_NOARGS, "Returns the occurrence bounding box." }
    , { "isValid"         , (PyCFunction)PyOccurrence_isValid         , METH_NOARGS, "Returns true if the occurrence is valid." }
    , { "hasProperty"     , (PyCFunction)PyOccurrence_hasProperty     , METH_NOARGS, "Returns true if the occurrence owns some properties." }
    , { "destroy"         , (PyCFunction)PyOccurrence_destroy         , METH_NOARGS
                          , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |               "PyOccurrence" Object Methods                 |
  // x-------------------------------------------------------------x



  DirectDeleteMethod(PyOccurrence_DeAlloc,PyOccurrence)
  PyTypeObjectLinkPyType(Occurrence)


#else  // End of Python Module Code Part.


// x=================================================================x
// |            "PyOccurrence" Shared Library Code Part              |
// x=================================================================x

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyOccurrence_create ()"

  PyObject* PyOccurrence_create ( PyObject *module, PyObject *args ) {
    trace << "PyOccurrence_create()" << endl;

    Occurrence* occurrence;
    PyObject*   arg0;
    PyObject*   arg1;

    __cs.init ("Occurrence.create");
    if ( ! PyArg_ParseTuple(args,"|O&O&:Occurrence.create"
                           ,Converter,&arg0
                           ,Converter,&arg1
                           )) {
        PyErr_SetString ( ConstructorError, "invalid number of parameters for Occurrence constructor. " );
        return NULL;
    }

    if      ( __cs.getObjectIds() == NO_ARG       ) { occurrence = new Occurrence (); }
    else if ( __cs.getObjectIds() == ENT_ARG      ) { occurrence = new Occurrence (  PYENTITY_O(arg0) ); }
    else if ( __cs.getObjectIds() == COMP_PATH_ARG) { occurrence = new Occurrence (  PYSEGMENT_O(arg0)
                                                                                  , *PYPATH_O(arg1) ); }
    else if ( __cs.getObjectIds() == ENT_PATH_ARG ) { occurrence = new Occurrence (  PYENTITY_O(arg0)
                                                                                  , *PYPATH_O(arg1) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Occurrence constructor. " );
      return ( NULL );
    }

    PyOccurrence* pyOccurrence = PyObject_NEW(PyOccurrence, &PyTypeOccurrence);
    if (pyOccurrence == NULL) return NULL;
    
    HTRY
    pyOccurrence->_object = occurrence;
    HCATCH

    return ( (PyObject*)pyOccurrence );
  }



  // ---------------------------------------------------------------
  // PyOccurrence Object Definitions.
  
  PyTypeObjectDefinitions(Occurrence)


#endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
