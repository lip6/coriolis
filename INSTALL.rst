Building Coriolis
=================

To build Coriolis, ensure the following prerequisites are met:

* A C++11 compliant compiler.
* Python 3.
* make or ninja.
* cmake.
* bzip2
* boost.
* bison & flex.
* Qt 5 & Qt 5 Svg.
* Qwt
* libxml2.
* RapidJSON, http://rapidjson.org/ .
* Eigen 3,  http://eigen.tuxfamily.org .
* Lemon, http://lemon.cs.elte.hu/trac/lemon .
* doxygen
* pelican
* latex

The build system relies on a fixed directory tree from the root
of the user currently building it. Thus first step is to get a clone of
the repository in the right place. Proceed as follow:

.. code-block:: bash

   $ git clone --recurse-submodules https://github.com/lip6/coriolis
   $ cd coriolis

Then, to build Python wheels:

.. code-block:: bash

   $ poetry build

The wheels can be found in dist and installed using pip:

.. code-block:: bash

   $ pip install dist/*.whl

Building for Mac OSX
====================

To build on mac, first install _Homebrew: https://brew.sh

To install the prereqisites:
  
.. code-block:: bash

   brew install doxygen latex qt@5 qwt-qt5 rapidjson boost
   brew install --cask mactex 

.. code-block:: bash

   CMAKE_FRAMEWORK_PATH=/opt/homebrew/opt/qt@5/lib/cmake/

Using Coriolis
==============

Prior to using Coriolis, you need to setup your UNIX environment by running
the following command:

.. code-block:: bash

    ego@home:~$ eval `~/coriolis-2.x/OS.DISTRIB/Release.Shared/install/etc/coriolis2/coriolisEnv.py`

Then you can launch the GUI:

.. code-block:: bash

    ego@home:~$ cgt -V
