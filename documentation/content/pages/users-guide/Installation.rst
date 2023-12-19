.. -*- Mode: rst -*-


Installation
============

We reccomend installing directly from Pypi <https://pypi.org/>`_. First install Python 3.9 or later as appropriate for your system, then:


.. code-block:: bash

   pip3 install coriolis-eda


Building from Source
====================



Building a Debug Enabled Version
--------------------------------

The ``Release.Shared`` default version of the |Coriolis| is built stripped of symbols
and optimized so that it makes analysing a core dump after a crash difficult. In the
(unlikely) case of a crash, you may want to build, alongside the optimized version,
a debug one which allows forensic examination by |gdb| (or |valgrind| or whatever).

Run again ``ccb.py``, adding the ``--debug`` argument:

.. code-block:: sh

   dummy@lepka:coriolis> ./bootstrap/ccb.py --project=support  \
                                            --project=coriolis \
                                            --make="-j4 install" --debug


As |cgt| is a |Python| script, the right command to run |gdb| is:

.. code-block:: sh

   dummy@lepka:work> gdb python core.XXXX 


.. Building the Devel Branch
.. -------------------------
.. 
.. In the |Coriolis| |git| repository, two branches are present:
.. 
.. * The :cb:`master` branch, which contains the latest stable version. This is the 
..   one used by default if you follow the above instructions.
.. 
.. * The :cb:`devel` branch, which obviously contains the latest commits from the
..   development team. To use it instead of the :cb:`master` one, do the following
..   command just after the first step: ::
.. 
..       dummy@lepka:coriolis> git checkout devel
..       dummy@lepka:coriolis> ./bootstrap/ccb.py --project=coriolis \
..                                                --make="-j4 install" --debug
.. 
..   Be aware that it may require newer versions of the dependencies and may introduce
..   incompatibilities with the stable version.

|newpage|


Installing on |Debian| 9, |Ubuntu| 18 or compatible distributions
-----------------------------------------------------------------

First, install or check that the required prerequisites are installed:

.. code-block:: sh

   dummy@lepka:~> sudo apt-get install -y build-essential binutils-dev                     \
                                          git cmake bison flex gcc python-dev              \
                                          libboost-all-dev libboost-python-dev             \
                                          zlib1g-dev libxml2-dev rapidjson-dev libbz2-dev

To use with Qt 4:

.. code-block:: sh

   dummy@lepka:~> sudo apt-get install -y qt4-dev-tools libqwt-dev python-qt4

To use with Qt 5:

.. code-block:: sh

   dummy@lepka:~> sudo apt-get install -y qtbase5-dev libqt5svg5-dev libqwt-qt5-dev \
                                          python-pyqt5

.. note:: **Do not install both versions of Qwt** (for Qt 4 and Qt 5),
          this will confuse the installer and end up with a non functional software
          (it uses the headers from one Qt and libraries from the other version).

Second step is to create the source directory and pull the |git| repository:

.. code-block:: sh

   dummy@lepka:~> mkdir -p ~/coriolis-2.x/src
   dummy@lepka:src> cd ~/coriolis-2.x/src
   dummy@lepka:src> git clone https://github.com/lip6/coriolis.git

Third and final step, build & install:

.. code-block:: sh

   dummy@lepka:src> cd coriolis
   dummy@lepka:coriolis> git checkout devel
   dummy@lepka:coriolis> ./bootstrap/ccb.py --project=coriolis \
                                            --make="-j4 install"


Additionnal Requirement under |MacOS|
-------------------------------------

|Coriolis| makes use of the :cb:`boost::python` module, but the |macports| |boost|
seems unable to work with the |Python| bundled with |MacOS|. So you have to install
both of them from |macports|:

.. code-block:: sh

   dummy@macos:~> port install boost +python27
   dummy@macos:~> port select python python27
   dummy@macos:-> export DYLD_FRAMEWORK_PATH=/opt/local/Library/Frameworks

The last two lines tell |MacOS| to use the |Python| from |macports| and *not* from
the system.

Then proceed with the generic install instructions.


|Coriolis| & Docker
^^^^^^^^^^^^^^^^^^^

Under ``bootstrap/docker/`` scripts and configuration files are provided that
allow to rebuild |Alliance| and |Coriolis| and perform the regression tests
of ``alliance-check-toolkit``. You may have a look at the ``Dockerfile.system``
configuration file to see exactly how to setup a vanilla system to build
|Coriolis|.

To run the docker tests, call the ``dockerManage.sh`` scripts with the relevant
arguments:

.. code-block:: sh

   ego@home:debian-9> ../../dockerManage.sh -bS  # build both system & coriolis images.
   ego@home:debian-9> ../../dockerManage.sh -r   # compile & check coriolis.
   ego@home:debian-9> ../../dockerManage.sh -C   # clear the images.



Packaging Coriolis
^^^^^^^^^^^^^^^^^^

Packager should not use |ccb|, instead ``bootstrap/Makefile.package`` is provided
to emulate a top-level ``autotool`` makefile. Just copy it in the root of the
|Coriolis| git repository (``~/corriolis-2.x/src/coriolis/``) and build.

Sligthly outdated packaging configuration files can also be found under ``bootstrap/``:

* ``bootstrap/coriolis2.spec.in`` for |rpm| based distributions.
* ``bootstrap/debian`` for |Debian| based distributions.


Hooking up into |Alliance|
^^^^^^^^^^^^^^^^^^^^^^^^^^

|Coriolis| relies on |Alliance| for the cell libraries. So after installing or
packaging, you must configure it so that it can found those libraries.

The easiest way is to setup the |Alliance| environment (i.e. sourcing
``.../etc/profile.d/alc_env.{sh,csh}``) **before** setting up |Coriolis| environment
(see the next section). To understand how |Coriolis| find/setup |Alliance| you may
have look to the `Alliance Helper`.


Setting up the Environment (coriolisEnv.py)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To simplify the tedious task of configuring your environment, a helper is provided
in the ``bootstrap`` source directory (also installed in the directory
``.../install/etc/coriolis2/``) :

.. code-block:: sh

   ~/coriolis-2.x/src/coriolis/bootstrap/coriolisEnv.py

Use it like this:

.. code-block:: sh

   dummy@lepka:~> eval `~/coriolis-2.x/src/coriolis/bootstrap/coriolisEnv.py`

.. note:: **Do not call that script in your environement initialisation.**
   When used under |RHEL6| or clones, it needs to be run in the |devtoolset|
   environement. The script then launch a new shell, which may cause an
   infinite loop if it's called again in, say :cb:`~/.bashrc`.

   Instead you may want to create an alias:

   .. code-block:: sh

      alias c2r='eval "`~/coriolis-2.x/src/coriolis/bootstrap/coriolisEnv.py`"'
