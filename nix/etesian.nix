{ generic, ... }:
let pkg =
  { doxygen, qt4, coriolis-coloquinte, coriolis-vlsisapd, coriolis-hurricane,
  , lib, coriolis-crlcore }:
  {
    name = "etesian";
    src = ../etesian;
    buildInputs = [
      qt4 coriolis-coloquinte coriolis-vlsisapd
      coriolis-hurricane coriolis-crlcore
    ];
    propagatedBuildInputs = [
      coriolis-coloquinte
    ];
    nativeBuildInputs = [ doxygen ];
    pythonImportsCheck = [ "Etesian" ];
    meta.license = lib.licenses.gpl2Plus;
  };
in generic pkg
