.. -*- Mode: rst -*-


About Alliance
==============


Circuit Designed with Alliance
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* Smartlabs_/Smarthome designed a complete circuit in the |XFAB| |XH035| technology
  (2014).
* `Tokai University`_ (`Shimizu Lab`_) designed the |SNX|, a 16 bits processor in
  the |ROHM| 0.18µm (2010).

Those are circuits that we know of because their designers were kind enough to inform
us (it is not comprehensive).


Useful Links
~~~~~~~~~~~~

* Improved Standard Cell libraries and documentation on how to design them,
  by Graham |Petley|: `VLSI Technology <http://www.vlsitechnology.org/>`_
* A book presenting |Alliance| in depth:
  `Introduction to VLSI CMOS Circuits design <http://www.cc.toin.ac.jp/sc/palacios/openbook/vlsie.pdf>`_
  by Carlos Silva :sc:`Cardenas`, Takeo :sc:`Yoshida` & Alberto Palacios :sc:`Pawlovsky`.
* For spanish locutors, a set of tutorials made by Miguel Eduardo |FloresGomez|
  from `Don Bosco University`_:
  `Tutorial de Alliance <http://microelectronicdesignandsimulation.blogspot.com/2015/10/tutoriales-de-alliance-vlsi.html>`_


Installing |Alliance| from a Distribution
=========================================

Binary packages are avalaibles for the following distributions:

* Fedora
* Ubuntu LTS
* MacOS X, through `MacPorts <https://www.macports.org/>`_


Fedora
~~~~~~

1. Pull & install the packages from the repository:

   .. code-block:: sh

      root@home:~# yum install alliance alliance-libs alliance-doc

   That's all folks. |Alliance| is ready to use.

.. note:: With the packaged version of |Alliance|, files and directories are not at
          the same places as in the default install. They have been made compliant
          with the |FHS|.

          ========================  ===============================
          **binaries**              /usr/lib/alliance/bin
          **cell libraries**        /usr/share/alliance/cells
          **man pages**             /usr/share/alliance/man
          **tutorials & examples**  /usr/share/doc/alliance-5.0/
          ========================  ===============================

          Environment variables should be automatically set.


Ubuntu LTS 18.04
~~~~~~~~~~~~~~~~

|Alliance| is available in the `Ubuntu Universe repository <http://packages.ubuntu.com/bionic/alliance>`_,
you may install it with the package manager.

   .. code-block:: sh

      ego@home:~> sudo apt-get install alliance

Before using it, you must source the environment (in each terminal
you want to use |Alliance|):

.. code-block:: sh
		
   ego@home:~> . /etc/alliance/profile.d/alc_env.sh


Rebuild From Source (Git)
=========================

If binary packages are not avalaibles for your distribution, you may want
to rebuild |Alliance| from source. To perform this, be aware that you must
have at least a basic knowlegde of how to build a C/C++ program from source.
Sources are available through the following |git| repository: 

    https://github.com/lip6/alliance.git

The stable version is under the branch :cb:`master`, while the development
is kept under :cb:`devel` (and will be merged into :cb:`master` from time
to time...)

In order to recompile, you will need to install the development tools in
your system. The main ones are listed below.

===================  =======================================  ===============================
Build Dependencies
---------------------------------------------------------------------------------------------
Software             RHEL, CentOS, Scientific Linux & Fedora             Debian/Ubuntu
===================  =======================================  ===============================
Basic devel tools    "Development tools" (group)              `build-essential`
Version system       `git`                                    `git`
GCC/G++ compiler     `gcc`, `g++`                             `gcc`, `g++`
Autotools            `autoconf`, `automake`                   `autotool-dev`, `automake`
                     `libtool`                                `libtool`
Parser (yacc&lex)    `bison`, `flex`                          `bison`, `flex`
X11 libraries        `libX11-devel`, `libXt-devel`,           `libx11-dev`, `libxt-dev`
                     `libXpm-devel`,                          `libxpm-dev`
                     `motif`, `motif-devel`                   `libmotif-dev`
Graphics             `xfig`, `ImageMagick`                    `xfig`, `ImageMagick`
Text Processing      `texlive`                                `texlive`, `texlive-pictures`,
                                                              `texlive-latex-extra`
===================  =======================================  ===============================


.. note:: Before running the :cb:`autotools`, you have to run the :cb:`autostuff`
          script in ``./alliance/src`` which generate the top-level automake files.

.. note:: If you happen to have forgotten one of the dependency and have to
          install it after running `configure`, please remove the *whole*
          build directory tree and re-run `configure`. The same rule applies
          if you switch from static libraries to dynamic ones.

.. note:: **Do not build in parallel**, always uses ``-j1``, the build process
          fail in strange ways when run in parallel (this is a known problem
          due to the way Alliance was developped).

.. note:: **Bison/Flex** versions. Alliance is very sensitive to the versions
          of thoses programs. The reference OSes for the build are |Scientific Linux|
          6 & 7, which supplies |bison| 2.4.1 and 2.7 (resp.), |flex| 2.5.35 and
          2.5.37 (resp.).


Debian 9 & Ubuntu 18
~~~~~~~~~~~~~~~~~~~~

In this example we show the case where |Alliance| is installed in the user's home
directory alongside with |Coriolis|.

1. Dependencies installation:

   .. code-block:: sh
   
      ego@home:~> sudo apt-get install build-essential binutils-dev         \
                                       git cmake bison flex gcc python-dev  \
                                       autotools-dev automake               \
                                       libxt-dev libxpm-dev libmotif-dev              

2. Getting the sources (cloning the |git| repository):

   .. code-block:: sh
   
      ego@home:~> mkdir -p coriolis-2.x/src
      ego@home:~> cd coriolis-2.x/src
      ego@home:src> git clone https://github.com/lip6/alliance.git

3. Compilation & installation. For that step, you can use the following shell
   script.

   .. note:: **The commonRoot variable**, the ``/Linux.x86_64/`` component of
	     the path is dependent on the |OS| you are using. It is determined
	     by looking to the kernel version as returned by ``uname -srm``.
	     In the following script, it has been set to what is choosen by the
	     |Coriolis| installer ``ccb.py`` for |Ubuntu|.
             
   .. code-block:: sh

      #!/bin/bash
      
            srcDir=${HOME}/coriolis-2.x/src/alliance/alliance/src
        commonRoot=${HOME}/coriolis-2.x/Linux.x86_64/Release.Shared
          buildDir=${commonRoot}/build
        installDir=${commonRoot}/install
      
        export    ALLIANCE_TOP=${installDir}
        export LD_LIBRARY_PATH=${installDir}/lib:${LD_LIBRARY_PATH}
      
        cd ${srcDir}
       # Skip doc generation to avoid pulling TeXLive in docker images.
       #sed -i 's,dirs="\$newdirs documentation",dirs="$newdirs",' ./autostuff
        ./autostuff clean
        ./autostuff
        mkdir -p ${buildDir}
        cd ${buildDir}
        ${srcDir}/configure --prefix=${ALLIANCE_TOP} --enable-alc-shared
        make -j1 install


RHEL, CentOS, Fedora
~~~~~~~~~~~~~~~~~~~~

The procedure is very similar as the one for Debian/Ubuntu, except for the
package manager step and the **commonRoot** variable value, which is set
to ``/Linux.SL7_64/`` (RHEL, CentOS & Scientific Linux)
or ``/Linux.fc_64/`` (Fedora).

1. Dependencies installation:

   .. code-block:: sh
   
      root@home:~> yum install git cmake bison flex gcc-c++ libstdc++-devel  \
                       make binutils-devel                                   \
                       autoconf automake libtool                             \
                       libX11-devel libXt-devel libXpm-devel                 \
                       motif motif-devel                                     \


2. Getting the sources (cloning the |git| repository):

   .. code-block:: sh
   
      ego@home:~> mkdir -p coriolis-2.x/src
      ego@home:~> cd coriolis-2.x/src
      ego@home:src> git clone https://github.com/lip6/alliance.git

3. Compilation & installation. For that step, you can use the following shell
   script.
             
   .. code-block:: sh

      #!/bin/bash
      
            srcDir=${HOME}/coriolis-2.x/src/alliance/alliance/src
        commonRoot=${HOME}/coriolis-2.x/Linux.SL7_64/Release.Shared
          buildDir=${commonRoot}/build
        installDir=${commonRoot}/install
      
        export    ALLIANCE_TOP=${installDir}
        export LD_LIBRARY_PATH=${installDir}/lib:${LD_LIBRARY_PATH}
      
        cd ${srcDir}
       # Skip doc generation to avoid pulling TeXLive in docker images.
       #sed -i 's,dirs="\$newdirs documentation",dirs="$newdirs",' ./autostuff
        ./autostuff clean
        ./autostuff
        mkdir -p ${buildDir}
        cd ${buildDir}
        ${srcDir}/configure --prefix=${ALLIANCE_TOP} --enable-alc-shared
        make -j1 install

