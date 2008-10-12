
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
// $Id: PyPlugLocator.cpp,v 1.12 2006/09/22 11:27:32 tsunami Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Sophie BELLOEIL               |
// |  E-mail      :       Sophie.Belloeil@asim.lip6.fr               |
// | =============================================================== |
// |  C++ Module  :       "./PyPlugLocator.cpp"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include "hurricane/isobar/PyPlugLocator.h"
#include "hurricane/isobar/PyPlug.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#undef   ACCESS_OBJECT
#define  ACCESS_OBJECT           _object
#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Locator<Plug*>,locator,function)


// x=================================================================x
// |            "PyPlugLocator" Python Module Code Part              |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                "PyPlugLocator" Attribute Methods            |
  // x-------------------------------------------------------------x


  // Standart Accessors (Attributes).


  // Standart Predicates (Attributes).
  DirectGetBoolAttribute(PyPlugLocator_isValid,isValid,PyPlugLocator,Locator<Plug*>)


  // Standart Locator Accessors (Attributes).
  LocatorProgressAttribute(Plug)
  LocatorGetElementAttribute(Plug)
  LocatorGetCloneAttribute(Plug)




  // ---------------------------------------------------------------
  // PyPlugLocator Attribute Method table.

  PyMethodDef PyPlugLocator_Methods[] =
    { { "isValid"        , (PyCFunction)PyPlugLocator_isValid    , METH_NOARGS , "Returns true while the walk has not exhausted the set of elements, else false." }
    , { "progress"       , (PyCFunction)PyPlugLocator_progress   , METH_NOARGS , "Moves forward the locator to the following element." }
    , { "getElement"     , (PyCFunction)PyPlugLocator_getElement , METH_NOARGS , "Returns the current element (or the value Type() when the locator is not or no longer valid)." }
    , { "getClone"       , (PyCFunction)PyPlugLocator_getClone   , METH_NOARGS , "This function allocates and returns a new locator that will have the same visiting course than the remaining one of the locator being cloned." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };


  // x-------------------------------------------------------------x
  // |                  "PyPlugLocator" Object Methods             |
  // x-------------------------------------------------------------x


  DirectDeleteMethod(PyPlugLocator_DeAlloc,PyPlugLocator)
  LocatorPyTypeObjectLinkPyType(Plug, Plug*)




#else  // End of Python Module Code Part.


// x=================================================================x
// |               "PyCell" Shared Library Code Part                 |
// x=================================================================x


  // ---------------------------------------------------------------
  // PyPlugLocator Object Definitions.
  PyTypeObjectDefinitions(PlugLocator)


#endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
 

