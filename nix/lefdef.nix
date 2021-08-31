{ version, meta }:

{ lib, stdenv, python2, cmake, boost, bison, flex
, libxml2, zlib, ninja, coriolis-bootstrap  }:

let boostWithPython = boost.override { enablePython = true; python = python2; }; in

stdenv.mkDerivation {
  pname = "coriolis-lefdef";

  src = ../lefdef;

  hardeningDisable = [ "format" ];

  buildInputs = [ python2 boostWithPython coriolis-bootstrap bison flex zlib ];
  nativeBuildInputs = [ cmake ninja ];

  inherit version;

  meta = meta // { license = lib.licenses.apsl20; };
}
