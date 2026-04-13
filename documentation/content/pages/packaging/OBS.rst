.. -*- Mode: rst -*-



1. Packaging with openSUSE Build Service (OBS)
==============================================

Under OBS, the build & install procedure has to be performed using ``Makefile.LIP6``.

.. note:: While the git repository and the tool itself is named ``coriolis``,
	  in order to minimize confusion, the associated package is ``coriolis-eda``.

.. note:: The devel package is currently almost empty. The development files
	  like C++ headers are not installed (yet) as it was easier when
	  making a PyPI package.


Summary of files involved in the packaging process:

* ``<>/Makefile.LIP6``
* ``<>/packaging/coriolis-eda.spec``
* ``<>/packaging/patchenv.sh``
* ``<>/packaging/find_files.sh``
* ``<>/packaging/mkArchives.sh``
* ``<>/packaging/coloquinte-clamp.patch``, fix a missing include (until fixed upstream).
* ``<>/packaging/git-archive-all.sh``, borrowed from the internet.


1.1 Virtual environement snapshot
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* Formerly, |pdm| was installed in the global environment of the user,
  ``~/.local/bin`` with |pip|. Now we install it *inside* the virtual
  environment dedicated to build Coriolis (``<>/coriolis/.venv``).

* Once all the dependencies are installed by |pdm| in the dedicated
  virtual environment, *we do not need* to go through |pdm| to run
  the build and install commands. As a consequence, once the snapshot
  is taken from a valid environment on the reference machine we only
  need to *activate* the virtual environment and run the commands right
  away. The ``Makefile.LIP6`` has been simplificated in that way.

* To run |pelican| and generate the documentation, we need at least
  Python 3.8 for the ``watchfiles`` module to be available. And, as
  it is binary dependent, we need to erase it from the virtual environment
  and install the one provided by the OS distribution. Hence the the
  dependency over ``python311-devel`` (SUSE) and ``python3.11-devel``
  (RHEL clones).

* A second problem with |pelican| is the incompatibilities/bug between
  |doxygen| and |ghostscript| in some older distributions.
  It completely prevent the rebuild of the documentation and it is
  outside our remit to rebuild upgraded versions of those programs.
  The workaround is, for those problematic distributions, to
  provide a snapshot of the documentation. It should not change
  as fast as the sources.

A dedicated script ``patchenv.sh`` has been created to manage the
virtual environment package removal, when needed.

* As the environment is being patched, the modification time of
  the directories are made more recents. This may trick the
  ``Makefile.LIP6`` rules to try to reinstall |pdm| or some other
  dependency. In order to avoid that, we have to ``touch`` the
  relevant files. For now, |pdm|, |meson| & |pelican|.


1.2 Helpers scripts for RPM
~~~~~~~~~~~~~~~~~~~~~~~~~~~

We also provides a ``find_files.sh`` script to generate the various ``%files``
section of RPM packages. It is specifically tailored to Coriolis and in the
event of changes in the way Coriolis is installed it must be reviewed.



1.3 Making archives and pushing to OBS
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A dedicated script has been made to handle the whole process of making
archives and pushing them to OBS : ``mkArchives.sh``.

* Using the ``git-archive-all.sh`` script, an archive of the current
  ``HEAD`` of the branch we are in is taken, *along with the submodules*.

* It copy all the configuration files from ``<>/coriolis/packaging/``
  into the checkout of the OBS package under ``<>/../coriolis-obs/home:jpc-lip6/coriolis-eda``.



