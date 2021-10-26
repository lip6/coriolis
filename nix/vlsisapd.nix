{ generic, ... }:
let pkg =
  { lib, libxml2, qt4, bison, flex, doxygen }:
  {
    name = "vlsisapd";
    src = ../vlsisapd;
    buildInputs = [ libxml2 qt4 ];
    nativeBuildInputs = [ bison flex doxygen ];
    pythonImportsCheck = [ "SPICE" "LIBERTY" "DTR" "Cfg" "CIF" "AGDS" ];

    meta.license = lib.licenses.gpl2Plus;
    meta.broken = true;
  };
in generic pkg
