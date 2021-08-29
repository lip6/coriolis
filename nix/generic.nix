{ version, meta }:

let f =
  { lib, stdenv, cmake, ninja, boost
  , coriolis-bootstrap, python2Packages }:
  { name
  , src
  , buildInputs ? []
  , nativeBuildInputs ? []
  , propagatedBuildInputs ? []
  , pythonImportsCheck
  }:
  let
    boostWithPython = boost.override { enablePython = true; inherit (python2Packages) python; };
    drv = stdenv.mkDerivation {
      pname = "coriolis-${name}";

      buildInputs = [ python2Packages.python boostWithPython ] ++ buildInputs;
      nativeBuildInputs = [
        coriolis-bootstrap cmake ninja
        python2Packages.pythonImportsCheckHook
      ] ++ nativeBuildInputs;
      inherit propagatedBuildInputs;

      preInstall = ''
          export PYTHONPATH="$out/${python2Packages.python.sitePackages}:$PYTHONPATH"
      '';

      inherit version meta src pythonImportsCheck;
    };
  in python2Packages.toPythonModule drv;
in

pkg:
{
  __functionArgs = builtins.functionArgs f // builtins.functionArgs pkg;
  __functor = self: args: f (builtins.intersectAttrs (builtins.functionArgs f) args) (pkg (builtins.intersectAttrs (builtins.functionArgs pkg) args));
}
