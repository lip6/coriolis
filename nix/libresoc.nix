{ alliance-check-toolkit, soclayout, pinmux, ... }:

{ stdenv, coriolis-combined, coriolis-unicorn, coriolis-cumulus
, python2Packages, alliance, yosys, writeShellScriptBin, fetchFromGitHub }:

let
  fakegit = writeShellScriptBin "git" "exit 0";
  yosys' = yosys.overrideAttrs (_: {
    version = "0.9+4008";
    src = fetchFromGitHub {
      owner = "YosysHQ";
      repo = "yosys";
      rev = "049e3abf9baf795e69b9ecb9c4f19de6131f8418";
      sha256 = "0h3w91jr5yws4wxk3n45h1pdq65jhj957d5pzy2799gzwkba5adh";
    };
  });
in
let yosys = yosys'; in
stdenv.mkDerivation rec {
  name = "libresoc-check";

  src = soclayout;

  YOSYS_TOP = "${yosys}";
  ALLIANCE_TOP = "${alliance}";
  CORIOLIS_TOP = "${coriolis-combined}";
  # The user configuration for verhaegs is empty,
  # which is why we use it.
  USER = "verhaegs";

  nativeBuildInputs = [ fakegit alliance coriolis-unicorn coriolis-cumulus yosys ];

  prePatch = ''
    rmdir pinmux
    cp -r ${pinmux} -T pinmux

    cp -r ${alliance-check-toolkit} -T alliance-check-toolkit

    chmod a+w -R alliance-check-toolkit pinmux

    export ALLIANCE_TOOLKIT="$PWD/alliance-check-toolkit"
    export CHECK_TOOLKIT="$ALLIANCE_TOOLKIT"
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
