.. -*- Mode: rst -*-


2. Fast Packages Deployment with RPM
====================================

This part is mostly a reminder to myself of the procedure to fast deploy
development versions of the packages on the LIP6 internal network.
However, the procedure could be adapted without too much effort to
other configurations. Currently the constructed RPMs are expected to
be for AlmaLinux 9.

The script to run is: ``coriolis/packaging/buildDevelPackages.sh``.

It perform the following operations:

1. Rebuild the requested packages using their ``packaging/uploadOBS.sh``
   ad-hoc scripts, with the ``--rpm`` switch.

   The list of packages is stored in ``repoDirs`` and should be adjusted
   to avoid rebuilding everything, everytime.

   .. note:: **Versioning.** In order for the fast generated package to
	     supersede the standard one provided by ``fossEDA``, increase
	     by *exactly* one the ``obsCI_CNT`` value from the version
	     of the base package.

   .. note:: The standard RPM directory tree will be located under: ::

	       <root_dir>/src/coriolis-rpm/
	       <root_dir>/src/coriolis-rpm/SOURCES/
	       <root_dir>/src/coriolis-rpm/BUILD/
	       <root_dir>/src/coriolis-rpm/BUILDROOT/
	       <root_dir>/src/coriolis-rpm/SRPMS/
	       <root_dir>/src/coriolis-rpm/RPMS/noarch/
	       <root_dir>/src/coriolis-rpm/RPMS/x68_64/

	     The macro file defining the location is in: ::

	       <root_dir>/src/coriolis-rpm/rpmmacros

2. Remove the ``*debug*`` additional packages, as we don't intend to debug
   that way.

3. Sign the packages, using ``rpmsign --addsign``, the user must have a
   ``%_gpg_name`` defined in it's ``~/.rpmmacros`` file in order for this
   to work.

4. Finaly move the package into the fast release repositories, which are,
   in LIP6 case: ::

       /dsk/l1/distributions/fossEDA/al/9_devel/Source/src
       /dsk/l1/distributions/fossEDA/al/9_devel/x86_64/noarch
       /dsk/l1/distributions/fossEDA/al/9_devel/x86_64/x86_64

   The repositories are then rebuilt and limited to the two last versions
   of each package with the following commands:

   .. code:: bash

      script> cd x86_64
      script> createrepo_c -d --update .
      script> dnf repomanage --keep 2 .
  

