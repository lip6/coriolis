{ generic, ... }:

let pkg =
  { coriolis-vlsisapd, coriolis-hurricane
  , coriolis-crlcore, qt4, coriolis-katana
  , coriolis-flute, coriolis-anabatic
  , coriolis-lefdef, qwt6_qt4, doxygen }:
  {
    name = "bora";
    src = ../bora;
    buildInputs = [
      coriolis-vlsisapd coriolis-hurricane coriolis-crlcore
      coriolis-katana qt4 coriolis-flute coriolis-anabatic
      coriolis-lefdef qwt6_qt4
    ];
    nativeBuildInputs = [ doxygen ];
    pythonImportsCheck = "Bora";
    meta.license = lib.licenses.gpl2Plus;
  };
in generic pkg
