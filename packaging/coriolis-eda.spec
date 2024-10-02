%define _unpackaged_files_terminate_build 1
%define docGithash  b12c8613c

Name:           coriolis
Version:        2.5.5
Release:        1
Summary:        Coriolis 2 EDA toolchain
Group:          Applications/Engineering
License:        BSD-1-Clause AND LGPL-1.0-only AND GPL-2.0-only AND Apache-1.0
URL:            https://coriolis.lip6.fr/
Packager:       Mazher Iqbal <mazher@lip6.fr>
Source0:        %{name}-%{version}.tar.gz
Source1:        venv-al9-2.5.5.tar.gz
Source2:        patchvenv.sh
Source3:        find_files.sh
Source10:       coriolis-docs-%{docGithash}.tar.gz
Patch0:         coloquinte-clamp.patch 


BuildRequires:  flex
BuildRequires:  gcc-c++
BuildRequires:  pkgconfig
BuildRequires:  bison
BuildRequires:  meson
BuildRequires:  cmake
BuildRequires:  doxygen
BuildRequires:  python3-pip
BuildRequires:  python3-devel
BuildRequires:  texlive-latex
BuildRequires:  texlive-dvips
BuildRequires:  libstdc++-devel
BuildRequires:  zlib-devel
BuildRequires:  libicu-devel
BuildRequires:  libzstd-devel
BuildRequires:  boost-devel
BuildRequires:  eigen3-devel
BuildRequires:  qwt-qt5-devel
BuildRequires:  libxml++-devel
BuildRequires:  rapidjson-devel
BuildRequires:  graphviz
BuildRequires:  graphviz-gd

#opensuse_Tumbleweed
%if 0%{?suse_version} > 1600 && 0%{?is_opensuse}
BuildRequires:  python311-watchfiles
BuildRequires:  python311-pyproject-metadata
BuildRequires:  libboost_system-devel
BuildRequires:  libboost_program_options-devel
BuildRequires:  libboost_filesystem-devel
BuildRequires:  libboost_iostreams-devel
BuildRequires:  libboost_test-devel
BuildRequires:  graphviz-gnome
%endif

#opensuse_leap-15.6
%if 0%{?sle_version} == 150600 && 0%{?is_opensuse}
%global python3_sitearch /usr/lib64/python3.11/site-packages

BuildRequires:  libbz2-devel
BuildRequires:  libexpat-devel
BuildRequires:  libboost_system-devel
BuildRequires:  libboost_program_options-devel
BuildRequires:  libboost_filesystem-devel
BuildRequires:  libboost_iostreams-devel
BuildRequires:  libboost_test-devel
BuildRequires:  python311
BuildRequires:  python311-devel
#BuildRequires:  python311-watchfiles
BuildRequires:  graphviz-gnome
BuildRequires:  %{python_module devel}
BuildRequires:  %{python_module setuptools}
%endif

#ALmaLinux-9
%if 0%{?rhel} >= 9 || 0%{?fedora} >= 39
BuildRequires:  clang
BuildRequires:  clang-devel
BuildRequires:  python3-build
BuildRequires:  bzip2-devel
BuildRequires:  expat-devel
BuildRequires:  python3-pdm-pep517
BuildRequires:  python-unversioned-command
BuildRequires:  xpdf
%endif

%if 0%{?fedora} >= 39
BuildRequires:  python3-watchfiles
%endif

#opensuse
%if 0%{?suse_version} > 1400 && 0%{?is_opensuse}
Requires(post): glibc
%else
Requires(post): ldconfig
%endif
Requires:       boost >= 1.53.0


%description
A FOSS EDA tool suite for physical design. Provides an integrated
place & route for ASICs. Support symbolic layout (Alliance) and
real layout (GDS). While mainly written in C++, it has an extensive
Python interface.


%package doc
Summary:   Documentation of the Coriolis EDA toolchain
Requires:  %{name} = %{version}-%{release}
BuildArch: noarch


%description doc
The documentation is a subset of the Coriolis website. It provides
informations on:
* The Hurricane database (C++).
* The Coriolis Core (CRL) infrastructure (C++).
* The dedicated C++/Python wrapper (Isobar,C/C++/Python). The Python
  interface has been made so that it mimics closely the C++ one, so
  the C++ doc can be used for both.
* Some technical specifications.
* A general introduction and user's manual.


%package devel
Summary:   Development files for the Coriolis EDA toolchain
Requires:  %{name} = %{version}-%{release}


%description devel
Files needed to extend the tools in C++.

Warning: This package is only a stub for now.


%prep
%setup -q -n %{name}-%{version} -a 1
%patch -P 0 -p1 -b .clamp 


%build


%install
 CFLAGS=`echo ${CFLAGS} | sed 's,-Werror[^ ]*,,'`
 CXXFLAGS=`echo ${CFLAGS} | sed 's,-Werror[^ ]*,,'`
 CORIOLIS_TOP=%{_prefix}; export CORIOLIS_TOP
 DESTDIR=%{buildroot};    export DESTDIR

 cp $RPM_SOURCE_DIR/patchvenv.sh $RPM_SOURCE_DIR/find_files.sh .
 chmod u+x patchvenv.sh find_files.sh .
 patchVEnvArgs="--use-system-packages"
 if [ \( 0%{?fedora} -ge 39 \) -o \( 0%{?suse_version}%{?sle_version} -ne 0 \) ]; then
   patchVEnvArgs="${patchVEnvArgs} --remove-venv-watchfiles"
 fi
 ./patchvenv.sh ${patchVEnvArgs}
  
 make PREFIX=%{_prefix} -f Makefile.LIP6 help install
 if [ \( 0%{?sle_version} -eq 150600 \) -a \( 0%{?is_opensuse} -ne 0 \) ]; then
   %{__mkdir_p} %{buildroot}%{_datadir}/doc
   pushd %{buildroot}%{_datadir}/doc
   tar zxf %{SOURCE10}
   popd
 else
   make PREFIX=%{_prefix} -f Makefile.LIP6 help install_docs
 fi
 ./find_files.sh --buildroot=%{buildroot} --main
 ./find_files.sh --buildroot=%{buildroot} --devel
 ./find_files.sh --buildroot=%{buildroot} --docs


%post -p /sbin/ldconfig


%postun -p /sbin/ldconfig


%files -f coriolis-files.main
%defattr(-,root,root,-)
%dir %{python3_sitearch}/coriolis
%{python3_sitearch}/coriolis/*


%files devel -f coriolis-files.devel
%defattr(-,root,root,-)


%files doc -f coriolis-files.docs


%changelog
* Fri Sep 27 2024 Jean-Paul Chaput <Jean-Paul.Chaput@lip6.fr> - 2.5.5-32
- Manage the new "venv" snaphot that correctly include pdm.

* Fri Aug 18 2023 Mazher IQBAL <Mazher.Iqbal@lip6.fr> - 2.5.5-1
- Package initial creation.


