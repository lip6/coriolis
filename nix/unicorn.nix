{ generic, ... }:

let pkg =
  { lib, coriolis-hurricane, coriolis-tutorial
  , coriolis-crlcore, qt4, coriolis-katana, coriolis-bora
  , coriolis-flute, coriolis-anabatic, coriolis-coloquinte
  , coriolis-lefdef, coriolis-knik, coriolis-kite
  , coriolis-katabatic, coriolis-etesian
  , libxml2, doxygen, python3Packages }:
  {
    name = "unicorn";
    src = ../unicorn;

    propagatedBuildInputs = [
      coriolis-hurricane coriolis-crlcore
      coriolis-katana qt4 coriolis-flute coriolis-anabatic
      coriolis-lefdef libxml2 coriolis-knik coriolis-kite
      coriolis-katabatic coriolis-etesian
      coriolis-bora coriolis-tutorial
    ];
    nativeBuildInputs = [ doxygen python3Packages.wrapPython ];
    pythonImportsCheck = [ "Unicorn" ];

    postFixup = ''
      wrapPythonPrograms
    '';

    meta.license = lib.licenses.gpl2Plus;
  };
in generic pkg
