{ generic, ... }:
let pkg =
  { coriolis-vlsisapd, coriolis-hurricane, coriolis-crlcore }:
  {
    name = "flute";
    src = ../flute;
    buildInputs = [ coriolis-vlsisapd coriolis-hurricane coriolis-crlcore ];
    nativeBuildInputs = [ ];
    pythonImportsCheck = [ "Flute" ];
  };
in generic pkg
