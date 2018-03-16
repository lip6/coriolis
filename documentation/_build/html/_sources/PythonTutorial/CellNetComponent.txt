.. -*- Mode: rst -*-
.. include:: ../etc/definitions.rst
.. include:: ./definitions.rst


3. Creating Cell, Net and Component
=======================================

In this part, we well show how to create and save a single Cell_.


3.1 The AllianceFramework (CRL Core)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The |Hurricane| database only manage objects in memory. To load or save
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
an UpdateSession_. At the closing of a session, created or modificateds
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


This is the simplest call to ``createCell()``, and it that case, the newly
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
two conversion functions are provideds:

* ``unit = DbU.fromLambda( lbd )`` convert a lambda :cb:`lbd` into a ``DbU``.
* ``lbd = DbU.toLambda( unit )`` convert a ``DbU`` into a lambda :cb:`lbd`.

In the weakly typed |Python| world, :cb:`lbd` is *float* while :cb:`unit`
is *integer*.


3.5 Setting up the Abutment Box
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To setup the abutment box, we use a Box_ which define a box from
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
will belongs to. In that example we also make it an *external* net, that is,
a part of the interface. Do not mistake the name of the net given as a string
argument :cb:`'i'` and the name of the *variable* :cb:`i` holding the Net_ object.
For the sake of clarity we try to give the variable a close name, but this is
not mandatory.

.. code-block:: Python

   i = Net.create( cell, 'i' )
   i.setExternal( True )


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
to declare some of it's components as physical connectors usable by the router.
This is done by calling the NetExternalComponents_ class:

.. code-block:: Python

   NetExternalComponents.setExternal( segment )


3.7 Saving to Disk (CRL Core)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Once you are finished building your cell, you have to save it on disk.
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
