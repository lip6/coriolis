{ version, meta }:

{ lib, stdenv, cmake, boost, bison, flex
, libxml2, qt4, ninja, coriolis-bootstrap, doxygen
, python2Packages }:

let boostWithPython = boost.override { enablePython = true; inherit (python2Packages) python; }; in

python2Packages.toPythonModule (stdenv.mkDerivation {
  pname = "coriolis-vlsisapd";

  src = ../vlsisapd;

  buildInputs = [ python2Packages.python boostWithPython coriolis-bootstrap libxml2 bison flex qt4 ];
  nativeBuildInputs = [ cmake ninja doxygen ];

  inherit version meta;
})
