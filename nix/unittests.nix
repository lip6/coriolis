{ version, meta }:

{ lib, stdenv, cmake, ninja, python2, boost
, coriolis-bootstrap, coriolis-vlsisapd, coriolis-hurricane
, coriolis-crlcore, coriolis-lefdef, qt4 }:

let boostWithPython = boost.override { enablePython = true; python = python2; }; in

stdenv.mkDerivation {
  pname = "coriolis-unittests";

  src = ../unittests;

  buildInputs = [
    python2 boostWithPython coriolis-bootstrap qt4
    coriolis-vlsisapd coriolis-hurricane coriolis-crlcore
    coriolis-lefdef
  ];
  nativeBuildInputs = [ cmake ninja ];

  inherit version meta;
}
