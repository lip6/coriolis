{ version, meta }:

{ lib, stdenv, cmake, ninja, python3, boost
, coriolis-bootstrap, coriolis-hurricane
, coriolis-crlcore, coriolis-lefdef, qt4 }:

let boostWithPython = boost.override { enablePython = true; python = python3; }; in

stdenv.mkDerivation {
  pname = "coriolis-unittests";

  src = ../unittests;

  buildInputs = [
    python3 boostWithPython coriolis-bootstrap qt4
    coriolis-hurricane coriolis-crlcore
    coriolis-lefdef
  ];
  nativeBuildInputs = [ cmake ninja ];

  doInstallCheck = true;

  installCheckPhase = ''
    runHook preInstallCheck
    $out/bin/unittests
    $out/bin/unittests --rb-tree
    $out/bin/unittests --intv-tree
    runHook postInstallCheck
  '';

  inherit version;

  meta = meta // { license = lib.licenses.gpl2Plus; };
}
