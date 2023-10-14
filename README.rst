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

Documentation
=============

The full documentation for Coriolis can be found at http://coriolis.lip6.fr/pages/documentation.html

