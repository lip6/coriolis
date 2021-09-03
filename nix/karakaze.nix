{ generic, ... }:

let pkg =
  { lib, coriolis-crlcore, coriolis-bora, coriolis-katana, coriolis-anabatic
  , coriolis-vlsisapd, coriolis-hurricane }:
  {
    name = "coriolis-karakaze";
    src = ../karakaze;

    propagatedBuildInputs = [
      coriolis-vlsisapd coriolis-crlcore coriolis-hurricane coriolis-katana
      coriolis-bora coriolis-anabatic
    ];
    postInstall = ''
      # for import check
      mkdir -p /build/coriolistop/etc/coriolis2
      export CORIOLIS_TOP=/build/coriolistop
    '';
    pythonImportsCheck = [ "karakaze" "karakaze.analogdesign" ];

    meta.license = lib.licenses.gpl2Plus;
  };
in generic pkg
