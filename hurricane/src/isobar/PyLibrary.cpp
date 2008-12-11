// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyLibrary.cpp"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include "hurricane/isobar/PyDataBase.h"
#include "hurricane/isobar/PyLibrary.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PyCellCollection.h"


namespace Isobar {

using namespace Hurricane;

extern "C" {


#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Library,lib,function)


// x=================================================================x
// |              "PyLibrary" Python Module Code Part                |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |              "PyLibrary" Attribute Methods                  |
  // x-------------------------------------------------------------x



  // ---------------------------------------------------------------
  // Attribute Method  :  "PyLibrary_getName ()"
  GetNameMethod(Library, lib)


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyLibrary_getCell ()"

  PyObject* PyLibrary_getCell ( PyLibrary* self, PyObject* args ) {
      trace << "PyLibrary_getCell ()" << endl;
      
      Cell* cell = NULL;

      HTRY
      METHOD_HEAD("Library.getCell()")
      char* name = NULL;
      if (PyArg_ParseTuple(args,"s:Library.getCell", &name)) {
          cell = lib->getCell (Name(name));
      } else {
          PyErr_SetString ( ConstructorError, "invalid number of parameters for getCell." );
          return NULL;
      }
      HCATCH

      return PyCell_Link(cell);
  }



  // ---------------------------------------------------------------
  // Attribute Method  :  "PyLibrary_getCells()"

  static PyObject* PyLibrary_getCells(PyLibrary *self) {
    trace << "PyLibrary_getCells()" << endl;

    METHOD_HEAD ( "Library.getCells()" )

    PyCellCollection* pyCellCollection = NULL;

    HTRY
    Cells* cells = new Cells(lib->getCells ());

    pyCellCollection = PyObject_NEW(PyCellCollection, &PyTypeCellCollection);
    if (pyCellCollection == NULL) { 
        return NULL;
    }

    pyCellCollection->_object = cells;
    HCATCH
    
    return (PyObject*)pyCellCollection;
  }

  // Standart Accessors (Attributes).


  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyLibrary_destroy, PyLibrary)

  // ---------------------------------------------------------------
  // PyLibrary Attribute Method table.

  PyMethodDef PyLibrary_Methods[] =
    {
      { "getName"      , (PyCFunction)PyLibrary_getName      , METH_NOARGS , "Returns the name of the library." }
    , { "getCell"        , (PyCFunction)PyLibrary_getCell        , METH_VARARGS, "Get the cell of name <name>" }
    , { "getCells", (PyCFunction)PyLibrary_getCells, METH_NOARGS , "Returns the collection of all cells of the library." }
    , { "destroy"       , (PyCFunction)PyLibrary_destroy       , METH_NOARGS
                       , "Destroy associated hurricane object The python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |                "PyLibrary" Object Methods                   |
  // x-------------------------------------------------------------x

  static PyObject* PyLibrary_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
      trace << "PyLibrary_new()" << endl;

      Library* library = NULL;

      HTRY
      char* name = NULL;
      PyDataBase* pyDataBase = NULL;
      PyLibrary* pyRootLibrary = NULL;
      if (PyArg_ParseTuple(args,"O!s:Library.new", &PyTypeDataBase, &pyDataBase, &name)) {
          DataBase* db = PYDATABASE_O(pyDataBase);
          library = Library::create(db, Name(name));
      } else if (PyArg_ParseTuple(args,"O!s:Library.new", &PyTypeLibrary, &pyRootLibrary, &name)) {
          Library* rootLibrary = PYLIBRARY_O(pyRootLibrary);
          library = Library::create(rootLibrary, Name(name));
      } else {
        PyErr_SetString ( ConstructorError, "invalid number of parameters for Library constructor." );
        return NULL;
      }
      HCATCH

      return PyLibrary_Link ( library );
  }

  DBoDeleteMethod(Library)
  PyTypeObjectLinkPyType(Library)
  PyTypeObjectConstructor(Library)


#else  // End of Python Module Code Part.


// x=================================================================x
// |             "PyLibrary" Shared Library Code Part                |
// x=================================================================x


  // Link/Creation Method.
  DBoLinkCreateMethod(Library)




  // ---------------------------------------------------------------
  // PyLibrary Object Definitions.

  PyTypeObjectDefinitions(Library)


#endif  // End of Shared Library Code Part.


}  // End of extern "C".

}  // End of Isobar namespace.
