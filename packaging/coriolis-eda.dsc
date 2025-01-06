Format:            1.0
Source:            coriolis-eda
Binary:            coriolis-eda, coriolis-eda-dev, coriolis-eda-doc
Architecture:      any
Version:           2.5.5
Maintainer:        Mazher Iqbal <mazher.iqbal@lip6.fr>
Homepage:          https://coriolis.lip6.fr/
Standards-Version: 2.5.5
Build-Depends:     debhelper-compat (= 13),
                   bison,
		   meson,
		   pelican,
		   cmake,
		   doxygen,
		   flex,
		   pkg-config,
		   libstdc++6,
		   libboost-all-dev,
		   libbz2-dev,
		   qtbase5-dev,
		   libqt5svg5-dev,
		   libqwt-qt5-dev,
		   libqt5opengl5-dev,
		   libeigen3-dev,
		   libxml2-dev,
		   libxpm-dev,
		   libmotif-common,
		   libmotif-dev,
		   libxm4,
		   ninja-build,
		   python3,
		   python3-pip,
		   python3-setuptools,
		   python3-wheel,
		   python3-virtualenv,
		   python3-venv,
		   rapidjson-dev,
		   build-essential,
		   texlive-latex-recommended,
		   graphviz,
		   xpdf
Package-List:
 coriolis-eda     deb Science/Electronics optional arch=any
 coriolis-eda-dev deb Science/Electronics optional arch=any
 coriolis-eda-doc deb Science/Electronics optional arch=any
DEBTRANSFORM-RELEASE:   1
DEBTRANSFORM-TAR:       coriolis-eda-2.5.5.tar.gz
