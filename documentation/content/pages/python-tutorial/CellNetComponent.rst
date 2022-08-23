.. -*- Mode: rst -*-



3. Making a Standard Cell -- Layout
===================================

In this part, we will show how to create and save a terminal Cell_,
that is, a cell without instances (the end point of a hierarchical
design). To illustrate the case we will draw the layout of a
standard cell.

We will introduce the following classes : Cell_, Net_, Component_
and its derived classes.


3.1 The AllianceFramework (CRL Core)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The |Hurricane| database only manages objects in memory. To load or save
something from the outside, we need to use a *framework*. As of today, only
one is available : the Alliance framework. It allows |Coriolis| to handle
|Alliance| libraries and cells in the exact same way.

.. note:: To see how the AllianceFramework_ is configured for your
	  installation, please have a look to ``alliance.conf`` in the
          ``etc/coriolis2`` directory. It must contains the same settings
	  as the various ``MBK_`` variables used for |Alliance|.
	  

3.2 Session Mechanism (Hurricane)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In the |Hurricane| database, all modifications must take place inside
an UpdateSession_. At the closing of a session, created or modificated
objects are fully inserted in the database. This is especially true for
the visualisation, a created component will be visible *only* only after
the session close.

.. note:: See ``QuadTree`` and ``Query``.


3.3 Creating a new Cell (CRL Core)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The creation of a new Cell_ occurs through the AllianceFramework_,
and, as stated above, inside a UpdateSession_. The AllianceFramework_
environment is provided by the |CRL| module.

.. code-block:: Python

   from Hurricane import *
   from CRL       import *
   from helpers.overlay import UpdateSession

   af = AllianceFramework.get()
   with UpdateSession():
       cell = af.createCell( 'my_inv' )
       # Build then save the Cell.


This is the simplest call to ``createCell()``, and in that case, the newly
created Cell_ will be saved in the *working library* (usually, the current
directory). You may supply a second argument telling into which library
you want the Cell_ to be created.

In the |Hurricane| Cell_ object, there is no concept of *view*, it contains
completly fused logical (netlist) and physical (layout) views.


3.4 The DbU Measurement Unit
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

All coordinates in the |Hurricane| database are expressed in DbU_
(for *Database Unit*) which are integer numbers of foundry grid.
To be more precise, they are fixed points numbers expressed in
hundredth of foundry grid (to allow transient non-integer
computation).

To work with symbolic layout, that is, using lambda based lengths,
two conversion functions are provided:

* ``unit = DbU.fromLambda( lbd )`` convert a lambda :cb:`lbd` into a ``DbU``.
* ``lbd = DbU.toLambda( unit )`` convert a ``DbU`` into a lambda :cb:`lbd`.

In the weakly typed |Python| world, :cb:`lbd` is *float* while :cb:`unit`
is *integer*.

In order to reduce the number of characters one has to code, the :cb:`helpers`
module provides three very short function to perform conversion *towards*
DbU_ :

.. code-block:: Python

   def l ( value ):
       """Convert a lambda into a DbU."""
       return DbU.fromLambda( value )

   def u ( value ):
       """Convert a length in micrometer into a DbU."""
       return DbU.fromPhysical( value, Hurricane.DbU.UnitPowerMicro )

   def n ( value ):
       """Convert a length in nanometer into a DbU."""
       return DbU.fromPhysical( value, Hurricane.DbU.UnitPowerNano  )


3.5 Setting up the Abutment Box
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To setup the abutment box, we use a Box_ which defines a box from
the coordinates of the lower left corner ``(x1,y1)`` and upper left
corner ``(x2,y2)``.

.. code-block:: Python

   b = Box( l( 0.0)    # x1
          , l( 0.0)    # y1
          , l(15.0)    # x2
          , l(50.0) )  # y2
   cell.setAbutmentBox( b )

Or more simply:

.. code-block:: Python

   cell.setAbutmentBox( Box( l( 0.0), l( 0.0), l(15.0), l(50.0) ) ) 


3.6 Adding Nets and Components
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In the |Hurricane| terminology, a **component** is any kind of physical object
among:

* Contact_
* Pad_
* RoutingPad_
* Horizontal_
* Vertical_
* Plug_ is the only exception and will be detailed later (see ??).

Components_ cannot be created *alone*. They must be part of a Net_.


3.6.1 Getting a Layer
---------------------

As physical elements, Components_ are created using a Layer_. So prior to
their creation, we must get one from the database. Layers_ are stored in the
Technology_, which in turn, is stored in the DataBase_. So, to get a
Layer_:

.. code-block:: Python

   layer = DataBase.getDB().getTechnology().getLayer( 'METAL1' )


.. note:: **Convention for layer names.** As the database can manage both real layers
	  and symbolic ones we adopt the following convention:

	  * **Real layers** are named in lowercase (``metal1``, ``nwell``).
	  * **Symbolic layers** are named in uppercase (``METAL1``, ``NWELL``).


3.6.2 Creating a Net
--------------------

As said above, prior to creating any Component_, we must create the Net_ it
will belong to. In that example we also make it an *external* net, that is,
a part of the interface. Do not mistake the name of the net given as a string
argument :cb:`'i'` and the name of the *variable* :cb:`i` holding the Net_ object.
For the sake of clarity we try to give the variable a close name, but this is
not mandatory.

.. code-block:: Python

   i = Net.create( cell, 'i' )
   i.setExternal( True )

.. note:: Unlike some other database models, in |Hurricane|,
	  **there is no explicit terminal object**, you only need to make the
	  net external. For more information about how to connect to an
	  external net, see `6.2 Creating Nets and connecting to Instances`_.
	  
 
3.6.3 Creating a Component
--------------------------

Finally, we get ready to create a Component_, we will make a Vertical_ segment
of ``METAL1``.

.. code-block:: Python

   segment = Vertical.create( i            # The owner Net.
                            , layer        # The layer.
			    , l(  5.0 )    # The X coordinate.
			    , l(  2.0 )    # The width.
			    , l( 10.0 )    # The Y source coordinate.
			    , l( 40.0 ) )  # The Y target coordinate.

With this overload of the ``Vertical.create()`` function the segment is created at an
absolute position. There is a second overload for creating a relatively placed
segment, see *articulated layout*.

If the net is external, that is, part of the interface of the cell, you may have
to declare some of its components as physical connectors usable by the router.
This is done by calling the NetExternalComponents_ class:

.. code-block:: Python

   NetExternalComponents.setExternal( segment )


3.7 Saving to Disk (CRL Core)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Once you have finished to build your cell, you have to save it on disk.
Using the AllianceFramework_ you can save it as a pair of file:

=========================  ===================================  =======================
View                       Flag                                 File extension
=========================  ===================================  =======================
Logical / Netlist          ``Catalog.State.Logical``            ``.vst``
Physical / Layout          ``Catalog.State.Physical``           ``.ap``
=========================  ===================================  =======================

To save both views, use the ``Catalog.State.Views`` flag. The files
will be written in the |Alliance| ``WORK_DIR``.

.. code-block:: Python

   af.saveCell( cell, Catalog.State.Views )

		   
3.8 The Complete Example File
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The example files can be found in the ``share/doc/coriolis2/examples/scripts/``
directory (under the the root of the |Coriolis| installation).

The code needed to run it through the |cgt| viewer has been added. For the
explanation of that part of the code, refer to `5. Make a script runnable through cgt`_.


.. code-block:: Python

   #!/usr/bin/python
   
   import sys
   from   Hurricane import DataBase, NetExternalComponents, Net, \
                           DbU, Point, Box, Horizontal, Vertical, Contact, RoutingPad, \
                           Breakpoint
   from   CRL       import AllianceFramework, Catalog
   from   helpers   import l
   from   helpers.overlay import UpdateSession
   
   
   def doBreak ( level, message ):
       """Put a breakpoint into the script."""
       Breakpoint.stop( level, message )
   
   
   def buildInvertor ( editor ):
       """Build step by step an invertor standard cell."""
       with UpdateSession():
           cell = AllianceFramework.get().createCell( 'invertor' )
           cell.setTerminalNetlist( True )
           cell.setAbutmentBox( Box( l(0.0), l(0.0), l(15.0), l(50.0) ) )
   
       if editor:
         editor.setCell( cell )
         editor.fit()
       
       with UpdateSession():
           technology = DataBase.getDB().getTechnology()
           metal1     = technology.getLayer( "METAL1"     ) 
           poly       = technology.getLayer( "POLY"       )
           ptrans     = technology.getLayer( "PTRANS"     )
           ntrans     = technology.getLayer( "NTRANS"     )
           pdif       = technology.getLayer( "PDIF"       )
           ndif       = technology.getLayer( "NDIF"       )
           contdifn   = technology.getLayer( "CONT_DIF_N" )
           contdifp   = technology.getLayer( "CONT_DIF_P" )
           nwell      = technology.getLayer( "NWELL"      )
           contpoly   = technology.getLayer( "CONT_POLY"  )
           ntie       = technology.getLayer( "NTIE"       )
           
           net = Net.create( cell, "nwell" )
           Vertical.create( net, nwell, l(7.5), l(15.0), l(27.0), l(51.0) )
           
           vdd = Net.create( cell, "vdd" )
           vdd.setExternal( True )
           vdd.setGlobal  ( True )
           h = Horizontal.create(vdd, metal1, l(47.0), l(6.0), l(0.0), l(15.0) )
           NetExternalComponents.setExternal( h )
           Contact.create ( vdd, contdifn, l(10.0), l(47.0), l( 1.0), l( 1.0) )
           Contact.create ( vdd, contdifp, l( 4.0), l(45.0), l( 1.0), l( 1.0) )
           Vertical.create( vdd, pdif    , l( 3.5), l( 4.0), l(28.0), l(46.0) )
           Vertical.create( vdd, ntie    , l(10.0), l( 3.0), l(43.0), l(48.0) )
       doBreak( 1, 'Done building vdd.' )
       
       with UpdateSession():
           vss = Net.create( cell, "vss" )
           vss.setExternal( True )
           vss.setGlobal  ( True )
           h = Horizontal.create(vss, metal1, l(3.0), l(6.0), l(0.0), l(15.0))
           NetExternalComponents.setExternal( h )
           Vertical.create( vss, ndif    , l(3.5), l(4.0), l(4.0), l(12.0) )
           Contact.create ( vss, contdifn, l(4.0), l(5.0), l(1.0), l( 1.0) )
       doBreak( 1, 'Done building vss.' )
       
       with UpdateSession():
           i = Net.create( cell, "i" )
           i.setExternal( True )
           v = Vertical.create ( i, metal1, l(5.0), l(2.0), l(10.0), l(40.0) )
           NetExternalComponents.setExternal( v )
           Vertical.create  ( i, ptrans  , l( 7.0), l( 1.0), l(26.0), l(39.0) )
           Vertical.create  ( i, ntrans  , l( 7.0), l( 1.0), l( 6.0), l(14.0) )
           Vertical.create  ( i, poly    , l( 7.0), l( 1.0), l(14.0), l(26.0) )
           Horizontal.create( i, poly    , l(20.0), l( 3.0), l( 4.0), l( 7.0) )
           Contact.create   ( i, contpoly, l( 5.0), l(20.0), l( 1.0), l( 1.0) )
       doBreak( 1, 'Done building i.' )
       
       with UpdateSession():
           nq = Net.create ( cell, "nq" )
           nq.setExternal( True )
           v = Vertical.create( nq, metal1, l(10.0), l(2.0), l(10.0), l(40.0) )
           NetExternalComponents.setExternal( v )
           Vertical.create( nq, pdif    , l(10.0), l( 3.0), l(28.0), l(37.0) )
           Vertical.create( nq, ndif    , l(10.0), l( 3.0), l( 8.0), l(12.0) )
           Contact.create ( nq, contdifp, l(10.0), l(35.0), l( 1.0), l( 1.0) )
           Contact.create ( nq, contdifp, l(10.0), l(30.5), l( 1.0), l( 1.0) )
           Contact.create ( nq, contdifn, l(10.0), l(10.0), l( 1.0), l( 1.0) )
       doBreak( 1, 'Done building q.' )
   
       AllianceFramework.get().saveCell( cell, Catalog.State.Views )
   
   
   def scriptMain ( **kw ):
       """The Mandatory function to be run by Coriolis interactively."""
       editor = None
       if 'editor' in kw and kw['editor']:
           editor = kw['editor']
       buildInvertor( editor )
       return True 
