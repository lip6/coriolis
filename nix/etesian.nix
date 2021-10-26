{ generic, ... }:
let pkg =
  { doxygen, qt4, coriolis-coloquinte, coriolis-hurricane
  , lib, coriolis-crlcore }:
  {
    name = "etesian";
    src = ../etesian;
    buildInputs = [
      qt4 coriolis-coloquinte
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
