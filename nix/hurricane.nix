{ version, meta }:

{ lib, stdenv, python2, cmake, boost, bison, flex
, libxml2, rapidjson, which, qt4, zlib, bzip2, ninja
, coriolis-bootstrap, coriolis-vlsisapd }:

let boostWithPython = boost.override { enablePython = true; python = python2; }; in

stdenv.mkDerivation {
  pname = "coriolis-hurricane";

  src = ../hurricane;

  cmakeFlags = [ "-DCMAKE_MODULE_PATH=${coriolis-bootstrap}/share/cmake/Modules;${coriolis-vlsisapd}/share/cmake/Modules" ];

  buildInputs = [ python2 boostWithPython coriolis-bootstrap libxml2 bison flex qt4 bzip2 coriolis-vlsisapd rapidjson ];
  nativeBuildInputs = [ cmake ninja ];

  inherit version meta;
}
