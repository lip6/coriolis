{ generic, ... }:
let pkg =
  { libxml2, qt4, bison, flex, doxygen }:
  {
    name = "vlsisapd";
    src = ../vlsisapd;
    buildInputs = [ libxml2 qt4 ];
    nativeBuildInputs = [ bison flex doxygen ];
    pythonImportsCheck = [ "SPICE" "LIBERTY" "DTR" "Cfg" "CIF" "AGDS" ];

    meta.license = lib.licenses.gpl2Plus;
  };
in generic pkg
