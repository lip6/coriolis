.. -*- Mode: rst -*-


9. Advanced Topics
==================

This is a place holder as well as a reminder to myself to write this part of
the documentation.


9.1 Occurrence
~~~~~~~~~~~~~~

The trans-hierarchical workhorse.


9.2 RoutingPads
~~~~~~~~~~~~~~~

Unlike the Plugs_ that only make connections between two **adjacent**
hierarchical levels, RoutingPads_ can refer to a deeply buried terminal.


9.3 HyperNets
~~~~~~~~~~~~~

This class is part of the *virtual flattening* mechanism, it allows to
go through all the components of a trans-hierarchical net.


9.4 Miscellaeous trans-hierarchical functions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For a starter, how to get all the leaf cells...


9.5 Dynamically decorating data-base objects
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
   

     
