{ version, meta }:

{ lib, stdenv, cmake, ninja, python2, boost
, coriolis-bootstrap, coriolis-vlsisapd, coriolis-hurricane
, coriolis-crlcore, coriolis-coloquinte, qt4 }:

let boostWithPython = boost.override { enablePython = true; python = python2; }; in

stdenv.mkDerivation {
  pname = "coriolis-etesian";

  src = ../etesian;

  buildInputs = [
    qt4 python2 boostWithPython coriolis-bootstrap coriolis-vlsisapd
    coriolis-hurricane coriolis-coloquinte coriolis-crlcore
  ];
  propagatedBuildInputs = [ coriolis-coloquinte ];
  nativeBuildInputs = [ cmake ninja ];

  inherit version meta;
}
