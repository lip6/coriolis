{ version, meta, ... }:

{ buildEnv
, coriolis-hurricane, coriolis-crlcore
, coriolis-anabatic, coriolis-katana, coriolis-bora
, coriolis-katabatic, coriolis-kite, coriolis-tutorial
, coriolis-unicorn, coriolis-oroshi, coriolis-cumulus
, coriolis-lefdef, coriolis-coloquinte, coriolis-flute
, coriolis-equinox, coriolis-knik, coriolis-karakaze
, coriolis-solstice, coriolis-stratus1, coriolis-etesian
}:

buildEnv {
  name = "coriolis-combined-${version}";

  paths = [
    coriolis-hurricane coriolis-crlcore coriolis-flute
    coriolis-etesian coriolis-anabatic coriolis-katana
    coriolis-bora coriolis-katabatic coriolis-kite
    coriolis-tutorial coriolis-unicorn coriolis-oroshi
    coriolis-cumulus coriolis-lefdef
    coriolis-coloquinte coriolis-equinox coriolis-knik
    coriolis-karakaze coriolis-solstice coriolis-stratus1
  ];

  inherit meta;
}
