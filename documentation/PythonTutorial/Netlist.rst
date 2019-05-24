.. -*- Mode: rst -*-
.. include:: ../etc/definitions.rst
.. include:: ./definitions.rst


6. Making a hierarchical Cell -- Netlist
========================================

To illustrate the topic, we will build the netlist of a fulladder from
standard cell.

|fulladder_1|


6.1 Creating an Instance_
~~~~~~~~~~~~~~~~~~~~~~~~~

Creating an Instance_ is straigthforward, the constructor needs only three
parameters:

#. The Cell_ **into which** the instance is to be created.
#. The name of the instance.
#. The **master cell**, the Cell_ model it refers to. The master cell
   will be part of the hierarchical level just below the ``fulladder``
   cell.

.. note:: Do not confuse the cell the instance is create into (``fulladder``)
	  and the cells it refers to (the *master cell* ``xr2_x2``).

.. code-block:: Python

   af     = AllianceFramework.get()
   xr2_x2 = af.getCell( 'xr2_x1', Catalog.State.Views )
    
   fulladder = af.createCell( 'fulladder' )
   xr2_1     = Instance.create( fulladder, 'xr2_1', xr2_x2 )
    

6.2 Creating Nets and connecting to Instances
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

An Instance_ as one Plug_ for each external net of the *master cell*.
The plug allows to create a **logical** connection bewteen a Net_ of
``fulladder`` and a net from an Instance_ *master cell*.

A plug is somewhat equivalent to an *instance terminal* in other
well known databases.

Therefore, a plug is related to two nets:

#. The net of the *master cell* it is linked to. Obviously that
   net cannot be changed. You can access the master net with the
   function ``plug.getMasterNet()``.

#. The net of ``fulladder`` the plug is connected to. This can
   be set, it is how we build the netlist. To set the net, use
   the function ``plug.setNet( net )``. If the argument is ``None``,
   the plug is *disconnected*.

To find the plug of an instance associated to a given net in
the *master cell*, use ``instance.getPlug( masterNet )``.
The ``masterNet`` argument being an object of class net (not
its name).

Building the :cb:`a` net of ``fulladder``:

.. code-block:: Python

   a = Net.create( fulladder, "a" )
   a.setExternal( True )
   xr2_1.getPlug( xr2_x2.getNet("i0") ).setNet( a )
   a2_1 .getPlug(  a2_x2.getNet("i0") ).setNet( a )


.. note:: **Limitation of Hurricane Netlists.** There is no explicit
	  terminal object in the |Hurricane| database. Plugs are
          generated *on the fly* for each *external net* of the
          instance. One important consequence is that a *net*
	  cannot appear on the interface as two differently named
	  terminals (sometimes referred as *weekly connected*
	  terminals). There is a strict bijection between external
	  nets and plugs.

	  While it may be restrictive, it enforces cleaner designs
          and make it possible for the HyperNet_ concept/class.


6.3 Power supplies special case
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For supplies, it may  be tedious to connect the Plugs_ of each  cell one by one
(and  create a  lot of  uneeded objects).  To avoid  that, we  may use  **Named
connections**.  If a  signal in ``fulladder`` is set to  *global*, then it will
be considered as connected  to any signal with the *same  name* and *global* in
the master cell of the instances.

.. code-block:: Python

   vdd = Net.create( fulladder, "vdd" )
   vdd.setExternal( True )
   vdd.setGlobal  ( True )  # Will be connected to all the instances.
	  

6.4 Creating the physical view of a Cell netlist
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Even if loaded in the viewer, an Instance will not be displayed
until it is placed.


6.4.1 Transformation
--------------------

To place an Instance, we apply a Transformation_ to the coordinate system
of the *master cell*. A transformation is composed of two operations:

#. An Orientation_, which can be a symmetry or a rotation (or a combination
   of those two). The Orientation **is applied first** to the coordinate
   system of the *master cell*. The complete list of Orientation and their
   codes are given on the Orientation documentation page.

#. A **Translation**, applied in second. Translation are represented by
   Points_.

The transformation is a change of coordinate system, be aware that if the
abutment box lower left corner of the *master* cell is **not** at ``(0,0)``
the result of the Transformation may not be what you expect. To simplify
the computation of the transformation of an instance, always place the
lower left corner of the abutment box at ``(0,0)``


6.4.2 Placing an Instance
-------------------------

Assuming that we want to place the cells of the ``fulladder`` into two rows,
that the abutment box lower left corner is at ``(0,0)`` (same for the
``xr2_x2`` *master cell* layout). Here is the code to place the ``xr2_1``
instance to left of the second row.

Setting the translation on an Instance_ is not enough to make it be displayed,
we also must set its *placement status* to ``Instance.PlacementStatus.PLACED``. 

.. code-block:: Python

   xr2_1.setTransformation( Transformation( DbU.fromLambda(  0.0)
                                          , DbU.fromLambda(100.0)
                                          , Transformation.Orientation.MY ) )
   xr2_1.setPlacementStatus( Instance.PlacementStatus.PLACED )


6.4.3 Nets -- From Plugs to RoutingPads
---------------------------------------

As was stated before, Plugs_ represent a logical connection between two
levels of hierarchy. To make the physical connection to the *master net*
in the instance, we now must create, in the ``fulladder``, a special
component which is a kind of *reference* to a component of the
*master net* (in the master cell).

The so called *special component* is a RoutingPad_.

The ``RoutingPad`` can be considered as an equivalent to ``pin`` in other
well known databases.

.. code-block:: Python

   rp = RoutingPad.create( a
                         , Occurrence( xr2_1.getPlug( xr2_x2.getNet("i0")) )
			 , RoutingPad.BiggestArea )

For the second parameter, we must pass an Occurrence_. Occurrence objects will
be explained in detail later, for now, let say that we must construct the
Occurrence object with one parameter : the Plug_ for which we want to create a
physical connection.

The RoutingPad_ ``rp`` will be a component of the ``a`` net.

The third argument ask the constructor of the RoutingPad_ to select in the
master net, the component which has the biggest area.

.. note:: **Component selection.** Not all the components of a net can be
	  selected for connection through a RoutingPad_. The candidates must
	  have been flagged with the NetExternalComponents_ class.

	  See `3.6.3 Creating a Component`_.


6.4.4 Nets -- Regular wiring
----------------------------

After the creation of the RoutingPads_, the wiring is to be created with
ordinary layout components (Horizontal_, Vertical_ and Contact_ possibly
articulated). Here is the complete code for net ``a``. We made an articulated
layout where contacts are created over RoutingPads_ then segments over
contacts.
	  
.. code-block:: Python

   # Build wiring for a.
   # Create RoutingPads first.
    rp1      = RoutingPad.create( a
                                , Occurrence( xr2_1.getPlug( xr2_x2.getNet("i0")) )
                                , RoutingPad.BiggestArea )
    rp2      = RoutingPad.create( a
                                , Occurrence(  a2_1.getPlug(  a2_x2.getNet("i0")) )
                                , RoutingPad.BiggestArea )

   # Then regular wiring.
    contact1 = Contact.create( rp1, via12, toDbU( 0.0), toDbU(-15.0) )
    contact2 = Contact.create( rp2, via12, toDbU( 0.0), toDbU( 10.0) )
    turn     = Contact.create( a  , via23, toDbU(10.0), toDbU( 35.0) )
    Horizontal.create( contact2, turn     , metal2, toDbU(35.0), toDbU(2.0) )
    Vertical  .create( turn    , contact1 , metal3, toDbU(10.0), toDbU(2.0) )


.. note:: In order to better see the layout of the wiring only, open the
	  ``Controller`` and in the :fboxtt:`Filter` tab, uncheck
          :fboxtt:`Process Terminal Cells`.

		   
6.5 The Complete Example File
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The example file ``fulladder.py`` can be found in the ``share/doc/coriolis2/examples/scripts/``
directory (under the the root of the |Coriolis| installation).

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
   
   
   def buildFulladder ( editor ):
   
      # Get the Framework and all the master cells.
       af     = AllianceFramework.get()
       xr2_x2 = af.getCell( 'xr2_x1', Catalog.State.Views )
       a2_x2  = af.getCell( 'a2_x2' , Catalog.State.Views )
       o2_x2  = af.getCell( 'o2_x2' , Catalog.State.Views )
   
       UpdateSession.open()
   
       fulladder = af.createCell( 'fulladder' )
       fulladder.setAbutmentBox( Box( toDbU(0.0), toDbU(0.0), toDbU(90.0), toDbU(100.0) ) )
   
       if editor:
         UpdateSession.close()
         editor.setCell( fulladder )
         editor.fit()
         UpdateSession.open()
       
      # Create Instances.
       a2_1  = Instance.create( fulladder,  'a2_1',  a2_x2 )
       a2_2  = Instance.create( fulladder,  'a2_2',  a2_x2 ) 
       xr2_1 = Instance.create( fulladder, 'xr2_1', xr2_x2 )
       xr2_2 = Instance.create( fulladder, 'xr2_2', xr2_x2 )
       o2_1  = Instance.create( fulladder,  'o2_1',  o2_x2 )
       
      # Create Nets.
       vss = Net.create( fulladder, "vss" )
       vss.setExternal( True )
       vss.setGlobal  ( True )
     
       vdd = Net.create( fulladder, "vdd" )
       vdd.setExternal( True )
       vdd.setGlobal  ( True )

       cin = Net.create( fulladder, "cin" )
       cin.setExternal( True )
       xr2_2.getPlug( xr2_x2.getNet('i0') ).setNet( cin )
       a2_2 .getPlug(  a2_x2.getNet('i0') ).setNet( cin )
       
       a = Net.create( fulladder, 'a' )
       a.setExternal( True )
       xr2_1.getPlug( xr2_x2.getNet('i0') ).setNet( a )
       a2_1 .getPlug(  a2_x2.getNet('i0') ).setNet( a )
       
       b = Net.create( fulladder, 'b' )
       b.setExternal( True )
       xr2_1.getPlug( xr2_x2.getNet('i1') ).setNet( b )
       a2_1 .getPlug(  a2_x2.getNet('i1') ).setNet( b )
       
       sout_1 = Net.create( fulladder, 'sout_1' )
       xr2_1.getPlug( xr2_x2.getNet('q' ) ).setNet( sout_1 )
       xr2_2.getPlug( xr2_x2.getNet('i1') ).setNet( sout_1 )
       a2_2 .getPlug(  a2_x2.getNet('i1') ).setNet( sout_1 )
       
       carry_1 = Net.create( fulladder, 'carry_1' )
       a2_1.getPlug( a2_x2.getNet('q' ) ).setNet( carry_1 )
       o2_1.getPlug( o2_x2.getNet('i1') ).setNet( carry_1 )
   
       carry_2 = Net.create( fulladder, 'carry_2' )
       a2_2.getPlug( a2_x2.getNet('q' ) ).setNet( carry_2 )
       o2_1.getPlug( o2_x2.getNet('i0') ).setNet( carry_2 )
   
       sout = Net.create( fulladder, 'sout' )
       sout.setExternal( True )
       xr2_2.getPlug( xr2_x2.getNet('q') ).setNet( sout )
       
       cout = Net.create( fulladder, 'cout' )
       cout.setExternal( True )
       o2_1.getPlug( o2_x2.getNet('q') ).setNet( cout )
   
      # Instances placement.
       a2_1.setTransformation( Transformation( toDbU(0.0)
                                             , toDbU(0.0)
                                             , Transformation.Orientation.ID ) )
       a2_1.setPlacementStatus( Instance.PlacementStatus.PLACED )
       doBreak( 1, 'Placed a2_1' )
   
       xr2_1.setTransformation( Transformation( toDbU(  0.0)
                                              , toDbU(100.0)
                                              , Transformation.Orientation.MY ) )
       xr2_1.setPlacementStatus( Instance.PlacementStatus.PLACED )
       doBreak( 1, 'Placed xr2_1' )
   
       a2_2.setTransformation( Transformation( toDbU(25.0)
                                             , toDbU( 0.0)
                                             , Transformation.Orientation.ID ) )
       a2_2.setPlacementStatus( Instance.PlacementStatus.PLACED )
       doBreak( 1, 'Placed a2_2' )
   
       xr2_2.setTransformation( Transformation( toDbU( 45.0)
                                              , toDbU(100.0)
                                              , Transformation.Orientation.MY ) )
       xr2_2.setPlacementStatus( Instance.PlacementStatus.PLACED )
       doBreak( 1, 'Placed xr2_2' )
   
       o2_1.setTransformation( Transformation( toDbU(65.0)
                                             , toDbU( 0.0)
                                             , Transformation.Orientation.ID ) )
       o2_1.setPlacementStatus( Instance.PlacementStatus.PLACED )
       doBreak( 1, 'Placed o2_1' )
   
      # Add filler cells.
       tie_x0    = af.getCell(    'tie_x0', Catalog.State.Views )
       rowend_x0 = af.getCell( 'rowend_x0', Catalog.State.Views )
       filler_1  = Instance.create( fulladder, 'filler_1',    tie_x0 )
       filler_2  = Instance.create( fulladder, 'filler_2', rowend_x0 )
   
       filler_1.setTransformation( Transformation( toDbU(50.0)
                                                 , toDbU( 0.0)
                                                 , Transformation.Orientation.ID ) )
       filler_1.setPlacementStatus( Instance.PlacementStatus.PLACED )
   
       filler_2.setTransformation( Transformation( toDbU(60.0)
                                                 , toDbU( 0.0)
                                                 , Transformation.Orientation.ID ) )
       filler_2.setPlacementStatus( Instance.PlacementStatus.PLACED )
       doBreak( 1, 'Filler cell placeds' )
       
      # Getting the layers.
       technology = DataBase.getDB().getTechnology()
       metal2     = technology.getLayer( "METAL2" ) 
       metal3     = technology.getLayer( "METAL3" ) 
       via12      = technology.getLayer( "VIA12"  ) 
       via23      = technology.getLayer( "VIA23"  ) 
   
      # Build wiring for a.
      # Create RoutingPads first.
       rp1      = RoutingPad.create( a
                                   , Occurrence( xr2_1.getPlug( xr2_x2.getNet("i0")) )
                                   , RoutingPad.BiggestArea )
       rp2      = RoutingPad.create( a
                                   , Occurrence(  a2_1.getPlug(  a2_x2.getNet("i0")) )
                                   , RoutingPad.BiggestArea )
   
      # Then regular wiring.
       contact1 = Contact.create( rp1, via12, toDbU( 0.0), toDbU(-15.0) )
       contact2 = Contact.create( rp2, via12, toDbU( 0.0), toDbU( 10.0) )
       turn     = Contact.create( a  , via23, toDbU(10.0), toDbU( 35.0) )
       Horizontal.create( contact2, turn     , metal2, toDbU(35.0), toDbU(2.0) )
       Vertical  .create( turn    , contact1 , metal3, toDbU(10.0), toDbU(2.0) )
   
       UpdateSession.close()

       af.saveCell( fulladder, Catalog.State.Views )
       return
   
   
   def ScriptMain ( **kw ):
       editor = None
       if kw.has_key('editor') and kw['editor']:
         editor = kw['editor']
   
       buildFulladder( editor )
       return True 
