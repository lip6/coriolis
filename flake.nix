# The license for this file is included in the `nix` directory next to this file.

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

      meta = with nixpkgs.lib; {
        description = "Coriolis is a free database, placement tool and routing tool for VLSI design.";
        homepage    = "http://coriolis.lip6.fr/";
        platforms   = platforms.all;
      };

      pythonOverlay = self: super: {
        python2Packages = super.python2Packages.override {
          overrides = pself: psuper: {
            pyqt4 = psuper.pyqt4.overridePythonAttrs (o: rec {
              version = "4.12.1";
              src = super.fetchurl {
                url = "mirror://sourceforge/pyqt/PyQt4_gpl_x11-${version}.tar.gz";
                sha256 = "RdckRhrliwGbm+lQdsoH4hLrcwhK7JEluyVIJcOgU4M=";
              };
              pythonImportsCheck = [ "PyQt4.QtCore" "PyQt4.QtGui" ];
            });
          };
        };
      };

      # Nixpkgs instantiated for supported system types.
      nixpkgsFor = forAllSystems (system: import nixpkgs { inherit system; overlays = [ pythonOverlay self.overlay ]; });


      override = drv:
        drv.overrideAttrs (o: {
          preConfigure = ''
            cmakeFlagsArray+=(-DCMAKE_MODULE_PATH="$(sed -e 's|:|/share/cmake/Modules;|g' <<< "$CMAKE_PREFIX_PATH:")")
          '' + (o.preConfigure or "");
          cmakeFlags = [ "-DBUILD_DOC=ON" ] ++ (o.cmakeFlags or []);
        });

      generic = import ./nix/generic.nix { inherit version meta; };

      pythonComponents = [
        "vlsisapd" "hurricane" "crlcore" "flute" "etesian"
        "anabatic" "katana" "bora" "katabatic" "kite"
        "tutorial" "unicorn"
      ];

      components = pythonComponents ++ [
        "lefdef" "bootstrap" "cumulus" "coloquinte"
        "equinox" "knik" "ispd" "karakaze" "nimbus"
        "metis" "mauka" "oroshi" "solstice" "stratus1"
        "documentation" "unittests"
      ];

      commonArgs = { inherit version meta generic; };

    in

    rec {
      overlay = final: prev:
        builtins.foldl'
          (acc: elem: acc // {
            "coriolis-${elem}" = override (final.callPackage (
              let f = import (./nix + "/${elem}.nix"); in
              f (builtins.intersectAttrs (builtins.functionArgs f) commonArgs)
            ) {});
          }) {} components;

      packages = forAllSystems (system: builtins.foldl' (acc: elem: acc // {
        ${elem} = nixpkgsFor.${system}.${"coriolis-${elem}"};
      }) {} components);

      defaultPackage = forAllSystems (system: self.packages.${system}.unicorn);

      devShells = forAllSystems (system:
        let
          pkgs = nixpkgsFor.${system};
          envFor = comp: (pkgs.python2.buildEnv.override {
            extraLibs = [ pkgs.${"coriolis-${comp}"} ];
          });
        in builtins.catAttrs (builtins.map (comp: { ${comp} = envFor comp; }) pythonComponents)
      );
      devShell = forAllSystems (system:
        let
          pkgs = nixpkgsFor.${system};
          env = pkgs.python2.buildEnv.override {
            extraLibs = builtins.map (x: pkgs.${"coriolis-${x}"}) pythonComponents;
          };
        in env.env
      );

      #hydraJobs.coriolis = self.defaultPackage;
    };
}
