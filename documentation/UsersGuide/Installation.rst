.. -*- Mode: rst -*-

.. include:: ../etc/definitions.rst

|newpage|


Installation
============

.. note::
   As the sources are being released, the binary packaging is dropped.
   You still may find older version here: http://asim.lip6.fr/pub/coriolis/2.0 .

In a nutshell, building source consistis in pulling the |git| repository then
running the |ccb| installer. 

Main building prerequisites:

* cmake
* C++11-capable compiler
* RapidJSON_
* python2.7
* boost
* libxml2
* bzip2
* yacc & lex
* Qt 4 or Qt 5

Building documentation prerequisites:

* doxygen
* latex
* latex2html
* python-docutils (for reStructuredText)

The following libraries gets directly bundled with |Coriolis|:

* LEF/DEF (from `SI2 <https://www.si2.org/>`_)
* FLUTE (from `Chris C. N. Chu <http://home.eng.iastate.edu/~cnchu/flute.html>`_)

For other distributions, refer to their own packaging system.

|newpage|


Fixed Directory Tree
~~~~~~~~~~~~~~~~~~~~

In order to simplify the work of the |ccb| installer, the source, build
and installation tree is fixed. To successfully compile |Coriolis| you must
follow it exactly. The tree is relative to the home directory of the user
building it (noted :fboxtt:`~/` or :fboxtt:`$HOME/`). Only the source
directory needs to be manually created by the user, all others will be
automatically created either by |ccb| or the build system.

+--------------------------------------------------------------------------------------------------------------+
| **Sources**                                                                                                  |
+------------------------------+-------------------------------------------------------------------------------+
| | Sources root               | | ~/coriolis-2.x/src                                                          |
| | **under git**              | | ~/coriolis-2.x/src/coriolis                                                 |
+------------------------------+-------------------------------------------------------------------------------+
| **Architecture Dependant Build**                                                                             |
+------------------------------+-------------------------------------------------------------------------------+
| | Linux, SL 7, 64b           | | ~/coriolis-2.x/Linux.el7_64/Release.Shared/build/<tool>                     |
| | Linux, SL 6, 32b           | | ~/coriolis-2.x/Linux.slsoc6x/Release.Shared/build/<tool>                    |
| | Linux, SL 6, 64b           | | ~/coriolis-2.x/Linux.slsoc6x_64/Release.Shared/build/<tool>                 |
| | Linux, Fedora, 64b         | | ~/coriolis-2.x/Linux.fc_64/Release.Shared/build/<tool>                      |
| | Linux, Fedora, 32b         | | ~/coriolis-2.x/Linux.fc/Release.Shared/build/<tool>                         |
| | FreeBSD 8, 32b             | | ~/coriolis-2.x/FreeBSD.8x.i386/Release.Shared/build/<tool>                  |
| | FreeBSD 8, 64b             | | ~/coriolis-2.x/FreeBSD.8x.amd64/Release.Shared/build/<tool>                 |
| | Windows 7, 32b             | | ~/coriolis-2.x/Cygwin.W7/Release.Shared/build/<tool>                        |
| | Windows 7, 64b             | | ~/coriolis-2.x/Cygwin.W7_64/Release.Shared/build/<tool>                     |
| | Windows 8.x, 32b           | | ~/coriolis-2.x/Cygwin.W8/Release.Shared/build/<tool>                        |
| | Windows 8.x, 64b           | | ~/coriolis-2.x/Cygwin.W8_64/Release.Shared/build/<tool>                     |
+------------------------------+-------------------------------------------------------------------------------+
| **Architecture Dependant Install**                                                                           |
+------------------------------+-------------------------------------------------------------------------------+
|   Linux, SL 6, 32b           |   ~/coriolis-2.x/Linux.slsoc6x/Release.Shared/install/                        |
+------------------------------+-------------------------------------------------------------------------------+
| **FHS Compliant Structure under Install**                                                                    |
+------------------------------+-------------------------------------------------------------------------------+
| | Binaries                   | | .../install/bin                                                             |
| | Libraries (Python)         | | .../install/lib                                                             |
| | Include by tool            | | .../install/include/coriolis2/<project>/<tool>                              |
| | Configuration files        | | .../install/etc/coriolis2/                                                  |
| | Doc, by tool               | | .../install/share/doc/coriolis2/en/html/<tool>                              |
+------------------------------+-------------------------------------------------------------------------------+

.. note:: *Alternate build types:* the ``Release.Shared`` means an optimized build
   with shared libraries. But there are also available ``Static`` instead of ``Shared``
   and ``Debug`` instead of ``Release`` and any combination of them.

   ``Static`` do not work because I don't know yet to mix statically linked binaries
   and Python modules (which must be dynamic).

|newpage|


Building Coriolis
~~~~~~~~~~~~~~~~~

First step is to install the prerequisites. Currently, only RapidJSON_.
As RapidJSON is evolving fast, if you encounter compatibility problems,
the exact version we compiled against is given below. ::

   dummy@lepka:~> mkdir -p ~/coriolis-2.x/src/support
   dummy@lepka:~> cd ~/coriolis-2.x/src/support
   dummy@lepka:~> git clone http://github.com/miloyip/rapidjson
   dummy@lepka:~> git checkout ec322005072076ef53984462fb4a1075c27c7dfd

The second step is to create the source directory and pull the |git| repository: ::

   dummy@lepka:~> mkdir -p ~/coriolis-2.x/src
   dummy@lepka:~> cd ~/coriolis-2.x/src
   dummy@lepka:~> git clone https://www-soc.lip6.fr/git/coriolis.git

Third and final step, build & install: ::

   dummy@lepka:src> ./bootstrap/ccb.py --project=support  \
                                       --project=coriolis \
                                       --make="-j4 install"
   dummy@lepka:src> ./bootstrap/ccb.py --project=support  \
                                       --project=coriolis \
                                       --doc --make="-j1 install"

We need to perform a separate installation of the documentation because it
does not support to be generated with a parallel build. So we compile & install in a first
stage in ``-j4`` (or whatever) then we generate the documentation in ``-j1``

Under |RHEL6| or clones, you must build using the |devtoolset|, the version is to
be given as argument: ::

   dummy@lepka:src> ./bootstrap/ccb.py --project=coriolis \
                                       --devtoolset=8 --make="-j4 install"

If you want to uses Qt 5 instead of Qt 4, you may add the ``--qt5`` argument.

The complete list of |ccb| functionalities can be accessed with the ``--help`` argument.
It also may be run in graphical mode (``--gui``).


Building the Devel Branch
-------------------------

In the |Coriolis| |git| repository, two branches are present:

* The :cb:`master` branch, which contains the latest stable version. This is the 
  one used by default if you follow the above instructions.

* The :cb:`devel` branch, which obviously contains the latest commits from the
  development team. To use it instead of the :cb:`master` one, do the following
  command just after the first step: ::

      dummy@lepka:~> git checkout devel
      dummy@lepka:src> ./bootstrap/ccb.py --project=coriolis \
                                          --make="-j4 install" --debug

  Be aware that it may require newer versions of the dependencies and may introduce
  incompatibilites with the stable version.

  In the (unlikely) event of a crash of |cgt|, as it is a |Python| script, the right
  command to run |gdb| on it is: ::

      dummy@lepka:work> gdb python core.XXXX 

|newpage|


Additionnal Requirement under |MacOS|
-------------------------------------

|Coriolis| make uses of the :cb:`boost::python` module, but the |macports| |boost|
seems unable to work with the |Python| bundled with |MacOS|. So you have to install
both of them from |macports|: ::

    dummy@macos:~> port install boost +python27
    dummy@macos:~> port select python python27
    dummy@macos:-> export DYLD_FRAMEWORK_PATH=/opt/local/Library/Frameworks

The last two lines tell |MacOS| to use the |Python| from |macports| and *not* from
the system.

Then proceed with the generic install instructions.


Packaging Coriolis
~~~~~~~~~~~~~~~~~~

Packager should not use |ccb|, instead ``bootstrap/Makefile.package`` is provided
to emulate a top-level ``autotool`` makefile. Just copy it in the root of the
|Coriolis| git repository (``~/corriolis-2.x/src/coriolis/``) and build.

Sligthly outdated packaging configuration files can also be found under ``bootstrap/``:

* ``bootstrap/coriolis2.spec.in`` for |rpm| based distributions.
* ``bootstrap/debian`` for |Debian| based distributions.


Hooking up into |Alliance|
~~~~~~~~~~~~~~~~~~~~~~~~~~

|Coriolis| relies on |Alliance| for the cell libraries. So after installing or
packaging, you must configure it so that it can found those libraries.

The easiest way is to setup the |Alliance| environment (i.e. sourcing
``.../etc/profile.d/alc_env.{sh,csh}``) **before** setting up |Coriolis| environment
(see the next section). To understand how |Coriolis| find/setup |Alliance| you may
have look to the :ref:`Alliance Helper`.


Setting up the Environment (coriolisEnv.py)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To simplify the tedious task of configuring your environment, a helper is provided
in the ``bootstrap`` source directory (also installed in the directory
``.../install/etc/coriolis2/``) : ::

    ~/coriolis-2.x/src/coriolis/bootstrap/coriolisEnv.py

Use it like this: ::

    dummy@lepka:~> eval `~/coriolis-2.x/src/coriolis/bootstrap/coriolisEnv.py`

.. note:: **Do not call that script in your environement initialisation.**
   When used under |RHEL6| or clones, it needs to be run in the |devtoolset2|
   environement. The script then launch a new shell, which may cause an
   infinite loop if it's called again in, say :cb:`~/.bashrc`.

   Instead you may want to create an alias: ::

       alias c2r='eval "`~/coriolis-2.x/src/coriolis/bootstrap/coriolisEnv.py`"'
