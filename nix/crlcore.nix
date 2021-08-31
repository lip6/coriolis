{ generic, ... }:
let pkg =
  { libxml2, qt4, bison, flex, doxygen, coriolis-vlsisapd
  , coriolis-lefdef, coriolis-hurricane, python2Packages }:
  {
    name = "crlcore";
    src = ../crlcore;
    buildInputs = [ libxml2 qt4 coriolis-lefdef coriolis-vlsisapd coriolis-hurricane python2Packages.pyqt4 python2Packages.sip_4 ];
    nativeBuildInputs = [ bison flex doxygen python2Packages.pyqt4 ];
    pythonImportsCheck = [
      "CRL" "Constant" "helpers.io" "helpers.technology"
      "helpers.utils" "helpers.analogtechno" "helpers" "helpers.overlay"
    ];
    continuation = drv: drv.overrideAttrs (o: {
      postInstall = (o.postInstall or "") + ''
        ln -s crlcore/helpers $out/${python2Packages.python.sitePackages}/helpers
      '';
    });
  };
in generic pkg
