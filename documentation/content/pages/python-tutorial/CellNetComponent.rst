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

   af = AllianceFramework.get()
   UpdateSession.open()

   cell = af.createCell( 'my_inv' )

   # Build then save the Cell.

   UpdateSession.close()


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


3.5 Setting up the Abutment Box
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To setup the abutment box, we use a Box_ which defines a box from
the coordinates of the lower left corner ``(x1,y1)`` and upper left
corner ``(x2,y2)``.

.. code-block:: Python

   b = Box( DbU.fromLambda( 0.0)    # x1
          , DbU.fromLambda( 0.0)    # y1
          , DbU.fromLambda(15.0)    # x2
          , DbU.fromLambda(50.0) )  # y2
   cell.setAbutmentBox( b )

Or more simply:

.. code-block:: Python

   cell.setAbutmentBox( Box( DbU.fromLambda( 0.0)
                           , DbU.fromLambda( 0.0)
                           , DbU.fromLambda(15.0)
                           , DbU.fromLambda(50.0) ) )


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

   segment = Vertical.create( i                         # The owner Net.
                            , layer                     # The layer.
			    , DbU.fromLambda(  5.0 )    # The X coordinate.
			    , DbU.fromLambda(  2.0 )    # The width.
			    , DbU.fromLambda( 10.0 )    # The Y source coordinate.
			    , DbU.fromLambda( 40.0 ) )  # The Y target coordinate.

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
   from   Hurricane import *
   from   CRL       import *
   
   
   def toDbU ( l ): return DbU.fromLambda(l)
   
   
   def doBreak ( level, message ):
       UpdateSession.close()
       Breakpoint.stop( level, message )
       UpdateSession.open()
   
   
   def buildInvertor ( editor ):
       UpdateSession.open()
   
       cell = AllianceFramework.get().createCell( 'invertor' )
       cell.setTerminal( True )
   
       cell.setAbutmentBox( Box( toDbU(0.0), toDbU(0.0), toDbU(15.0), toDbU(50.0) ) )
   
       if editor:
         UpdateSession.close()
         editor.setCell( cell )
         editor.fit()
         UpdateSession.open()
       
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
       Vertical.create( net, nwell, toDbU(7.5), toDbU(15.0), toDbU(27.0), toDbU(51.0) )
       
       vdd = Net.create( cell, "vdd" )
       vdd.setExternal( True )
       vdd.setGlobal  ( True )
       h = Horizontal.create(vdd, metal1, toDbU(47.0), toDbU(6.0), toDbU(0.0), toDbU(15.0) )
       NetExternalComponents.setExternal( h )
       Contact.create ( vdd, contdifn, toDbU(10.0), toDbU(47.0), toDbU( 1.0), toDbU( 1.0) )
       Contact.create ( vdd, contdifp, toDbU( 4.0), toDbU(45.0), toDbU( 1.0), toDbU( 1.0) )
       Vertical.create( vdd, pdif    , toDbU( 3.5), toDbU( 4.0), toDbU(28.0), toDbU(46.0) )
       Vertical.create( vdd, ntie    , toDbU(10.0), toDbU( 3.0), toDbU(43.0), toDbU(48.0) )
       doBreak( 1, 'Done building vdd.' )
       
       vss = Net.create( cell, "vss" )
       vss.setExternal( True )
       vss.setGlobal  ( True )
       h = Horizontal.create(vss, metal1, toDbU(3.0), toDbU(6.0), toDbU(0.0), toDbU(15.0))
       NetExternalComponents.setExternal( h )
       Vertical.create( vss, ndif    , toDbU(3.5), toDbU(4.0), toDbU(4.0), toDbU(12.0) )
       Contact.create ( vss, contdifn, toDbU(4.0), toDbU(5.0), toDbU(1.0), toDbU( 1.0) )
       doBreak( 1, 'Done building vss.' )
       
       i = Net.create( cell, "i" )
       i.setExternal( True )
       v = Vertical.create ( i, metal1, toDbU(5.0), toDbU(2.0), toDbU(10.0), toDbU(40.0) )
       NetExternalComponents.setExternal( v )
       Vertical.create  ( i, ptrans  , toDbU( 7.0), toDbU( 1.0), toDbU(26.0), toDbU(39.0) )
       Vertical.create  ( i, ntrans  , toDbU( 7.0), toDbU( 1.0), toDbU( 6.0), toDbU(14.0) )
       Vertical.create  ( i, poly    , toDbU( 7.0), toDbU( 1.0), toDbU(14.0), toDbU(26.0) )
       Horizontal.create( i, poly    , toDbU(20.0), toDbU( 3.0), toDbU( 4.0), toDbU( 7.0) )
       Contact.create   ( i, contpoly, toDbU( 5.0), toDbU(20.0), toDbU( 1.0), toDbU( 1.0) )
       doBreak( 1, 'Done building i.' )
       
       nq = Net.create ( cell, "nq" )
       nq.setExternal( True )
       v = Vertical.create( nq, metal1, toDbU(10.0), toDbU(2.0), toDbU(10.0), toDbU(40.0) )
       NetExternalComponents.setExternal( v )
       Vertical.create( nq, pdif    , toDbU(10.0), toDbU( 3.0), toDbU(28.0), toDbU(37.0) )
       Vertical.create( nq, ndif    , toDbU(10.0), toDbU( 3.0), toDbU( 8.0), toDbU(12.0) )
       Contact.create ( nq, contdifp, toDbU(10.0), toDbU(35.0), toDbU( 1.0), toDbU( 1.0) )
       Contact.create ( nq, contdifp, toDbU(10.0), toDbU(30.5), toDbU( 1.0), toDbU( 1.0) )
       Contact.create ( nq, contdifn, toDbU(10.0), toDbU(10.0), toDbU( 1.0), toDbU( 1.0) )
       doBreak( 1, 'Done building q.' )
   
       UpdateSession.close()
       AllianceFramework.get().saveCell( cell, Catalog.State.Views )

       return
   
   
   def scriptMain ( **kw ):
       editor = None
       if kw.has_key('editor') and kw['editor']:
         editor = kw['editor']
   
       buildInvertor( editor )
       return True 
