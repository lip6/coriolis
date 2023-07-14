.. -*- Mode: rst -*-

.. include:: ../etc/definitions.rst

.. URLs that changes between the various backends.
.. _Coriolis Tools Documentation:   file:///usr/share/doc/coriolis2/index.html

				      

.. |CoriolisSoftSchema|             image:: ./images/Coriolis-Soft-Schema.png
                                    :alt:   Coriolis Software Schematic
                                    :align: middle
                                    :width: 60%


Coriolis Configuration & Initialisation
=======================================


General Software Architecture
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

|Coriolis| has been built with respect of the classical paradigm that the
computational intensive parts have been written in C++, and almost
everything else in |Python|. To build the |Python| interface we used
two methods:

* For self-contained modules :cb:`boost::python` (mainly in :cb:`vlsisapd`).
* For all modules based on |Hurricane|, we created our own wrappers due
  to very specific requirements such as shared functions between modules
  or C++/|Python| secure bi-directional object deletion.

.. note:: **Python Documentation:**
   Most of the documentation is related to the C++ API and implemetation of
   the tools. However, the |Python| bindings have been created so they
   mimic *as closely as possible* the C++ interface, so the documentation
   applies to both languages with only minor syntactic changes.

|bcenter| |CoriolisSoftSchema| |ecenter|


Configuration & User's Settings
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

All configurations files are shipped under the form of |Python| modules.
They are to be loaded through ``import`` statements. The user's configuration
files must be put in a ``./coriolis2/`` directory under the working directory.
It must be made a |Python| module so it must contains a ``__init__.py`` file
(kept empty most of the time). And as they are true |Python| files, you may use
in them any valid code you see fit.

If no user configuration files are present, |Coriolis| will use the default
``symbolic.cmos`` technology which matches the |Alliance| symbolic default one.

Contents of the user's configuration directory ``./coriolis2/``:

+----------------------------------+----------------------------------------------+
| File                             | Contents/Meaning                             |
+==================================+==============================================+
| :cb:`./coriolis2/__init__.py`    | **Mandatory.** Tells |Python| this directory |
|                                  | *is* a module. Can be left empty             |
+----------------------------------+----------------------------------------------+
| :cb:`./coriolis2/settings.py`    | **Mandatory.** The user's settings, it must  |
|                                  | setup the technology intended for use and    |
|                                  | perform any configuration variable settings  |
+----------------------------------+----------------------------------------------+
| :cb:`./coriolis2/ioring.py`      | **Optional.** Define how the I/O pads are to |
|                                  | be placed on the periphery of the chip along |
|                                  | the chip and core sizes                      |
+----------------------------------+----------------------------------------------+
| :cb:`./coriolis2/katana.py`      | **Optional.** Hook file for |Katana|, run    |
|                                  | just after the tool has been created for a   |
|                                  | ``Cell``. Mostly to setup ``Nets`` to be     |
|                                  | traced                                       |
+----------------------------------+----------------------------------------------+

For example, to use |MOSIS| 180nm, you can put in your :cb:`./coriolis2/setting.py`:

.. code-block:: python

   # -*- Mode:Python -*-
    
   import node180.scn6m_deep_09


A Comprehensive Example of :cb:`./coriolis2/setting.py`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: python

   import os
   import Cfg
   import Viewer
   import CRL
   import node180.scn6m_deep_09
   from   helpers       import l, u, n

   allianceTop = None
   if os.environ.has_key('ALLIANCE_TOP'):
     allianceTop = os.environ['ALLIANCE_TOP']
     if not os.path.isdir(allianceTop):
       allianceTop = None
   
   if not allianceTop: allianceTop = '/soc/alliance'
   
   Cfg.Configuration.pushDefaultPriority( Cfg.Parameter.Priority.UserFile )
   
   Viewer.Graphics.setStyle( 'Alliance.Classic [black]' )
   
   cellsTop = allianceTop+'/cells'
   
   # Alliance related settings.
   af  = CRL.AllianceFramework.get()
   env = af.getEnvironment()
   
   env.setSCALE_X        ( 100 )
   env.setCATALOG        ( 'CATAL' )
   env.setIN_LO          ( 'vst'   )
   env.setIN_PH          ( 'ap'    )
   env.setOUT_LO         ( 'vst'   )
   env.setOUT_PH         ( 'ap'    )
   env.setPOWER          ( 'vdd'   )
   env.setGROUND         ( 'vss'   )
   env.setCLOCK          ( '.*ck.*|.*nck.*'   )
   env.setBLOCKAGE       ( 'blockage[Nn]et.*' )
   env.setPad            ( '.*_mpx$'          )
   
   env.setWORKING_LIBRARY( '.' )
   env.addSYSTEM_LIBRARY ( library=cellsTop+'/nsxlib', mode=CRL.Environment.Append )
   env.addSYSTEM_LIBRARY ( library=cellsTop+'/mpxlib', mode=CRL.Environment.Append )

   # Misc. setting parameters.
   Cfg.getParamBool      ( 'misc.logMode'                ).setBool      ( False     )
   Cfg.getParamBool      ( 'misc.verboseLevel1'          ).setBool      ( True      )
   Cfg.getParamBool      ( 'misc.verboseLevel2'          ).setBool      ( True      )

   # P&R related parameters.
   Cfg.getParamString    ( 'anabatic.routingGauge'       ).setString    ( 'msxlib4' )
   Cfg.getParamString    ( 'anabatic.topRoutingLayer'    ).setString    ( 'METAL4'  )
   Cfg.getParamInt       ( 'katana.hTracksReservedLocal' ).setInt       ( 6         )
   Cfg.getParamInt       ( 'katana.vTracksReservedLocal' ).setInt       ( 3         )

   Cfg.Configuration.popDefaultPriority()


|noindent| The example above shows the user's configuration file, with all the
available settings for |Alliance| and a small subset for other tools. Some remarks
about this file:

* The ``Cfg.Configuration.pushDefaultPriority()`` and ``Cfg.Configuration.popDefaultPriority()``
  statements are there so the value sets by the user will not be overriden by system
  ones event if they are setup afterwards. This priority system is introduced so the
  various configuration files could be loaded in out of order.

* The ``Viewer.Graphics.setStyle()`` allows you to choose the look of your liking from
  the start.

* For |Alliance|, the user does not need to redefine all the settings,
  just the one he wants to change. In most of the cases, the ``addSYSTEM_LIBRARY()``,
  the ``setWORKING_LIBRARY()`` and the special net names (at this point there is not
  much alternatives for the others settings).

* ``addSYSTEM_LIBRARY()`` adds a directory to the library search path.
  Each library entry will be added to the search path according to the second parameter:

  * :cb:`CRL.Environment::Append`:  append to the search path.

  * :cb:`CRL.Environment::Prepend`: insert in head of the search path.

  * :cb:`CRL.Environment::Replace`: look for a library of the same name and replace
    it, whithout changing the search path order. If no library of that name
    already exists, it is appended.

  A library is identified by its name, this name is the last component of the
  path name. For instance: ``/soc/alliance/sxlib`` will be named ``sxlib``.
  Implementing the |Alliance| specification, when looking for a |Cell| ``name``,
  the system will browse sequentially through the library list and returns
  the first |Cell| whose name match.

* For ``setPOWER()``, ``setGROUND()``, ``setCLOCK()`` and ``setBLOCKAGE()`` net names,
  a regular expression (|GNU| regexp) is expected.

* For other tools parameters, just use getter and setter according to their types:

  +------------------+-----------------------------------------------------------+
  | Type             | Getter/Setter                                             |
  +==================+===========================================================+
  | ``Bool``         | ``Cgt.getParamBool('name').setBool( True )``              |
  +------------------+-----------------------------------------------------------+
  | ``Int``          | ``Cgt.getParamInt('name').setBool( 12 )``                 |
  +------------------+-----------------------------------------------------------+
  | ``Enumerate``    | ``Cgt.getParamEnumerate('name').setBool( 12 )``           |
  +------------------+-----------------------------------------------------------+
  | ``Double``       | ``Cgt.getParamDouble('name').setDouble( 254.5 )``         |
  +------------------+-----------------------------------------------------------+
  | ``Percentage``   | ``Cgt.getParamPercentage('name').setPercentage( 75.0 )``  |
  +------------------+-----------------------------------------------------------+
  | ``String``       | ``Cgt.getParamString('name').setString( 'machin' )``      |
  +------------------+-----------------------------------------------------------+

  Lists of the configurable parameters of most interest to the user are given in
  :ref:`Viewer & Tools`.
    
