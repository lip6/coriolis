{ soclayout, pinmux, ... }:

{ stdenv }:
stdenv.mkDerivation {
  name = "libresoc-check";
  src = soclayout;

  prePatch = ''
    rm pinmux
    ln -s ${pinmux} pinmux
  '';

  meta.isBroken = true;
}
