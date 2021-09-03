{ generic, ... }:

let pkg =
  { lib, coriolis-crlcore, coriolis-vlsisapd, coriolis-hurricane
  , python2Packages, coriolis-cumulus }:
  {
    name = "coriolis-stratus1";
    src = ../stratus1;

    propagatedBuildInputs = [
      coriolis-vlsisapd coriolis-crlcore coriolis-hurricane coriolis-cumulus
    ];
    postInstall = ''
      ln -s -t $out/${python2Packages.python.sitePackages} \
        $out/${python2Packages.python.sitePackages}/stratus/*
    '';
    pythonImportsCheck = [ "stratus" "patterns" "patread" "synopsys" "utils" "util" ];

    meta.license = lib.licenses.gpl2;
  };
in generic pkg
