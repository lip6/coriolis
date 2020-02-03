.. -*- Mode: rst -*-
.. include:: ../etc/definitions.rst
.. include:: ./definitions.rst



5. Make a script runnable through |cgt|
=======================================

To use your script you may run it directly like any other |Python| script.
But, for debugging purpose it may be helpful to run it through the
interactive layout viewer |cgt|.

For |cgt| to be able to run your script, you must add to your script
file a function named :cb:`ScriptMain()`, which takes a dictionary
as sole argument (:cb:`**kw`). The ``kw`` dictionary contains, in
particular, the CellViewer_ object we are running under with the
keyword ``editor``. You can then load your cell into the viewer
using the menu:

* :fboxtt:`Tools` |rightarrow| :fboxtt:`Python Script`. The script
  file name must be given without the ``.py`` extension.

.. note:: If you use breakpoints and want to see the progress of your
	  script in the viewer, do not use the ``--script`` option of
	  |cgt|.

	  .. code-block:: console

             ego@home:~> cgt -V --script=invertor

	  Because the script is run **before** the viewer is launched,
	  you will only see the end result of your script.
  

.. code-block:: Python

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

       # The rest of the script...

       return


   def ScriptMain ( **kw ):
       editor = None
       if kw.has_key('editor') and kw['editor']:
         editor = kw['editor']
   
       buildInvertor( editor )
       return True 


5.1 Using Breakpoints
~~~~~~~~~~~~~~~~~~~~~

It is possible to add breakpoints inside a script by calling the ``Breakpoint.stop()``
function. To be able to see exactly what has just been mofied, we must close the
UpdateSession_ just before calling the breakpoint and reopen it just after.
The ``Breakpoint.stop()`` function takes two arguments:

#. The ``level`` above which it will be active.
#. An informative message about the purpose of the breakpoint.

We can create a little function to ease the work:

.. code-block:: Python

   def doBreak ( level, message ):
       UpdateSession.close()
       Breakpoint.stop( level, message )
       UpdateSession.open()
