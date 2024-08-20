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

}:

mkDerivation {
  pname = "clip4";
  version = "4.2.1-beta";

  src = ./.;

  buildInputs = [
    eigen
    libtiff
    qtbase
  ];

  nativeBuildInputs = [
    cmake
    wrapQtAppsHook
  ];
}
