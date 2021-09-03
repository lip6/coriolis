{ generic, ... }:

let pkg =
  { coriolis-vlsisapd, coriolis-hurricane, coriolis-tutorial
  , coriolis-crlcore, qt4, coriolis-katana, coriolis-bora
  , coriolis-flute, coriolis-anabatic, coriolis-coloquinte
  , coriolis-lefdef, coriolis-knik, coriolis-kite
  , coriolis-katabatic, coriolis-etesian, coriolis-equinox
  , coriolis-solstice, libxml2, doxygen, python2Packages }:
  {
    name = "unicorn";
    src = ../unicorn;

    propagatedBuildInputs = [
      coriolis-vlsisapd coriolis-hurricane coriolis-crlcore
      coriolis-katana qt4 coriolis-flute coriolis-anabatic
      coriolis-lefdef libxml2 coriolis-knik coriolis-kite
      coriolis-katabatic coriolis-etesian coriolis-equinox
      coriolis-solstice coriolis-bora coriolis-tutorial
    ];
    nativeBuildInputs = [ doxygen python2Packages.wrapPython ];
    pythonImportsCheck = [ "Unicorn" ];

    postFixup = ''
      wrapPythonPrograms
    '';
  };
in generic pkg
