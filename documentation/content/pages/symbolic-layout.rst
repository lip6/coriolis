.. -*- Mode: rst -*-


===============
Symbolic Layout
===============


:slug:     symbolic-layout
:date:     2019-12-27 16:00
:author:   Jean-Paul Chaput
:Contact:  <Jean-Paul.Chaput@lip6.fr>
:Version:  June 4, 2019 (jpc)
:category: Symbolic


.. include:: ../../etc/definitions.rst

.. |symbolic_1|               image:: {filename}/images/symbolic-layout/symbolic-1.png
                              :alt:   Symbolic, Mead & Conway
                              :align: middle
                              :width: 80%

.. |symbolic_2|               image:: {filename}/images/symbolic-layout/symbolic-2.png
                              :alt:   Symbolic, Alliance
                              :align: middle
                              :width: 80%

.. |rds_1|                    image:: {filename}/images/symbolic-layout/rds-1.png
                              :alt:   Symbolic to Real translation
                              :align: middle
                              :width: 80%

.. contents::


Making an |ASIC|
================

This section is a short introduction to the terminology of |ASIC| making.

The end product of a |VLSI| design flow is basically a *drawing*. This drawing
is what you send to the foundry in order to fabricate it. Usually, this is a
file in |GDS| format, which contains a lot of geometric shapes expressed in
microns or nanometers.

This drawing is called a **layout**.

In the layout, each geometrical shape is associated with a *layer*. For example,
there are layers for each metal level like ``metal1`` or ``metal2``. 

**Layers** tells in what *material* you want the geometrical shape to be build.
(this is an oversimplification)

All shapes in a given layer constitute a **mask**, analogous to an overlay in
classical drawing programs.

The foundry will not accept *any* layout. In order to be successfully fabricated, 
all the shapes of a layout must respect a set of rules. For example, to ensure
that after fabrication, two separated shapes of ``metal1`` are indeed separated,
they must respect a *minimal distance*, for example 0.5µ. This special set of
rules is called the **Design rules**.

**Design Rules** gives many insigth about a process and is subjected to |NDA|.
For the same reason your whole layout covered by it, meaning that you cannot
publish it in any way.


Symbolic Layout
===============

**Symbolic Layout** is a way of making the layout of a chip independant of a given
technological node. This technique is based on the observation that, between two
processes in the same technological node (say, for example, 350nm of |AMS| and
350nm of |TSMC|), there are only minors rules variations. Moreover, even between
different nodes (350nm |AMS| and 180nm |AMS|), the *shrink rate* of the various
layers of the process are the same.

Symbolic layout consist of drawing in a blank unit called the |lambda| (lambda).
Then, the value of the |lambda| is calculated for the target technology so that the
layout fit it's particular design rules. This approach was first introduced by
|Mead| & |Conway| [VLSISYS]_.

As |Coriolis| can manage both symbolic and real layers in the same design,
it uses the following convention for layer naming:

* ``METAL1`` : uppercase named layers are for *symbolic layers*. Those layer
  shapes will change when mapped toward a real technology.
  
* ``metal1`` : lowercase named layers are for *real layers*. THeir shapes will
  be exported exacltly *as is*.


|symbolic_1|

The symbolic layout of |Alliance|, refine this approach, by adding width and cap
extentions factors to allow a closer fitting of the technology.

|symbolic_2|


Symbolic To Real Translation
============================

Contrary to commercial design flows wich directly creates a layout for a target
node, our flow create a *symbolic layout* which you have to translate into one
on the target process. This is done with the |s2r| tool which stands for
"Symbolic To Real". And this tool must have a configuration file for the
intended technology (this is the ``.rds`` file). As the ``.rds`` file is
written using the **Design Rules** so is under |NDA|. Writting the ``.rds``
to get the best fit for target process is still largely a craft.

|rds_1|


Pros & Cons of Symbolic Layout
==============================

Cons:

* As it cannot make use of the finest features of the target process, there is an
  unvoidable loss of area. That is, the layout once translated will be bigger than
  if it has been done directly. The loss is below 10%.

Pros:

* You do not have to build you chip for each target process. You only need to write
  a new ``.rds`` file.

* Symbolic layout is not subjected to |NDA|, so it can be freely published and
  exchanged.


A Note About Analog Designs
===========================

The symbolic layout approach is not suited for analog designs. Analog designs are
closely related to the target process. So we developped a different methodology
to ensure portability.


.. [VLSISYS] |Mead|, Carver; |Conway|, Lynn (1980). Introduction to VLSI systems.
	     Reading, Mass.: Addison-Wesley. ISBN 0201043580. OCLC 4641561
