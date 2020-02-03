.. -*- Mode: rst -*-


.. URLs that changes between the various backends.
.. _Stratus Documentation:          file:///usr/share/doc/coriolis2/en/html/stratus/index.html
				      
.. .. |ChipStructure-1|                image:: ./images/chip-structure-1.png
..                                     :alt:   Chip Top Structure
..                                     :align: middle
..                                     :width: 90%


.. _Python Interface to Coriolis:

|newpage|


Python Interface for |Hurricane| / |Coriolis|
=============================================

The (almost) complete interface of |Hurricane| is exported as a |Python| module
and some part of the other components of |Coriolis| (each one in a separate
module). The interface has been made to mirror as closely as possible the
C++ one, so the C++ doxygen documentation could be used to write code with
either languages.

`Summary of the C++ Documentation <file:../../../index.html>`_

A script could be run directly in text mode from the command line or through
the graphical interface (see `Python Scripts in Cgt`).

Aside for this requirement, the python script can contain anything valid
in |Python|, so don't hesitate to use any package or extention.

Small example of Python/Stratus script:

.. code-block:: python

   from Hurricane import *
   from Stratus   import *

   def doSomething ():
       # ...
       return

   def ScriptMain ( **kw ):
     editor = None
     if kw.has_key('editor') and kw['editor']:
       editor = kw['editor']
       stratus.setEditor( editor )

     doSomething()
     return
   
   if __name__ == "__main__" :
     kw           = {}
     success      = ScriptMain( **kw )
     shellSuccess = 0
     if not success: shellSuccess = 1
     
     sys.exit( shellSuccess )
         ScriptMain ()

This typical script can be executed in two ways:

#. Run directly as a |Python| script, thanks to the

   .. code-block:: python

      if __name__ == "__main__" :

   part (this is standart |Python|). It is a simple adapter that will
   calls  :cb:`ScriptMain()`.
#. Through |cgt|, either in text or graphical mode. In that case, the
   :cb:`ScriptMain()` is directly called trough a sub-interpreter.
   The arguments of the script are passed through the ``**kw`` dictionnary.
   
   +----------------------+-----------------------------------------------+
   | \*\*kw Dictionnary                                                   |
   +----------------------+-----------------------------------------------+
   | Parameter Key/Name   | Contents type                                 |
   +======================+===============================================+
   | ``'cell'``           | A Hurricane cell on which to work. Depending  |
   |                      | on the context, it may be ``None``.           |
   |                      | For example, when run from |cgt|, it the cell |
   |                      | currently loaded in the viewer, if any.       |
   +----------------------+-----------------------------------------------+
   | ``'editor'``         | The viewer from which the script is run, when |
   |                      | lauched through |cgt|.                        |
   +----------------------+-----------------------------------------------+


Plugins
~~~~~~~

Plugins are |Python| scripts specially crafted to integrate with |cgt|.
Their entry point is a :cb:`ScriptMain()` method as described in 
`Python Interface to Coriolis`_. They can be called by user scripts
through this method.



Chip Placement
--------------

Automatically perform the placement of a complete chip. This plugin, as well
as the other P&R tools expect a specific top-level hierarchy for the design.
The top-level hierarchy must contains the instances of all the I/O pads and
**exactly one** instance named ``corona`` of an eponym cell ``corona``.
The ``corona`` cell in turn containing the instance of the chip's core model.

The intermediate ``corona`` hierarchical level has been introduced to handle
the possible discoupling between real I/O pads supplied by a foundry and a
symbolic core. So the *chip* level contains only real layout and the corona
and below only symbolic layer.

.. note:: This do not prevent having a design either fully symbolic (pads and core)
	  or fully real.

.. note:: The ``corona`` also avoid the router to actually have to manage directly
	  the pads which simplificate it's configuration and accessorily avoid
	  to have the pads stuffed with blockages.

|bcenter| |ChipStructure-1| |ecenter|

The designer must provide a configuration file that defines the rules for the
placement of the top-level hierarchy (that is, the pads and the core).
This file must be named after the chip's name, by appending ``_ioring.py``
(obviously, it is a |Python| file). For instance if the chip netlist file
is called ``amd2901.vst``, then the configuration file must be named
``amd2901_ioring.vst``.

Example of chip placement configuration file (for ``AM2901``):

.. code-block:: python

   from helpers import l, u, n

   chip = \
     { 'pads.ioPadGauge' : 'pxlib'
     , 'pads.south'      : [ 'p_a3'     , 'p_a2'     , 'p_a1'     , 'p_r0'
                           , 'p_vddick0', 'p_vssick0', 'p_a0'     , 'p_i6'
                           , 'p_i8'     , 'p_i7'     , 'p_r3'     ]
     , 'pads.east'       : [ 'p_zero'   , 'p_i0'     , 'p_i1'     , 'p_i2'
                           , 'p_vddeck0', 'p_vsseck0', 'p_q3'     , 'p_b0'
                           , 'p_b1'     , 'p_b2'     , 'p_b3'     ]
     , 'pads.north'      : [ 'p_noe'    , 'p_y3'     , 'p_y2'     , 'p_y1'
                           , 'p_y0'     , 'p_vddeck1', 'p_vsseck1', 'p_np'
                           , 'p_ovr'    , 'p_cout'   , 'p_ng'     ]
     , 'pads.west'       : [ 'p_cin'    , 'p_i4'     , 'p_i5'     , 'p_i3'
                           , 'p_ck'     , 'p_d0'     , 'p_d1'     , 'p_d2'
                           , 'p_d3'     , 'p_q0'     , 'p_f3'     ]
     , 'core.size'       : ( l(1500), l(1500) )
     , 'chip.size'       : ( l(3000), l(3000) )
     , 'chip.clockTree'  : True
     }

The file must contain *one dictionnary* named ``chip``.
   
+----------------------+-------------------------------------------------------+
| Chip Dictionnary                                                             |
+----------------------+-------------------------------------------------------+
| Parameter Key/Name   | Value/Contents type                                   |
+======================+=======================================================+
| ``'pad.ioPadGauge'`` | The routing gauge to use for the pad. Must be given   |
|                      | as it differs from the one used to route standard     |
|                      | inside the core                                       |
+----------------------+-------------------------------------------------------+
| ``'pad.south'``      | Ordered list (left to right) of pad instances names   |
|                      | to put on the south side of the chip                  |
+----------------------+-------------------------------------------------------+
| ``'pad.east'``       | Ordered list (down to up) of pad instances names      |
|                      | to put on the east side of the chip                   |
+----------------------+-------------------------------------------------------+
| ``'pad.north'``      | Ordered list (left to right) of pad instances names   |
|                      | to put on the north side of the chip                  |
+----------------------+-------------------------------------------------------+
| ``'pad.west'``       | Ordered list (down to up) of pad instances names      |
|                      | to put on the west side of the chip                   |
+----------------------+-------------------------------------------------------+
| ``'core.size'``      | The size of the core (to be used by the placer)       |
+----------------------+-------------------------------------------------------+
| ``'chip.size'``      | The size of the whole chip. The sides must be great   |
|                      | enough to accomodate all the pads                     |
+----------------------+-------------------------------------------------------+
| ``'chip.clockTree'`` | Whether to generate a clock tree or not. This calls   |
|                      | the ClockTree plugin                                  |
+----------------------+-------------------------------------------------------+

Configuration parameters, defaults are defined in ``etc/coriolis2/<STECHNO>/plugins.conf``.

+-----------------------------------+------------------+----------------------------+
| Parameter Identifier              |   Type           |  Default                   |
+===================================+==================+============================+
| **Chip Plugin Parameters**                                                        |
+-----------------------------------+------------------+----------------------------+
|``chip.block.rails.count``         | TypeInt          | :cb:`5`                    |
|                                   +------------------+----------------------------+
|                                   | The minimum number of rails around the core   |
|                                   | block. Must be odd and suppérior to 5.        |
|                                   | One rail for the clock and at least two pairs |
|                                   | of power/grounds                              |
+-----------------------------------+------------------+----------------------------+
|``chip.block.rails.hWidth``        | TypeInt          | :cb:`12` |lambda|          |
|                                   +------------------+----------------------------+
|                                   | The horizontal with of the rails              |
+-----------------------------------+------------------+----------------------------+
|``chip.block.rails.vWidth``        | TypeInt          | :cb:`12` |lambda|          |
|                                   +------------------+----------------------------+
|                                   | The vertical with of the rails                |
+-----------------------------------+------------------+----------------------------+
|``chip.block.rails.hSpacing``      | TypeInt          | :cb:`6`  |lambda|          |
|                                   +------------------+----------------------------+
|                                   | The spacing, *edge to edge* of two adjacent   |
|                                   | horizontal rails                              |
+-----------------------------------+------------------+----------------------------+
|``chip.block.rails.vSpacing``      | TypeInt          | :cb:`6`  |lambda|          |
|                                   +------------------+----------------------------+
|                                   | The spacing, *edge to edge* of two adjacent   |
|                                   | vertical rails                                |
+-----------------------------------+------------------+----------------------------+

.. note::
   If no clock tree is generated, then the clock rail is *not* created.
   So even if the requested number of rails ``chip.block.rails.count`` is, say 5,
   only four rails (2* ``power``, 2* ``ground``) will be generateds.


Clock Tree
----------

Inserts a clock tree into a block. The clock tree uses the H strategy.
The clock net is splitted into sub-nets, one for each branch of the
tree.

* On **chip** design, the sub-nets are created in the model of the
  core block (then trans-hierarchically flattened to be shown at
  chip level).
* On **blocks**, the sub nets are created directly in the top block.
* The sub-nets are named according to a simple geometrical scheme.
  A common prefix ``ck_htree``, then one postfix by level telling
  on which quarter of plane the sub-clock is located:

  #. ``_bl``: bottom left plane quarter.
  #. ``_br``: bottom right plane quarter.
  #. ``_tl``: top left plane quarter.
  #. ``_tr``: top right plane quarter.

  We can have ``ck_htree_bl``, ``ck_htree_bl_bl``, ``ch_htree_bl_tl`` and so on.

The clock tree plugin works in four steps:

#. Builds the clock tree: creates the top-block abutment box, compute the
   required levels of H tree and places the clock buffers.
#. Once the clock buffers are placed, calls the placer (|etesian|) to place
   the ordinary standard cells, whithout disturbing clock H-tree buffers.
#. At this point we know the exact positions of all the DFFs, so we can
   connect them to the nearest H-tree leaf clock signal.
#. Leaf clock signals that are not connected to any DFFs are removed.

Netlist reorganisation:

* Obviously the top block or chip core model netlist is modified to
  contain all the clock sub-nets. The interface is *not* changed.
* If the top block contains instances of other models *and* those models
  contain DFFs that get re-connected to the clock sub-nets (from the
  top level). Changes both the model netlist and interface to propagate
  the relevant clock sub-nets to the instanciated model. The new model
  with the added clock signal is renamed with a ``_cts`` suffix.
  For example, the sub-block model ``ram.vst`` will become ``ram_cts.vst``.

.. note::
  If you are to re-run the clock tree plugin on a netlist, be careful
  to erase any previously generated ``_cts`` file (both netlist and
  layout: ``rm *_cts.{ap,vst}``). And restart |cgt| to clear its
  memory cache.

Configuration parameters, defaults are defined in ``etc/coriolis2/<STECHNO>/plugins.conf``.

+-----------------------------------+------------------+----------------------------+
| Parameter Identifier              |   Type           |  Default                   |
+===================================+==================+============================+
| **ClockTree Plugin Parameters**                                                   |
+-----------------------------------+------------------+----------------------------+
|``clockTree.minimumSide``          | TypeInt          | :cb:`300` |lambda|         |
|                                   +------------------+----------------------------+
|                                   | The minimum size below which the clock tree   |
|                                   | will stop to perform quadri-partitions        |
+-----------------------------------+------------------+----------------------------+
|``clockTree.buffer``               | TypeString       | :cb:`buf_x2`               |
|                                   +------------------+----------------------------+
|                                   | The buffer model to use to drive sub-nets     |
+-----------------------------------+------------------+----------------------------+


Recursive-Save (RSave)
----------------------

Performs a recursive top down save of all the models from the top cell
loaded in |cgt|. Forces a write of any non-terminal model. This plugin is used
by the clock tree plugin after the netlist clock sub-nets creation.


A Simple Example: AM2901
~~~~~~~~~~~~~~~~~~~~~~~~

To illustrate the capabilities of |Coriolis| tools and |Python| scripting, a small
example, derived from the |Alliance| :cb:`AM2901` is supplied.

This example contains only the synthetized netlists and the :cb:`doChip.py` script
which perform the whole P&R of the design.

You can generate the chip using one of the following method:

#. **Command line mode:** directly run the script:

   .. code-block:: sh

      dummy@lepka:AM2901> ./doChip -V --cell=amd2901

#. **Graphic mode:** launch |cgt|, load chip netlist ``amd2901`` (the top cell)
   then run the |Python| script :cb:`doChip.py`.

.. note::
   Between two consecutive run, be sure to erase the netlist/layout generateds:

   .. code-block:: sh

      dummy@lepka:AM2901> rm *_cts*.vst *.ap
