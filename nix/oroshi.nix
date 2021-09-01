{ generic, ... }:

let pkg =
  { qt4, coriolis-crlcore, doxygen, lib, python2Packages
  , coriolis-vlsisapd, coriolis-hurricane }:
  {
    name = "oroshi";
    src = ../oroshi;

    buildInputs = [
      coriolis-vlsisapd qt4 python2Packages.pyqt4
      coriolis-crlcore coriolis-hurricane qt4
      python2Packages.numpy
    ];
    nativeBuildInputs = [ doxygen ];

    postInstall = ''
      # for import check
      mkdir -p /build/coriolistop/etc/coriolis2
      export CORIOLIS_TOP=/build/coriolistop
    '';
    pythonImportsCheck = [
      "oroshi.wip_transistor"
      "oroshi.wip_dp"
      "oroshi.wip_csp"
      "oroshi.stack"
      "oroshi.resistorsnake"
      "oroshi.resistor"
      "oroshi.paramsmatrix"
      "oroshi.nonunitcapacitor"
      "oroshi.multicapacitor"
      "oroshi.dtr"
      "oroshi.capacitorvrtracks"
      "oroshi.capacitorunit"
      "oroshi.capacitorroutedsingle"
      "oroshi.capacitorrouted"
      "oroshi.capacitormatrix"
      "oroshi"
    ];

    meta.license = lib.licenses.gpl2Plus;
  };
in generic pkg
