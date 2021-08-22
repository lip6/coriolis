{
  description = "Coriolis is a free database, placement tool and routing tool for VLSI design.";

  # Nixpkgs / NixOS version to use.
  inputs.nixpkgs.url = "github:NixOS/nixpkgs?ref=nixos-21.05";

  outputs = { self, nixpkgs }:
    let

      # Generate a user-friendly version numer.
      version = builtins.substring 0 8 self.lastModifiedDate;

      # System types to support.
      supportedSystems = [ "x86_64-linux" "x86_64-darwin" "aarch64-linux" "aarch64-darwin" ];

      # Helper function to generate an attrset '{ x86_64-linux = f "x86_64-linux"; ... }'.
      forAllSystems = nixpkgs.lib.genAttrs supportedSystems;

      # Nixpkgs instantiated for supported system types.
      nixpkgsFor = forAllSystems (system: import nixpkgs { inherit system; overlays = [ self.overlay ]; });

      /*
      coriolis =
        { lib, stdenv, python2, cmake_2_8, boost, bison, flex
        , libxml2, rapidjson, which, qt4, zlib, bzip2 }:
        let boostWithPython = boost.override { enablePython = true; python = python2; }; in
        stdenv.mkDerivation {
          name = "coriolis-${version}";

          src = ./.;

          postPatch = ''
            export HOME=/build
            mkdir -p /build/coriolis-2.x/src
            dir="$PWD"
            mv "$PWD" -T /build/coriolis-2.x/src/coriolis

            patchShebangs .
            fixCmakeFiles .
          '';

          configurePhase = ''
            runHook preConfigure
            makeCmakeFindLibs
            export LDFLAGS="$NIX_LDFLAGS"
            runHook postConfigure
          '';

          hardeningDisable = [ "format" ];

          buildPhase = ''
            runHook preBuild
            ./bootstrap/ccb.py --project=coriolis --make="-j$NIX_BUILD_CORES install"
            runHook postBuild
          '';

          installPhase = ''
            mkdir $out
            mv /build/coriolis-2.x/*.* -t $out
          '';

          checkPhase = "true";

          buildInputs = [ python2 boostWithPython libxml2 rapidjson qt4 zlib bzip2 ];
          nativeBuildInputs = [ cmake_2_8 bison flex which ];

          meta = with lib; {
            description = "Coriolis is a free database, placement tool and routing tool for VLSI design.";
            homepage    = "http://coriolis.lip6.fr/";
            license     = licenses.gpl3;
            platforms   = platforms.all;
          };
        };
      */

      meta = with nixpkgs.lib; {
        description = "Coriolis is a free database, placement tool and routing tool for VLSI design.";
        homepage    = "http://coriolis.lip6.fr/";
        license     = licenses.gpl3;
        platforms   = platforms.all;
      };

    in

    rec {
      overlay = final: prev: {
        coriolis-vlsisapd = final.callPackage (import ./nix/vlsisapd.nix { inherit version meta; }) {};
        coriolis-bootstrap = final.callPackage (import ./nix/bootstrap.nix { inherit version meta; }) {};
        coriolis-hurricane = final.callPackage (import ./nix/hurricane.nix { inherit version meta; }) {};
      };

      packages = forAllSystems (system:
        with nixpkgsFor.${system}; {
          vlsisapd = coriolis-vlsisapd;
          bootstrap = coriolis-bootstrap;
          hurricane = coriolis-hurricane;
        });

      defaultPackage = forAllSystems (system: self.packages.${system}.coriolis);
      devShell = defaultPackage;

      #hydraJobs.coriolis = self.defaultPackage;
    };
}
