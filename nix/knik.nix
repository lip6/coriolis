{ version, meta }:

{ lib, stdenv, cmake, ninja, python2, boost
, coriolis-bootstrap, coriolis-vlsisapd, coriolis-hurricane
, coriolis-crlcore, coriolis-flute, qt4 }:

let boostWithPython = boost.override { enablePython = true; python = python2; }; in

stdenv.mkDerivation {
  pname = "coriolis-knik";

  src = ../knik;

  buildInputs = [
    python2 boostWithPython coriolis-bootstrap
    coriolis-vlsisapd coriolis-hurricane coriolis-crlcore
    coriolis-flute qt4
  ];
  nativeBuildInputs = [ cmake ninja ];

  inherit version meta;
}
