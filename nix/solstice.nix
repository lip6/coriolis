{ version, meta }:

{ lib, stdenv, cmake, ninja, python2, qt4, coriolis-crlcore
, coriolis-bootstrap, coriolis-vlsisapd, coriolis-hurricane
, coriolis-equinox, boost }:

let boostWithPython = boost.override { enablePython = true; python = python2; }; in

stdenv.mkDerivation {
  pname = "coriolis-solstice";

  src = ../solstice;

  buildInputs = [
    python2 coriolis-bootstrap coriolis-vlsisapd
    coriolis-crlcore coriolis-hurricane qt4 boostWithPython
    coriolis-equinox
  ];
  nativeBuildInputs = [ cmake ninja ];

  inherit version meta;
}
