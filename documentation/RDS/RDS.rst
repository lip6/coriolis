.. -*- Mode: rst -*-

.. role:: ul
.. role:: cb
.. role:: sc
.. role:: fboxtt

.. Acronyms & names.
.. |GNU|                            replace:: :sc:`gnu`
.. |LGPL|                           replace:: :sc:`lgpl`
.. |GPL|                            replace:: :sc:`gpl`
.. |UPMC|                           replace:: :sc:`upmc`
.. |RDS|                            replace:: :sc:`rds`

.. Tools 
.. |ocp|                            replace:: ``ocp``
.. |nero|                           replace:: ``nero``
.. |ring|                           replace:: ``ring``
.. |s2r|                            replace:: ``s2r``
.. |druc|                           replace:: ``druc``
.. |graal|                          replace:: ``graal``
.. |cougar|                         replace:: ``cougar``
.. |cif|                            replace:: ``cif``
.. |gds|                            replace:: ``gds``

.. RDS file syntax.
.. |MBK_TO_RDS_SEGMENT|             replace:: ``MBK_TO_RDS_SEGMENT``
.. |MBK_TO_RDS_VIA|                 replace:: ``MBK_TO_RDS_VIA``
.. |MBK_TO_RDS_BIGVIA_HOLE|         replace:: ``MBK_TO_RDS_BIGVIA_HOLE``
.. |MBK_TO_RDS_BIGVIA_METAL|        replace:: ``MBK_TO_RDS_BIGVIA_METAL``
.. |MBK_WIRESETTING|                replace:: ``MBK_WIRESETTING``
.. |ALL|                            replace:: ``ALL``
.. |DRC|                            replace:: ``DRC``
.. |EXT|                            replace:: ``EXT``
.. |VW|                             replace:: ``VW``
.. |LCW|                            replace:: ``LCW``
.. |RCW|                            replace:: ``RCW``
.. |ALU1|                           replace:: ``ALU1``
.. |NDIF|                           replace:: ``NDIF``
.. |NTRANS|                         replace:: ``NTRANS``
.. |CONT_DIF_P|                     replace:: ``CONT_DIF_P``
.. |RDS_NDIF|                       replace:: ``RDS_NDIF``
.. |RDS_NIMP|                       replace:: ``RDS_NIMP``
.. |RDS_ACTIV|                      replace:: ``RDS_ACTIV``
.. |RDS_GATE|                       replace:: ``RDS_GATE``
.. |RDS_POLY|                       replace:: ``RDS_POLY``
.. |RDS_ALU1|                       replace:: ``RDS_ALU1``


:Date:    16, september 2014
:Authors: Jean-Paul Chaput
:Contact: <alliance-users@soc.lip6.fr>
:Version: 0.1

|medskip|

**Disclaimer:** This document is still far from complete.

|medskip|

===============
RDS File Format
===============


.. contents::

|medskip|

|newpage|


Introduction
============


The RDS file control how a symbolic layout is transformed into it's real
conterpart.

.. note:: **Unit used inside the RDS file:** all units are expressed in micrometers.

Alliance tools relying on the RDS file, and what layers are active for them:

=======================================  =============  ===============================
Tool                                     Name           RDS Flags
=======================================  =============  ===============================
Layout editor                            |graal|        |ALL|  
Design Rule Checker                      |druc|         |ALL|, |DRC|
Electrical extractor                     |cougar|       |ALL|, |EXT|
The symbolic to real layout translator   |s2r|          |ALL|  
=======================================  =============  ===============================


Physical Grid & Lambda Value
============================

RDS file: ::

    DEFINE  PHYSICAL_GRID  0.005
    DEFINE  LAMBDA         0.09

Tells that the physical grid (founder grid) step is 0.005µm and the lambda has
a value of 0.09µm. That is, one lambda is 18 grid steps.

We can distinguish two kind of |RDS| files: 

* The *1µm* kind, odd segment widths and coordinates are allowed, but the ``LAMBDA``
  value **must** represent an *even* number of foundry grid step.
* The *2µm* kind, segments widths and coordinates must all be even. And in that case
  the ``LAMBDA`` value can be any multiple of the foundry grid.


The |MBK_TO_RDS_SEGMENT| table
==============================

The |MBK_TO_RDS_SEGMENT| table control the way segments are translated into
real rectangles. Be aware that we are translating *segments* and not *rectangles*.
Segments are defined by their axis (source & target points) and their width.
The geometrical transformations are described according to that model.
Obviously, they are either horizontal or vertical.

The translation method of a symbolic segment is as follow:

1. The segment is translated into one or more physical rectangles.
   The generated rectangles depends on the tool which is actually
   using |RDS| and the flag for the considered real layer.
   For instance, real layers flagged with |DRC| will be generated
   for |s2r| (for the |cif| or |gds|) and |druc|, but will not
   be shown under |graal|.

2. Translation into one real layer. *First* the source & target coordinates and width
   of the symbolic segment are multiplied by the ``LAMBDA`` value to obtain a real
   segment. *Then* one of the |VW|, |LCW| or |RCW| transformation is applied to
   that segment to get the final real rectangle.

   * |VW| for Variable Width, expand the real layer staying centered from the
     original one. In those rules, the third number is not used, it is only here
     to make the life easier for the parser...

     |RDS_VW|

   * |LCW| or |RCW| for Left/Right Constant Width, create an off-center rectangle
     of fixed width relatively to the real segment. Note that the ``SP`` number
     is the distance *between the edge* of the real segment and the edge of the
     generated real rectangle (*not* from the axis). It is often zero.

     |RDS_LCW|


|newpage|

Examples: ::

    TABLE MBK_TO_RDS_SEGMENT

        # (Case 1)
        ALU1       RDS_ALU1   VW  0.18  0.09  0.0  ALL

        # (Case 2)
        NDIF       RDS_NDIF   VW  0.18  0.0   0.0  ALL \
                   RDS_ACTIV  VW  0.18  0.0   0.0  DRC \
                   RDS_NIMP   VW  0.36  0.36  0.0  DRC

        # (Case 3)
        NTRANS     RDS_POLY   VW  0.27  0.00  0.0  ALL \
                   RDS_GATE   VW  0.27  0.00  0.0  DRC \
                   RDS_NDIF  LCW  0.0   0.27  0.0  EXT \
                   RDS_NDIF  RCW  0.0   0.27  0.0  EXT \
                   RDS_NDIF   VW  0.0   0.72  0.0  DRC \
                   RDS_ACTIV  VW  0.0   0.72  0.0  ALL \
                   RDS_NIMP   VW  0.18  1.26  0.0  DRC

    END

:fboxtt:`Case 1` the |ALU1| is translated in exacltly one real rectangle of
|RDS_ALU1|, both ends are extended by 0.18µm and it's width is increased
by 0.09µm.

:fboxtt:`Case 2` the |NDIF| will be translated into only one segment
under |graal|, for symbolic visualization. And into three real rectangles
for |s2r| and |druc|.

:fboxtt:`Case 3` the |NTRANS|, associated to a transistor is a little bit
more complex, the generated shapes are different for the extractor |cougar|
in one hand, and for both |druc| & |s2r| in the other hand.

* For the extractor (|EXT| & |ALL| flags) there will be four rectangles
  generateds:

  1. The gate (|RDS_GATE|)
  2. The left diffusion of the transistor (source or drain) (|RDS_NDIF|).
  3. The right diffusion of the transistor (drain or source) (|RDS_NDIF|).
  4. The active area (|RDS_ACTIV|).

  As the extractor must kept separate the source and the drain of the transistor,
  they are generated as two offset rectangles, using the |LCW| and |RCW| directives.

* For |s2r| and |druc| (|DRC| and |ALL|), five rectangles are generateds:

  1. The poly (|RDS_POLY|).
  2. The gate (|RDS_GATE|).
  3. The diffusion, as one rectangle that covers both the |LCW| and the |RCW| (|RDS_NDIF|).
  4. The active area (|RDS_ACTIV|).
  5. The N implantation (|RDS_NIMP|).

  In the layout send to the foundry, the source & drain are draw as one rectangle
  across the gate area (the transistor being defined by the intersection of both
  rectangles).


|newpage|

The |MBK_TO_RDS_VIA| table
==========================

This table is to translate *default* VIAs into real via. In the symbolic layout
the default VIA is simply a point and a set of layers. All layers are converted
in squares shapes centered on the VIA coordinate. The one dimension given is the
size of the side of that square.

Note that although we are refering to VIAs, which for the purists are between two
metal layers, this table also describe *contacts*.

Example: ::

    TABLE MBK_TO_RDS_VIA

        CONT_DIF_P RDS_PDIF  0.54 ALL \
                   RDS_CONT  0.18 ALL \
                   RDS_ALU1  0.36 ALL \
                   RDS_ACTIV 0.54 DRC \
                   RDS_PIMP  0.90 DRC

        CONT_POLY  RDS_POLY  0.54 ALL \
                   RDS_CONT  0.18 ALL \
                   RDS_ALU1  0.36 ALL

        CONT_VIA   RDS_ALU1  0.45 ALL \
                   RDS_VIA1  0.27 ALL \
                   RDS_ALU2  0.45 ALL

    END

.. note:: **In CONT_DIF_P** you may see that only three layers will be shown under
          |graal|, but five will be generated in the |gds| layout.


The |MBK_TO_RDS_BIGVIA_HOLE| table
==================================

In |s2r|, when generating BIGVIAs, the matrix of holes they contains is
not draw relative to the position of the BIGVIA itself, but on a grid which
is common througout all the design real layout. This is to allow overlap
between two BIGVIA without risking the holes matrix to be not exactly overlapping.
As a consequence, when visualizing the |gds| file, the holes may not be centerend
inside one individual BIGVIA.

The |MBK_TO_RDS_BIGVIA_HOLE| table define the global hole matrix for the whole
design. The first number is the individual hole side and the second the grid step
(center to center).

Example: ::

    TABLE MBK_TO_RDS_BIGVIA_HOLE
    
        CONT_VIA   RDS_VIA1 0.27 0.27 ALL
        CONT_VIA2  RDS_VIA2 0.27 0.27 ALL
        CONT_VIA3  RDS_VIA3 0.27 0.27 ALL
        CONT_VIA4  RDS_VIA4 0.27 0.27 ALL
        CONT_VIA5  RDS_VIA5 0.36 0.36 ALL
    
    END


|newpage|

The |MBK_TO_RDS_BIGVIA_METAL| table
===================================

This table describe how the metal part of a BIGVIA is expanded (for the hole
part, see the previous table |MBK_TO_RDS_BIGVIA_HOLE|). The rule give for each
metal:

1. The *delta-with* (have to ask Franck).
2. The *overhang*, the length the real rectangle is expanded on each side from
   the symbolic rectange.

Example: ::

    TABLE MBK_TO_RDS_BIGVIA_METAL
    
        CONT_VIA  RDS_ALU1 0.0 0.09  ALL \
                  RDS_ALU2 0.0 0.09  ALL

        CONT_VIA2 RDS_ALU2 0.0 0.09  ALL \
                  RDS_ALU3 0.0 0.09  ALL

        CONT_VIA3 RDS_ALU3 0.0 0.09  ALL \
                  RDS_ALU4 0.0 0.09  ALL

        CONT_VIA4 RDS_ALU4 0.0 0.09  ALL \
                  RDS_ALU5 0.0 0.09  ALL

        CONT_VIA5 RDS_ALU5 0.0 0.09  ALL \
                  RDS_ALU6 0.0 0.18  ALL
    
    END


|newpage|

The |MBK_WIRESETTING| table
===========================

From a strict standpoint this table shouldn't be here but put in a separate
configuration file, because it contains informations only used by the symbolic
layout tools (|ocp|, |nero|, |ring|).

This table defines the cell gauge the routing pitch and minimal (symbolic)
wire width and minimal spacing for the routers. They are patly redundant.

Example: ::

    TABLE MBK_WIRESETTING

        X_GRID             10
        Y_GRID             10
        Y_SLICE           100
        WIDTH_VDD          12
        WIDTH_VSS          12
        TRACK_WIDTH_ALU8    0
        TRACK_WIDTH_ALU7    4
        TRACK_WIDTH_ALU6    4
        TRACK_WIDTH_ALU5    4
        TRACK_WIDTH_ALU4    3
        TRACK_WIDTH_ALU3    3
        TRACK_WIDTH_ALU2    3
        TRACK_WIDTH_ALU1    3
        TRACK_SPACING_ALU8  0
        TRACK_SPACING_ALU7  4
        TRACK_SPACING_ALU6  4
        TRACK_SPACING_ALU5  4
        TRACK_SPACING_ALU4  4
        TRACK_SPACING_ALU3  4
        TRACK_SPACING_ALU2  4
        TRACK_SPACING_ALU1  3

    END

