.. -*- Mode: rst -*-


2. Setting up the Environment
=============================

2.1 Setting up the Paths
~~~~~~~~~~~~~~~~~~~~~~~~~

To simplify the tedious task of configuring your environment, a helper is provided.
It will setup or modify the :cb:`PATH`, :cb:`LD_LIBRARY_PATH` (or :cb:`DYLD_LIBRARY_PATH`
under |Darwin|), :cb:`PYTHONPATH` and :cb:`CORIOLIS_TOP` variables.
It should automatically adapt to your kind of shell (``Bourne`` like
or ``C-Shell`` like). ::

    <CORIOLIS_INSTALL>/etc/coriolis2/coriolisEnv.py

Use it like this (don't forget the ``eval`` **and** the backquotes):

.. code-block:: console

   dummy@lepka:~> eval `<CORIOLIS_INSTALL>/etc/coriolis2/coriolisEnv.py`

.. note:: **Do not call that script in your environment initialisation.**
   When used under |RHEL6| or clones, it needs to be run in the |devtoolset|
   environment. The script then launches a new shell, which may cause an
   infinite loop if it's called again in, say :cb:`~/.bashrc`.

   Instead you may want to create an alias:

   .. code-block:: sh

      alias c2r='eval "`<CORIOLIS_INSTALL>/etc/coriolis2/coriolisEnv.py`"'


2.2 User's Configurations File
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

You may create, in the directory you are lanching |Coriolis| tools, a special
sub-directory ``coriolis2/`` that can contain the configuration files:

* ``__init__.py`` to tell |Python| this directory is a module. 
* ``settings.py`` can override almost any default configuration setting.

.. note:: Those two files will by processed by the |Python| interpreter,
	  so they can contain any code in addition to the mandatory
	  variables.
  

2.3 The :cb:`settings.py` File
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The attributes name and definitions of :cb:`cfg` are detailed
in `CGT - The Graphical Interface <../UsersGuide/ViewerTools.html>`_.

**Selecting the Technology**

The important line here is:

.. code-block:: Python

   import symbolic.cmos


This import loads and setup the technology used througout this run of
|Coriolis|. One and only one technology can be loaded in a |Coriolis| run.

Example of file:

.. code-block:: Python

   # -*- Mode:Python -*-
   
   import os
   import Cfg 
   import Viewer
   import CRL 
   import symbolic.cmos
   from   helpers       import overlay
   
   if 'CELLS_TOP' in os.environ:
       cellsTop = os.environ['CELLS_TOP']
   else:
       cellsTop = '../../../cells'
   
   with overlay.CfgCache(priority=Cfg.Parameter.Priority.UserFile) as cfg:
       cfg.misc.catchCore              = False
       cfg.misc.info                   = False
       cfg.misc.paranoid               = False
       cfg.misc.bug                    = False
       cfg.misc.logMode                = True
       cfg.misc.verboseLevel1          = True
       cfg.misc.verboseLevel2          = True
       cfg.misc.minTraceLevel          = 1900
       cfg.misc.maxTraceLevel          = 3000
       cfg.katana.eventsLimit          = 1000000
       cfg.katana.termSatReservedLocal = 6 
       cfg.katana.termSatThreshold     = 9 
       Viewer.Graphics.setStyle( 'Alliance.Classic [black]' )
       af  = CRL.AllianceFramework.get()
       env = af.getEnvironment()
       env.setCLOCK( '^ck$|m_clock|^clk$' )
       env.addSYSTEM_LIBRARY( library=cellsTop+'/nsxlib', mode=CRL.Environment.Prepend )
       env.addSYSTEM_LIBRARY( library=cellsTop+'/niolib', mode=CRL.Environment.Prepend )
