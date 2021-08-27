{ version, meta }:

{ lib, stdenv, cmake, ninja, coriolis-bootstrap, python3Packages }:

stdenv.mkDerivation {
  pname = "coriolis-documentation";

  src = ../documentation;

  cmakeFlags = [ "-DBUILD_DOC=ON" ];

  buildInputs = [];
  nativeBuildInputs = [ cmake ninja coriolis-bootstrap python3Packages.pelican ];

  inherit version;

  meta = meta // { broken = true; };
}
