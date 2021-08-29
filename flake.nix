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

      meta = with nixpkgs.lib; {
        description = "Coriolis is a free database, placement tool and routing tool for VLSI design.";
        homepage    = "http://coriolis.lip6.fr/";
        license     = licenses.gpl3;
        platforms   = platforms.all;
      };

      override = drv: drv.overrideAttrs (o: {
        preConfigure = ''
          cmakeFlagsArray+=(-DCMAKE_MODULE_PATH="$(sed -e 's|:|/share/cmake/Modules;|g' <<< "$CMAKE_PREFIX_PATH:")")
        '' + (o.preConfigure or "");
        cmakeFlags = [ "-DBUILD_DOC=ON" ] ++ (o.cmakeFlags or []);
      });

      components = [
        "vlsisapd" "lefdef" "bootstrap" "hurricane" "crlcore"
        "cumulus" "flute" "etesian" "anabatic" "coloquinte"
        "katana" "bora" "equinox" "knik" "katabatic" "ispd"
        "karakaze" "kite" "nimbus" "metis" "mauka" "oroshi"
        "solstice" "stratus1" "tutorial" "unicorn" "documentation"
        "unittests"
      ];

    in

    rec {
      overlay = final: prev:
        builtins.foldl'
          (acc: elem: acc // {
            "coriolis-${elem}" = override (final.callPackage (
              import "${self}/nix/${elem}.nix" { inherit version meta; }
            ) {});
          }) {} components;

      packages = forAllSystems (system: builtins.foldl' (acc: elem: acc // {
        ${elem} = nixpkgsFor.${system}.${"coriolis-${elem}"};
      }) {} components);

      defaultPackage = forAllSystems (system: self.packages.${system}.unicorn);

      devShell = forAllSystems (system:
        let
          pkgs = nixpkgsFor.${system};
          env = pkgs.python2.buildEnv.override {
            extraLibs = [ pkgs.coriolis-vlsisapd ];
          };
        in env.env
      );

      #hydraJobs.coriolis = self.defaultPackage;
    };
}
