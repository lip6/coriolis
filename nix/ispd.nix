{ version, meta }:

{ lib, stdenv, cmake, ninja, python2, boost
, coriolis-bootstrap, coriolis-vlsisapd, coriolis-hurricane
, coriolis-crlcore, coriolis-flute, coriolis-knik
, coriolis-lefdef, coriolis-katabatic, libxml2, qt4 }:

let boostWithPython = boost.override { enablePython = true; python = python2; }; in

stdenv.mkDerivation {
  pname = "coriolis-ispd";

  src = ../ispd;

  buildInputs = [
    python2 boostWithPython coriolis-bootstrap libxml2
    coriolis-vlsisapd coriolis-hurricane coriolis-crlcore
    coriolis-flute coriolis-knik coriolis-lefdef
    coriolis-katabatic qt4
  ];
  nativeBuildInputs = [ cmake ninja ];

  inherit version;

  meta = meta // { broken = true; license = lib.licenses.gpl2Plus; };
}
