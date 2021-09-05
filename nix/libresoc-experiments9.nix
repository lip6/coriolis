{ inputs, ... }:

{ stdenv }:
stdenv.mkDerivation {
  name = "libresoc-experiments9";
  src = inputs.soclayout;

  meta.isBroken = true;
}
