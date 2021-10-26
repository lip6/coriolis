{ version, meta }:

{ lib, stdenv, cmake, ninja, python3, boost
, coriolis-bootstrap, coriolis-hurricane
, coriolis-crlcore, coriolis-flute, coriolis-knik
, coriolis-lefdef, coriolis-katabatic, libxml2, qt4 }:

let boostWithPython = boost.override { enablePython = true; python = python3; }; in

stdenv.mkDerivation {
  pname = "coriolis-ispd";

  src = ../ispd;

  buildInputs = [
    python3 boostWithPython coriolis-bootstrap libxml2
    coriolis-hurricane coriolis-crlcore
    coriolis-flute coriolis-knik coriolis-lefdef
    coriolis-katabatic qt4
  ];
  nativeBuildInputs = [ cmake ninja ];

  inherit version;

  meta = meta // { broken = true; license = lib.licenses.gpl2Plus; };
}
