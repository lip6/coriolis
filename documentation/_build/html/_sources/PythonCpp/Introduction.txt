.. -*- Mode: rst -*-

.. include:: ../etc/definitions.rst


1. Introduction
=================

* This document is written for people already familiar with the
  `Python/C API Reference Manual`_.

* The macros provided by the Hurricane Python/C API are written using
  the standard Python C/API. That is, you may not use them and write
  directly your functions with the original API or any mix between.
  You only have to respect some naming convention.

* Coriolis is build against Python 2.7.


1.1 First, A Disclaimer
~~~~~~~~~~~~~~~~~~~~~~~~~

The Hurricane Python/C++ API has been written about ten years ago, at a time
my mastering of template programming was less than complete. This is why this
interface is build with old fashioned C macro instead of C++ template.

It is my hope that at some point in the future I will have time to completly
rewrite it, borrowing the interface from ``boost::python``.


1.2 About Technical Choices
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Some would say, why not use *off the shelf* wrappers like  ``swig``
or ``boost::python``, here are some clues.

#. **Partial exposure of the C++ class tree.** We expose at Python level
   C++ base classes, only if they provides common methods that we want
   to see. Otherwise, we just show them as base classes under Python.
   For instance ``Library`` is derived from ``DBo``, but we won't see
   it under Python.

#. **Bi-directional communication.** When a Python object is deleted, the
   wrapper obviously has a pointer toward the underlying C++ object and
   is able to delete it. But, the reverse case can occurs, meaning that
   you have a C++ object wrapped in Python and the database delete the
   underlying object. The wrapped Python object *must* be informed that
   it no longer refer a valid C++ one. Moreover, as we do not control
   when Python objects gets deleteds (that is, when their reference count
   reaches zero), we can have valid Python object with a dangling
   C++ pointer. So our Python objects can be warned by the C++ objects
   that they are no longer valid and any other operation than the
   deletion should result in a severe non-blocking error.

   To be precise, this apply to persistent object in the C++ database,
   like ``Cell``, ``Net``, ``Instance`` or ``Component``. Short lived
   objects like ``Box`` or ``Point`` retains the classic Python behavior.

   Another aspect is that, for all derived ``DBo`` objects, one and only
   one Python object is associated. For one given ``Instance`` object we
   will always return the *same* ``PyInstance`` object, thanks to the
   bi-directional link. Obviously, the *reference count* of the
   ``PyInstance`` is managed accordingly. This mechanism is implemented
   by the ``PyInstance_Link()`` function.

#. **Linking accross modules.** As far as I understand, the wrappers
   are for monolithic libraries. That is, you wrap the entire library
   in one go. But Hurricane has a modular design, the core database
   then various tools. We do not, and cannot, have one gigantic wrapper
   that would encompass all the libraries in one go. We do one Python
   module for one C++ library.

   This brings another issue, at Python level this time. The Python
   modules for the libraries have to share some functions. Python
   provides a mechanism to pass C function pointers accross modules,
   but I did found it cumbersome. Instead, all our modules are split
   in two:

   * The first part contains the classic Python module code.
   * The second part is to be put in a separate dynamic library that
     will hold the shared functions. The Python module is dynamically linked
     against that library like any other. And any other Python module
     requiring the functions will link against the associated shared
     library.

   Each module file will be compiled *twice*, once to build the Python
   module (``__PYTHON_MODULE`` is defined) and once to build the supporting
   shared library (``__PYTHON_MODULE__`` **not** defined). This tricky
   double compilation is taken care of though the ``add_python_module``
   ``cmake`` macro.

   For the core Hurricane library we will have:

   * ``Hurricane.so`` the Python module (use with: ``import Hurricane``).
   * ``libisobar.so.1.0`` the supporting shared library.
 
   The ``PyLibrary.cpp`` file will have the following structure:

   .. code-block:: c++

      #include "hurricane/isobar/PyLibrary.h"

      namespace  Isobar {
      
        extern "C" {
      
      #if defined(__PYTHON_MODULE__)
      
        // +=================================================================+
        // |               "PyLibrary" Python Module Code Part               |
        // +=================================================================+
        //
        // The classic part of a Python module. Goes into Hurricane.so.
      
      
      #else  // End of Python Module Code Part.
      
        // x=================================================================x
        // |              "PyLibrary" Shared Library Code Part               |
        // x=================================================================x
        //
        // Functions here will be part of the associated shared library and
        // made available to all other Python modules. Goes into libisobar.so.1.0
      
      
      # endif  // Shared Library Code Part.
      
        }  // extern "C".
      
      }  // Isobar namespace.


   This way, we do not rely upon a pointer transmission through Python
   modules, but directly uses linker capabilities.


1.3 Botched Design
~~~~~~~~~~~~~~~~~~~~

The mechanism to compute the signature of a call to a Python function,
the ``__cs`` object, is much too complex and, in fact, not needed.
At some point I may root it out, but it is used in so many places...

What I should have used the ``"O!"`` capablity of ``PyArg_ParseTuple()``,
like in the code below:

|newpage|

.. code-block:: c++

   static PyObject* PyContact_create ( PyObject*, PyObject *args )
   {
     Contact* contact = NULL;
     HTRY
       PyNet*       pyNet       = NULL;
       PyLayer*     pyLayer     = NULL;
       PyComponent* pyComponent = NULL;
       DbU::Unit    x           = 0;
       DbU::Unit    y           = 0;
       DbU::Unit    width       = 0;
       DbU::Unit    height      = 0;

       if (PyArg_ParseTuple( args, "O!O!ll|ll:Contact.create"
                           , &PyTypeNet  , &pyNet
                           , &PyTypeLayer, &pyLayer
                           , &x, &y, &width, &height)) {
         contact = Contact::create( PYNET_O(pyNet), PYLAYER_O(pyLayer)
                                  , x, y, width, height );
       } else {
         PyErr_Clear();
         if (PyArg_ParseTuple( args, "O!O!ll|ll:Contact.create"
                             , &PyTypeComponent, &pyComponent
                             , &PyTypeLayer    , &pyLayer
                             , &x, &y, &width, &height)) {
           contact = Contact::create( PYCOMPONENT_O(pyComponent), PYLAYER_O(pyLayer)
                                    , x, y, width, height );
         } else {
           PyErr_SetString( ConstructorError
                          , "invalid number of parameters for Contact constructor." );
           return NULL;
         }
       }
     HCATCH
     return PyContact_Link( contact );
   }
