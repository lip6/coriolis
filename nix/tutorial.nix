{ generic, ... }:

let pkg =
  { lib, coriolis-hurricane
  , coriolis-crlcore, qt4, coriolis-lefdef, libxml2
  , doxygen }:
  {
    name = "tutorial";
    src = ../tutorial;

    buildInputs = [
      coriolis-hurricane coriolis-crlcore
      coriolis-lefdef libxml2 qt4
    ];
    nativeBuildInputs = [ doxygen ];
    pythonImportsCheck = [ "Tutorial" ];
    meta.license = lib.licenses.gpl2Plus;
  };
in generic pkg
