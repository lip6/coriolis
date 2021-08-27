{ version, meta }:

{ lib, stdenv, cmake, ninja, python2, boost
, coriolis-bootstrap, coriolis-vlsisapd, coriolis-hurricane
, coriolis-crlcore, qt4, coriolis-katana
, coriolis-flute, coriolis-anabatic
, coriolis-lefdef, qwt6_qt4, doxygen }:

let boostWithPython = boost.override { enablePython = true; python = python2; }; in

stdenv.mkDerivation {
  pname = "coriolis-bora";

  src = ../bora;

  buildInputs = [
    python2 boostWithPython coriolis-bootstrap
    coriolis-vlsisapd coriolis-hurricane coriolis-crlcore
    coriolis-katana qt4 coriolis-flute coriolis-anabatic
    coriolis-lefdef qwt6_qt4
  ];
  nativeBuildInputs = [ cmake ninja doxygen ];

  inherit version meta;
}
