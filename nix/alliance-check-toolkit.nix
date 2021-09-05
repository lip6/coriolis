{ alliance-src }:

{ stdenv, coriolis-combined, coriolis-unicorn, coriolis-cumulus
, python2Packages, alliance, yosys }:

stdenv.mkDerivation {
  pname = "alliance-check-toolkit";
  version = builtins.substring 0 8 alliance-src.lastModifiedDate;
  src = alliance-src;

  nativeBuildInputs = [ alliance coriolis-unicorn coriolis-cumulus yosys ];

  YOSYS_TOP = "${yosys}";
  ALLIANCE_TOP = "${alliance}";
  CORIOLIS_TOP = "${coriolis-combined}";
  # The user configuration for verhaegs is empty,
  # which is why we use it.
  USER = "verhaegs";

  postPatch = "patchShebangs .";
  configurePhase = "export CHECK_TOOLKIT=\"$PWD\"";
  buildPhase = ''
    cd benchs
    ../bin/go.sh
  '';
  installPhase = "touch $out";
  fixupPhase = "true";
}
