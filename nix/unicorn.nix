{ generic, ... }:

let pkg =
  { coriolis-vlsisapd, coriolis-hurricane
  , coriolis-crlcore, qt4, coriolis-katana
  , coriolis-flute, coriolis-anabatic, coriolis-coloquinte
  , coriolis-lefdef, coriolis-knik, coriolis-kite
  , coriolis-katabatic, coriolis-etesian, coriolis-equinox
  , coriolis-solstice, libxml2, doxygen }:
  {
    name = "unicorn";
    src = ../unicorn;

    buildInputs = [
      coriolis-vlsisapd coriolis-hurricane coriolis-crlcore
      coriolis-katana qt4 coriolis-flute coriolis-anabatic
      coriolis-lefdef libxml2 coriolis-knik coriolis-kite
      coriolis-katabatic coriolis-etesian coriolis-equinox
      coriolis-solstice
    ];
    nativeBuildInputs = [ doxygen ];
    pythonImportsCheck = [ "Unicorn" ];
  };
in generic pkg
