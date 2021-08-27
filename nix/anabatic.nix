{ version, meta }:

{ lib, stdenv, cmake, ninja, python2, boost
, coriolis-bootstrap, coriolis-vlsisapd, coriolis-hurricane
, coriolis-crlcore, coriolis-etesian, qt4
, coriolis-flute }:

let boostWithPython = boost.override { enablePython = true; python = python2; }; in

stdenv.mkDerivation {
  pname = "coriolis-anabatic";

  src = ../anabatic;

  buildInputs = [
    python2 boostWithPython coriolis-bootstrap
    coriolis-vlsisapd coriolis-hurricane coriolis-crlcore
    coriolis-etesian qt4 coriolis-flute
  ];
  nativeBuildInputs = [ cmake ninja ];

  inherit version meta;
}
