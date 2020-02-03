.. -*- Mode: rst -*-

.. include:: ../etc/definitions.rst


3. Case 1 - DBo Derived, Standalone 
======================================

As example, we take ``Library``. This a ``DBo`` derived class, but we
choose not to export the parent classes. From Python, it will appear
as a base class.

.. _3.1:

.. _3.1 Class Associated Header File:

3.1 Class Associated Header File
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Here is the typical content of a header file (for ``PyLibrary``):

.. code-block:: c++

   #ifndef  PY_LIBRARY_H
   #define  PY_LIBRARY_H
   
   #include "hurricane/isobar/PyHurricane.h"
   #include "hurricane/Library.h"
   
   namespace  Isobar {
     using namespace Hurricane;
   
     extern "C" {
   
       typedef struct {
           PyObject_HEAD
           Library* _object;
       } PyLibrary;
   
       extern  PyTypeObject  PyTypeLibrary;
       extern  PyMethodDef   PyLibrary_Methods[];
       extern  PyObject*     PyLibrary_Link       ( Hurricane::Library* lib );
       extern  void          PyLibrary_LinkPyType ();
   
   
   #define IsPyLibrary(v) ( (v)->ob_type == &PyTypeLibrary )
   #define PYLIBRARY(v)   ( (PyLibrary*)(v) )
   #define PYLIBRARY_O(v) ( PYLIBRARY(v)->_object )
   
     }  // extern "C".
   }  // Isobar namespace.

   #endif  // PY_LIBRARY_H


The code is organized as follow:

1. It must have, *as the first include* ``PyHurricane.h``, which provides
   the complete bunch of macros needed to build the module. Then the include
   of the C++ class we want to wrap (``Library.h``).

2. As Python is written in C, all the wrapper code has to be but inside
   an ``extern "C"`` namespace.

3. Definition of the wrapped |struct|, ``PyLibrary``. It is standard Python here.

   .. note::
      For our set of macros to work, the name of the pointer to the
      C++ class must always be **_object**, and the various functions and
      macros defined here must take the name of the class (either in
      lowercase, camel case or capitals).

4. Declaration of the Python type ``PyTypeLibrary`` (standard).

5. Declaration of the Python type table of methods ``PyLibrary_Methods`` (standard).

.. _3.6:

6. Declaration of ``PyLibrary_Link()``, helper to convert a C++ ``Lybrary`` into
   a ``PyLibrary`` (put in the support shared library).

7. Declaration of ``PyLibrary_LinkPyType()``, this function setup the class-level
   function of the new Python type (here, ``PyTypeLibrary``). 

8. And, lastly, three macros to:

   * ``IsPylibrary()``, know if a Python object is a ``PyLibrary``
   * ``PYLIBRARY()``, force cast (C style) of a ``PyObject`` into a ``PyLibrary``.
   * ``PYLIBRARY_O()``, extract the C++ object (``Library*``) from the Python
     object (``PyLibrary``).


.. _3.2 Class Associated File:

3.2 Class Associated File
~~~~~~~~~~~~~~~~~~~~~~~~~~~

3.2.1 Head of the file
------------------------

.. code-block:: c++

   #include "hurricane/isobar/PyLibrary.h"
   #include "hurricane/isobar/PyDataBase.h"
   #include "hurricane/isobar/PyCell.h"
   
   namespace Isobar {
     using namespace Hurricane;
   
     extern "C" {
   
   #define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Library,lib,function)

As for the header, all the code must be put inside a ``extern "C"`` namespace.

A convenience macro ``METHOD_HEAD()`` must be defined, by refining
``GENERIC_METHOD_HEAD()``. This macro will be used in the method wrappers
below to cast the ``_object`` field of the Python object into the
appropriate C++ class, this is done using a C-style cast.
The parameters of that macro are:

#. The C++ encapsulated class (``Library``).
#. The name of the *variable* that will be used to store a pointer
   to the C++ working object.
#. The name of the C++ method which is to be wrapped.


3.2.2 The Python Module Part
------------------------------

First, we have to build all the wrappers to the C++ methods of
the class. For common predicates, accessors, and mutators macros
are supplied.

Wrapping of the ``Library::getCell()`` method:

.. code-block:: c++

   static PyObject* PyLibrary_getCell ( PyLibrary* self, PyObject* args )
   {
     Cell* cell = NULL;
 
     HTRY
       METHOD_HEAD( "Library.getCell()" )
       char* name = NULL;
       if (PyArg_ParseTuple(args,"s:Library.getCell", &name)) {
         cell = lib->getCell( Name(name) );
       } else {
         PyErr_SetString( ConstructorError
                        , "invalid number of parameters for Library::getCell." );
         return NULL;
       }
     HCATCH
 
     return PyCell_Link(cell);
   }

Key points about this method wrapper:

#. The ``HTRY`` / ``HCATCH`` macros provides an insulation from the C++
   exceptions. If one is emitted, it will be catched and transformed in
   a Python one. This way, the Python program will be cleanly interrupted
   and the usual stack trace displayed.

#. The returned value of this method is of type ``Cell*``, we have to
   transform it into a Python one. This is done with ``PyCell_Link()``.
   This macro is supplied by the ``PyCell.h`` header and this is why
   it must be included.

|newpage|


Wrapping of the ``Library::create()`` method:

.. code-block:: c++

   static PyObject* PyLibrary_create( PyObject*, PyObject* args )
   {
     PyObject* arg0;
     PyObject* arg1;
     Library* library = NULL;
 
     HTRY
       __cs.init( "Library.create" );                          // Step (1).
       if (not PyArg_ParseTuple( args, "O&O&:Library.create"
                               , Converter, &arg0
                               , Converter, &arg1 )) {         // Step (2).
         PyErr_SetString( ConstructorError
                        , "invalid number of parameters for Library constructor." );
         return NULL;
       }
       if (__cs.getObjectIds() == ":db:string") {              // Step (3.a)
         DataBase* db = PYDATABASE_O(arg0);
         library = Library::create( db, Name(PyString_AsString(arg1)) );
       } else if (__cs.getObjectIds() == ":library:string") {  // Step (3.b) 
         Library* masterLibrary = PYLIBRARY_O(arg0);
         library = Library::create( masterLibrary, Name(PyString_AsString(arg1)) );
       } else {
         PyErr_SetString( ConstructorError
                        , "invalid number of parameters for Library constructor." );
         return NULL;
       }
     HCATCH
 
     return PyLibrary_Link( library );
   }

Key point about this constructor:

#. We want the Python interface to mimic as closely as possible the
   C++ API. As such, Python object will be created using a static
   ``.create()`` method. So we do not use the usual Python allocation
   mechanism.

#. As it is a *static* method, there is no first argument.

#. Python do not allow function overload like C++. To emulate that
   behavior we use the ``__cs`` object (which is a global variable).

   #. Init/reset the ``__cs`` object: see *step (1)*.

   #. Call ``PyArg_ParseTuple()``, read every mandatory or optional
      argument as a Python object (``"O&"``) and use ``Converter``
      on each one. ``Converter`` will determine the real type of
      the Python object given as argument by looking at the
      encapsulated C++ class. It then update the ``__cs`` object.
      Done in *step (2)*

   #. After the call to ``PyArg_ParseTuple()``, the function
      ``__cs.getObjectIds()`` will return the *signature* of
      the various arguments. In our case, the valid signatures
      will be ``":db:string"`` (*step (3.a)*a) and ``":library:string"``
      (*step (3.b)*).

   #. Call the C++ method after extracting the C++ objects from
      the Python arguments. Note the use of the ``PYLIBRARY_O()``
      and ``PYDATABSE_O()`` macros to perform the conversion.

#. Return the result, encapsulated through a call to ``PyLibrary_Link()``.

|newpage|


Wrapping of the ``Library::destroy()`` method:

.. code-block:: c++

   DBoDestroyAttribute(PyLibrary_destroy, PyLibrary)

For C++ classes **that are derived** from ``DBo``, the destroy method
wrapper must be defined using the macro ``DBoDestroyAttribute()``.
This macro implements the bi-directional communication mechanism
using ``Hurricane::Property``. It **must not** be used for
non ``DBo`` derived classes.


Defining the method table of the PyLibrary type:

.. code-block:: c++

   PyMethodDef PyLibrary_Methods[] =
     { { "create"    , (PyCFunction)PyLibrary_create , METH_VARARGS|METH_STATIC
                     , "Creates a new library." }
     , { "getCell"   , (PyCFunction)PyLibrary_getCell, METH_VARARGS
                     , "Get the cell of name <name>" }
     , { "destroy"   , (PyCFunction)PyLibrary_destroy, METH_NOARGS
                     , "Destroy associated hurricane object The python object remains." }
     , {NULL, NULL, 0, NULL}           /* sentinel */
     };


This is standard Python/C API. The name of the ``PyMethodDef`` table  must be
named from the class: ``PyLibrary_Methods``.


3.2.3 Python Type Linking
---------------------------

Defining the ``PyTypeLibrary`` class methods and the type linking function.

Those are the functions for the Python object itself to work, not the
wrapped method from the C++ class.

.. note::
   At this point we **do not** define the ``PyTypeLibrary`` itself.
   Only it's functions and a function to set them up *once* the
   type will be defined.

.. code-block:: c++

   DBoDeleteMethod(Library)
   PyTypeObjectLinkPyType(Library)


The macro ``DBoDeleteMethod()`` define the function to delete a
``PyLibrary`` *Python* object. Again, do not mistake it for the deletion
of the C++ class (implemented by ``DBoDestroyAttribute()``).
Here again, ``DBoDeleteMethod()`` is specially tailored for
``DBo`` derived classes.

.. _PyLibrary_LinkPyType():

To define ``PyLibrary_LinkPyType()``, use the ``PyTypeObjectLinkPyType()``
macro. This macro is specific for ``DBo`` derived classes that are seen as
base classes under Python (i.e. we don't bother exposing the base
class under Python). ``PyLibrary_LinkPyType()`` setup the class functions
in the ``PyTypeLibrary`` type object, it **must** be called in the
Python module this class is part of (in this case: ``PyHurricane.cpp``).
This particular flavor of the macro *will define* and setup the
following class functions:

* ``PyTypeLibrary.tp_compare`` (defined by the macro). 
* ``PyTypeLibrary.tp_repr`` (defined by the macro). 
* ``PyTypeLibrary.tp_str`` (defined by the macro). 
* ``PyTypeLibrary.tp_hash`` (defined by the macro). 
* ``PyTypeLibrary.tp_methods`` sets to the previously defined ``PyLibrary_Methods`` table.
* ``PyTypeLibrary.tp_dealloc`` is set to a function that *must* be named ``PyLibrary_DeAlloc``,
  this is what ``DBoDeleteMethod`` does. It is *not* done by ``PyTypeObjectLinkPyType``.

Defining the ``PyTypeLibrary`` type:


3.2.4 The Shared Library Part
-------------------------------

This part will be put in a separate supporting shared library, allowing
other Python module to link against it (and make use of its symbols).

.. code-block:: c++

   DBoLinkCreateMethod(Library)
   PyTypeObjectDefinitions(Library)


To define ``PyTypeLibrary``, use the ``PyTypeObjectDefinitions()`` macro.
This macro is specific for classes that, as exposed by Python,
are neither *derived* classes nor *base* classes for others.
That is, they are standalone from the inheritance point of view.

The ``DBoLinkCreateMethod()`` macro will define the ``PyLibrary_Link()``
function which is responsible for encapsulating a C++ ``Library`` object
into a Python ``PyLibrary`` one.


3.3 Python Module (C++ namespace)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

We use the Python module to replicate the C++ *namespace*. Thus, for the
``Hurricane`` namespace we create a Python ``Hurricane`` module which is
defined in the ``PyHurricane.cpp`` file, then we add into that module
dictionary all the Python types encapsulating the C++ classes of that
namespace.

.. code-block:: c++

   DL_EXPORT(void) initHurricane ()
   {
     PyLibrary_LinkPyType();  // step 1.
 
     PYTYPE_READY( Library )  // step 2.
 
     __cs.addType( "library", &PyTypeLibrary, "<Library>", false ); // step 3.
 
     PyObject* module = Py_InitModule( "Hurricane", PyHurricane_Methods );
     if (module == NULL) {
       cerr << "[ERROR]\n"
            << "  Failed to initialize Hurricane module." << endl;
       return;
     }
 
     Py_INCREF( &PyTypeLibrary );                                        // step 4.
     PyModule_AddObject( module, "Library", (PyObject*)&PyTypeLibrary ); // step 4.
   }

The ``initHurricane()`` initialisation function shown above has
been scrubbed of everything not relevant to the ``PyLibrary`` class.
The integration of the ``PyLibrary`` class into the module needs
four steps:

#. A call to `PyLibrary_LinkPyType()`_ to hook the Python type functions
   in the Python type object.

#. A call to the ``PYTYPE_READY()`` macro (standard Python).

#. Registering the type into the ``__cs`` object, with ``addType()``.
   The arguments are self explanatory, save for the last which is a
   boolean to tell if this is a *derived* class or not.

#. Adding the type object (``PyTypeLibrary``) into the dictionnary of
   the module itself. This allow to mimic closely the C++ syntax:

   .. code-block:: python

      import Hurricane
      lib = Hurricane.Library.create( db, 'root' )
