{ version, meta }:

{ lib, stdenv, python3, cmake, boost, qt4, ninja, coriolis-bootstrap, doxygen }:

let boostWithPython = boost.override { enablePython = true; python = python3; }; in

stdenv.mkDerivation {
  pname = "coriolis-coloquinte";

  src = ../coloquinte;

  buildInputs = [ boostWithPython coriolis-bootstrap qt4 ];
  nativeBuildInputs = [ cmake ninja doxygen ];

  inherit version;

  meta = meta // { license = lib.licenses.gpl2Plus; };
}
