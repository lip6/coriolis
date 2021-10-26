{ version, meta }:

{ lib, stdenv, cmake, ninja, python3, qt4, coriolis-crlcore
, coriolis-bootstrap, coriolis-hurricane
, coriolis-equinox, boost }:

let boostWithPython = boost.override { enablePython = true; python = python3; }; in

stdenv.mkDerivation {
  pname = "coriolis-solstice";

  src = ../solstice;

  buildInputs = [
    python3 coriolis-bootstrap
    coriolis-crlcore coriolis-hurricane qt4 boostWithPython
    coriolis-equinox
  ];
  nativeBuildInputs = [ cmake ninja ];

  inherit version;

  meta = meta // { license = lib.licenses.gpl2Plus; };
}
