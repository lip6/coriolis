{ alliance-src, ... }:

{ stdenv, coriolis-unicorn }:
stdenv.mkDerivation {
  pname = "alliance-check-toolkit";
  version = builtins.substring 0 8 alliance-src.lastModifiedDate;
  src = alliance-src;

  nativeBuildInputs = [ coriolis-unicorn ];

  configurePhase = "true";
  buildPhase = ''
    cd benchs/adder/cmos
    echo 'doing make druc'
    make druc
    echo 'doing make lvx'
    make lvx
  '';
  installPhase = "true";
}
