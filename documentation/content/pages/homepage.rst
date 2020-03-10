.. -*- Mode: rst -*-


=============================
Coriolis |VLSI| Backend Tools
=============================


:slug:    homepage
:date:    2019-12-20 16:00
:author:  Jean-Paul Chaput
:Contact: <Jean-Paul.Chaput@lip6.fr>
:Version: June 4, 2019 (jpc)


.. include:: ../../etc/definitions.rst
	 
.. contents::


Abstract
========

.. image:: {attach}/images/common/amd2901_chip_routed.png 
   :width: 50%
   :align: center
   :alt:   Routed AM2901 layout picture.


|Alliance| is a complete toolchain for |VLSI| design. It provides a |VHDL| compiler
and simulator, logic synthetiser, automatic place & route and portable |CMOS|
library. It has been in used in research projects such as the 875K transistors
|StaCS| superscalar microprocessor or the 400K transistors |IEEE| gigabit |HSL|
router. It has been actively developped during the 1990-2000 years and is maintained
since. Its practical limit for one standard cell block (flat) is about 10K gates,
above that limit you would need to use hierarchy and manually build a floorplan.
|Alliance| is entirely written in C.

|Coriolis| was started in the year 2000 as a replacement for the place & route stage
of |Alliance|. As such it is able to handle standard cells block (flat) of at least 150K
gates. It was later extended to support analog design re-implementing the methodology  
introduced by the |CIAN| team of |LIP6| / |SU| in |CAIRO| / |CAIRO+|. The tools have
been designed from the ground up to support digital only, analog only or mixed circuits.
|Coriolis| is written in a mix of C++ and |Python|.

|Alliance| / |Coriolis| is free software. All source code is realeased under the GPL_
license, except for |Hurricane| which is under LGPL_ and the Si2_ |LEF| / |DEF|
parser drivers that are under `Apache License, Version 2.0`_.

`Short introduction to Symbolic Layout. <{filename}/pages/symbolic-layout.rst>`_


Design Flow
~~~~~~~~~~~

We are also upgrading the complete design flow by replacing |Alliance| obsoleting
tools by more advanced |FOSS| alternatives:

* Yosys_ for logical synthesis.
* GHDL_ for |VHDL| simulation.



Design & Features of |Coriolis|
===============================

The |Coriolis| toolchain is build upon the |Hurricane| database.

* **Tool Integration.** To better manages the challenges arising in the
  increasingly bigger designs, the various tools have to work together
  as a whole, in a tight integration at runtime. The |Hurricane| database
  has been develop to address this problem and as a results, all |Coriolis|
  tools are built upon it and can communicate through that core database.

* **Deterministic.** The database and the tools are completely deterministics.
  Two runs in *exactly* the same context gives the same result.

* **C++ & Python.** The |Hurricane| database and all tools are fully exported
  in |Python| providing a seamless integration. The whole toolchain is build
  as a mix of C++ for computational intensive part and |Python| scripts for
  everything else. There is not even a binary, the main program *is* a
  |Python| script and can be rewritten in any way you like.

* **Graphic Interface.** |Hurricane| also come with a graphical interface
  allowing to display, explore or check your design. The graphic interface
  can be extended (in C++) do display any custom overlay.

* **Symbolic Layout.** For digital part of the designs, we use a symbolic
  layer approach. Note that |Coriolis| can make digital designs using real
  technology, but it's a largely untested feature yet.

* **Analog Design.** A complete user-guided methodology for designing analog
  circuits is available. It is fully integrated in all the tools and will
  allow mixed design in the near future.


Disclaimer
==========

|Alliance| / |Coriolis| tools, at the time being, are not suitable for deep submicron
technological nodes (below 130nm / 180nm).

For portability across foundries, technological nodes and freedom from fondries |NDA|,
|Alliance| / |Coriolis| rely on *portable* layout (or *scalable* layout, or *symbolic* layout).
To produce a valid |GDS| file, you would need to use the |s2r| translation tool which
needs a configuration file suited for the target technology, and *that* file is under
|NDA|. So, either should be written by you, or given to you through services like
MyCMP_.

