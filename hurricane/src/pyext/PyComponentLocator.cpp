
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




# include  "PyComponentLocator.h"
# include  "PyComponent.h"


namespace  Isobar {


USING_NAMESPACE_HURRICANE


extern "C" {


# define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Locator<Component*>,locator,function)


// x=================================================================x
// |          "PyComponentLocator" Python Module Code Part           |
// x=================================================================x

# if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |           "PyComponentLocator" Attribute Methods            |
  // x-------------------------------------------------------------x


  // Standart Accessors (Attributes).

  // Standart Predicates (Attributes).
  DirectGetBoolAttribute(PyComponentLocator_isValid,isValid,PyComponentLocator,Locator<Component*>)


  // Standart Locator Accessors (Attributes).
  LocatorProgressAttribute(Component)
  LocatorGetElementEntityAttribute(Component)
  LocatorGetCloneAttribute(Component)

  // Standart Delete (Attribute).
  DirectDestroyAttribute(PyComponentLocator_destroy, PyComponentLocator)




  // ---------------------------------------------------------------
  // PyComponent Attribute Method table.

  PyMethodDef PyComponentLocator_Methods[] =
    { { "isValid"        , (PyCFunction)PyComponentLocator_isValid    , METH_NOARGS , "Returns true while the walk has not exhausted the set of elements, else false." }
    , { "progress"       , (PyCFunction)PyComponentLocator_progress   , METH_NOARGS , "Moves forward the locator to the following element." }
    , { "getElement"     , (PyCFunction)PyComponentLocator_getElement , METH_NOARGS , "Returns the current element (or the value Type() when the locator is not or no longer valid)." }
    , { "getClone"       , (PyCFunction)PyComponentLocator_getClone   , METH_NOARGS , "This function allocates and returns a new locator that will have the same visiting course than the remaining one of the locator being cloned." }
    , { "destroy"        , (PyCFunction)PyComponentLocator_destroy    , METH_NOARGS
                         , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };


  // x-------------------------------------------------------------x
  // |             "PyComponentLocator" Object Methods             |
  // x-------------------------------------------------------------x

  DirectDeleteMethod(PyComponentLocator_DeAlloc,PyComponentLocator)
  LocatorPyTypeObjectLinkPyType(Component, Component*)


#else  // End of Python Module Code Part.


// x=================================================================x
// |         "PyComponentLocator" Shared Library Code Part           |
// x=================================================================x


  // ---------------------------------------------------------------
  // PyComponentLocator Object Definitions.
  PyTypeRootObjectDefinitions(ComponentLocator)


# endif  // End of Shared Library Code Part.


}  // End of extern "C".


}  // End of Isobar namespace.
 
