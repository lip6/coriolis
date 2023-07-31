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

The user interface ``cgt`` is the prefered way to run Coriolis, but all
Coriolis tools are Python modules and thus scriptable.


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

The build system relies on a fixed directory tree from the root
of the user currently building it. Thus first step is to get a clone of
the repository in the right place. Proceed as follow:

.. code-block:: bash

   ego@home:~$ mkdir -p ~/coriolis-2.x/src/
   ego@home:~$ cd ~/coriolis-2.x/src
   ego@home:src$ git clone --recurse-submodules https://github.com/lip6/coriolis
   ego@home:src$ cd coriolis

Then, build the tool:

.. code-block:: bash

   ego@home:coriolis$ make install

Coriolis gets installed at the root of the following tree: ::

    ~/coriolis-2.x/OS.DISTRIB/Release.Shared/install/

Where ``OS`` is the name of your operating system and ``DISTRIB`` your
distribution (mileage may vary).


Using Coriolis
==============

Prior to using Coriolis, you need to setup your UNIX environment by running
the following command:

.. code-block:: bash

    ego@home:~$ eval `~/coriolis-2.x/OS.DISTRIB/Release.Shared/install/etc/coriolis2/coriolisEnv.py`

Then you can launch the GUI:

.. code-block:: bash

    ego@home:~$ cgt -V
