{ version, meta }:

{ lib, stdenv, python2, cmake, boost, bison, flex
, libxml2, rapidjson, which, qt4, zlib, bzip2, ninja }:

stdenv.mkDerivation {
  pname = "coriolis-bootstrap";

  src = ../bootstrap;

  buildInputs = [ python2 ];
  nativeBuildInputs = [ cmake ninja ];

  inherit version meta;
}
