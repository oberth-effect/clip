{
  mkDerivation,
  lib,

  # build
  cmake,
  wrapQtAppsHook,

  # dependecy
  eigen,
  libtiff,
  qtbase,
  qtsvg,

}:

mkDerivation {
  pname = "clip4";
  version = "4.2.1-beta";

  src = ./.;

  buildInputs = [
    eigen
    libtiff
    qtbase
    qtsvg
  ];

  nativeBuildInputs = [
    cmake
    wrapQtAppsHook
  ];
}
