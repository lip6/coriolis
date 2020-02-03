.. -*- Mode: rst -*-


2. Basic File Structure and CMake configuration
=================================================

As a first example we will consider the ``Hurrican::Library``
class. To export a class into Python, we must create three files:

#. ``PyLibrary.h``, defines the ``PyLibrary`` C-Struct and the functions
   needed outside the module istself (mostly for ``PyHurricane.cpp``).

#. ``PyLibrary.cpp``, contains the complete wrapping of the class and
   the Python type definition (``PyTypeLibrary``).

#. ``PyHurricane.cpp``, the definition of the Python module into which
   the classes are registered. The module act as a ``namespace`` in
   Python so it is good practice to give it the same name as it's
   associated C++ namespace.

|newpage|

To build a Python module in |cmake|, use the following macro:

.. code-block:: cmake

                 set( pyCpps      PyLibrary.cpp
                                  PyHurricane.cpp )
                 set( pyIncludes  hurricane/isobar/PyLibrary.h

   add_python_module( "${pyCpps}"
                      "${pyIncludes}"
                      "isobar;1.0;1"     # Name & version of the supporting
                                         # shared library.
                      Hurricane          # Name of the Python module will give:
                                         #   Hurricane.so
                      "${depLibs}"       # List of dependency libraries.
                      include/coriolis2/hurricane/isobar
                                         # Where to install the include files.
                    )
