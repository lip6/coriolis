{ version, meta, generic, ... }:

let pkg =
  { lib, libxml2, bzip2, rapidjson, qt4, bison, flex, doxygen }:
  {
    name = "hurricane";
    src = ../hurricane;
    buildInputs = [ libxml2 qt4 bzip2 rapidjson ];
    nativeBuildInputs = [ bison flex doxygen ];
    pythonImportsCheck = [ "Viewer" "Hurricane" "Cfg" "Analog" ];
    meta.license = lib.licenses.lgpl3Plus;
  };
in generic pkg
