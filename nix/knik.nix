{ version, meta }:

{ lib, stdenv, cmake, ninja, python3, boost
, coriolis-bootstrap, coriolis-hurricane
, coriolis-crlcore, coriolis-flute, qt4 }:

let boostWithPython = boost.override { enablePython = true; python = python3; }; in

stdenv.mkDerivation {
  pname = "coriolis-knik";

  src = ../knik;

  buildInputs = [
    python3 boostWithPython coriolis-bootstrap
    coriolis-hurricane coriolis-crlcore
    coriolis-flute qt4
  ];
  nativeBuildInputs = [ cmake ninja ];

  inherit version;

  meta = meta // { license = lib.licenses.gpl2Plus; };
}
