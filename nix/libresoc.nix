{ alliance-check-toolkit, soclayout, pinmux, ... }:

{ stdenv, coriolis-combined, coriolis-unicorn, coriolis-cumulus, fetchurl
, python3Packages, alliance, yosys, writeShellScriptBin, fetchFromGitHub }:

let
  C4MLogo = fetchurl {
    url = "https://ftp.libre-soc.org/C4MLogo.gds";
    sha256 = "0s1a2l4rh6vc1xg94jq87lsdkiqv68b0zi2rnv6wwcl9i2smacbf";
  };
  lip6 = fetchurl {
    url = "https://ftp.libre-soc.org/lip6.gds";
    sha256 = "0jj4z2d0is1vr3fziaqva39s7d85j0xqmmib0mdqnf2bg9iv91s4";
  };
  sorbonne_logo = fetchurl {
    url = "https://ftp.libre-soc.org/sorbonne_logo.gds";
    sha256 = "1p3plmb90kqljwsba2fahqzsy18iapmw2zvgkjh745c87lynn3dq";
  };
  libresoc_logo = fetchurl {
    url = "https://ftp.libre-soc.org/libresoc_logo.gds";
    sha256 = "1wbc5qrjlyrykaixdgp5d7brv5imlx71909y49xss907mil76bwh";
  };

  fakegit = writeShellScriptBin "git" "exit 0";
  fakewget = writeShellScriptBin "wget" "exit 0";
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

  nativeBuildInputs = [ fakewget fakegit alliance coriolis-unicorn coriolis-cumulus yosys ];

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

    ln -s ${C4MLogo} C4MLogo.gds
    ln -s ${lip6} lip6.gds
    ln -s ${sorbonne_logo} sorbonne_logo.gds
    ln -s ${libresoc_logo} libresoc_logo.gds

    ./mksym.sh
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
