{ generic, ... }:
let pkg =
  { doxygen, qt4, lib, coriolis-coloquinte
  , coriolis-hurricane, coriolis-crlcore
  , coriolis-flute, coriolis-etesian }:
  {
    name = "anabatic";
    src = ../anabatic;
    buildInputs = [
      qt4 coriolis-coloquinte
      coriolis-hurricane coriolis-crlcore
      coriolis-flute coriolis-etesian
    ];
    nativeBuildInputs = [ doxygen ];
    pythonImportsCheck = [ "Anabatic" ];
    meta.license = lib.licenses.gpl2Plus;
  };
in generic pkg
