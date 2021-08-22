{ version, meta }:

{ lib, stdenv, python2, cmake, boost, bison, flex
, libxml2, rapidjson, which, qt4, zlib, bzip2, ninja
, coriolis-bootstrap, coriolis-vlsisapd }:

let boostWithPython = boost.override { enablePython = true; python = python2; }; in

stdenv.mkDerivation {
  pname = "coriolis-lefdef";

  src = ../lefdef;

  hardeningDisable = [ "format" ];

  cmakeFlags = [ "-DCMAKE_MODULE_PATH=${coriolis-bootstrap}/share/cmake/Modules" ];

  buildInputs = [ python2 boostWithPython coriolis-bootstrap bison flex zlib ];
  nativeBuildInputs = [ cmake ninja ];

  inherit version meta;
}
