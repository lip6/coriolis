{ generic, ... }:
let pkg =
  { lib, libxml2, qt4, bison, flex, doxygen, coriolis-vlsisapd
  , coriolis-lefdef, coriolis-hurricane, python2Packages }:
  {
    name = "crlcore";
    src = ../crlcore;
    buildInputs = [ libxml2 qt4 coriolis-lefdef coriolis-vlsisapd coriolis-hurricane python2Packages.pyqt4 ];
    nativeBuildInputs = [ bison flex doxygen python2Packages.pyqt4 ];
    pythonImportsCheck = [
      "CRL" "Constant" "helpers.io" "helpers.technology"
      "helpers.utils" "helpers.analogtechno" "helpers" "helpers.overlay"
    ];
    postInstall = ''
      ln -s crlcore/helpers $out/${python2Packages.python.sitePackages}/helpers
    '';
    meta.license = lib.licenses.gpl2Plus;
  };
in generic pkg
