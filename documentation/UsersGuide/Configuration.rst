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
computational instensive parts have been written in C++, and almost
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

All configuration & initialization files are Python scripts, despite their
|dot_conf| extention. From a syntactic point of view, there is no difference
between the system-wide configuration files and the user's configuration, 
they use the same Python helpers.
|medskip|

Configuration is done in two stages:

#. Selecting the technology.
#. Loading the complete configuration for the given technology
   and the user's settings.


First Stage: Technology Selection
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

|noindent|
The initialization process is done by executing, in order, the following file(s):

+-------+----------------------------------+----------------------------------------------+
| Order | Meaning                          | File                                         |
+=======+==================================+==============================================+
| **1** | The system setting               | :cb:`/etc/coriolis2/techno.conf`             |
+-------+----------------------------------+----------------------------------------------+
| **2** | The user's global setting        | :cb:`${HOME}/.coriolis2/techno.py`           |
+-------+----------------------------------+----------------------------------------------+
| **3** | The user's local setting         | :cb:`<CWD>/.coriolis2/techno.py`             |
+-------+----------------------------------+----------------------------------------------+

Those files must provide only one variable, the name of the technology. Each technology
will provide configuration for both the symbolic part and the real part. |Coriolis| can
work with purely symbolic technology (``symbolic/cmos``) in that case, the real technology
part is a dummy one.

For example, to use |MOSIS| 180nm: ::

    # -*- Mode:Python -*-
    
    technology = '180/scn6m_deep_09'


Second Stage: Technology Configuration Loading
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

|noindent|
The :cb:`technology` variable is set by the first stage and it's the name of the
technology. A directory of that name, with all the configuration files,
must exist in the configuration directory (:cb:`/etc/coriolis2`).
In addition to the technology-specific directories, a :cb:`common/` directory is
there to provide a trunk for all the identical data across the various technologies.
The initialization process is done by executing, in order, the following file(s):

+-------+----------------------------------+-----------------------------------------------+
| Order | Meaning                          | File                                          |
+=======+==================================+===============================================+
| **1** | The system initialization        | :cb:`/etc/coriolis2/<technology>/<TOOL>.conf` |
+-------+----------------------------------+-----------------------------------------------+
| **2** | The user's global initialization | :cb:`${HOME}/.coriolis2/settings.py`          |
+-------+----------------------------------+-----------------------------------------------+
| **3** | The user's local initialization  | :cb:`<CWD>/.coriolis2/settings.py`            |
+-------+----------------------------------+-----------------------------------------------+

.. note:: *The loading policy is not hard-coded.* It is implemented
   at Python level in :cb:`/etc/coriolis2/coriolisInit.py`, and thus may be easily
   amended to whatever site policy.

   The truly mandatory requirement is the existence of :cb:`coriolisInit.py`
   which *must* contain a :cb:`coriolisConfigure()` function with no argument.

   The :cb:`coriolisInit.py` script execution is triggered by the *import* of
   the ``CRL`` module:

   .. code:: python

      import sys
      import os.path
      import Cfg
      import Hurricane
      import CRL        # Triggers execution of "coriolisInit.py". 
      import Viewer
 

Configuration Helpers
~~~~~~~~~~~~~~~~~~~~~

To ease the writing of configuration files, a set of small helpers
is available. They allow to setup the configuration parameters through
simple assembly of tuples. The helpers are installed under the directory: ::

    <install>/etc/coriolis2/

Where :cb:`<install>/` is the root of the installation.


.. _Alliance Helper:

|Alliance| Helper
-----------------

The configuration file must provide an :cb:`allianceConfig` tuple as shown below.
Like all the |Coriolis| configuration file, it is to be executed through |Python|,
so we can use it to perform a not so dumb search of the |Alliance| installation
directory. Our default policy is to try to read the ``ALLIANCE_TOP`` environment
variable, and if not found, default to ``/soc/alliance``.

.. code:: python
	  
    import os
    from helpers.Alliance import AddMode
    from helpers.Alliance import Gauge
    
    allianceTop = None
    if os.environ.has_key('ALLIANCE_TOP'):
      allianceTop = os.environ['ALLIANCE_TOP']
      if not os.path.isdir(allianceTop):
        allianceTop = None
    
    if not allianceTop: allianceTop = '/soc/alliance'
    
    cellsTop = allianceTop+'/cells/'
    
    
    allianceConfig = \
        ( ( 'CATALOG'            , 'CATAL')
        , ( 'WORKING_LIBRARY'    , '.')
        , ( 'SYSTEM_LIBRARY'     , ( (cellsTop+'sxlib'   , AddMode.Append)
                                   , (cellsTop+'dp_sxlib', AddMode.Append)
                                   , (cellsTop+'ramlib'  , AddMode.Append)
                                   , (cellsTop+'romlib'  , AddMode.Append)
                                   , (cellsTop+'rflib'   , AddMode.Append)
                                   , (cellsTop+'rf2lib'  , AddMode.Append)
                                   , (cellsTop+'pxlib'   , AddMode.Append)
                                   , (cellsTop+'padlib'  , AddMode.Append) ) )
        , ( 'IN_LO'              , 'vst')
        , ( 'IN_PH'              , 'ap')
        , ( 'OUT_LO'             , 'vst')
        , ( 'OUT_PH'             , 'ap')
        , ( 'POWER'              , 'vdd')
        , ( 'GROUND'             , 'vss')
        , ( 'CLOCK'              , '.*ck.*|.*nck.*')
        , ( 'BLOCKAGE'           , '^blockage[Nn]et*')
        , ( 'PAD'                , '.*_px$')
        )

|newpage|


|noindent| The example above shows the system configuration file, with all the
available settings. Some important remarks about those settings:

* In its configuration file, the user does not need to redefine all the settings,
  just the one he wants to change. In most of the cases, the ``SYSTEM_LIBRARY``,
  the ``WORKING_LIBRARY`` and the special net names (at this point there is not
  much alternatives for the others settings).

* ``SYSTEM_LIBRARY`` setting: Setting up the library search path.
  Each library entry in the tuple will be added to the search path according
  to the second parameter:

  * :cb:`AddMode::Append`:  append to the search path.

  * :cb:`AddMode::Prepend`: insert in head of the search path.

  * :cb:`AddMode::Replace`: look for a library of the same name and replace
    it, whithout changing the search path order. If no library of that name
    already exists, it is appended.

  A library is identified by its name, this name is the last component of the
  path name. For instance: ``/soc/alliance/sxlib`` will be named ``sxlib``.
  Implementing the |Alliance| specification, when looking for a |Cell| ``name``,
  the system will browse sequentially through the library list and returns
  the first |Cell| whose name match.

* For ``POWER``, ``GROUND``, ``CLOCK`` and ``BLOCKAGE`` net names, a regular
  expression (|GNU| regexp) is expected.

.. * The ``helpers.sysConfDir`` variable is supplied by the helpers, it is the
..   directory in which the system-wide configuration files are located.
..   For a standard installation it would be: ``/soc/coriolis2``.
.. 
.. .. * Trick and naming convention about ``SYMBOLIC_TECHNOLOGY``, ``REAL_TECHNOLOGY``
..   and ``DISPLAY``. In the previous releases, those files were to be read by
..   XML parsers, and still do if you triggers the XML compatibility mode.
..   But now, they have Python conterparts. In the configuration files, you
..   still have to name them as XML files, the Python file name will be
..   deduced from this one with those two translation rules: 
.. 
..   #. In the filename, all dots, except for the last (the file extention), 
..      are replaced by underscores.
.. 
..   #. The ``.xml`` extention is substituted by a ``.conf``.
..   
..   For the symbolic technology, it would give: ::
.. 
..       /soc/coriolis2/technology.symbolic.xml
..                              --> /soc/coriolis2/technology_symbolic.conf

A typical user's configuration file would be: ::

    import os

    homeDir = os.getenv('HOME')

    allianceConfig = \
        ( ('WORKING_LIBRARY'    , homeDir+'/worklib')
        , ('SYSTEM_LIBRARY'     , ( (homeDir+'/mylib', Environment.Append) ) )
        , ('POWER'              , 'vdd.*')
        , ('GROUND'             , 'vss.*')
        )


Tools Configuration Helpers
---------------------------

All the tools use the same helper to load their configuration (a.k.a. 
*Configuration Helper*). Currently the following configuration system-wide
configuration files are defined:

* :cb:`misc.conf`: common settings or not belonging specifically to a tool.
* :cb:`etesian.conf`: for the |Etesian| tool.
* :cb:`kite.conf`: for the |Kite| tool.
* :cb:`stratus1.conf`: for the |stratus1| tool.

|newpage|


Here is the contents of :cb:`etesian.conf`: ::

    # Etesian parameters.
    parametersTable = \
        ( ('etesian.aspectRatio'    , TypePercentage, 100    , { 'min':10, 'max':1000 } )
        , ('etesian.spaceMargin'    , TypePercentage, 5      )
        , ('etesian.uniformDensity' , TypeBool      , False  )
        , ('etesian.routingDriven'  , TypeBool      , False  )
        , ("etesian.effort"         , TypeEnumerate , 2
          , { 'values':( ("Fast"    , 1)
                       , ("Standard", 2)
                       , ("High"    , 3)
                       , ("Extreme" , 4) ) }
          )
        , ("etesian.graphics"       , TypeEnumerate , 2
          , { 'values':( ("Show every step"  , 1)
                       , ("Show lower bound" , 2)
                       , ("Show result only" , 3) ) }
          )
        )
    
    layoutTable = \
        ( (TypeTab   , 'Etesian', 'etesian')
    
        , (TypeTitle , 'Placement area')
        , (TypeOption, "etesian.aspectRatio"   , "Aspect Ratio, X/Y (%)", 0 )
        , (TypeOption, "etesian.spaceMargin"   , "Space Margin"         , 1 )
        , (TypeRule  ,)
        , (TypeTitle , 'Etesian - Placer')
        , (TypeOption, "etesian.uniformDensity", "Uniform density"      , 0 )
        , (TypeOption, "etesian.routingDriven" , "Routing driven"       , 0 )
        , (TypeOption, "etesian.effort"        , "Placement effort"     , 1 )
        , (TypeOption, "etesian.graphics"      , "Placement view"       , 1 )
        , (TypeRule  ,)
        )


Taxonomy of the file:

* It must contain, at least, the two tables:

  * ``parametersTable``, defines & initialises the configuration variables.

  * ``layoutTables``, defines how the various parameters will be displayed
    in the configuration window (:ref:`The Settings Tab`).

* The ``parametersTable``, is a tuple (list) of tuples. Each entry in the list
  describes a configuration parameter. In its simplest form, it's a quadruplet
  :cb:`(TypeOption, 'paramId', ParameterType, DefaultValue)` with:

  #. ``TypeOption``, tells that this tuple describes a parameter.

  #. ``paramId``, the identifier of the parameter. Identifiers are defined
     by the tools. The list of parameters is detailed in each tool section.

  #. ``ParameterType``, the kind of parameter. Could be:

     * ``TypeBool``, boolean.
     * ``TypeInt``, signed integer.
     * ``TypeEnumerate``, enumerated type, needs extra entry.
     * ``TypePercentage``, percentage, expressed between 0 and 100.
     * ``TypeDouble``, float.
     * ``TypeString``, character string.
  
  #. ``DefaultValue``, the default value for that parameter.


Hacking the Configuration Files
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Aside from the symbols that get used by the configuration helpers like
:cb:`allianceConfig` or :cb:`parametersTable`, you can put pretty much anything
in :cb:`<CWD>/.coriolis2/settings.py` (that is, written in |Python|).

For example: ::

    # -*- Mode:Python -*-
    
    defaultStyle = 'Alliance.Classic [black]'
    
    # Regular Coriolis configuration.
    parametersTable = \
        ( ('misc.catchCore'           , TypeBool      , False  )
        , ('misc.info'                , TypeBool      , False  )
        , ('misc.paranoid'            , TypeBool      , False  )
        , ('misc.bug'                 , TypeBool      , False  )
        , ('misc.logMode'             , TypeBool      , True   )
        , ('misc.verboseLevel1'       , TypeBool      , False  )
        , ('misc.verboseLevel2'       , TypeBool      , True   )
        , ('misc.minTraceLevel'       , TypeInt       , 0      )
        , ('misc.maxTraceLevel'       , TypeInt       , 0      )
        )
    
    # Some ordinary Python script...
    import os
    
    print '       o  Cleaning up ClockTree previous run.'
    for fileName in os.listdir('.'):
      if fileName.endswith('.ap') or (fileName.find('_clocked.') >= 0):
        print '          - <%s>' % fileName
        os.unlink(fileName)


See :ref:`Python Interface to Coriolis` for more details those capabilities.
