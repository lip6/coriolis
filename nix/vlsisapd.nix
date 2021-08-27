{ version, meta }:

{ lib, stdenv, python2, cmake, boost, bison, flex
, libxml2, qt4, ninja, coriolis-bootstrap, doxygen }:

let boostWithPython = boost.override { enablePython = true; python = python2; }; in

stdenv.mkDerivation {
  pname = "coriolis-vlsisapd";

  src = ../vlsisapd;

  buildInputs = [ python2 boostWithPython coriolis-bootstrap libxml2 bison flex qt4 ];
  nativeBuildInputs = [ cmake ninja doxygen ];

  inherit version meta;
}
