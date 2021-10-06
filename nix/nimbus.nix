{ version, meta }:

{ lib, stdenv, cmake, ninja, python2, qt4, coriolis-crlcore
, coriolis-bootstrap, coriolis-vlsisapd, coriolis-hurricane
, doxygen, boost }:

let boostWithPython = boost.override { enablePython = true; python = python2; }; in

stdenv.mkDerivation {
  pname = "coriolis-nimbus";

  src = ../nimbus;

  buildInputs = [
    python2 coriolis-bootstrap coriolis-vlsisapd
    coriolis-crlcore coriolis-hurricane qt4 boostWithPython
  ];
  nativeBuildInputs = [ cmake ninja doxygen ];

  inherit version;

  meta = meta // { broken = true; license = lib.licenses.gpl2Plus; };
}
