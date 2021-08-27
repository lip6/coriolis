{ version, meta }:

{ lib, stdenv, cmake, ninja, python2
, coriolis-bootstrap, coriolis-vlsisapd, coriolis-hurricane
, coriolis-crlcore }:

stdenv.mkDerivation {
  pname = "coriolis-cumulus";

  src = ../cumulus;

  buildInputs = [ python2 coriolis-bootstrap coriolis-vlsisapd coriolis-hurricane coriolis-crlcore ];
  nativeBuildInputs = [ cmake ninja ];

  inherit version meta;
}
