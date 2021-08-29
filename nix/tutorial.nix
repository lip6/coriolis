{ generic, ... }:

let pkg =
  { coriolis-vlsisapd, coriolis-hurricane
  , coriolis-crlcore, qt4, coriolis-lefdef, libxml2
  , doxygen }:
  {
    name = "tutorial";
    src = ../tutorial;

    buildInputs = [
      coriolis-vlsisapd coriolis-hurricane coriolis-crlcore
      coriolis-lefdef libxml2 qt4
    ];
    nativeBuildInputs = [ doxygen ];
    pythonImportsCheck = [ "Tutorial" ];
  };
in generic pkg
