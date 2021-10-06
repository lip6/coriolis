{ generic, ... }:
let pkg =
  { doxygen, qt4, coriolis-coloquinte, coriolis-vlsisapd
  , coriolis-hurricane, coriolis-crlcore, libxml2
  , coriolis-flute, coriolis-etesian, coriolis-lefdef
  , coriolis-anabatic }:
  {
    name = "katana";
    src = ../katana;
    buildInputs = [
      qt4 coriolis-coloquinte coriolis-vlsisapd
      coriolis-hurricane coriolis-crlcore
      coriolis-flute coriolis-etesian libxml2
      coriolis-lefdef coriolis-anabatic
    ];
    nativeBuildInputs = [ doxygen ];
    pythonImportsCheck = [ "Katana" ];
    meta.license = lib.licenses.gpl2Plus;
  };
in generic pkg
