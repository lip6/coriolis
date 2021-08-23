{ version, meta }:

{ lib, stdenv, python2, cmake, ninja }:

stdenv.mkDerivation {
  pname = "coriolis-bootstrap";

  src = ../bootstrap;

  buildInputs = [ python2 ];
  nativeBuildInputs = [ cmake ninja ];

  inherit version meta;
}
