{ alliance-check-toolkit, soclayout, pinmux, ... }:

{ stdenv, coriolis-combined, coriolis-unicorn, coriolis-cumulus
, python2Packages, alliance, yosys, writeShellScriptBin }:

let
  fakegit = writeShellScriptBin "git" "exit 0";
in stdenv.mkDerivation rec {
  name = "libresoc-check";
  src = soclayout;

  YOSYS_TOP = "${yosys}";
  ALLIANCE_TOP = "${alliance}";
  ALLIANCE_TOOLKIT = "${alliance-check-toolkit}";
  CHECK_TOOLKIT = ALLIANCE_TOOLKIT;
  CORIOLIS_TOP = "${coriolis-combined}";
  # The user configuration for verhaegs is empty,
  # which is why we use it.
  USER = "verhaegs";

  nativeBuildInputs = [ fakegit alliance coriolis-unicorn coriolis-cumulus yosys ];

  prePatch = ''
    rmdir pinmux
    ln -s ${pinmux} pinmux
  '';
  postPatch = "patchShebangs .";

  buildPhase = ''
    runHook preBuild
    cd experiments9
    ./mksym.sh
    ln -s ${alliance-check-toolkit}/etc/mk/users.d/user-${USER}.mk mk/users.d/user-${USER}.mk
    ./build_full_4ksram.sh
    runHook postBuild
  '';

  installPhase = ''
    runHook preInstall
    mv -T "$PWD" "$out"
    runHook postInstall
  '';

  fixupPhase = "true";
}
