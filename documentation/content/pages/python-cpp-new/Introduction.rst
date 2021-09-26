.. -*- Mode: rst -*-


1. Introduction
===============

* This document is written for people already familiar with the
  `Python/C API Reference Manual`_.

* The macros provided by the Hurricane Python/C API are written using
  the standard Python C/API. That is, you may not use them and write
  directly your functions with the original API or any mix between.
  You only have to respect some naming convention.

* Coriolis is build against Python 3.6.


1.1 About Technical Choices
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Some would say, why not use *off the shelf* wrappers like  ``swig``,
``boost::python`` or ``pybind11``, here are some clues.

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
   by the ``PyTypeManager::_link()`` method.

#. **Linking accross modules.** As far as I understand, the wrappers
   are for monolithic libraries. That is, you wrap the entire library
   in one go. But Coriolis has a modular design, the core database
   then various tools. We do not, and cannot, have one gigantic wrapper
   that would encompass all the libraries in one go. We do one Python
   module for each C++ library.

   This brings another issue, at Python level this time. The Python
   modules for the libraries have to share some functions. Python
   provides a mechanism to pass C function pointers accross modules,
   (``Capsule``) but I did not fully understand it.

   Instead, we register all the newly defined Python type object
   in the ``PyTypeManager`` and we link the associated C++ library
   into all Python modules. So all types and ancillary functions can
   easily be seen accross modules.

   This way, we do not rely upon a pointer transmission through Python
   modules, but directly uses linker capabilities.

   **The PyTypeManager** approach also suppress the need to *split* into
   two libraries the Python modules like in the C-Macros implementation,
   and the double compilation pass.

