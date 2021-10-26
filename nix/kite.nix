{ generic, ... }:

let pkg =
  { lib, coriolis-lefdef, coriolis-knik, coriolis-katabatic
  , coriolis-vlsisapd, coriolis-hurricane
  , coriolis-crlcore, qt4, coriolis-flute, libxml2, doxygen }:
  {
    name = "kite";
    src = ../kite;
    buildInputs = [
      coriolis-vlsisapd coriolis-hurricane coriolis-crlcore
      coriolis-katabatic qt4 coriolis-flute coriolis-knik libxml2
      coriolis-lefdef
    ];
    nativeBuildInputs = [ doxygen ];
    pythonImportsCheck = [ "Kite" ];
    meta.license = lib.licenses.gpl2Plus;
  };
in generic pkg
