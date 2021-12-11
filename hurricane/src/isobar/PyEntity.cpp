// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU 2010-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                   Jean-Paul Chaput               |
// |  E-mail      :           Jean-Paul.Chaput@lip6.fr               |
// | =============================================================== |
// |  C++ Module  :       "./PyEntity.cpp"                           |
// +-----------------------------------------------------------------+


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
#include "hurricane/isobar/PyPad.h"
#include "hurricane/isobar/PyRectilinear.h"
#include "hurricane/isobar/PyPolygon.h"
#include "hurricane/isobar/PyPin.h"
#include "hurricane/isobar/PyRoutingPad.h"
#include "hurricane/Cell.h"

namespace Isobar {

using namespace Hurricane;


extern "C" {


// +=================================================================+
// |               "PyEntity" Python Module Code Part                |
// +=================================================================+

#if defined(__PYTHON_MODULE__)

#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Entity,entity,function)

  GetBoundStateAttribute(PyEntity_isPyBound,      PyEntity,Entity)
  DirectGetUIntAttribute(PyEntity_getId    ,getId,PyEntity,Entity)
  DBoDestroyAttribute(PyEntity_destroy ,PyEntity)


  static PyObject* PyEntity_getCell ( PyEntity* self )
  {
    cdebug_log(20,0) << "PyEntity_getCell()" << endl;
    Cell* cell = NULL;
    HTRY
      METHOD_HEAD( "Entity.getCell()" )
      cell = entity->getCell();
    HCATCH
    return PyCell_Link( cell );
  }


  static PyObject* PyEntity_getBoundingBox ( PyEntity* self )
  {
    cdebug_log(20,0) << "PyEntity_getBoundingBox()" << endl;
    METHOD_HEAD( "Entity.getBoundingBox()" )
    PyBox* boundingBox = PyObject_NEW( PyBox, &PyTypeBox );
    if (not boundingBox) return NULL;
    HTRY
      boundingBox->_object = new Box ( entity->getBoundingBox() );
    HCATCH
    return ( (PyObject*)boundingBox );
  }


  PyMethodDef PyEntity_Methods[] =
    { { "getCell"         , (PyCFunction)PyEntity_getCell       , METH_NOARGS , "Returns the entity cell." }
    , { "getId"           , (PyCFunction)PyEntity_getId         , METH_NOARGS , "Returns unique object (DBo) identifier." }
    , { "isBound"         , (PyCFunction)PyEntity_isPyBound     , METH_NOARGS , "Returns true if the Entity is bounded to it's Hurricane counterpart." }    
    , { "getBoundingBox"  , (PyCFunction)PyEntity_getBoundingBox, METH_NOARGS , "Returns entity bounding box." }    
    , { "destroy"         , (PyCFunction)PyEntity_destroy       , METH_NOARGS
                          , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}    /* sentinel */
    };


  DBoDeleteMethod(Entity)
  PyTypeObjectLinkPyType(Entity) 

#else  // End of Python Module Code Part.

// +=================================================================+
// |              "PyEntity" Shared Library Code Part                |
// +=================================================================+


  // ---------------------------------------------------------------
  // Allocator Method  :  "PyEntity_NEW ()"
  //
  // No PyEntity should ever be created, it's not a terminal object
  // of the class hierarchy. Instead create the real underlying PyObject.

  PyObject* PyEntity_NEW ( Entity* entity )
  {
    if (not entity) {
      PyErr_SetString( HurricaneError, "Invalid Entity (bad occurrence)" );
      return NULL;
    }

    Cell* cell = dynamic_cast<Cell*>( entity );
    if (cell) return PyCell_Link( cell );

    Instance* instance = dynamic_cast<Instance*>( entity );
    if (instance) return PyInstance_Link( instance );

    Reference* reference = dynamic_cast<Reference*>( entity );
    if (reference) return PyReference_Link( reference );

    RoutingPad* rp = dynamic_cast<RoutingPad*>( entity );
    if (rp) return PyRoutingPad_Link( rp );

    Horizontal* horizontal = dynamic_cast<Horizontal*>( entity );
    if (horizontal) return PyHorizontal_Link( horizontal );
    
    Vertical* vertical = dynamic_cast<Vertical*>( entity );
    if (vertical) return PyVertical_Link( vertical );

    Pad* pad = dynamic_cast<Pad*>( entity );
    if (pad) return PyPad_Link( pad );
    
    Contact* contact = dynamic_cast<Contact*>( entity );
    if (contact) return PyContact_Link( contact );
    
    Plug* plug = dynamic_cast<Plug*>( entity );
    if (plug) return PyPlug_Link( plug );
    
    Pin* pin = dynamic_cast<Pin*>( entity );
    if (pin) return PyPin_Link( pin );
    
    Polygon* polygon = dynamic_cast<Polygon*>( entity );
    if (polygon) return PyPolygon_Link( polygon );
    
    Rectilinear* rectilinear = dynamic_cast<Rectilinear*>( entity );
    if (rectilinear) return PyRectilinear_Link( rectilinear );
    
    Net* net = dynamic_cast<Net*>( entity );
    if (net) return PyNet_Link( net );
    
    Py_RETURN_NONE;
  }


  PyTypeRootObjectDefinitions(Entity)


  // ---------------------------------------------------------------
  // PyEntity Object Definitions.


#endif  // Shared Library Code Part.

}  // extern "C".


// +=================================================================+
// |              "PyEntity" Shared Library Code Part                |
// +=================================================================+


# if !defined(__PYTHON_MODULE__)

Hurricane::Entity* EntityCast ( PyObject* derivedObject ) {
  if ( IsPyCell      (derivedObject) ) return PYCELL_O(derivedObject);
  if ( IsPyInstance  (derivedObject) ) return PYINSTANCE_O(derivedObject);
  if ( IsPyReference (derivedObject) ) return PYREFERENCE_O(derivedObject);
  if ( IsPyPlug      (derivedObject) ) return PYPLUG_O(derivedObject);
  if ( IsPyHorizontal(derivedObject) ) return PYHORIZONTAL_O(derivedObject);
  if ( IsPyVertical  (derivedObject) ) return PYVERTICAL_O(derivedObject);
  if ( IsPyRoutingPad(derivedObject) ) return PYROUTINGPAD_O(derivedObject);
  if ( IsPyContact   (derivedObject) ) return PYCONTACT_O(derivedObject);
  if ( IsPyPin       (derivedObject) ) return PYPIN_O(derivedObject);
  if ( IsPyNet       (derivedObject) ) return PYNET_O(derivedObject);

  return NULL;
}

#endif

}  // Isobar namespace.
