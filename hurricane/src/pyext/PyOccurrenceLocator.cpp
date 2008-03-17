
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
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Sophie BELLOEIL               |
// |  E-mail      :              Damien.Dupuis@lip6.fr               |
// | =============================================================== |
// |  C++ Module  :       "./PyNetLocator.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




# include  "PyOccurrence.h"
# include  "PyOccurrenceLocator.h"


namespace  Isobar {


USING_NAMESPACE_HURRICANE


extern "C" {


# define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Locator<Occurrence>,locator,function)


// x=================================================================x
// |         "PyOccurrenceLocator" Python Module Code Part           |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |           "PyOccurrenceLocator" Attribute Methods           |
  // x-------------------------------------------------------------x


  // Standart Predicates (Attributes).
  DirectGetBoolAttribute(PyOccurrenceLocator_IsValid,IsValid,PyOccurrenceLocator,Locator<Occurrence>)

  // Standart destroy (Attribute).
  DirectDestroyAttribute(PyOccurrenceLocator_destroy, PyOccurrenceLocator)


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyOccurrenceLocator_Progress ()"

  static PyObject* PyOccurrenceLocator_Progress ( PyOccurrenceLocator *self ) {
    trace << "OccurrenceLocator.Progress()" << endl;

    HTRY
    METHOD_HEAD ( "OccurrenceLocator.Progress()" )

    locator->Progress ();
    HCATCH

    Py_RETURN_NONE;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyOccurrenceLocator_getElement ()"

  static PyObject* PyOccurrenceLocator_getElement ( PyOccurrenceLocator *self ) {
    trace << "OccurrenceLocator.getElement()" << endl;
    
    METHOD_HEAD ( "OccurrenceLocator.getElement()" )
      
    PyOccurrence* pyOccurrence = PyObject_NEW ( PyOccurrence, &PyTypeOccurrence );
    if ( pyOccurrence == NULL ) return NULL;

    HTRY
    Occurrence  occurrence = locator->getElement ();
    if ( occurrence == NULL )
        Py_RETURN_NONE;

    pyOccurrence->_object = new Occurrence ( occurrence );
    HCATCH
    
    return (PyObject*)pyOccurrence;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyOccurrenceLocator_getClone ()"

  static PyObject* PyOccurrenceLocator_getClone ( PyOccurrenceLocator *self )
  {
    trace << "OccurrenceLocator.getClone()" << endl;
    
    METHOD_HEAD ( "OccurrenceLocator.getClone()" )

    PyOccurrenceLocator* pyOccurrenceLocator = NULL;

    HTRY
    pyOccurrenceLocator = PyObject_NEW ( PyOccurrenceLocator, &PyTypeOccurrenceLocator );
    if ( pyOccurrenceLocator == NULL ) { return NULL; }
    
    pyOccurrenceLocator->_object = locator->getClone();
    HCATCH

    return ( (PyObject*)pyOccurrenceLocator );
  }


  // ---------------------------------------------------------------
  // PyOccurrenceLocator Attribute Method table.

  PyMethodDef PyOccurrenceLocator_Methods[] =
    { { "IsValid"        , (PyCFunction)PyOccurrenceLocator_IsValid    , METH_NOARGS , "Returns true while the walk has not exhausted the set of elements, else false." }
    , { "Progress"       , (PyCFunction)PyOccurrenceLocator_Progress   , METH_NOARGS , "Moves forward the locator to the following element." }
    , { "getElement"     , (PyCFunction)PyOccurrenceLocator_getElement , METH_NOARGS , "Returns the current element (or the value Type() when the locator is not or no longer valid)." }
    , { "getClone"       , (PyCFunction)PyOccurrenceLocator_getClone   , METH_NOARGS , "This function allocates and returns a new locator that will have the same visiting course than the remaining one of the locator being cloned." }
    , { "destroy"        , (PyCFunction)PyOccurrenceLocator_destroy    , METH_NOARGS
                         , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };


  // x-------------------------------------------------------------x
  // |            "PyOccurrenceLocator" Object Methods             |
  // x-------------------------------------------------------------x

  DirectDeleteMethod(PyOccurrenceLocator_DeAlloc,PyOccurrenceLocator)
  LocatorPyTypeObjectLinkPyType(Occurrence, Occurrence)


# else  // End of Python Module Code Part.


// x=================================================================x
// |         "PyOccurrenceLocator" Shared Library Code Part          |
// x=================================================================x


  // ---------------------------------------------------------------
  // PyOccurrenceLocator Object Definitions.

  PyTypeRootObjectDefinitions(OccurrenceLocator)

# endif  // End of Shared Library Code Part.

}  // End of extern "C".

}  // End of Isobar namespace.
 
