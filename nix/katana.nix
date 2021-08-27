{ version, meta }:

{ lib, stdenv, cmake, ninja, python2, boost
, coriolis-bootstrap, coriolis-vlsisapd, coriolis-hurricane
, coriolis-crlcore, coriolis-etesian, qt4
, coriolis-flute, coriolis-anabatic
, coriolis-lefdef, libxml2, doxygen }:

let boostWithPython = boost.override { enablePython = true; python = python2; }; in

stdenv.mkDerivation {
  pname = "coriolis-katana";

  src = ../katana;

  buildInputs = [
    python2 boostWithPython coriolis-bootstrap
    coriolis-vlsisapd coriolis-hurricane coriolis-crlcore
    coriolis-etesian qt4 coriolis-flute coriolis-anabatic
    coriolis-lefdef libxml2
  ];
  nativeBuildInputs = [ cmake ninja doxygen ];

  inherit version meta;
}
