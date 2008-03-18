
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
// $Id: PyHyperNet.cpp,v 1.2 2007/05/10 11:15:56 d2 Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                      Damien DUPUIS               |
// |  E-mail      :              Damien.Dupuis@lip6.fr               |
// | =============================================================== |
// |  C++ Module  :       "./PyHyperNet.cpp"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




# include  "PyHyperNet.h"
# include  "PyCell.h"
# include  "PyOccurrence.h"
# include  "PyOccurrenceLocator.h" 

USING_NAMESPACE_HURRICANE


namespace  Isobar {


USING_NAMESPACE_HURRICANE


extern "C" {


# define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(HyperNet,hyperNet,function)


// x=================================================================x
// |                 "PyHyperNet" Python Module Code Part                 |
// x=================================================================x

# if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                "PyHyperNet" Attribute Methods                    |
  // x-------------------------------------------------------------x



  
  // Standart Predicates (Attributes).
  DirectGetBoolAttribute(PyHyperNet_IsValid  ,IsValid  ,PyHyperNet,HyperNet)


  // Standart Delete (Attribute).
  DirectDestroyAttribute(PyHyperNet_destroy, PyHyperNet)




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyHyperNet_getPlugsLocator ()"

  static PyObject* PyHyperNet_getLeafPlugOccurrenceLocator ( PyHyperNet *self )
  {
    trace << "PyHyperNet_getLeafPlugOccurrenceLocator ()" << endl;

    METHOD_HEAD ( "HyperNet.getLeafPlugOccurrenceLocator()" )

    Occurrences occurrences = hyperNet->getLeafPlugOccurrences ();

    PyOccurrenceLocator* pyOccurrenceLocator = PyObject_NEW ( PyOccurrenceLocator, &PyTypeOccurrenceLocator );
    if (pyOccurrenceLocator == NULL) { return NULL; }
  
    trace_in ();
    trace << "new PyOccurrenceLocator [" << hex << pyOccurrenceLocator << "]" << endl;
    trace_out ();

    HTRY

    pyOccurrenceLocator->_object = occurrences.getLocator ();

    HCATCH

    return ( (PyObject*)pyOccurrenceLocator );
  }

    
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyHyperNet_getCell ()"

  static PyObject* PyHyperNet_getCell ( PyHyperNet *self )
  {
    trace << "PyHyperNet_getCell ()" << endl;

    Cell* cell = NULL;
    
    HTRY
    METHOD_HEAD ( "HyperNet.getCell()" )
    cell = hyperNet->getCell ();
    HCATCH

    return PyCell_Link ( cell );
  }





  // ---------------------------------------------------------------
  // PyHyperNet Attribute Method table.

  PyMethodDef PyHyperNet_Methods[] =
    { { "getCell"                     , (PyCFunction)PyHyperNet_getCell                     , METH_NOARGS , "Returns the hyperNet cell." }
    , { "IsValid"                     , (PyCFunction)PyHyperNet_IsValid                     , METH_NOARGS , "Returns trus if the HyperNet isValid." }
    , { "getLeafPlugOccurrenceLocator", (PyCFunction)PyHyperNet_getLeafPlugOccurrenceLocator, METH_NOARGS 
                                      , "Returns the collection of leaf occurrences" }
    , { "destroy"                     , (PyCFunction)PyHyperNet_destroy                     , METH_NOARGS
                                      , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |                  "PyHyperNet" Object Methods                     |
  // x-------------------------------------------------------------x
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyHyperNet_new ()"

  static PyObject* PyHyperNet_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    trace << "PyHyperNet_new()" << endl;

    HyperNet*      hyperNet = NULL;
    PyObject* arg0;
    
    if ( ! ParseOneArg ( "HyperNet.new()", args, ":occur", &arg0 ) ) return ( NULL );

    hyperNet =  new HyperNet ( *PYOCCURRENCE_O(arg0) );

    PyHyperNet* pyHyperNet;
    pyHyperNet = PyObject_NEW(PyHyperNet, &PyTypeHyperNet);
    if (pyHyperNet == NULL) return NULL;

    HTRY
    pyHyperNet->_object = hyperNet;
    HCATCH

    return ( (PyObject*)pyHyperNet );
  }

  DirectDeleteMethod(PyHyperNet_DeAlloc,PyHyperNet)
  PyTypeObjectLinkPyType(HyperNet)
  PyTypeObjectConstructor(HyperNet)


# else  // End of Python Module Code Part.


// x=================================================================x
// |                "PyHyperNet" Shared Library Code Part                 |
// x=================================================================x





  // ---------------------------------------------------------------
  // PyHyperNet Object Definitions.

  PyTypeRootObjectDefinitions(HyperNet)


# endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
