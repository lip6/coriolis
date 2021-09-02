{ alliance-src, ... }:

{ stdenv, coriolis-unicorn, alliance, yosys }:
stdenv.mkDerivation {
  pname = "alliance-check-toolkit";
  version = builtins.substring 0 8 alliance-src.lastModifiedDate;
  src = alliance-src;

  nativeBuildInputs = [ alliance coriolis-unicorn yosys ];

  YOSYS_TOP = "${yosys}";
  ALLIANCE_TOP = "${alliance}";
  CORIOLIS_TOP = "${coriolis-unicorn}";
  # The user configuration for verhaegs is empty,
  # which is why we use it.
  USER = "verhaegs";

  configurePhase = "true";
  buildPhase = ''
    cd benchs/adder/cmos
    echo 'doing make druc'
    make druc
    echo 'doing make lvx'
    make lvx
  '';
  installPhase = "true";
}
