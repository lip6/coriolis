.. -*- Mode: rst -*-
.. include:: ../etc/definitions.rst
.. include:: ./definitions.rst


.. _2.1 The Alliance Framework:  ./CellNetComponent.html#the-allianceframework-crl-core


4. Manipulating Cells, Nets and Components
==========================================

In this part, we will show how to navigate through the Nets_ and Components_ of a Cell_.


4.1 Hurricane Collections
~~~~~~~~~~~~~~~~~~~~~~~~~

In |Hurricane| all kind of set of objects, whether organized in a real container
like a ``map<>`` (dictionary / ``dict``) or a ``vector<>`` (table / ``list``) or
an algorithmic walkthrough of the database can be accessed through a Collection_.

C++ Collections object are exposed in |Python| through the *iterable* protocol,
allowing to simply write:

.. code-block:: Python

   for net in cell.getNets():
     print 'Components of', net
     for component in net.getComponents():
       print '|', component


In C++ we would have written:

.. code-block:: C++

   for ( Net* net : cell->getNets() ) {
     cout << "Components of " << net << endl;
     for ( Component* component : net->getComponents() ) {
       cout << "| " << component << endl,
     }
   }


4.1.1 Restrictions about using Collections
------------------------------------------

**Never delete or create an element while you are iterating over a Collection.**

Results can be unpredictable, you may just end up with a core dump, but more
subtly, some element of the Collection_ may be skipped or processed twice.
If you want to create or delete an element, do it outside the collection
loop. For example:

.. code-block:: Python

   cellNets = []
   for net in cell.getNets():
     cellNets.append( net )

   # Remove all the anonymous nets.
   for net in cellNets:
      if net.getName().endswith('nymous_'):
        print 'Destroy', net
        net.destroy()



4.2 Loading a Cell with AllianceFramework
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

As presented in `2.1 The Alliance Framework`_, the Cell_ that will be returned by
the ``getCell()`` call wil be:

#. If a Cell_ of that name is already loaded into memory, it will be returned.

   .. note:: It means that it shadows any modifications that could have been on
             disk since it was first loaded.  Conversely, if the Cell_ has been
             modified in memory, you will get those modifications.

#. Search, in the ordered list of libraries, the first Cell_ that matches the
   requested name.

   .. note:: It means that if cells with the same name exist in different
	     libraries, only the one in the first library will be ever used.
	     Be also weary of cell files that may remain in the ``WORK_LIB``,
	     they may unexpectedly shadow cells from the libraries.
	     

.. code-block:: Python

   cell = af.getCell( 'inv_x1', Catalog.State.Views )

