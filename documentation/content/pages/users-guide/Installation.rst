.. -*- Mode: rst -*-


Installation
============

We reccomend installing directly from Pypi <https://pypi.org/>`_. First install Python 3.9 or later as appropriate for your system, then:


.. code-block:: bash

   pip3 install coriolis-eda


You can also install coriolis and alliance using RPM and DEB packages.


.. code-block:: bash

   rpm -Uvh coriolis-2.5.5-1.el9.x86_64.rpm alliance-5.0.0-1.el9.x86_64.rpm


Building from Source
====================
Here, we describe steps to build and install |Coriolis| for both popular distributions (Linux/Debian). You can follow the steps accordingly.    


Installing on |Debian| 12, |Ubuntu| 22 or compatible distributions
------------------------------------------------------------------
First, install or check that the required prerequisites are installed:

.. code-block:: sh

   dummy@lepka:~> sudo apt install build-essential git ccache python3 python3-pip \
                 		 python3-venv doxygen pelican texlive-latex-recommended \
                 		bison flex qtbase5-dev libqt5svg5-dev libqwt-qt5-dev \
                 		libbz2-dev rapidjson-dev libboost-all-dev libeigen3-dev \
          			 libxml2-dev pkg-config meson 
          			       			 
  
To install pdm:	

.. code-block:: sh
   
   curl -sSL https://pdm-project.org/install-pdm.py | python3 -


.. .. note:: **Do not install both versions of Qwt** (for Qt 4 and Qt 5),
          this will confuse the installer and end up with a non functional software
          (it uses the headers from one Qt and libraries from the other version).

     
Second step is to create the source directory and pull the |git| repository:

.. code-block:: sh

   dummy@lepka:~> mkdir -p ~/coriolis-2.x/src
   dummy@lepka:src> cd ~/coriolis-2.x/src
   dummy@lepka:src> git clone --recurse-submodules https://github.com/lip6/coriolis


Third and final step is to build & install |Coriolis|:

.. code-block:: sh

   dummy@lepka:src> cd coriolis
   dummy@lepka:coriolis> make -f Makefile.LIP6 install


To install documentation

.. code-block:: sh

   dummy@lepka:coriolis> make -f Makefile.LIP6 install_docs




Installing on |RHEL| 9, or compatible distributions
---------------------------------------------------

First,  install and check taht the required prerequisites are installed:

.. code-block:: sh

	dummy@lepka:~> sudo yum install python3-pip git cmake gcc-c++ clang \ 
				libstdc++-devel libxml2-devel flex bison boost-devel \
	 				boost-python3 boost-filesystem boost-regex boost-wave \
	 					 bzip2-devel python3-devel texlive 


Install pdm:   

.. code-block:: sh

	dummy@lepka:~> curl -sSL https://pdm-project.org/install-pdm.py | python3 -


Install meson, ninja and pelican

.. code-block:: sh

	 dummy@lepka:~> python3 -m pip install meson ninja pelican


Install Eign3 and doxygen 

.. code-block:: sh

	dummy@lepka:~> sudo dnf --enablerepo=crb install eigen3-devel doxygen qt5 qt5-devel


Install  Rapidjson: 

.. code-block:: sh

	dummy@lepka:~> git clone https://github.com/Tencent/rapidjson.git
	dummy@lepka:~> git submodule update --init
	dummy@lepka:~> cd rapidjson
	dummy@lepka:rapidjson> mkdir build 
	dummy@lepka:rapidjson> cd build
	dummy@lepka:rapidjson> cmake ..
	dummy@lepka:rapidjson> sudo make install


After installing all required dependencies, next step is to create the source directory and pull the |git| repository:

.. code-block:: sh

   dummy@lepka:~> mkdir -p ~/coriolis-2.x/src
   dummy@lepka:src> cd ~/coriolis-2.x/src
   dummy@lepka:src> git clone --recurse-submodules https://github.com/lip6/coriolis


Last step is to build & install:

.. code-block:: sh

   dummy@lepka:src> cd coriolis
   dummy@lepka:coriolis> make -f Makefile.LIP6 install


To install documentation:

.. code-block:: sh

   dummy@lepka:coriolis> make -f Makefile.LIP6 install_docs


Hooking up into |Alliance|
^^^^^^^^^^^^^^^^^^^^^^^^^^
|Coriolis| relies on |Alliance| for the cell libraries. So after installing or
packaging, you must install it so that it can found those libraries.

First clone,

.. code-block:: sh

   dummy@lepka:src> git clone https://github.com/lip6/alliance

Then install it.

.. code-block:: sh

   dummy@lepka:coriolis> make -f Makefile.LIP6 install_alliance

|newpage|
