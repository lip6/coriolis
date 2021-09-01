{ version, meta }:

{ lib, stdenv, python2, cmake, boost, qt4, ninja, coriolis-bootstrap, doxygen }:

let boostWithPython = boost.override { enablePython = true; python = python2; }; in

stdenv.mkDerivation {
  pname = "coriolis-coloquinte";

  src = ../coloquinte;

  buildInputs = [ boostWithPython coriolis-bootstrap qt4 ];
  nativeBuildInputs = [ cmake ninja doxygen ];

  cmakeFlags = [ "-DWITH_OPENMP:STRING=TRUE" ];

  inherit version meta;
}
