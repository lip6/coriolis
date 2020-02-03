.. -*- Mode: rst -*-
.. include:: ../etc/definitions.rst
.. include:: ./definitions.rst




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

   Instead you may want to create an alias: ::

       alias c2r='eval "`<CORIOLIS_INSTALL>/etc/coriolis2/coriolisEnv.py`"'


2.2 User's Configurations File
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

You may create, in the directory you are lanching |Coriolis| tools, a special
sub-directory ``.coriolis2/`` that can contain two configuration files:

* ``techno.py`` tells which technology to use.
* ``settings.py`` can override almost any default configuration setting.

Those two files are *optional*, if they do not exist the default settings
will be used and the technology is ``symbolic/cmos`` (i.e. purely symbolic).

.. note:: Those two files will by processed by the |Python| interpreter,
	  so they can contain any code in addition to the mandatory
	  variables.

2.2.1 The :cb:`techno.py` File
------------------------------

Must provide one variable named :cb:`technology` which values the path towards
the technology file. The available technologies are installed under
``<CORIOLIS_INSTALL>/etc/coriolis2``. For example, to use the 45nm FreeDPK
which is in: ::

    <CORIOLIS_INSTALL>/etc/coriolis2/45/freepdk_45/

The ``techno.py`` file must contain:

.. code-block:: Python

   technology = '45/freepdk_45'
  

2.2.2 The :cb:`settings.py` File
--------------------------------

The entries of the ``parametersTable`` and their definitions are detailed
in `CGT - The Graphical Interface <../UsersGuide/ViewerTools.html>`_.

Example of file:

.. code-block:: Python

   defaultStyle = 'Alliance.Classic [black]'
   
   parametersTable = \
       ( ('misc.catchCore'           , TypeBool      , False   )
       , ('misc.info'                , TypeBool      , False   )
       , ('misc.paranoid'            , TypeBool      , False   )
       , ('misc.bug'                 , TypeBool      , False   )
       , ('misc.logMode'             , TypeBool      , False   )
       , ('misc.verboseLevel1'       , TypeBool      , False   )
       , ('misc.verboseLevel2'       , TypeBool      , True    )
       )
