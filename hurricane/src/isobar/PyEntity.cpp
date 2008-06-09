
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
// $Id: PyEntity.cpp,v 1.8 2007/04/26 13:38:47 d2 Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                   Jean-Paul Chaput               |
// |  E-mail      :      Jean-Paul.Chaput@asim.lip6.fr               |
// | =============================================================== |
// |  C++ Module  :       "./PyEntity.cpp"                           |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyLayer.h"
#include "hurricane/isobar/PyPoint.h"
#include "hurricane/isobar/PyBox.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PyInstance.h"
#include "hurricane/isobar/PyReference.h"
#include "hurricane/isobar/PyComponent.h"
#include "hurricane/isobar/PyPlug.h"
#include "hurricane/isobar/PyHorizontal.h"
#include "hurricane/isobar/PyVertical.h"
#include "hurricane/isobar/PyContact.h"
#include "hurricane/isobar/PyPin.h"

namespace Isobar {


using namespace Hurricane;


extern "C" {


// x=================================================================x
// |               "PyEntity" Python Module Code Part                |
// x=================================================================x

#if defined(__PYTHON_MODULE__)

#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Entity,entity,function)


  // x-------------------------------------------------------------x
  // |                "PyEntity" Attribute Methods                 |
  // x-------------------------------------------------------------x

  // Standart destroy (Attribute).
  DBoDestroyAttribute(PyEntity_destroy ,PyEntity)

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyEntity_getCell ()"

  static PyObject* PyEntity_getCell ( PyEntity *self )
  {
    trace << "PyEntity_getCell ()" << endl;

    Cell* cell = NULL;
    
    HTRY
    METHOD_HEAD ( "Entity.getCell()" )
    cell = entity->getCell ();
    HCATCH

    return PyCell_Link ( cell );
  }




  // ---------------------------------------------------------------
  // PyEntity Attribute Method table.

  PyMethodDef PyEntity_Methods[] =
    { { "getCell"              , (PyCFunction)PyEntity_getCell       , METH_NOARGS , "Returns the entity cell." }
    , { "destroy"               , (PyCFunction)PyEntity_destroy      , METH_NOARGS
                               , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |                 "PyEntity" Object Methods                   |
  // x-------------------------------------------------------------x

  DBoDeleteMethod(Entity)
  PyTypeObjectLinkPyType(Entity) 


# else  // End of Python Module Code Part.


// x=================================================================x
// |              "PyEntity" Shared Library Code Part                |
// x=================================================================x


  // ---------------------------------------------------------------
  // Allocator Method  :  "PyEntity_NEW ()"
  //
  // No PyEntity should ever be created, it's not a terminal object
  // of the class hierarchy. Instead create the real underlying PyObject.

  PyObject* PyEntity_NEW ( Entity* entity )
  {
    if ( !entity ) {
      PyErr_SetString ( HurricaneError, "Invalid Entity (bad occurrence)" );  \
      return NULL;
    }

    Cell* cell = dynamic_cast<Cell*>(entity);
    if ( cell ) return PyCell_Link ( cell );

    Instance* instance = dynamic_cast<Instance*>(entity);
    if ( instance ) return PyInstance_Link ( instance );

    Reference* reference = dynamic_cast<Reference*>(entity);
    if ( reference ) return PyReference_Link ( reference );

    Horizontal* horizontal = dynamic_cast<Horizontal*>(entity);
    if ( horizontal ) return PyHorizontal_Link ( horizontal );
    
    Vertical* vertical = dynamic_cast<Vertical*>(entity);
    if ( vertical ) return PyVertical_Link ( vertical );
    
    Contact* contact = dynamic_cast<Contact*>(entity);
    if ( contact ) return PyContact_Link ( contact );
    
    Plug* plug = dynamic_cast<Plug*>(entity);
    if ( plug ) return PyPlug_Link ( plug );
    
    Pin* pin = dynamic_cast<Pin*>(entity);
    if ( pin ) return PyPin_Link ( pin );
    
    Net* net = dynamic_cast<Net*>(entity);
    if ( net ) return PyNet_Link ( net );
    
    Py_RETURN_NONE;
  }


  PyTypeRootObjectDefinitions(Entity)

  // ---------------------------------------------------------------
  // PyEntity Object Definitions.


# endif  // End of Shared Library Code Part.


}  // End of extern "C".




# if !defined(__PYTHON_MODULE__)

Hurricane::Entity* EntityCast ( PyObject* derivedObject )
{
  if ( IsPyCell      (derivedObject) ) return PYCELL_O(derivedObject);
  if ( IsPyInstance  (derivedObject) ) return PYINSTANCE_O(derivedObject);
  if ( IsPyReference (derivedObject) ) return PYREFERENCE_O(derivedObject);
  if ( IsPyPlug      (derivedObject) ) return PYPLUG_O(derivedObject);
  if ( IsPyHorizontal(derivedObject) ) return PYHORIZONTAL_O(derivedObject);
  if ( IsPyVertical  (derivedObject) ) return PYVERTICAL_O(derivedObject);
  if ( IsPyContact   (derivedObject) ) return PYCONTACT_O(derivedObject);
  if ( IsPyPin       (derivedObject) ) return PYPIN_O(derivedObject);
  if ( IsPyNet       (derivedObject) ) return PYNET_O(derivedObject);

  return NULL;
}

# endif


}  // End of Isobar namespace.
 
