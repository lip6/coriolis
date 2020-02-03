.. -*- Mode: rst -*-


|newpage|


4. Case 2 - Hierarchy of DBo Derived Classes
==============================================

Now we want to export the following C++ class hierarchy into Python: ::

    PyEntity <-- PyComponent <-+- PyContact
                               +- PySegment <-+- PyHorizontal
                                              +- PyVertical


4.1 Base Class Header
~~~~~~~~~~~~~~~~~~~~~~~

**Remark:** this is only a partial description of the tree for the sake of
clarity.

One important fact to remember is that ``PyEntity`` and ``PyComponent`` 
being related to C++ abstract classes, no objects of those types will be
created, only ``PyContact``, ``PyHorizontal`` or ``PyVertical`` will.

The consequence is that there is no ``PyEntity_Link()`` like in `3.1`
but instead two functions:

#. ``PyEntity_NEW()`` which create the relevant ``PyEntity`` *derived*
   object from the ``Entity`` one. For example, if the ``Entity*`` given
   as argument is in fact a ``Horizontal*``, then the function will
   return a ``PyHorizontal*``.

#. ``EntityCast()`` do the reverse of ``PyEntity_NEW()`` that is, from
   a ``PyEntity``, return the C++ *derived* object. Again, if the
   ``PyEntity*`` is a ``PyHorizontal*``, the function will cast it as
   a ``Horizontal*`` *then* return it as an ``Entity*``.

.. code-block:: python

   #ifndef ISOBAR_PY_ENTITY_H
   #define ISOBAR_PY_ENTITY_H

   #include "hurricane/isobar/PyHurricane.h"
   #include "hurricane/Entity.h"
   
   namespace  Isobar {
     extern "C" {
   
       typedef struct {
           PyObject_HEAD
           Hurricane::Entity* _object;
       } PyEntity;
   
       extern  PyObject*     PyEntity_NEW        ( Hurricane::Entity* entity );
       extern  void          PyEntity_LinkPyType ();
       extern  PyTypeObject  PyTypeEntity;
       extern  PyMethodDef   PyEntity_Methods[];
   
   
   #define IsPyEntity(v)    ( (v)->ob_type == &PyTypeEntity )
   #define PYENTITY(v)      ( (PyEntity*)(v) )
   #define PYENTITY_O(v)    ( PYENTITY(v)->_object )
   
     }  // extern "C".
   
     Hurricane::Entity*  EntityCast ( PyObject* derivedObject );
   
   }  // Isobar namespace.

   #endif  // ISOBAR_PY_ENTITY_H

|newpage|


4.2 Base Class File
~~~~~~~~~~~~~~~~~~~~~

Changes from `3.2 Class Associated File` are:

#. No call to ``DBoLinkCreateMethod()`` because there must be no ``PyEntity_Link()``,
   but the definitions of ``PyEntity_NEW()`` and ``EntityCast``.

#. For defining the ``PyTypeEntity`` Python type, we call a different
   macro: ``PyTypeRootObjectDefinitions``, dedicated to base classes.


.. code-block:: c++

   #include "hurricane/isobar/PyCell.h"
   #include "hurricane/isobar/PyHorizontal.h"
   #include "hurricane/isobar/PyVertical.h"
   #include "hurricane/isobar/PyContact.h"
   
   namespace Isobar {
     using namespace Hurricane;
   
     extern "C" {
   
   #if defined(__PYTHON_MODULE__)
   
   #define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Entity,entity,function)
   
       DBoDestroyAttribute(PyEntity_destroy ,PyEntity)
   
       static PyObject* PyEntity_getCell ( PyEntity *self )
       {
         Cell* cell = NULL;
         HTRY
           METHOD_HEAD( "Entity.getCell()" )
           cell = entity->getCell();
         HCATCH
         return PyCell_Link( cell );
       }
   
       PyMethodDef PyEntity_Methods[] =
         { { "getCell", (PyCFunction)PyEntity_getCell, METH_NOARGS
                      , "Returns the entity cell." }
         , { "destroy", (PyCFunction)PyEntity_destroy, METH_NOARGS
                      , "Destroy associated hurricane object, the python object remains." }
         , {NULL, NULL, 0, NULL}  /* sentinel */
         };
   
   
       DBoDeleteMethod(Entity)
       PyTypeObjectLinkPyType(Entity) 
   
   #else  // End of Python Module Code Part.
   
       PyObject* PyEntity_NEW ( Entity* entity )
       {
         if (not entity) {
           PyErr_SetString ( HurricaneError, "Invalid Entity (bad occurrence)" );
           return NULL;
         }

         Horizontal* horizontal = dynamic_cast<Horizontal*>(entity);
         if (horizontal) return PyHorizontal_Link( horizontal );
         
         Vertical* vertical = dynamic_cast<Vertical*>(entity);
         if (vertical) return PyVertical_Link( vertical );
         
         Contact* contact = dynamic_cast<Contact*>(entity);
         if (contact) return PyContact_Link( contact );
         
         Py_RETURN_NONE;
       }
       
       PyTypeRootObjectDefinitions(Entity)
   
   #endif  // Shared Library Code Part (1).

   }  // extern "C".
   
   
   #if !defined(__PYTHON_MODULE__)
   
   Hurricane::Entity* EntityCast ( PyObject* derivedObject ) {
     if (IsPyHorizontal(derivedObject)) return PYHORIZONTAL_O(derivedObject);
     if (IsPyVertical  (derivedObject)) return PYVERTICAL_O(derivedObject);
     if (IsPyContact   (derivedObject)) return PYCONTACT_O(derivedObject);
     return NULL;
   }
   
   #endif  // Shared Library Code Part (2).
   
   }  // Isobar namespace.

|newpage|


4.3 Intermediate Class Header
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Changes from `3.1 Class Associated Header File` are:

#. As for ``PyEntity``, and because this is still an abstract class,
   there is no ``PyComponent_Link()`` function.

#. The definition of the ``PyComponent`` |struct| is differs. There is
   no ``PyObject_HEAD`` (it is a Python *derived* class). The only
   field is of the base class type ``PyEntity`` and for use with
   Coriolis macros, **it must** be named ``_baseObject`` (note that
   this is *not* a pointer but a whole object).

.. code-block:: c++

   #ifndef ISOBAR_PY_COMPONENT_H
   #define ISOBAR_PY_COMPONENT_H
   
   #include "hurricane/isobar/PyEntity.h"
   #include "hurricane/Component.h"
   
   namespace  Isobar {
     extern "C" {
   
       typedef struct {
           PyEntity  _baseObject;
       } PyComponent;
       
       extern  PyTypeObject  PyTypeComponent;
       extern  PyMethodDef   PyComponent_Methods[];
       extern  void          PyComponent_LinkPyType ();
   
   #define IsPyComponent(v) ((v)->ob_type == &PyTypeComponent)
   #define PYCOMPONENT(v)   ((PyComponent*)(v))
   #define PYCOMPONENT_O(v) (static_cast<Component*>(PYCOMPONENT(v)->_baseObject._object))
   
     }  // extern "C".
   }  // Isobar namespace.
   
   #endif


4.4 Intermediate Class File
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Changes from `3.2 Class Associated File` are:

1. Redefinition of the default macros ``ACCESS_OBJECT`` and ``ACCESS_CLASS``. 

   * The pointer to the C++ encapsulated object (attribute ``_object``) is hold
     by the base class ``PyEntity``. The ``ACCESS_OBJECT`` macro which is tasked
     to give access to that attribute is then ``_baseObject._object`` as
     ``PyComponent`` is a direct derived class of ``PyEntity``.

   * ``ACCESS_CLASS`` is similar to ``ACCESS_OBJECT`` for accessing the base
     class, that is a pointer to ``PyEntity``.

|newpage|

2. For defining the ``PyTypeComponent`` Python type, we call a yet different
   macro: ``PyTypeInheritedObjectDefinitions()``, dedicated to derived classes.
   For this this macro we need to give as argument the derived class and the
   base class.

.. code-block:: c++

   #include "hurricane/isobar/PyComponent.h"
   #include "hurricane/isobar/PyNet.h"
   
   namespace  Isobar {
     using namespace Hurricane;
   
     extern "C" {
   
   #undef   ACCESS_OBJECT
   #undef   ACCESS_CLASS
   #define  ACCESS_OBJECT           _baseObject._object
   #define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
   #define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Component,component,function)
   
   #if defined(__PYTHON_MODULE__)
   
       DirectGetLongAttribute(PyComponent_getX,getX,PyComponent,Component)
       DirectGetLongAttribute(PyComponent_getY,getY,PyComponent,Component)
       DBoDestroyAttribute(PyComponent_destroy,PyComponent)
     
       static PyObject* PyComponent_getNet ( PyComponent *self )
       {
         Net* net = NULL;
         HTRY
           METHOD_HEAD( "Component.getNet()" )
           net = component->getNet( );
         HCATCH
         return PyNet_Link( net );
       }
     
       PyMethodDef PyComponent_Methods[] =
         { { "getX"   , (PyCFunction)PyComponent_getX   , METH_NOARGS
                      , "Return the Component X value." }
         , { "getY"   , (PyCFunction)PyComponent_getY   , METH_NOARGS
                      , "Return the Component Y value." }
         , { "getNet" , (PyCFunction)PyComponent_getNet , METH_NOARGS
                      , "Returns the net owning the component." }
         , { "destroy", (PyCFunction)PyComponent_destroy, METH_NOARGS
                      , "destroy associated hurricane object, the python object remains." }
         , {NULL, NULL, 0, NULL}    /* sentinel */
         };
     
       DBoDeleteMethod(Component)
       PyTypeObjectLinkPyType(Component)
   
   #else  // Python Module Code Part.
   
       PyTypeInheritedObjectDefinitions(Component, Entity)
   
   #endif  // Shared Library Code Part.
   
     }  // extern "C".
   }  // Isobar namespace.


4.5 Terminal Class Header
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The contents of this file is almost identical to `4.3 Intermediate Class Header`_,
save for the presence of a ``PyContact_Link()`` function. She is present
at this level because the class is a concrete one and can be instanciated.

.. code-block:: c++

   #ifndef ISOBAR_PY_CONTACT_H
   #define ISOBAR_PY_CONTACT_H
   
   #include "hurricane/isobar/PyComponent.h"
   #include "hurricane/Contact.h"
   
   namespace  Isobar {
     extern "C" {
   
       typedef struct {
         PyComponent _baseObject;
       } PyContact;
   
       extern PyTypeObject  PyTypeContact;
       extern PyMethodDef   PyContact_Methods[];
       extern PyObject*     PyContact_Link       ( Hurricane::Contact* object );
       extern void          PyContact_LinkPyType ();
   
   #define IsPyContact(v)    ( (v)->ob_type == &PyTypeContact )
   #define PYCONTACT(v)      ( (PyContact*)(v) )
   #define PYCONTACT_O(v)    ( PYCONTACT(v)->_baseObject._baseObject._object )
   
     }  // extern "C".
   }  // Isobar namespace.
   
   #endif  // ISOBAR_PY_CONTACT_H


4.6 Terminal Class File
~~~~~~~~~~~~~~~~~~~~~~~~~

Changes from `4.4 Intermediate Class File`_ are:

#. As previously, we have to redefine the macros ``ACCESS_OBJECT`` and ``ACCESS_CLASS``. 
   But, as we are one level deeper into the hierarchy, one more level of
   indirection using ``_baseObject`` must be used.

   * ``ACCESS_OBJECT`` becomes ``_baseObject._baseObject._object``.

   * ``ACCESS_CLASS`` becomes ``&(_pyObject->_baseObject._baseObject)``.

#. For defining the ``PyTypeContact`` Python type, we call again
   ``PyTypeInheritedObjectDefinitions()``. It is the same whether the class is
   terminal or not.

#. And, this time, as the Python class is concrete, we call the macro
   ``DBoLinkCreateMethod()`` to create the ``PyContact_Link()`` function.


.. code-block:: c++

   #include "hurricane/isobar/PyContact.h"
   
   namespace  Isobar {
     using namespace Hurricane;
   
     extern "C" {
   
   #undef  ACCESS_OBJECT
   #undef  ACCESS_CLASS
   #define ACCESS_OBJECT           _baseObject._baseObject._object
   #define ACCESS_CLASS(_pyObject) &(_pyObject->_baseObject._baseObject)
   #define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Contact,contact,function)
   
   #if defined(__PYTHON_MODULE__)
   
       DirectGetLongAttribute(PyContact_getWidth , getWidth , PyContact,Contact)
       DirectGetLongAttribute(PyContact_getHeight, getHeight, PyContact,Contact)
       DBoDestroyAttribute(PyContact_destroy, PyContact)
     
       static PyObject* PyContact_create ( PyObject*, PyObject *args )
       {
         Contact* contact = NULL;
         HTRY
           // Usual signature then arguments parsing.
         HCATCH
         return PyContact_Link(contact);
       }
     
       PyMethodDef PyContact_Methods[] =
         { { "create"   , (PyCFunction)PyContact_create   , METH_VARARGS|METH_STATIC
                        , "Create a new Contact." }
         , { "destroy"  , (PyCFunction)PyContact_destroy  , METH_NOARGS
                        , "Destroy associated hurricane object, the python object remains." }
         , { "getWidth" , (PyCFunction)PyContact_getWidth , METH_NOARGS
                        , "Return the contact width." }
         , { "getHeight", (PyCFunction)PyContact_getHeight, METH_NOARGS
                        , "Return the contact height." }
         , {NULL, NULL, 0, NULL}  /* sentinel */
         };
     
       DBoDeleteMethod(Contact)
       PyTypeObjectLinkPyType(Contact)
   
   #else  // Python Module Code Part.
   
       DBoLinkCreateMethod(Contact)
       PyTypeInheritedObjectDefinitions(Contact, Component)
   
   #endif  // Shared Library Code Part.
   
     }  // extern "C".
   }  // Isobar namespace.


4.8 Python Module
~~~~~~~~~~~~~~~~~~~

.. code-block:: c++

   DL_EXPORT(void) initHurricane ()
   {
     PyEntity_LinkPyType();  // step 1.
     PyComponent_LinkPyType();
     PyContact_LinkPyType();
 
     PYTYPE_READY( Entity )  // step 2.
     PYTYPE_READY_SUB( Component, Entity )
     PYTYPE_READY_SUB( Contact  , Component )
 
     __cs.addType( "ent"    , &PyTypeEntity   , "<Entity>"   , false ); // step 3.
     __cs.addType( "comp"   , &PyTypeComponent, "<Component>", false, "ent"  );
     __cs.addType( "contact", &PyTypeContact  , "<Contact>"  , false, "comp" );
 
     PyObject* module = Py_InitModule( "Hurricane", PyHurricane_Methods );
     if (module == NULL) {
       cerr << "[ERROR]\n"
            << "  Failed to initialize Hurricane module." << endl;
       return;
     }
 
     Py_INCREF( &PyTypeContact );                                        // step 4.
     PyModule_AddObject( module, "Contact", (PyObject*)&PyTypeContact ); // step 4.
   }
