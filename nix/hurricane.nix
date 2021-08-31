{ version, meta, generic, ... }:

let pkg =
  { lib, libxml2, bzip2, rapidjson, qt4, bison, flex, doxygen, coriolis-vlsisapd }:
  {
    name = "hurricane";
    src = ../hurricane;
    buildInputs = [ libxml2 qt4 bzip2 rapidjson coriolis-vlsisapd ];
    nativeBuildInputs = [ bison flex doxygen ];
    pythonImportsCheck = [ "Viewer" "Hurricane" "Cfg2" "Analog" ];
    meta.license = lib.licenses.lgpl3Plus;
  };
in generic pkg
