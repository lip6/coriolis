{ generic, ... }:

let pkg =
  { lib, coriolis-hurricane
  , coriolis-crlcore, coriolis-flute, coriolis-knik
  , qt4, doxygen }:
  {
    name = "katabatic";
    src = ../katabatic;

    buildInputs = [
      coriolis-hurricane coriolis-crlcore
      coriolis-flute coriolis-knik qt4
    ];
    nativeBuildInputs = [ doxygen ];
    pythonImportsCheck = [ "Katabatic" ];
    meta.license = lib.licenses.gpl2Plus;
  };
in generic pkg
