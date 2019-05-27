.. -*- Mode: rst -*-
.. include:: ../etc/definitions.rst
.. include:: ./definitions.rst


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

All |Hurricane| objects implements the |Python| ``__str__()`` function,
they print the result of C++ ``::getString()``.


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
Net_             A grouping of electrically connecteds components
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

In ``hurricane/Session.h`` header we have:

.. code-block:: c++

   namespace Hurricane {

      class UpdateSession {
        public:
          static void  open  ();
          static void  close ();
      };

   }


So we can use it the following way in C++:
 
.. code-block:: c++

   #include "hurricane/Session.h"

   using namespace Hurricane;

   void  doSomething ()
   {
     UpdateSession::open();
     // Something...
     UpdateSession::close();
   }


The equivalent |Python| code will be:

.. code-block:: Python

   from Hurricane import *

   def doSomething ():
       UpdateSession.open()
      # Something...
       UpdateSession.close()


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

      def myfunc():
        bb = Box( DbU.fromLambda( 0.0)
                , DbU.fromLambda( 0.0)
                , DbU.fromLambda(15.0)
                , DbU.fromLambda(50.0) )
        return                            # bb will be freed at that point.
