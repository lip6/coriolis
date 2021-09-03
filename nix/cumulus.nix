{ generic, ... }:

let pkg =
  { lib, coriolis-vlsisapd, coriolis-hurricane, coriolis-crlcore
  , coriolis-etesian, coriolis-anabatic, coriolis-katana
  , coriolis-unicorn }:
  {
    name = "cumulus";

    src = ../cumulus;

    postInstall = ''
      # for import check
      mkdir -p /build/coriolistop/etc/coriolis2
      export CORIOLIS_TOP=/build/coriolistop
    '';

    propagatedBuildInputs = [
      coriolis-vlsisapd coriolis-hurricane coriolis-crlcore
      coriolis-etesian coriolis-anabatic coriolis-katana
      coriolis-unicorn
    ];
    pythonImportsCheck = [ "plugins" ];

    meta.license = lib.licenses.gpl2Plus;
  };
in generic pkg
