{ version, meta }:

{ lib, stdenv, python3, cmake, ninja }:

stdenv.mkDerivation {
  pname = "coriolis-bootstrap";

  src = ../bootstrap;

  buildInputs = [ python3 ];
  nativeBuildInputs = [ cmake ninja ];

  inherit version;

  meta = meta // { license = lib.licenses.gpl2Plus; };
}
