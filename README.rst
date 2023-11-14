.. -*- Mode: rst -*-

|Python wheel builds|

.. |Python wheel builds| image:: https://github.com/lip6/coriolis/actions/workflows/wheels.yml/badge.svg
   :target: https://github.com/lip6/coriolis/actions/workflows/wheels.yml

.. image:: documentation/content/images/common/Coriolis-logo-blue-4.png
   :alt:   Coriolis Logo
   :align: center
   :width: 10%


======================
Coriolis Open EDA Tool
======================

Coriolis is a free database, placement and routing tool for VLSI design.

This project is hosted at: https://github.com/lip6/coriolis

Main documentation is available at: https://coriolis.lip6.fr/

Development discussion can be found `on our Matrix Channel <https://matrix.to/#/#coriolis:matrix.org>`_ and in our `GitHub Discussions <https://github.com/lip6/coriolis/discussions>`_.

Coriolis is part of a set of three repositories that are closely relateds.

* Alliance

  A venerable EDA toolchain, working only on symbolic layout. Some of it's
  checking tools (LVS, DRC) are used for the regression tests in Alliance Check Toolkit.

  https://github.com/lip6/alliance

* Alliance Check Toolkit

  A wide set of examples of designs and scripts to use by Coriolis.

  https://github.com/lip6/alliance-check-toolkit


Purpose
=======

Coriolis provides several tools to perform the layout of VLSI circuits.  Its
main components are the Hurricane database, the Etesian placer and the Katana
router, but other tools can use the Hurricane database and the parsers
provided.

Coriolus can be used via both the  ```cgt`` graphical tool <http://coriolis.lip6.fr/pages/users-guide.html#id24>`_ and via the `rich Python API <http://coriolis.lip6.fr/pages/python-tutorial.html>`_.

Installation
============

Python 3.7 or later is required to use Coliolis. It can then be installed from `Pypi <https://pypi.org/>`_.

.. code-block:: bash

   pip3 install coriolis-eda

Note for Windows, we currently only support running under WSL2.

=================
Building Coriolis
=================

Most folk will only need to install using pip, but if you want to develop or need to build from scratch, read on!

Ubuntu/Windows WSL2 Build Environment
=====================================

If you haven't already got them, install `build-essential` and `git`

.. code-block:: bash

   sudo apt install build-essential git ccache

Clone the repo:

.. code-block:: bash

   git clone --recurse-submodules https://github.com/lip6/coriolis
   cd coriolis

Install the build dependencies:

.. code-block:: bash

   sudo apt install python3 python3-pip python3.10-venv \
                    doxygen pelican texlive-latex-recommended \
                    bison flex \
                    qtbase5-dev libqt5svg5-dev libqwt-qt5-dev libbz2-dev \
                    rapidjson-dev libboost-all-dev libeigen3-dev libxml2-dev


Mac OSX Build Environment
=========================

To build on Mac, first install _Homebrew: https://brew.sh. Be sure to follow all the instructions it gives after install  so HOMEBREW_PREFIX gets set!

Clone the repo:

.. code-block:: bash

   git clone --recurse-submodules https://github.com/lip6/coriolis
   cd coriolis


To install the prereqisites:

.. code-block:: bash

   brew install ccache doxygen pelican qt@5 qwt-qt5 rapidjson boost eigen
   brew install --cask mactex 


We need to set some environment variables for finding the cask only components. We use dotenv to set these in the PDM venv (see below)

.. code-block:: bash

   pip install python-dotenv
   dotenv set CMAKE_FRAMEWORK_PATH "$HOMEBREW_PREFIX/opt/qt@5/lib/cmake/"
   dotenv set PKG_CONFIG_PATH "$HOMEBREW_PREFIX/opt/qt@5/lib/pkgconfig:$HOMEBREW_PREFIX/opt/qwt-qt5/lib/pkgconfig"



Building Coriolis
=================

To build Python wheels, you'll need the `build` package if you don't already have it installed:

.. code-block:: bash

   pip3 install build


.. code-block:: bash

   python3 -m build -w

The wheels can be found in dist and installed using pip:

.. code-block:: bash

   pip3 install dist/*.whl

Development environment
=======================

For day-to-day development, its currently best to use meson and ninja directly. Currently there are `issues with using a Python editable install`_.


We use PDM_ to manage our development environment, which uses Python's venv_ system.

.. code-block:: bash

   pip install pdm

Before starting, you'll likely want to install the `shell completion`_ and read about `using pdm with virtual environments`_.  The basics are very simple, you just use pdm to run the build commands in an environment it automatially handles.

First we set up our pdm virtual environment:

.. code-block:: bash

   pdm install --no-self -d --plugins
   
Below we are just using a local directory call builddir for our _meson build directory, but you can put it wherever you like.

.. code-block:: bash

   pdm run meson setup builddir
   pdm run ninja -C builddir install

You can then use `pdm run` to run any programs or scripts dependant on Coriolis, for example:

.. code-block:: bash

   pdm run unittests/python/test_hurricane.py


You can also install locally using:

.. code-block:: bash

   pdm run meson setup builddir -Dpython.install_env=system
   pdm run ninja -C builddir install


For more configuration and install options, see:

.. code-block:: bash

   pdm run meson configure


.. _issues with using a Python editable install: https://github.com/lip6/coriolis/issues/67
.. _venv: https://www.dataquest.io/blog/a-complete-guide-to-python-virtual-environments/#how-to-use-python-environments
.. _shell completion: https://pdm.fming.dev/latest/#shell-completion
.. _using pdm with virtual environments: https://pdm.fming.dev/latest/usage/venv/
.. _PDM: https://pdm-project.org/
.. _meson: https://mesonbuild.com/
   
Other operating systems
=======================

Coriolis is currently tested on CentOS 7 (via manylinux 2014), latest Ubuntu and on MacOSX.

We welcome patches for other operating systems, though please add appropriate CI!

You'll need the following prerequisites:

* A C++11 compliant compiler.
* Python 3.9 or later
* Pip
* ninja.
* bzip2
* boost
* bison & flex.
* Qt 5 & Qt 5 Svg.
* Qwt
* libxml2.
* RapidJSON, http://rapidjson.org/ .
* Eigen 3,  http://eigen.tuxfamily.org .
* doxygen
* pelican
* latex




Documentation
=============

The full documentation for Coriolis can be found at http://coriolis.lip6.fr/pages/documentation.html

