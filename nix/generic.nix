{ version, meta }:

let
  meta' = meta;
  f =
    { lib, stdenv, cmake, ninja, boost
    , coriolis-bootstrap, python2Packages }:
    let self =
      { name
      , buildInputs ? []
      , nativeBuildInputs ? []
      , meta ? {}
      , pythonImportsCheck
      , continuation ? (x: x)
      , ...
      }@args':
      let
        args = builtins.removeAttrs args' (builtins.attrNames (builtins.functionArgs self));
        boostWithPython = boost.override { enablePython = true; inherit (python2Packages) python; };
        drv = stdenv.mkDerivation ({
          pname = "coriolis-${name}";

          buildInputs = [ python2Packages.python boostWithPython ] ++ buildInputs;
          nativeBuildInputs = [
            coriolis-bootstrap cmake ninja
            python2Packages.pythonImportsCheckHook
          ] ++ nativeBuildInputs;

          preInstall = ''
              export PYTHONPATH="$out/${python2Packages.python.sitePackages}:$PYTHONPATH"
          '';

          meta = meta // meta';

          inherit version pythonImportsCheck;
        } // args);
      in continuation (python2Packages.toPythonModule drv);
    in self;
in

pkg:
{
  __functionArgs = builtins.functionArgs f // builtins.functionArgs pkg;
  __functor = self: args: f (builtins.intersectAttrs (builtins.functionArgs f) args) (pkg (builtins.intersectAttrs (builtins.functionArgs pkg) args));
}
