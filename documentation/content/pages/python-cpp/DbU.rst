.. -*- Mode: rst -*-


6. Encapsulating DbU
======================

While ``Hurricane::DbU`` is a class, the ``Hurricane::DbU::Unit`` is only
a ``typedef`` over ``uint64_t``. The ``DbU`` class only provides a set of
static methods to manipulate and convert to and from other units.
At Python level, ``DbU::Unit`` will be stored in plain ``long long``.

When a ``DbU::Unit`` argument is expected in a Python functions, just use
the ``DbU::Unit  PyAny_AsLong( PyObject* )`` function to convert it.

For example, if we explicit the expension of:

.. code-block:: c++

   DirectSetLongAttribute(PyPoint_SetX,setX,PyPoint,Point)

|newpage|

We would get:

.. code-block:: c++

   static PyObject* PyPoint_setX ( PyPoint *self, PyObject* args )
   {
     Point* cobject = static_cast<Point*>( self->_object );
     if (cobject == NULL) {
       PyErr_SetString( ProxyError
                      , "Attempt to call Point.setX() on an unbound Hurricane object" );
       return NULL;
     }
 
     HTRY
       PyObject* arg0 = NULL;
       if (not PyArg_ParseTuple( args, "O:Point.setX()", &arg0 ))
         return ( NULL );
       cobject->setX( Isobar::PyAny_AsLong(arg0) );
     HCATCH
     Py_RETURN_NONE;
   }


For the other way around, use ``PyObject* PyDbU_FromLong( DbU::Unit )``.

.. code-block:: c++

   DirectGetLongAttribute(PyPoint_GetX,getX,PyPoint,Point)

We would get:

.. code-block:: c++

   static PyObject* PyPoint_GetX ( PyPoint *self, PyObject* args )
   {
     Point* cobject = static_cast<Point*>( self->_object );
     if (cobject == NULL) {
       PyErr_SetString( ProxyError
                      , "Attempt to call Point.getX() on an unbound Hurricane object" );
       return NULL;
     }
     return Isobar::PyDbU_FromLong(cobject->getX());
   }
