{ generic, ... }:

let pkg =
  { lib, coriolis-hurricane, coriolis-crlcore
  , coriolis-etesian, coriolis-anabatic, coriolis-katana
  , coriolis-unicorn, python3Packages }:
  {
    name = "cumulus";

    src = ../cumulus;

    postInstall = ''
      ln -s -t $out/${python3Packages.python.sitePackages} $out/${python3Packages.python.sitePackages}/cumulus/*

      # for import check
      mkdir -p /build/coriolistop/etc/coriolis2
      export CORIOLIS_TOP=/build/coriolistop
    '';

    propagatedBuildInputs = [
      coriolis-hurricane coriolis-crlcore
      coriolis-etesian coriolis-anabatic coriolis-katana
      coriolis-unicorn
    ];
    pythonImportsCheck = [ "plugins" ];

    meta.license = lib.licenses.gpl2Plus;
  };
in generic pkg
