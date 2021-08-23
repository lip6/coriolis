{ version, meta }:

{ lib, stdenv, python2, cmake, boost, qt4, ninja, coriolis-bootstrap }:

let boostWithPython = boost.override { enablePython = true; python = python2; }; in

stdenv.mkDerivation {
  pname = "coriolis-coloquinte";

  src = ../coloquinte;

  cmakeFlags = [ "-DCMAKE_MODULE_PATH=${coriolis-bootstrap}/share/cmake/Modules" ];

  buildInputs = [ boostWithPython coriolis-bootstrap qt4 ];
  nativeBuildInputs = [ cmake ninja ];

  inherit version meta;
}
