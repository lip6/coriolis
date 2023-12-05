.. -*- Mode: rst -*-


December 2023
=============


|BUG| Bad Topology in ``NetBuilderHV::_do_xG_1M1()``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In case of three globals (one ``M1`` terminal) and *no* south global,
the generated topology was wrong. We were accessing the terminal directly
with a vertical segment. It was creating a layer gap between *that*
vertical, which is ``M3`` and and terminal in ``M1``. This is also
too rigid a configuration.

Fix: add an horizontal, in ``M2``, to access the terminal.

Yoyo

Occured on ``benchs/RISC-V/Minerva/sky130_c4m/``.

.. image:: {static}/images/notes/Katana-Debug_3.png
   :alt:   Faulty _do_xG_1M3 (no South) configuration.
   :align: center
   :width: 40%


|CHANGE| Reorganization of the size detection of ``RoutingPad``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Instead of detecting the size characteristics of a ``RoutingPad`` at a late
stage when calling ``NetBuilder::_doRp_Access()``, do it once when first
loading the net in ``Dijkstra::load()`` with ``Configuration::selectRpcomponent()``.

Add a new flag attribute on ``RoutingPad`` in order to store various
additionnal informations. Currently, the size characteristics.

Add an ``M1Offgrid`` flags for the terminal characteristics set.

Move ``getPositions()`` and ``checkRoutingPadSize()`` from ``NetBuilder`` to
``Anabatic::Configuration()``.

In ``Anabatic::Configuration``, add support for the ``M1Offgrid`` options,
that allows fully offgrid terminals.

The ``Anabatic::Configuration::checkRoutingpadSize()`` now store the
characteristics of the terminal (``HSmall``, ``VSmall``, ...) directly in
the flags of the ``RoutingPad``.

For the whole function stack, work directly on ``RoutingPad*`` instead of
``Component*`` :

* ``NetBuilder::_doRp_StairCaseH()``
* ``NetBuilder::_doRp_StairCaseV()``
* ``NetBuilderHV::_doRp_Access()``
* ``NetBuilderHV::_doRp_AutoContacts()``
* ``NetBuilderHybridHV::_doRp_Access()``
* ``NetBuilderHybridHV::_doRp_AutoContacts()``
* ``NetBuilderM2::_doRp_Access()``
* ``NetBuilderM2::_doRp_AutoContacts()``


|NEW| Implementation of Pin Rectilinear Support
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Add a ``PinRectilinearFilter`` to the ``LefImport`` parser in order to select
on which kind of biggest rectangle extracted from the ``Rectilinear`` we want
to create the ``Vertical`` or ``Horizontal`` *only* external component.
We can choose betwen the *tallest*, *widest* or *largest* (area).
There is also a minimum size filtering (to prune too small rectangle
from the start).

Make use of the new feature in ``gf180mcu/mcu9t5v0.py``.

