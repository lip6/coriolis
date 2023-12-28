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

Occured on ``benchs/RISC-V/Minerva/sky130_c4m/``.

.. image:: {static}/images/notes/Katana-Debug_3.png
   :alt:   Faulty _do_xG_1M3 (no South) configuration.
   :align: center
   :width: 60%


|CHANGE| Reorganization of the size detection of RoutingPad
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Instead of detecting the size characteristics of a RoutingPad_ at a late
stage when calling ``NetBuilder::_doRp_Access()``, do it once when first
loading the net in ``Dijkstra::load()`` with ``Configuration::selectRpcomponent()``.

Add a new flag attribute on RoutingPad_ in order to store various
additionnal informations. Currently, the size characteristics.

Add an ``M1Offgrid`` flags for the terminal characteristics set.

Move ``getPositions()`` and ``checkRoutingPadSize()`` from ``NetBuilder`` to
``Anabatic::Configuration()``.

In ``Anabatic::Configuration``, add support for the ``M1Offgrid`` options,
that allows fully offgrid terminals.

The ``Anabatic::Configuration::checkRoutingpadSize()`` now store the
characteristics of the terminal (``HSmall``, ``VSmall``, ...) directly in
the flags of the ``RoutingPad``.

For the whole function stack, work directly on RoutingPad_ instead of
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


|NEW| Support for offgrid M1 & VH topologies (WIP 1)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. Add a flag attribute in RoutingPad_ 
---------------------------------------

To avoid re-computing informations about the RoutingPad_, namely it's size
characteristics (h-small, v-small, punctual, offgrid), add a ``flag`` attribute
in order to store those informations. They are computed in
``Anabatic::Configuration::selectRpComponent()``.


2. Implement horizontal dragging ``AutoContactTerminal::updateGeometry()``
--------------------------------------------------------------------------

Dragging can occurs when an ``AutoContactTerminal`` is anchored on an
horizontal RoutingPad_ **and** the opposite of the connecting segment
is also horizontal. In that case, the connecting segment, which is
*perpandicular* can be *dragged* over the RoutingPad_ as to minimize
the length of the opposite horizontal.

This feature was until now, only implemenred for verticals.

.. image:: {static}/images/notes/Katana-Debug_7.png
   :alt:   Vertical dragging configuration.
   :align: center
   :width: 60%


3. Clarify sourcePosition & targetPosition on AutoSegments
----------------------------------------------------------

The ``_sourcePosition`` and ``_targetPosition`` of ``AutoHorizontal`` or
``AutoVertical`` have slighty different meanings whether the segment is
in preferred routing direction or not.

A. **In preferred routing direction**  (fig. a) the position extend from
   the segment for *half* the minimum spacing (plus half the size of the
   starting or ending VIA). With neighboring segments extentions
   computed the same, we ensure that the minimal spacing is always kept.
   The complex work of computing the value of the extension of the end
   cap of a segment is done by ``AutoSegment::getExtensionCap()``.

B. **In perpandicular routing direction** (fig. b) the ``_sourcePosition``
   to ``_targetPosition`` interval should cross all the perpandicular
   track axis that it has a footprint on. As it may be misaligned and
   likely to impinge on the leftmost or rightmost one. In that case,
   we must extend by the *minimal spacing* (not it's half).

.. image:: {static}/images/notes/Katana-Debug_9.png
   :alt:   Preferred & non-preferred track footprint.
   :align: center
   :width: 60%


The figure below is a simplificated description on how ``AutoSegment::getExtensionCap()``
compute the extension from the ``sourceU`` coordinate. ``sourceU`` being the
starting or ending coordinate of segment, usually the axis of the perpandicular
it is connected to.

.. image:: {static}/images/notes/Katana-Debug_6.png
   :alt:   Preferred & non-preferred track footprint.
   :align: center
   :width: 60%


4. Write topology for NetBuilderVH::_do_2G_1M1()
------------------------------------------------

.. image:: {static}/images/notes/Katana-Debug_5.png
   :alt:   topology of NetBuilderVH::_do_2G_1M1().
   :align: center
   :width: 60%


4. Allow selection of HV or VH topologies for GF180MCU
------------------------------------------------------

In order to ease testing whithout constantly changing the configuration
file, provide a boolean in ``technos.setupGF180MCU_GF()`` to select it.

In the ``dodo.py``, add:

.. code-block:: python

   setupGF180MCU_GF( checkToolkit='../../..', pdkTop='../../../../gf180mcu-pdk', useHV=False )


|NEW| Display the "ripup ratio" in Katana::Statistics
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This ratio is an attempt at characterizing the *hardness* of a design
(very elusive concept). It is crudely done for now, as it is just a
ratio between the initial number of events (which is equal to the
number of |TrackElements| to place). It do not take into account the
dogleg creation process that will introduce more primary events
as the net topologie are slackened.
