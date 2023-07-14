{ generic, ... }:
let pkg =
  { lib, coriolis-hurricane, coriolis-crlcore }:
  {
    name = "flute";
    src = ../flute;
    buildInputs = [ coriolis-hurricane coriolis-crlcore ];
    nativeBuildInputs = [ ];
    pythonImportsCheck = [ "Flute" ];
    meta.license = lib.licenses.gpl2Plus;
  };
in generic pkg
