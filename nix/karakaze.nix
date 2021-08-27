{ version, meta }:

{ lib, stdenv, cmake, ninja, python2, qt4, coriolis-crlcore
, coriolis-bootstrap, coriolis-vlsisapd, coriolis-hurricane }:

stdenv.mkDerivation {
  pname = "coriolis-karakaze";

  src = ../karakaze;

  buildInputs = [
    python2 coriolis-bootstrap coriolis-vlsisapd
    coriolis-crlcore coriolis-hurricane qt4
  ];
  nativeBuildInputs = [ cmake ninja ];

  inherit version meta;
}
