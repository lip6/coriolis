{ version, meta }:

{ lib, stdenv, cmake, ninja, python2, boost
, coriolis-bootstrap, coriolis-vlsisapd, coriolis-hurricane
, coriolis-crlcore, qt4, coriolis-katana
, coriolis-flute, coriolis-anabatic, coriolis-coloquinte
, coriolis-lefdef, coriolis-knik, coriolis-kite
, coriolis-katabatic, coriolis-etesian, coriolis-equinox
, coriolis-solstice, libxml2, doxygen }:

let boostWithPython = boost.override { enablePython = true; python = python2; }; in

stdenv.mkDerivation {
  pname = "coriolis-unicorn";

  src = ../unicorn;

  buildInputs = [
    python2 boostWithPython coriolis-bootstrap
    coriolis-vlsisapd coriolis-hurricane coriolis-crlcore
    coriolis-katana qt4 coriolis-flute coriolis-anabatic
    coriolis-lefdef libxml2 coriolis-knik coriolis-kite
    coriolis-katabatic coriolis-etesian coriolis-equinox
    coriolis-solstice
  ];
  nativeBuildInputs = [ cmake ninja doxygen ];

  inherit version meta;
}
