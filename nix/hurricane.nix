{ version, meta, generic, ... }:

let pkg =
  { libxml2, bzip2, rapidjson, qt4, bison, flex, doxygen, coriolis-vlsisapd }:
  {
    name = "hurricane";
    src = ../hurricane;
    buildInputs = [ libxml2 qt4 bzip2 rapidjson coriolis-vlsisapd ];
    nativeBuildInputs = [ bison flex doxygen ];
    pythonImportsCheck = [ "Viewer" "Hurricane" "Cfg2" "Analog" ];
  };
in generic pkg
