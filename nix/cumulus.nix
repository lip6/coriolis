{ version, meta }:

{ lib, stdenv, cmake, ninja, python2
, coriolis-bootstrap, coriolis-vlsisapd, coriolis-hurricane
, coriolis-crlcore }:

stdenv.mkDerivation {
  pname = "coriolis-cumulus";

  src = ../cumulus;

  cmakeFlags = [
    (
      "-DCMAKE_MODULE_PATH="
        + "${coriolis-bootstrap}/share/cmake/Modules"
        + ";${coriolis-vlsisapd}/share/cmake/Modules"
        + ";${coriolis-hurricane}/share/cmake/Modules"
        + ";${coriolis-crlcore}/share/cmake/Modules"
    )
  ];

  buildInputs = [ python2 coriolis-bootstrap coriolis-vlsisapd coriolis-hurricane coriolis-crlcore ];
  nativeBuildInputs = [ cmake ninja ];

  inherit version meta;
}
