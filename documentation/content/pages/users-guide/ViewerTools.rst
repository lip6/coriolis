.. -*- Mode: rst -*-


CGT - The Graphical Interface
=============================

Overview
--------

The |Coriolis| graphical interface is split up into two windows.

* The **Viewer**, with the following features:

  * Basic load/save capabilities.
  * Displays the current working cell. Could be empty if the design
    is not yet placed.
  * Executes Stratus Scripts.
  * Menu to run the tools (placement, routage).

Features are detailed in `Viewer & Tools`_.

|ViewerSnapShot_1|

* The **Controller**, which allows to:

  * Tweak what is displayed by the *Viewer*. Through the *Look*,
    *Filter* and *Layers&Gos* tabs.
  * Browse the |netlist| with eponym tab.
  * Show the list of selected objects (if any) with *selection*
  * Walk through the Database, the Cell or the Selection with *Inspector*.
    This is an advanced feature, reserved for experimented users.
  * The tab *Settings* which gives access to all the settings.
    They are closely related to Configuration & Initialisation.

|bcenter| |ControllerSnapShot_1| |ecenter|


.. _Viewer & Tools:

Viewer & Tools
--------------

|Stratus| Netlist Capture
^^^^^^^^^^^^^^^^^^^^^^^^^

|Stratus| is the replacement for |GenLib| procedural netlist capture language.
It is designed as a set of |Python| classes, and comes with it's own documentation
(`Stratus Documentation`_)


The |Hurricane| Data-Base
^^^^^^^^^^^^^^^^^^^^^^^^^

The |Alliance| flow is based on the |MBK| data-base, which has one data-structure
for each view. That is, |LOFIG| for the *logical* view and |PHFIG| for the *physical*
view. The place and route tools were responsible for maintaining (or not) the
coherency between views. Reflecting this weak coupling between views, each one
was stored in a separate file with a specific format. The *logical* view is stored
in a |vst| file in |VHDL| format and the *physical* in an |ap| file in an ad-hoc format.

The |Coriolis| flow is based on the |Hurricane| data-base, which has a unified
structure for *logical* and *physical* view. That data structure is the |Cell| object.
The |Cell| can have any state between pure netlist and completly placed and
routed design. Although the memory representation of the views has deeply
changed we still use the |Alliance| files format, but they now really represent
views of the same object. The point is that one must be very careful about
view coherency when going to and from |Coriolis|.

As for the second release, |Coriolis| can be used only for three purposes :

* **Placing a design**, in which case the |netlist| view must be present.
* **Routing a design**, in that case the |netlist|
  view and the |layout| view must be present and  |layout| view must contain
  a placement. Both views must have the same name. When saving the routed design,
  it is advised to change the design name otherwise the original unrouted placement
  in the |layout| view will be overwritten.
* **Viewing a design**, the |netlist| view must be present, if a |layout|
  view is present it still must have the same name but it can be in any
  state. 


Synthetizing and loading a design
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

|Coriolis| supports several file formats. It can load all file format
from the |Alliance| toolchain (.ap for layout, behavioural and structural vhdl .vbe and .vst),
BLIF netlist format as well as benchmark formats from the ISPD contests.

It can be compiled with LEF/DEF support, although it requires acceptance of the SI2 license
and may not be compiled in your version of the software.

Synthesis under Yosys
.....................

You can create a BLIF file from the |Yosys| synthetizer, which can be imported under Coriolis.
Most libraries are specified as a .lib liberty file and a .lef LEF file.
|Yosys| opens most .lib files with minor modifications, but LEF support in Coriolis relies on SI2.
If Coriolis hasn't been compiled against it, the library is given in |Alliance| .ap format.
`Some free libraries <http://vlsitechnology.org>`_ already provide both .ap and .lib files.

Once you have installed a common library under |Yosys| and Coriolis, just synthetize your design
with |Yosys| and import it (as Blif without the extension) under Coriolis to perform place&route.

Synthesis under Alliance
........................

|Alliance| is an older toolchain but has been extensively used for years. Coriolis can import
and write Alliance designs and libraries directly.


Etesian -- Placer
^^^^^^^^^^^^^^^^^

The |Etesian| placer is a state of the art (as of 2015) analytical placer. It is 
within ``5%`` of other placers' solutions, but is normally a bit worse than ePlace.
This |Coriolis| tool is actually an encapsulation of |Coloquinte| which *is* the placer.

.. note:: *Instance Uniquification:* a same logical instance cannot have
   two different placements. So, if you don't supply a placement for it, it will be
   uniquified (cloned) and you will see the copy files appears on disk upon saving.


|noindent|
**Hierarchical Placement**

The placement area is defined by the top cell abutment box.

When placing a complete hierarchy, the abutment boxes of the cells (models) other than
the top cell are set identical to the one of the top cell and their instances are
all placed at position ``(0,0,ID)``. That is, all the abutments boxes, whatever the
hierarchical level, define the same area (they are exactly superposed).

We choose this scheme because the placer will see all the instances as virtually
flattened, so they can be placed anywhere inside the top-cell abutment box.

|bcenter| |Etesian-1| |ecenter|


|noindent|
**Computing the Placement Area**

The placement area is computed using the ``etesian.aspectRatio`` and ``etesian.spaceMargin``
parameters only if the top-cell has an empty abutment box. If the top-cell abutment
box has to be set, then it is propagated to all the instances models recursively.


|noindent|
**Reseting the Placement**

Once a placement has been done, the placer cannot reset it (will be implemented
later). To perform a new placement, you must restart |cgt|. In addition, if you
have saved the placement on disk, you must erase any :cb:`.ap` file, which are
automatically reloaded along with the netlist (:cb:`.vst`).

|noindent|
**Limitations**

Etesian supports standard cells and fixed macros. As for the Coriolis 2.1 version,
it doesn't support movable macros, and you must place every macro beforehand.
Timing and routability analysis are not included either, and the returned placement
may be unroutable.


Etesian Configuration Parameters
................................

+-----------------------------------+------------------+----------------------------+
| Parameter Identifier              |   Type           |  Default                   |
+===================================+==================+============================+
| **Etesian Parameters**                                                            |
+-----------------------------------+------------------+----------------------------+
|``etesian.aspectRatio``            | TypePercentage   | :cb:`100`                  |
|                                   +------------------+----------------------------+
|                                   | Define the height on width ``H/W`` aspect     |
|                                   | ratio, can be comprised between 10 and 1000   |
+-----------------------------------+------------------+----------------------------+
|``etesian.spaceMargin``            | TypePercentage   | :cb:`5`                    |
|                                   +------------------+----------------------------+
|                                   | The extra white space added to the total area |
|                                   | of the standard cells                         |
+-----------------------------------+------------------+----------------------------+
|``etesian.densityVariation``       | TypePercentage   | :cb:`5`                    |
|                                   +------------------+----------------------------+
|                                   | Control deviation from uniform density in the |
|                                   | placement, as a percentage of area            |
+-----------------------------------+------------------+----------------------------+
|``etesian.effort``                 | TypeInt          | :cb:`2`                    |
|                                   +------------------+----------------------------+
|                                   | Sets the balance between the speed of the     |
|                                   | placer and the solution quality               |
+-----------------------------------+------------------+----------------------------+
|``etesian.routingDriven``          | TypeBool         | :cb:`False`                |
|                                   +------------------+----------------------------+
|                                   | Whether the tool will try routing iterations  |
|                                   | and whitespace allocation to improve          |
|                                   | routability; to be implemented                |
+-----------------------------------+------------------+----------------------------+
|``etesian.graphics``               | TypeInt          | :cb:`2`                    |
|                                   +------------------+----------------------------+
|                                   | How often the display will be refreshed       |
|                                   | More refreshing slows the placer.             |
|                                   |                                               |
|                                   | * ``1`` shows both upper and lower bounds     |
|                                   | * ``2`` only shows lower bound results        |
|                                   | * ``3`` only shows the final results          |
+-----------------------------------+-----------------------------------------------+

|newpage|


Katana -- Global Router
^^^^^^^^^^^^^^^^^^^^^^^

The quality of |Katana| global routing solutions are equivalent to those of FGR_ 1.0.
For an in-depth description of |Katana| algorithms, you may download the thesis of
D. |Dupuis| avalaible from here~: `Knik Thesis`_ (|Knik| has been rewritten as part
of |Katana|, the algorithms remains essentially the same).

The global router is now deterministic.


Katana -- Detailed Router
^^^^^^^^^^^^^^^^^^^^^^^^^

|Katana| no longer suffers from the limitations of |Nero|. It can route big designs
as its runtime and memory footprint is almost linear (with respect to the number
of gates). It has successfully routed design of more than `150K` gates.
|medskip|

.. note::
   **Slow Layer Assignment.** Most of the time, the layer assignment stage is
   fast (less than a dozen seconds), but in some instances it can take more
   than a dozen *minutes*. This is a known bug and will be corrected in later
   releases.

After each run, |Katana| displays a set of *completion ratios* which must all
be equal to `100%` or (``NNNN+0``) if the detailed routing has been successfull.
In the event of a failure, on a saturated design, you may tweak the three
following configuration parameters:

#. ``katana.hTracksReservedLocal``, the number of track reserved for local routing,
   that quantity is substracted from the edge capacities (global routing) to
   give a sense of the cluttering inside the GCells.
#. ``katana.vTracksReservedLocal``, same as above.
#. ``etesian.spaceMargin``, increases the free area of the overall design so the
   routing density decreases.

The idea is to increase the horizontal and vertical local track reservation until
the detailed router succeeds. But in doing so we make the task of the global router
more and more difficult as the capacity of the edges decreases, and at some point
it will fail too. So this is a balance.

Routing a design is done in four ordered steps:

#. Detailed pre-route :math:`\textbf{P\&R} \rightarrow \textbf{Step by Step} \rightarrow \textbf{Detailed PreRoute}`
#. Global routing     :math:`\textbf{P\&R} \rightarrow \textbf{Step by Step} \rightarrow \textbf{Global Route}`
#. Detailed routing   :math:`\textbf{P\&R} \rightarrow \textbf{Step by Step} \rightarrow \textbf{Detailed Route}`
#. Finalize routing   :math:`\textbf{P\&R} \rightarrow \textbf{Step by Step} \rightarrow \textbf{Finalize Route}`

It is possible to supply to the router a complete wiring for some nets that the user
wants to be routed according to a specific topology. The supplied topology must respect
the building rules of the |Anabatic| database (contacts must be, *terminals*, *turns*, *h-tee*
& *v-tee* only). During the first step :fboxtt:`Detailed Pre-Route` the router will solve
overlaps between the segments, without making any dogleg. If no pre-routed topologies
are present, this step may be ommited. Any net routed at this step is then fixed and
become unmovable for the later stages.

After the detailed routing step the |Katana| data-structure is still active
(the Hurricane wiring is decorated). The finalize step performs the removal of
the |Katana| data-structure, and it is not advisable to save the design before
that step.

You may visualize the density (saturation) of either the edges (global routing)
or the GCells (detailed routing) until the routing is finalized. Special layers appear
to that effect in the `The Layers&Go Tab`_.


Katana Configuration Parameters
...............................

The |Anabatic| parameters control the layer assignment step.

All the defaults value given below are from the default |Alliance| technology
(:cb:`cmos` and :cb:`SxLib` cell gauge/routing gauge).

+-----------------------------------+------------------+----------------------------+
| Parameter Identifier              |   Type           |  Default                   |
+===================================+==================+============================+
| **Anabatic Parameters**                                                           |
+-----------------------------------+------------------+----------------------------+
|``anabatic.topRoutingLayer``       | TypeString       | :cb:`METAL5`               |
|                                   +------------------+----------------------------+
|                                   | Define the highest metal layer that will be   |
|                                   | used for routing (inclusive).                 |
+-----------------------------------+------------------+----------------------------+
|``anabatic.globalLengthThreshold`` | TypeInt          | :cb:`1450`                 |
|                                   +------------------+----------------------------+
|                                   | This parameter is used by a layer assignment  |
|                                   | method which is no longer used (did not give  |
|                                   | good results)                                 |
+-----------------------------------+------------------+----------------------------+
| ``anabatic.saturateRatio``        | TypePercentage   | :cb:`80`                   |
|                                   +------------------+----------------------------+
|                                   | If ``M(x)`` density is above this ratio,      |
|                                   | move up feedthru  global segments up from     |
|                                   | depth ``x`` to ``x+2``                        |
+-----------------------------------+------------------+----------------------------+
| ``anabatic.saturateRp``           | TypeInt          | :cb:`8`                    |
|                                   +------------------+----------------------------+
|                                   | If a GCell contains more terminals            |
|                                   | (:cb:`RoutingPad`) than that number, force a  |
|                                   | move up of the connecting segments to those   |
|                                   | in excess                                     |
+-----------------------------------+------------------+----------------------------+
| **Katana Parameters**                                                             |
+-----------------------------------+------------------+----------------------------+
| ``katana.hTracksReservedLocal``   | TypeInt          | :cb:`3`                    |
|                                   +------------------+----------------------------+
|                                   | To take account the tracks needed *inside* a  |
|                                   | GCell to build the *local* routing, decrease  |
|                                   | the capacity of the edges of the global       |
|                                   | router. Horizontal and vertical locally       |
|                                   | reserved capacity can be distinguished for    |
|                                   | more accuracy.                                |
+-----------------------------------+------------------+----------------------------+
| ``katana.vTracksReservedLocal``   | TypeInt          | :cb:`3`                    |
|                                   +------------------+----------------------------+
|                                   | cf. ``kite.hTracksReservedLocal``             |
+-----------------------------------+------------------+----------------------------+
| ``katana.eventsLimit``            | TypeInt          | :cb:`4000002`              |
|                                   +------------------+----------------------------+
|                                   | The maximum number of segment displacements,  |
|                                   | this is a last ditch safety against infinite  |
|                                   | loop. It's perhaps a  little too low for big  |
|                                   | designs                                       |
+-----------------------------------+------------------+----------------------------+
| ``katana.ripupCost``              | TypeInt          | :cb:`3`                    |
|                                   +------------------+----------------------------+
|                                   | Differential introduced between two ripup     |
|                                   | costs to avoid a loop between two ripped up   |
|                                   | segments                                      |
+-----------------------------------+------------------+----------------------------+
| ``katana.strapRipupLimit``        | TypeInt          | :cb:`16`                   |
|                                   +------------------+----------------------------+
|                                   | Maximum number of ripup for *strap* segments  |
+-----------------------------------+------------------+----------------------------+
| ``katana.localRipupLimit``        | TypeInt          | :cb:`9`                    |
|                                   +------------------+----------------------------+
|                                   | Maximum number of ripup for *local* segments  |
+-----------------------------------+------------------+----------------------------+
| ``katana.globalRipupLimit``       | TypeInt          | :cb:`5`                    |
|                                   +------------------+----------------------------+
|                                   | Maximum number of ripup for *global* segments,|
|                                   | when this limit is reached, triggers topologic|
|                                   | modification                                  |
+-----------------------------------+------------------+----------------------------+
| ``katana.longGlobalRipupLimit``   | TypeInt          | :cb:`5`                    |
|                                   +------------------+----------------------------+
|                                   | Maximum number of ripup for *long global*     |
|                                   | segments, when this limit is reached, triggers|
|                                   | topological modification                      |
+-----------------------------------+------------------+----------------------------+



.. _Python Scripts in Cgt:

Executing Python Scripts in Cgt
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Python/Stratus scripts can be executed either in text or graphical mode.

.. note:: **How Cgt Locates Python Scripts:**
   |cgt| uses the Python ``import`` mechanism to load Python scripts.
   So you must give the name of your script whithout ``.py`` extension and
   it must be reachable through the ``PYTHONPATH``. You may use the
   dotted module notation.

A Python/Stratus script must contain a function called ``scriptMain()``
with one optional argument, the graphical editor into which it may be
running (will be set to ``None`` in text mode). The Python interface to
the editor (type: :cb:`CellViewer`) is limited to basic capabilities
only.

Any script given on the command line will be run immediatly *after* the
initializations and *before* any other argument is processed.

For more explanation on Python scripts see `Python Interface to Coriolis`.


Printing & Snapshots
^^^^^^^^^^^^^^^^^^^^

Printing or saving into a |pdf| is fairly simple, just use the **File -> Print**
menu or the |CTRL_P| shortcut to open the dialog box.

The print functionality uses exactly the same rendering mechanism as for the
screen, beeing almost *WYSIWYG*. Thus, to obtain the best results it is advisable
to select the ``Coriolis.Printer`` look (in the *Controller*), which uses a
white background and well suited for high resolutions ``32x32`` pixels patterns  

There is also two modes of printing selectable through the *Controller*
**Settings -> Misc -> Printer/Snapshot Mode**:

===============  =================  =====================================================
Mode             DPI (approx.)      Intended Usage
---------------  -----------------  -----------------------------------------------------
**Cell Mode**    150                For single ``Cell`` printing or very small designs.
                                    Patterns will be bigger and more readable. 
**Design Mode**  300                For designs (mostly commposed of wires and cells
                                    outlines).
===============  =================  =====================================================

.. note:: *The pdf file size*
          Be aware that the generated |pdf| files are indeed only pixmaps.
          So they can grew very large if you select paper format above ``A2``
          or similar.


|noindent|
Saving into an image is subject to the same remarks as for |pdf|.


Memento of Shortcuts in Graphic Mode
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The main application binary is |cgt|.

+---------------+-------------------+-----------------------------------------------------------+
| Category      | Keys              | Action                                                    |
+===============+===================+===========================================================+
| **Moves**     | | |KeyUp|,        | Shifts the view in the according direction                |
|               |   |KeyDown|       |                                                           |
|               | | |KeyLeft|,      |                                                           |
|               |   |KeyRight|      |                                                           |
+---------------+-------------------+-----------------------------------------------------------+
| **Fit**       |   |KeyF|          | Fits to the Cell abutment box                             |
+---------------+-------------------+-----------------------------------------------------------+
| **Refresh**   |   |CTRL_L|        | Triggers a complete display redraw                        |
+---------------+-------------------+-----------------------------------------------------------+
| **Goto**      |   |KeyG|          | *apperture* is the minimum side of the area               |
|               |                   | displayed around the point to go to. It's an              |
|               |                   | alternative way of setting the zoom level                 |
+---------------+-------------------+-----------------------------------------------------------+
| **Zoom**      |   |KeyZ|,         | Respectively zoom by a 2 factor and *unzoom*              |
|               |   |KeyM|          | by a 2 factor                                             |
|               +-------------------+-----------------------------------------------------------+
|               | | |BigMouse|      | You can perform a zoom to an area.                        |
|               | | Area Zoom       | Define the zoom area by *holding down the left            |
|               |                   | mouse button* while moving the mouse.                     |
+---------------+-------------------+-----------------------------------------------------------+
| **Selection** | | |BigMouse|      | You can select displayed objects under an area.           |
|               | | Area Selection  | Define the selection area by *holding down the            |
|               |                   | right mouse button* while moving the mouse.               |
|               +-------------------+-----------------------------------------------------------+
|               | | |BigMouse|      | You can toggle the selection of one object under          |
|               | | Toggle Selection| the mouse position by pressing |CTRL| and                 |
|               |                   | pressing down *the right mouse button*. A popup           |
|               |                   | list of what's under the position shows up into           |
|               |                   | which you can toggle the selection state of one           |
|               |                   | item.                                                     |
|               +-------------------+-----------------------------------------------------------+
|               |   |KeyCapS|       | Toggle  the selection visibility                          |
+---------------+-------------------+-----------------------------------------------------------+
| **Controller**| |CTRL_I|          | Show/hide the controller window.                          |
|               |                   |                                                           |
|               |                   | It's the Swiss Army Knife of the viewer.                  |
|               |                   | From it, you can fine-control the display and             |
|               |                   | inspect almost everything in your design.                 |
+---------------+-------------------+-----------------------------------------------------------+
| **Rulers**    | |KeyK|,           | One stroke on |KeyK| enters the ruler mode, in            |
|               | |KeyESC|          | which you can draw one ruler. You can exit the            |
|               |                   | ruler mode by pressing |KeyESC|. Once in ruler            |
|               |                   | mode, the first click on the *left mouse button*          |
|               |                   | sets the ruler's starting point and the second            |
|               |                   | click the ruler's end point. The second click             |
|               |                   | exits automatically the ruler mode.                       |
|               +-------------------+-----------------------------------------------------------+
|               | |KeyCapK|         | Clears all the drawn rulers                               |
+---------------+-------------------+-----------------------------------------------------------+
| **Print**     | |CTRL_P|          | Currently rather crude. It's a direct copy of             |
|               |                   | what's displayed in pixels. So the resulting              |
|               |                   | picture will be a little blurred due to                   |
|               |                   | anti-aliasing mechanism.                                  |
+---------------+-------------------+-----------------------------------------------------------+
| **Open/Close**| |CTRL_O|          | Opens a new design. The design name must be               |
|               |                   | given without path or extention.                          |
|               +-------------------+-----------------------------------------------------------+
|               | |CTRL_W|          | Closes the current viewer window, but does not quit       |
|               |                   | the application.                                          |
|               +-------------------+-----------------------------------------------------------+
|               | |CTRL_Q|          | `CTRL+Q` quits the application                            |
|               |                   | (closing all windows).                                    |
+---------------+-------------------+-----------------------------------------------------------+
| **Hierarchy** | |CTRL_Down|       | Goes one hierarchy level down. That is, if there          |
|               |                   | is an *instance* under the cursor position, loads         |
|               |                   | its *model* Cell in place of the current one.             |
|               +-------------------+-----------------------------------------------------------+
|               | |CTRL_Up|         | Goes one hierarchy level up. If we have entered           |
|               |                   | the current model through |CTRL_Down|                     |
|               |                   | reloads the previous model (the one                       |
|               |                   | in which this model is instanciated).                     |
+---------------+-------------------+-----------------------------------------------------------+


Cgt Command Line Options
^^^^^^^^^^^^^^^^^^^^^^^^

Appart from the obvious ``--text`` options, all can be used for text and graphical mode.

+-----------------------------+------------------------------------------------+
| Arguments                   | Meaning                                        |
+=============================+================================================+
| `-t|--text`                 | Instructs |cgt| to run in text mode.           |
+-----------------------------+------------------------------------------------+
| `-L|--log-mode`             | Disables the use of |ANSI| escape sequence on  |
|                             | the |tty|. Useful when the output is           |
|                             | redirected to a file.                          |
+-----------------------------+------------------------------------------------+
| `-c <cell>|--cell=<cell>`   | The name of the design to load, without        |
|                             | leading path or extention.                     |
+-----------------------------+------------------------------------------------+
| `-m <val>|--margin=<val>`   | Percentage *val* of white space for the placer |
|                             | (|Etesian|).                                   |
+-----------------------------+------------------------------------------------+
| `--events-limit=<count>`    | The maximal number of events after which the   |
|                             | router will stop. This is mainly a failsafe    |
|                             | against looping. The limit is set to 4         |
|                             | millions of iteration which should suffice to  |
|                             | any design of `100K`. gates. For bigger        |
|                             | designs you may want to increase this limit.   |
+-----------------------------+------------------------------------------------+
| `-G|--global-route`         | Runs the global router (|Katana|).             |
+-----------------------------+------------------------------------------------+
| `-R|--detailed-route`       | Runs the detailed router (|Katana|).           |
+-----------------------------+------------------------------------------------+
| `-s|--save-design=<routed>` | The design into which the routed layout will   |
|                             | be saved. It is strongly recommanded to choose |
|                             | a different name from the source (unrouted)    |
|                             | design.                                        |
+-----------------------------+------------------------------------------------+
| `--stratus-script=<module>` | Run the Python/Stratus script ``module``.      |
|                             | See `Python Scripts in Cgt`_.                  |
+-----------------------------+------------------------------------------------+

|newpage|


Some Examples :

* Run both global and detailed router, then save the routed design:

  .. code-block:: sh

      > cgt -v -t -G -R --cell=design --save-design=design_r


Miscellaneous Settings
^^^^^^^^^^^^^^^^^^^^^^

+---------------------------------------+------------------+----------------------------+
| Parameter Identifier                  |   Type           |  Default                   |
+=======================================+==================+============================+
| **Verbosity/Log Parameters**                                                          |
+---------------------------------------+------------------+----------------------------+
| ``misc.info``                         | TypeBool         | :cb:`False`                |
|                                       +------------------+----------------------------+
|                                       | Enables display of *info* level message       |
|                                       | (:cb:`cinfo` stream)                          |
+---------------------------------------+------------------+----------------------------+
| ``misc.bug``                          | TypeBool         | :cb:`False`                |
|                                       +------------------+----------------------------+
|                                       | Enables display of *bug* level message        |
|                                       | (:cb:`cbug` stream), messages can be a little |
|                                       | scarry                                        |
+---------------------------------------+------------------+----------------------------+
| ``misc.logMode``                      | TypeBool         | :cb:`False`                |
|                                       +------------------+----------------------------+
|                                       | If enabled, assumes that the output device    |
|                                       | is not a ``tty`` and suppresses any escape    |
|                                       | sequences                                     |
+---------------------------------------+------------------+----------------------------+
| ``misc.verboseLevel1``                | TypeBool         | :cb:`True`                 |
|                                       +------------------+----------------------------+
|                                       | First level of verbosity, disables level 2    | 
+---------------------------------------+------------------+----------------------------+
| ``misc.verboseLevel2``                | TypeBool         | :cb:`False`                |
|                                       +------------------+----------------------------+
|                                       | Second level of verbosity                     | 
+---------------------------------------+------------------+----------------------------+
| **Development/Debug Parameters**                                                      |
+---------------------------------------+------------------+----------------------------+
| ``misc.minTraceLevel``                | TypeInt          | :cb:`0`                    |
+---------------------------------------+------------------+----------------------------+
| ``misc.maxTraceLevel``                | TypeInt          | :cb:`0`                    |
|                                       +------------------+----------------------------+
|                                       | Displays trace information *between* those two|
|                                       | levels (:cb:`cdebug` stream)                  | 
+---------------------------------------+------------------+----------------------------+
| ``misc.catchCore``                    | TypeBool         | :cb:`False`                |
|                                       +------------------+----------------------------+
|                                       | By default, |cgt| does not dump core.         |
|                                       | To generate one set this flag to :cb:`True`   |
+---------------------------------------+------------------+----------------------------+

|newpage|


.. _The Controller:

The Controller
--------------

The *Controller* window is composed of seven tabs:

#. `The Look Tab`_ to select the display style.
#. `The Filter Tab`_ the hierarchical levels to be displayed, the look of
   rubbers and the dimension units.
#. `The Layers&Go Tab`_ to selectively hide/display layers.
#. `The Netlist Tab`_ to browse through the |netlist|. Works in association
   with the *Selection* tab.
#. `The Selection Tab`_ allows to view all the currently selected elements.
#. `The Inspector Tab`_ browses through either the DataBase, the Cell or
   the current selection.
#. `The Settings Tab`_ accesses all the tool's configuration settings.


.. _The Look Tab:

The Look Tab
^^^^^^^^^^^^

You can select how the layout will be displayed. There is a special one
``Printer.Coriolis`` specifically designed for `Printing & Snapshots`_.
You should select it prior to calling the print or snapshot dialog boxes.

|bcenter| |ControllerLook_1| |ecenter|

|newpage|


.. _The Filter Tab:

The Filter Tab
^^^^^^^^^^^^^^

The filter tab let you select what hierarchical levels of your design will be
displayed. Hierarchy level are numbered top-down: the level 0 corresponds to
the top-level cell, the level one to the instances of the top-level Cell and
so on.

There are also check boxes to enable/disable the processing of Terminal Cell,
Master Cells and Components. The processing of Terminal Cell (hierarchy leaf
cells) is disabled by default when you load a hierarchical design and enabled
when you load a single Cell.

You can choose what kind of form to give to the rubbers and the type of
unit used to display coordinates.

.. note:: *What are Rubbers:* |Hurricane| uses *Rubbers* to materialize
   physical gaps in net topology. That is, if some wires are missing to
   connect two or more parts of net, a *rubber* will be drawn between them
   to signal the gap.

   For example, after the detailed routing no *rubber* should remain.
   They have been made *very* visible as big violet lines...

|bcenter| |ControllerFilter_1| |ecenter|

|newpage|


.. _The Layers&Go Tab:

The Layers&Go Tab
^^^^^^^^^^^^^^^^^

Control the individual display of all *layers* and *Gos*.

* *Layers* correspond to true physical layers. From a |Hurricane| point of
  view they are all the *BasicLayers* (could be matched to GDSII).
* *Gos* stands from *Graphical Objects*, they are drawings that have no
  physical existence but are added by the various tools to display extra
  information. One good exemple is the density map of the detailed router,
  to easily locate congested areas.

For each layer/Go there are two check boxes:

* The normal one triggers the display.
* The red-outlined allows objects of that layer to be selectable or not.

|bcenter| |ControllerLayersGos_1| |ecenter|


.. _The Netlist Tab:

The Netlist Tab
^^^^^^^^^^^^^^^

The *Netlist* tab shows the list of nets... By default the tab is not
*synched* with the displayed Cell. To see the nets you must check the
**Sync Netlist** checkbox. You can narrow the set of displayed nets by
using the filter pattern (supports regular expressions).

A very useful feature is to enable the **Sync Selection**, which will
automatically select all the components of the selected net(s). You can
select multiple nets. In the figure the net ``auxsc35`` is selected and
is highlighted in the *Viewer*.

|bcenter| |ControllerNetlist_1| |ecenter|
|bcenter| |ViewerNetlist_1|     |ecenter|


.. _The Selection Tab:

The Selection Tab
^^^^^^^^^^^^^^^^^

The *Selection* tab lists all the components currently selected. They
can be filtered thanks to the filter pattern.

Used in conjunction with the *Netlist* **Sync Selection** you will all see
all the components part of *net*.

In this list, you can toggle individually the selection of component by
pressing the ``t`` key. When unselected in this way a component is not
removed from the the selection list but instead displayed in red italic.
To see where a component is you may make it blink by repeatedly press
the ``t`` key...

|bcenter| |ControllerSelection_1| |ecenter|


.. _The Inspector Tab:

The Inspector Tab
^^^^^^^^^^^^^^^^^

This tab is very useful, but mostly for |Coriolis| developpers. It allows
to browse through the live DataBase. The *Inspector* provides three entry points:

* **DataBase**: Starts from the whole |Hurricane| DataBase.
* **Cell**: Inspects the currently loaded Cell.
* **Selection**: Inspects the object currently highlighted in the *Selection* tab.

Once an entry point has been activated, you may recursively expore all
its fields using the right/left arrows.

.. note:: *Do not put your fingers in the socket:* when inspecting 
   anything, do not modify the DataBase. If any object under inspection
   is deleted, you will crash the application...

.. note:: *Implementation Detail:* the inspector support is done with
   ``Slot``, ``Record`` and ``getString()``.
   
|bcenter| |ControllerInspector_1| |ecenter|
|bcenter| |ControllerInspector_2| |ecenter|
|bcenter| |ControllerInspector_3| |ecenter|


.. _The Settings Tab:

The Settings Tab
^^^^^^^^^^^^^^^^

Here comes the description of the *Settings* tab.

|bcenter| |ControllerSettings_1| |ecenter|
