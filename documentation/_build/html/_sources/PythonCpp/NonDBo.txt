.. -*- Mode: rst -*-

.. include:: ../etc/definitions.rst


5. Case 3 - Non-DBo Standalone Classe
=======================================

Let's have a look at the encapsulation of ``Hurricane::Point``.

Non-BDo derived classes do not support the bi-directionnal communication.
So each Python object is associated with one C++ object. The C++ object
is created and deleted along with the Python one. This behavior implies
that the C++ object is *copy constructible* (which should be the case).


5.1 Class Header
~~~~~~~~~~~~~~~~~~

Changes from :ref:`3.1 Class Associated Header File`:

* There is no ``PyPoint_Link()`` function, as it's related to the
  bi-directional communication mechanism.

.. note::
   **About the _object attribute** of the PyPoint. As the C++ object life span
   (``Point``) is linked to the Python (``PyPoint``) one, we may have used a
   value instead of a pointer. It is best to keep a pointer as the macros
   written for ``DBo`` derived classes will remain usables. 


.. code-block:: c++

   #ifndef ISOBAR_PY_POINT_H
   #define ISOBAR_PY_POINT_H
   
   #include "hurricane/isobar/PyHurricane.h"
   #include "hurricane/Point.h"
   
   namespace  Isobar {
     extern "C" {
   
       typedef struct {
           PyObject_HEAD
           Hurricane::Point* _object;
       } PyPoint;
   
       extern  PyTypeObject  PyTypePoint;
       extern  PyMethodDef   PyPoint_Methods[];
       extern  void          PyPoint_LinkPyType();
   
   #define IsPyPoint(v)    ( (v)->ob_type == &PyTypePoint )
   #define PYPOINT(v)      ( (PyPoint*)(v) )
   #define PYPOINT_O(v)    ( PYPOINT(v)->_object )
   
     }  // extern "C".
   }  // Isobar namespace.
   
   #endif  // ISOBAR_PY_POINT_H

|newpage|


5.2 Class File
~~~~~~~~~~~~~~~~

Changes from :ref:`3.2 Class Associated File`:

* As there is no ``PyPoint_Link()`` function, there is no call to any
  flavor of the ``DBoLinkcreatemethod()`` macro (obvious as it's *not*
  a ``DBo``).

* To use the standard Python constructor, we have to define ``PyPoint_NEW()``
  and ``PyPoint_Init()`` functions, I'm not absolutely certain that the later
  needs to be defined (that part is still not clear to me from the Python doc).

* As it's not a ``DBo`` there is no ``destroy()`` method, so no call to
  ``DirectDestroyMethod()``

* Lastly, as this object has a ``PyPoint_NEW()`` (field ``tp_new``) and
  a ``PyPoint_Init()`` (field ``tp_init``) we have to use the macro
  ``PyTypeObjectLinkPyTypeNewInit()`` to define ``PyPoint_LinkPyType()``.


.. code-block:: c++

   #include "hurricane/isobar/PyPoint.h"
   
   namespace Isobar {
     using namespace Hurricane;
   
     extern "C" {
   
   #define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Point,point,function)
   
   #if defined(__PYTHON_MODULE__)
   
       static PyObject* PyPoint_NEW ( PyObject* module, PyObject *args )
       {
         Point* point = NULL;
         HTRY
           PyObject* arg0 = NULL;
           PyObject* arg1 = NULL;
           
           __cs.init( "Point.Point" );
           if (not PyArg_ParseTuple( args,"|O&O&:Point.Point"
                                   , Converter,&arg0
                                   , Converter,&arg1 )) {
             PyErr_SetString ( ConstructorError
                             , "invalid number of parameters for Point constructor." );
             return NULL;
           }
           
           if      (__cs.getObjectIds() == "")
                   { point = new Point()); }
           else if (__cs.getObjectIds() == ":point")
                   { point = new Point( *PYPOINT_O(arg0) ); }
           else if (__cs.getObjectIds() == ":int:int")
                   { point = new Point( PyAny_AsLong(arg0), PyAny_AsLong(arg1) ); }
           else {
             PyErr_SetString ( ConstructorError
                             , "invalid number of parameters for Point constructor." );
             return NULL;
           }
           
           PyPoint* pyPoint = PyObject_NEW( PyPoint, &PyTypePoint );
           if (pyPoint == NULL) { delete point; return NULL; }
           pyPoint->_object = point;
         HCATCH
       
         return (PyObject*)pyPoint;
       }
       
       static int  PyPoint_Init ( PyPoint* self, PyObject* args, PyObject* kwargs )
       { return 0; }
       
       DirectGetLongAttribute(PyPoint_getX,getX,PyPoint,Point)
       DirectGetLongAttribute(PyPoint_getY,getY,PyPoint,Point)
       DirectSetLongAttribute(PyPoint_SetX,setX,PyPoint,Point)
       DirectSetLongAttribute(PyPoint_SetY,setY,PyPoint,Point)
       
       PyMethodDef PyPoint_Methods[] =
         { { "getX"   , (PyCFunction)PyPoint_getX     , METH_NOARGS
                      , "Return the Point X value." }
         , { "getY"   , (PyCFunction)PyPoint_getY     , METH_NOARGS 
                      , "Return the Point Y value." }
         , { "setX"   , (PyCFunction)PyPoint_SetX     , METH_VARARGS
                      , "Modify the Point X value." }
         , { "setY"   , (PyCFunction)PyPoint_SetY     , METH_VARARGS
                      , "Modify the Point Y value." }
         , {NULL, NULL, 0, NULL}  /* sentinel */
         };
       
       DirectDeleteMethod(PyPoint_DeAlloc,PyPoint)
       PyTypeObjectLinkPyTypeNewInit(Point)
   
   #else  // Python Module Code Part.
   
       PyTypeObjectDefinitions(Point)
   
   #endif  // Shared Library Code Part.
   
     }  // extern "C".
   }  // Isobar namespace.


5.2 Class File
~~~~~~~~~~~~~~~~

To put it bluntly, there is no difference in the Python module for
a standalone ``DBo`` class and a non-``DBo`` class.
