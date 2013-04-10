// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                   Jean-Paul Chaput               |
// |  E-mail      :      Jean-Paul.Chaput@asim.lip6.fr               |
// | =============================================================== |
// |  C++ Module  :       "./PyContact.cpp"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyLayer.h"
#include "hurricane/isobar/PyContact.h"
#include "hurricane/isobar/PyHorizontal.h"
#include "hurricane/isobar/PyVertical.h"


namespace  Isobar {


using namespace Hurricane;


extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject) &(_pyObject->_baseObject._baseObject)
#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Contact,contact,function)


// x=================================================================x
// |              "PyContact" Python Module Code Part                |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |               "PyContact" Attribute Methods                 |
  // x-------------------------------------------------------------x


  // Standart Accessors (Attributes).
  DirectGetLongAttribute(PyContact_getWidth     , getWidth     , PyContact,Contact)
  DirectGetLongAttribute(PyContact_getHalfWidth , getHalfWidth , PyContact,Contact)
  DirectGetLongAttribute(PyContact_getHeight    , getHeight    , PyContact,Contact)
  DirectGetLongAttribute(PyContact_getHalfHeight, getHalfHeight, PyContact,Contact)

  DirectGetLongAttribute(PyContact_getDx, getDx, PyContact,Contact)
  DirectGetLongAttribute(PyContact_getDy, getDy, PyContact,Contact)

  DirectSetLongAttribute(PyContact_setX ,setX ,"Contact.setX" ,PyContact,Contact)
  DirectSetLongAttribute(PyContact_setY ,setY ,"Contact.setY" ,PyContact,Contact)
  DirectSetLongAttribute(PyContact_setDx,setDx,"Contact.setDx",PyContact,Contact)
  DirectSetLongAttribute(PyContact_setDy,setDy,"Contact.setDy",PyContact,Contact)

  
  // Standart destroy (Attribute).
  DBoDestroyAttribute(PyContact_destroy, PyContact)


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyContact_translate ()"

  static PyObject* PyContact_translate ( PyContact *self, PyObject* args ) {
    trace << "PyContact_translate ()" << endl;
    
    HTRY
    METHOD_HEAD ( "Contact.translate()" )
    DbU::Unit dx=0, dy=0;
    if (PyArg_ParseTuple(args,"ll:Contact.translate", &dx, &dy)) {
        contact->translate(dx, dy);
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Contact.translate()" );
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }


  // ---------------------------------------------------------------
  // PyContact Attribute Method table.

  PyMethodDef PyContact_Methods[] =
    { { "destroy"         , (PyCFunction)PyContact_destroy         , METH_NOARGS
                         , "Destroy associated hurricane object, the python object remains." }
    , { "getWidth"       , (PyCFunction)PyContact_getWidth       , METH_NOARGS , "Return the contact width." }
    , { "getHalfWidth"   , (PyCFunction)PyContact_getHalfWidth   , METH_NOARGS , "Return the contact half width." }
    , { "getHeight"      , (PyCFunction)PyContact_getHeight      , METH_NOARGS , "Return the contact height." }
    , { "getHalfHeight"  , (PyCFunction)PyContact_getHalfHeight  , METH_NOARGS , "Return the contact half height." }
    , { "getDx"          , (PyCFunction)PyContact_getDx          , METH_NOARGS , "Return the contact dx value." }
    , { "getDy"          , (PyCFunction)PyContact_getDy          , METH_NOARGS , "Return the contact dy value." }
    , { "translate"      , (PyCFunction)PyContact_translate      , METH_VARARGS, "Translates the Contact of dx and dy." }
    , { "setX"           , (PyCFunction)PyContact_setX           , METH_VARARGS, "Sets the contact X value." }
    , { "setY"           , (PyCFunction)PyContact_setY           , METH_VARARGS, "Sets the contact Y value." }
    , { "setDx"          , (PyCFunction)PyContact_setDx          , METH_VARARGS, "Sets the contact dx value." }
    , { "setDy"          , (PyCFunction)PyContact_setDy          , METH_VARARGS, "Sets the contact dy value." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |                "PyContact" Object Methods                   |
  // x-------------------------------------------------------------x
  


  DBoDeleteMethod(Contact)
  PyTypeObjectLinkPyType(Contact)


#else  // End of Python Module Code Part.


// x=================================================================x
// |             "PyContact" Shared Library Code Part                |
// x=================================================================x
  
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyContact_create ()"

  PyObject* PyContact_create ( PyObject *module, PyObject *args ) {
    trace << "PyContact_create()" << endl;

    Contact* contact = NULL;

    HTRY
    PyNet* pyNet = NULL;
    PyLayer* pyLayer = NULL;
    PyComponent* pyComponent = NULL;
    DbU::Unit x=0, y=0, width=0, height=0;
   if (PyArg_ParseTuple(args, "O!O!ll|ll:Contact.create",
                &PyTypeNet, &pyNet, &PyTypeLayer, &pyLayer,
                &x, &y, &width, &height)) {
        contact = Contact::create(PYNET_O(pyNet), PYLAYER_O(pyLayer), x, y, width, height);
    } else {
      PyErr_Clear ();
      if (PyArg_ParseTuple(args, "O!O!ll|ll:Contact.create",
                &PyTypeComponent, &pyComponent, &PyTypeLayer, &pyLayer,
                &x, &y, &width, &height)) {
        contact = Contact::create(PYCOMPONENT_O(pyComponent), PYLAYER_O(pyLayer), x, y, width, height);
      } else {
        PyErr_SetString ( ConstructorError, "invalid number of parameters for Contact constructor." );
        return NULL;
      }
    }


    HCATCH

    return PyContact_Link(contact);
  }




  // Link/Creation Method.
  DBoLinkCreateMethod(Contact)



  // ---------------------------------------------------------------
  // PyContact Object Definitions.

  PyTypeInheritedObjectDefinitions(Contact, Component)

#endif  // End of Shared Library Code Part.


}  // End of extern "C".


}  // End of Isobar namespace.
