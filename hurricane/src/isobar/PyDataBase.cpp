
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
// $Id: PyDataBase.cpp,v 1.1 2008/02/07 19:09:57 xtof Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyDataBase.cpp"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include "hurricane/isobar/PyDataBase.h"
#include "hurricane/isobar/PyTechnology.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(DataBase,db,function)


// x=================================================================x
// |             "PyDataBase" Python Module Code Part               |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


// x-------------------------------------------------------------x
// |             "PyDataBase" Attribute Methods                 |
// x-------------------------------------------------------------x




// ---------------------------------------------------------------
// Attribute Method  :  "PyDataBase_getDataBase ()"

  PyObject* PyDataBase_getDataBase ( PyObject* module ) {
    trace << "PyDataBase_getDataBase()" << endl;

    DataBase* db = NULL;

    HTRY
    db = DataBase::getDB ();
    if ( db == NULL )
      PyErr_SetString ( HurricaneError, "DataBase has not been created yet" );
    HCATCH

    return PyDataBase_Link ( db );
  }

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyDataBase_getTechnology ()"

  PyObject* PyDataBase_getTechnology ( PyDataBase* self ) {
    trace << "PyDataBase_getTechnology()" << endl;

    Technology* techno = NULL;

    HTRY
    METHOD_HEAD("DataBase.getTechnology()")

    techno = db->getTechnology ();
    
    if ( techno == NULL )
        Py_RETURN_NONE;
    HCATCH

    return PyTechnology_Link ( techno );
  }

  
  // Standart Accessors (Attributes).


  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyDataBase_destroy,PyDataBase)




  // ---------------------------------------------------------------
  // PyDataBase Attribute Method table.

  PyMethodDef PyDataBase_Methods[] =
    { { "getTechnology", (PyCFunction)PyDataBase_getTechnology, METH_NOARGS, "Return the Technology" }
    , { "destroy"      , (PyCFunction)PyDataBase_destroy      , METH_NOARGS
                       , "Destroy associated hurricane object The python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |               "PyDataBase" Object Methods                  |
  // x-------------------------------------------------------------x

  DBoDeleteMethod(DataBase)
  PyTypeObjectLinkPyType(DataBase)

#else  // End of Python Module Code Part.


// x=================================================================x
// |               "PyDataBase" Shared Library Code Part             |
// x=================================================================x

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyDataBase_create ()"

  PyObject* PyDataBase_create ( PyObject *module ) {
    trace << "PyDataBase_create()" << endl;

    DataBase* db = NULL;
    
    HTRY
    db = DataBase::create ();
    HCATCH

    return PyDataBase_Link(db);
  }



  // Link/Creation Method.

  DBoLinkCreateMethod(DataBase)

  // ---------------------------------------------------------------
  // PyDataBase Object Definitions.
      
  PyTypeObjectDefinitions(DataBase)

#endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
 
