.. -*- Mode: rst -*-


1. Introduction
===============

This tutorial is aimed at two goals :

* Presenting how to use Python scripts to control |Coriolis|.

* Make a basic introduction about the |Hurricane| database and its
  concepts.

While this tutorial is aimed at presenting the |Hurricane| database,
do not feel limited by it. You can use |Hurricane| objects as attributes
of |Python| objects or use |Python| containers to store them.
The only limitation is that you may not use |Hurricane| classes as base
classes in |Python|.

All the example scripts given in this tutorial con be found under: ::

    <CORIOLIS_INSTALL>/share/doc/coriolis2/examples/scripts/

Provided scripts:

=======================  ==============================================================
**Script**               **Feature**
=======================  ==============================================================
:cb:`coriolisLogo.py`    Draw a layout of the |Coriolis| logo
:cb:`diagonals.py`       Test the :cb:`Hurricane::Diagonal` class
:cb:`polygons.py`        Test the :cb:`Hurricane::Polygon` class
:cb:`rectilinear.py`     Test the :cb:`Hurricane::rectilinear` class
:cb:`invertor.py`        Procedural build of the layout of an invertor standard cell
:cb:`fulladder.py`       Procedural build of a small netlist along with it's manual
                         placement and the routing of one net (:cb:`"a"`)
:cb:`toolengines.py`     Build the netlist (only) of the full adder then call the
                         place and route engines
=======================  ==============================================================


1.1 Terminology
~~~~~~~~~~~~~~~

In the |Hurricane| database, the *logical* (netlist) and *physical* (layout)
views are fused. As the main goal of the database is to support place & route
tools, we usually starts with a *pure* netlist which is progessively enriched
to become a layout. Cell, in particular, is able to be in any intermediate
state. Many of our objects have self-explanatory names, but some
don't. Thus we summarize below the more important ones:

===============  =====================================================
**Class**        **Meaning**
===============  =====================================================
Cell_            The model. A Cell does not have terminals, only nets
                 flagged as *external*
Instance_        An instance of a model
Net_             A grouping of electrically connected components
Plug_            A terminal of an instance
RoutingPad_      A physical connexion (*pin*) to an instance
===============  =====================================================


1.2 Generalities
~~~~~~~~~~~~~~~~

The C++ API has been exported in Python as closely as possible. Meaning
that, save for a slight change in syntax, any code written in |Python|
could be easily transformed into C++ code. There is no specific documentation
written for the |Python| interface, you may directly use the C++ one.

Mostly:

* C++ namespaces are exported as |Python| modules.
* The *scope resolution operator* :fboxtt:`::` converts into :fboxtt:`.`.
* C++ blocks (between braces :fboxtt:`{}`) are replaced by indentations.
* In C++, names are managed through a dedicated ``Name`` class.
  It has not been exported to the |Python| interface, you only have
  to use ``string``.
* Coordinates are expressed in ``DbU`` which are ``long`` with a special
  semantic (see ??).
* All |Hurricane| objects implements the |Python| ``__str__()`` function,
  they print the result of the C++ method ``::getString()``.

In ``hurricane/Net.h`` header we have:

.. code-block:: c++

   namespace Hurricane {

      class Net : public Entity {
        public:
	  class Direction {
            public: enum Code { DirIn        = 0x0001
                              , DirOut       = 0x0002
                              , DirUndefined = 0x0000
                              , ConnTristate = 0x0100
                              , ConnWiredOr  = 0x0200
                              , UNDEFINED    = DirUndefined
                              , IN           = DirIn
                              , OUT          =         DirOut
                              , INOUT        = DirIn | DirOut
                              , TRISTATE     =         DirOut | ConnTristate
                              , TRANSCV      = DirIn | DirOut | ConnTristate
                              , WOR_OUT      =         DirOut | ConnWiredOr
                              , WOR_INOUT    = DirIn | DirOut | ConnWiredOr
                              , DirMask      = DirIn | DirOut | DirUndefined
                              };
            // [The rest of Class Direction]
         };
        public:
          static       Net*       create       ( Cell* , const Name& );
                       bool       isGlobal     ();
                       bool       isExternal   ();
                 const Direction& getDirection ();
                       void       setName      ( Name );
                       void       setGlobal    ( bool );
                       void       setExternal  ( bool );
                       void       setDirection ( const Direction& );
          // [The rest of Class Net]
      };

   }


So we can use it the following way in C++:
 
.. code-block:: c++

   #include "hurricane/Net.h"

   using namespace Hurricane;

   void  addNetToCell ( Cell* cell )
   {
      Net* net = Net::create( cell, "new_net" );
      net->setGlobal   ( false );
      net->setExternal ( true );
      net->setDirection( Net.Direction.IN );
      cout << "Created " << net << endl;
      return net;
   }


The equivalent |Python| code will be:

.. code-block:: Python

   from Hurricane import Net

   def addNetToCell ( cell ):
       net = Net.create( cell, "new_net" )
       net.setGlobal  ( False )
       net.setExternal( True )
       net.setDirection( Net.Direction.IN )
       print( "Created", net )
       return net


1.3 Various Kinds of Constructors
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Regarding the memory allocation, the |Hurricane| database contains two kind of objects.

#. Objects that are linked to others in the database and whose creation or deletion
   implies coherency operations. This is the case for Net_ or Horizontal_.
   They must be created using the static :cb:`create()` method of their class
   and destroyed with their :cb:`destroy()` method.

   And, of course, they cannot be copied (the copy constructor has been disabled).

   .. code-block:: Python

      net = Net.create( cell, 'tmp' )  # Call the static Net.create() function.
                                       # Work with this net.
                                       # ...
      net.destroy()                    # Call the dynamic destroy() method.

#. Objects that are *standalone*, like Point_ or Box_, uses the usual construction
   methods. They also use the |Python| garbage collector mechanism and do not need
   to be explicitly deleted.

   .. code-block:: Python

      from Hurricane import DbU, Box

      def myfunc():
          bb = Box( DbU.fromLambda( 0.0)
                  , DbU.fromLambda( 0.0)
                  , DbU.fromLambda(15.0)
                  , DbU.fromLambda(50.0) )
          return                            # bb will be freed at that point.


1.4 Collections and Iterators
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Hurricane Collection_ behave like containers under |Python| and provide
support for the :cb:`iterator` protocol.

   .. code-block:: Python

      from Hurricane import Net, Horizontal

      def delAllHorizontals ( net ):
          horizontals = []
          for component in net.getComponents():
	      if isinstance(component,Horizontal):
                  horizontals.append( component )
          # Now we can remove the Horizontals.
          for horizontal in horizontals:
	      horizontal.destroy()

.. note:: **Never remove an element from a Collection_ while iterating over it**.
          You must save the to be removed elements in an auxiliary container
	  then remove them, like shown in the example above


1.5 Dynamically decorating data-base objects
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When writing algorithms directly in Python, it may come in handy to be
able to add attributes over the Hurricane data-base objects. As C++
objects exposed to the Python realm cannot natively do so (it would
means to be able to modify a C++ aobject attributes *at runtime*),
we add a special Property tasked with handling the extra Python
attributes. The syntax has been made as simple as possible.

.. code-block:: python

   from Hurricane import PythonAttributes
	  
   class MyAttribute ( object ):
       count = 0
   
       def __init__ ( self ):
           self.value = MyAttribute.count 
           print( '{} has been created'.format(self) )
           MyAttribute.count += 1
   
       def __del__ ( self ):
           print( '{} has been deleted'.format(self) )
   
       def __str__ ( self ):
           return '<MyAttribute {}>'.format(self.value)
   
   
   def demoAttributes ( cell ):
       PythonAttributes.enable( cell )
       cell.myAttribute0 = MyAttribute()
       cell.myAttribute1 = MyAttribute()
       print( 'cell.myAttribute0 =', cell.myAttribute0 )
       del cell.myAttribute0
       PythonAttributes.disable( cell )


Detailing the life cycle of Python attributes on a DBo_:

1. Enabling the addition of Python attribute on a DBo_:

   .. code-block:: python

      PythonAttributes.enable( cell )

2. Adding/removing properties on the DBo_:

   .. code-block:: python

      cell.myAttribute0 = MyAttribute()
      cell.myAttribute1 = MyAttribute()
      print( 'cell.myAttribute0 =', cell.myAttribute0 )
      del cell.myAttribute0

3. And finally disabling the use of Python attributes on the DBo.
   Any still attached Python attributes will be released.

   .. code-block:: python

      PythonAttributes.disable( cell )

   .. note:: 

      When the attributes of a DBo_ are released it does not automatically
      imply that they are removed. Their reference count is decreased, and
      if they are only referenced here, they will be deleted. But if other
      variables still holds reference onto them, they will stay allocateds.

4. There is no need to keep track of all the DBo_ that have Python
   attributes to disable them. One can directly call:

   .. code-block:: python

      PythonAttributes.disableAll()
	 

1.6 Adapting C++ : Overlay
~~~~~~~~~~~~~~~~~~~~~~~~~~

Sometimes, the use of a wrapped C++ feature would be counter intuitive regarding
the |Python| feature. For those cases the :cb:`overlay` module provide an
adaptation of the C++ API for a more *Python-like* code. A typical example is
with the UpdateSession_ mechanism.

Using directly the C++ wrapper, we would write a code like this:

.. code-block:: python

   from Hurricane import UpdateSession, Net, Vertical
   from helpers   import l

   def editCell ( cell ):
       UpdateSession.open()
       net = Net.create( cell, "nwell" )
       Vertical.create( net, nwell, l(7.5), l(15.0), l(27.0), l(51.0) )
       # Continued cell's layout building.
       # ...
       UpdateSession.close()


But, using the :cb:`overlay` we got the more *pythonic* code:

.. code-block:: python

   from Hurricane       import Net, Vertical
   from helpers         import l
   from helpers.overlay import UpdateSession

   def editCell ( cell ):
       with UpdateSession():
           net = Net.create( cell, "nwell" )
           Vertical.create( net, nwell, l(7.5), l(15.0), l(27.0), l(51.0) )
           # Continued cell's layout building.
           # ...
