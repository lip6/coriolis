{ generic, ... }:

let pkg =
  { coriolis-vlsisapd, coriolis-hurricane
  , coriolis-crlcore, coriolis-flute, coriolis-knik
  , qt4, doxygen }:
  {
    name = "katabatic";
    src = ../katabatic;

    buildInputs = [
      coriolis-vlsisapd coriolis-hurricane coriolis-crlcore
      coriolis-flute coriolis-knik qt4
    ];
    nativeBuildInputs = [ doxygen ];
    pythonImportsCheck = [ "Katabatic" ];
    meta.license = lib.licenses.gpl2Plus;
  };
in generic pkg
