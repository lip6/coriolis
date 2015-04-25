.. -*- Mode: rst -*-


===============
Coriolis README
===============


Coriolis is a free database, placement tool and routing tool for VLSI designs.


Purpose
=======

Coriolis provides several tools to perform the layout of VLSI circuits.  Its
main components are the Hurricane database, the Etesian placer and the Kite
router, but other tools can use the Hurricane database and the parsers
provided.

The user interface <cgt> is the prefered way to use Coriolis, but all
Coriolis tools are Python modules and thus scriptables.


Documentation
=============

The complete documentation is available here, both in pdf & html:

   ./documentation/UsersGuide/UsersGuide.html
   ./documentation/UsersGuide/UsersGuide.pdf

The documentation of the latest *stable* version is also
available online. It may be quite outdated from the *devel*
version.

    https://soc-extras.lip6.fr/en/coriolis/coriolis2-users-guide/


Building Coriolis
=================

To build Coriolis, ensure the following prerequisites are met:

* Python 2.7.
* cmake.
* boost.
* bison & flex.
* Qt 4 or 5.
* libxml2.
* A C++11 compliant compiler.

The build system relies on a fixed directory tree from the root
of the user currently building it. Thus first step is to get a clone of
the repository in the right place. Proceed as follow: ::

   ego@home:~$ mkdir -p ~/coriolis-2.x/src
   ego@home:~$ cd ~/coriolis-2.x/src
   ego@home:src$ git clone https://www-soc.lip6.fr/git/coriolis.git
   ego@home:src$ cd coriolis

If you want to use the *devel* branch: ::

    ego@home:coriolis$ git checkout devel

Then, build the tool, where ``N`` is your number of threads: ::

    ego@home:coriolis$ ./bootstrap/ccb.py --project=coriolis --make="-jN install"

Coriolis gets installed at the root of the following tree: ::

    ~/coriolis-2.x/<OS>.<DISTRIB>/Release.Shared/install/

Where ``<OS>`` is the name of your operating system and ``<DISTRIB>`` your
distribution.


Using Coriolis
==============

A helper is provided to setup the environment ``coriolisEnv.py``: ::

    ego@home:~$ eval `~/coriolis-2.x/src/coriolis/bootstrap/coriolisEnv.py`
    ego@home:~$ cgt -V

