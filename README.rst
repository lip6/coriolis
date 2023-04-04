.. -*- Mode: rst -*-


===============
Coriolis README
===============

Coriolis is a free database, placement tool and routing tool for VLSI design.


Purpose
=======

Coriolis provides several tools to perform the layout of VLSI circuits.  Its
main components are the Hurricane database, the Etesian placer and the Katana
router, but other tools can use the Hurricane database and the parsers
provided.

The user interface <cgt> is the prefered way to use Coriolis, but all
Coriolis tools are Python modules and thus scriptable.


Documentation
=============

The complete documentation is available here, both in pdf & html:

   ./documentation/output/html
   ./documentation/UsersGuide/UsersGuide.pdf

The documentation of the latest *stable* version is also
available online. It may be quite outdated from the *devel*
version.

    https://www-soc.lip6.fr/sesi-docs/coriolis2-docs/coriolis2/en/latex/users-guide/UsersGuide.pdf


Building Coriolis
=================

To build Coriolis, ensure the following prerequisites are met:

* Python 3,
* cmake,
* boost,
* bison & flex,
* Qt 4 or 5,
* libxml2,
* RapidJSON,
* A C++11 compliant compiler.

The build system relies on a fixed directory tree from the root
of the user currently building it. Thus first step is to get a clone of
the repository in the right place. Proceed as follow: ::

   ego@home:~$ mkdir -p ~/coriolis-2.x/src/
   ego@home:~$ cd ~/coriolis-2.x/src
   ego@home:src$ git clone https://gitlab.lip6.fr/vlsi-eda/coriolis.git
   ego@home:src$ cd coriolis

Then, build the tool: ::

    ego@home:coriolis$ make install

If you encounter issues, please consult SUPPORT.rst for tips.
Coriolis gets installed at the root of the following tree: ::

    ~/coriolis-2.x/<OS>.<DISTRIB>/Release.Shared/install/

Where ``<OS>`` is the name of your operating system and ``<DISTRIB>`` your
distribution.


Using Coriolis
==============

The Coriolis main interface can be launched with the command: ::

    ego@home:~: ~/coriolis-2.x/<OS>.<DISTRIB>/Release.Shared/install/bin/coriolis

The ``coriolis`` script detects its location and setups the UNIX
environment appropriately, then lauches ``cgt`` (or *any* command, with the
``--run=<COMMAND>`` option).

Conversely, you can setup the current shell environment for Coriolis by
using the helper ``coriolisEnv.py``, then run any Coriolis tool: ::

    ego@home:~$ eval `~/coriolis-2.x/src/coriolis/bootstrap/coriolisEnv.py`
    ego@home:~$ cgt -V

