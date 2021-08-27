{ version, meta }:

{ lib, stdenv, cmake, ninja, python2, boost
, coriolis-lefdef, coriolis-knik, coriolis-katabatic
, coriolis-bootstrap, coriolis-vlsisapd, coriolis-hurricane
, coriolis-crlcore, qt4, coriolis-flute, libxml2, doxygen }:

let boostWithPython = boost.override { enablePython = true; python = python2; }; in

stdenv.mkDerivation {
  pname = "coriolis-kite";

  src = ../kite;

  buildInputs = [
    python2 boostWithPython coriolis-bootstrap
    coriolis-vlsisapd coriolis-hurricane coriolis-crlcore
    coriolis-katabatic qt4 coriolis-flute coriolis-knik libxml2
    coriolis-lefdef
  ];
  nativeBuildInputs = [ cmake ninja doxygen ];

  inherit version meta;
}
