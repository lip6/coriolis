{ version, meta, ... }:

{ lib, stdenv, cmake, ninja, python2, boost
, coriolis-bootstrap, coriolis-vlsisapd, coriolis-hurricane
, coriolis-crlcore, qt4 }:

let boostWithPython = boost.override { enablePython = true; python = python2; }; in

stdenv.mkDerivation {
  pname = "coriolis-equinox";

  src = ../equinox;

  buildInputs = [
    python2 boostWithPython coriolis-bootstrap
    coriolis-vlsisapd coriolis-hurricane coriolis-crlcore
    qt4
  ];
  nativeBuildInputs = [ cmake ninja ];

  inherit version;

  meta = meta // { license = lib.licenses.gpl2Plus; }
}
