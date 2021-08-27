{ version, meta }:

{ lib, stdenv, python2, cmake, boost, bison, flex, libxml2, qt4, ninja
, coriolis-bootstrap, coriolis-vlsisapd, coriolis-hurricane
, coriolis-lefdef, doxygen }:

let boostWithPython = boost.override { enablePython = true; python = python2; }; in

stdenv.mkDerivation {
  pname = "coriolis-crlcore";

  src = ../crlcore;

  buildInputs = [
    python2 boostWithPython coriolis-bootstrap coriolis-lefdef
    coriolis-hurricane coriolis-vlsisapd libxml2 bison flex qt4
  ];
  nativeBuildInputs = [ cmake ninja doxygen ];

  inherit version meta;
}
