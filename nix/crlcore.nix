{ version, meta, generic, ... }:
let pkg =
  { libxml2, qt4, bison, flex, doxygen, coriolis-vlsisapd
  , coriolis-lefdef, coriolis-hurricane }:
  {
    name = "crlcore";
    src = ../crlcore;
    buildInputs = [ libxml2 qt4 coriolis-lefdef coriolis-vlsisapd coriolis-hurricane ];
    nativeBuildInputs = [ bison flex doxygen ];
    pythonImportsCheck = [ "CRL" "Constant" ];
  };
in generic pkg
