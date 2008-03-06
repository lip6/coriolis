
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
// $Id: PyLibrary.cpp,v 1.17 2006/09/22 11:27:31 tsunami Exp $
//
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




#include "PyDataBase.h"
#include "PyName.h"
#include "PyLibrary.h"
#include "PyCell.h"
#include "PyCellLocator.h"


namespace Isobar {


USING_NAMESPACE_HURRICANE

extern "C" {


# define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Library,lib,function)


// x=================================================================x
// |              "PyLibrary" Python Module Code Part                |
// x=================================================================x

# if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |              "PyLibrary" Attribute Methods                  |
  // x-------------------------------------------------------------x



  // ---------------------------------------------------------------
  // Attribute Method  :  "PyLibrary_GetName ()"

  static PyObject* PyLibrary_GetName ( PyLibrary *self ) {
    trace << "PyLibrary_GetName ()" << endl;
    METHOD_HEAD ( "Library.GetName()" )

    PyName* pyName = PyObject_NEW ( PyName, &PyTypeName );
    if ( pyName == NULL ) { return NULL; }

    HTRY
    pyName->_object = new Name ( lib->GetName() );
    HCATCH
    
    return ( (PyObject*)pyName );
  
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyLibrary_GetCell ()"

  PyObject* PyLibrary_GetCell ( PyLibrary* self, PyObject* args ) {
    trace << "PyLibrary_GetCell ()" << endl;
    
    Cell* cell = NULL;

    HTRY
    METHOD_HEAD ( "Library.GetCell()" )

    PyObject* arg0;
    if ( ! ParseOneArg ( "Library.GetCell", args, NAME_ARG, &arg0 ) ) return ( NULL );

    cell = lib->GetCell ( *PYNAME_O(arg0) );
    HCATCH

    return PyCell_Link ( cell );
  }



  // ---------------------------------------------------------------
  // Attribute Method  :  "PyLibrary_GetCellsLocator ()"

  static PyObject* PyLibrary_GetCellsLocator ( PyLibrary *self ) {
    trace << "PyLibrary_GetCellsLocator ()" << endl;
    METHOD_HEAD ( "Library.GetCellsLocator()" )

    Cells cells = lib->GetCells ();

    PyCellLocator* pyCellLocator = PyObject_NEW ( PyCellLocator, &PyTypeCellLocator );
    if (pyCellLocator == NULL) { return NULL; }

    HTRY
    pyCellLocator->_object = cells.GetLocator ();
    HCATCH

    return ( (PyObject*)pyCellLocator );
  }


  // Standart Accessors (Attributes).


  // Standart Delete (Attribute).
  DBoDeleteAttribute(PyLibrary_Delete,PyLibrary)

  // ---------------------------------------------------------------
  // PyLibrary Attribute Method table.

  PyMethodDef PyLibrary_Methods[] =
    {
      { "GetName"      , (PyCFunction)PyLibrary_GetName      , METH_NOARGS , "Returns the name of the library." }
    , { "GetCell"        , (PyCFunction)PyLibrary_GetCell        , METH_VARARGS, "Get the cell of name <name>" }
    , { "GetCellsLocator", (PyCFunction)PyLibrary_GetCellsLocator, METH_NOARGS , "Returns the collection of all cells of the library." }
    , { "Delete"       , (PyCFunction)PyLibrary_Delete       , METH_NOARGS
                       , "Delete associated hurricane object The python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |                "PyLibrary" Object Methods                   |
  // x-------------------------------------------------------------x

  static PyObject* PyLibrary_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
      trace << "PyLibrary_new()" << endl;

      PyObject* arg0;
      PyObject* arg1;
      Library* library = NULL;
      if (ParseTwoArg("Library.new", args, ":db:name", &arg0, &arg1)) {
          HTRY
          DataBase* db = PYDATABASE_O(arg0);
          library = Library::Create(db, *PYNAME_O(arg1));
          HCATCH
      } else if (ParseTwoArg("Library.new", args, ":db:name", &arg0, &arg1)) { 
          HTRY
          Library* masterLibrary = PYLIBRARY_O(arg0);
          library = Library::Create(masterLibrary, *PYNAME_O(arg1));
          HCATCH
      } else {
          return NULL;
      }

      return PyLibrary_Link ( library );
  }

  DBoDeleteMethod(Library)
  PyTypeObjectLinkPyType(Library)
  PyTypeObjectConstructor(Library)


# else  // End of Python Module Code Part.


// x=================================================================x
// |             "PyLibrary" Shared Library Code Part                |
// x=================================================================x


  // Link/Creation Method.
  DBoLinkCreateMethod(PyLibrary_Link,PyLibrary,PyTypeLibrary,Library)




  // ---------------------------------------------------------------
  // PyLibrary Object Definitions.

  PyTypeRootObjectDefinitions(Library)


# endif  // End of Shared Library Code Part.


}  // End of extern "C".

}  // End of Isobar namespace.
