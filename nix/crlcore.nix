{ generic, ... }:
let pkg =
  { lib, libxml2, qt4, bison, flex, doxygen, coriolis-vlsisapd
  , coriolis-lefdef, coriolis-hurricane, python3Packages }:
  rec {
    name = "crlcore";
    src = ../crlcore;
    propagatedBuildInputs = [ libxml2 qt4 coriolis-lefdef coriolis-vlsisapd coriolis-hurricane python3Packages.pyqt4 ];
    nativeBuildInputs = [ bison flex doxygen python3Packages.pyqt4 ];
    postInstall = ''
      ln -s crlcore/helpers $out/${python3Packages.python.sitePackages}/helpers
      ln -s -t $out/${python3Packages.python.sitePackages} $out/etc/coriolis2/*

      # for import check
      mkdir -p /build/coriolistop/etc/coriolis2
      export CORIOLIS_TOP=/build/coriolistop
    '';
    pythonImportsCheck = [
      "CRL" "Constant" "helpers.io" "helpers.technology"
      "helpers.utils" "helpers.analogtechno" "helpers" "helpers.overlay"
    ];
    meta.license = lib.licenses.gpl2Plus;
  };
in generic pkg
