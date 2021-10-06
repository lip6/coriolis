{ version, meta }:

{ lib, stdenv, cmake, ninja, python2, qt4, coriolis-crlcore
, coriolis-bootstrap, coriolis-vlsisapd, coriolis-hurricane
, coriolis-nimbus, doxygen, boost }:

let boostWithPython = boost.override { enablePython = true; python = python2; }; in

stdenv.mkDerivation {
  pname = "coriolis-metis";

  src = ../metis;

  buildInputs = [
    python2 coriolis-bootstrap coriolis-vlsisapd coriolis-nimbus
    coriolis-crlcore coriolis-hurricane qt4 boostWithPython
  ];
  nativeBuildInputs = [ cmake ninja doxygen ];

  inherit version;

  meta = meta // { broken = true; license = lib.licenses.gpl2Plus; };
}
