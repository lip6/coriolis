{ version, meta }:

{ lib, stdenv, cmake, ninja, python2, boost
, coriolis-bootstrap, coriolis-vlsisapd, coriolis-hurricane
, coriolis-crlcore }:

let boostWithPython = boost.override { enablePython = true; python = python2; }; in

stdenv.mkDerivation {
  pname = "coriolis-flute";

  src = ../flute;

  cmakeFlags = [
    (
      "-DCMAKE_MODULE_PATH="
        + "${coriolis-bootstrap}/share/cmake/Modules"
        + ";${coriolis-vlsisapd}/share/cmake/Modules"
        + ";${coriolis-hurricane}/share/cmake/Modules"
        + ";${coriolis-crlcore}/share/cmake/Modules"
    )
  ];

  buildInputs = [ python2 boostWithPython coriolis-bootstrap coriolis-vlsisapd coriolis-hurricane coriolis-crlcore ];
  nativeBuildInputs = [ cmake ninja ];

  inherit version meta;
}
