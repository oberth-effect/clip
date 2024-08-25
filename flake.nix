{
  description = "Cologne Laue Indexation Program";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";

  outputs =
    { self, nixpkgs }:
    let
      supportedSystems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];
      forEachSupportedSystem =
        f:
        nixpkgs.lib.genAttrs supportedSystems (
          system:
          f {
            pkgs = import nixpkgs { inherit system; };
            system = system;
          }
        );
      clipDerivation =
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
          qtwayland,

        }:

        mkDerivation {
          pname = "clip4";
          version = "99.99.99-nightly";

          src = ./.;

          buildInputs = [
            eigen
            libtiff
            qtbase
            qtsvg
            qtwayland
          ];

          nativeBuildInputs = [
            cmake
            wrapQtAppsHook
          ];
          installPhase = ''
            mkdir -p $out/bin
            cp clip4 $out/bin/
          '';
        };
    in
    {
      packages = forEachSupportedSystem (
        { pkgs, system }:
        rec {
          default = clip4;
          clip4 = pkgs.libsForQt5.callPackage clipDerivation { };
        }
      );

      apps = forEachSupportedSystem (
        { pkgs, system }:
        rec {
          default = clip4;
          clip4 = {
            type = "app";
            program = "${self.packages.${system}.clip4}/bin/clip4";
          };
        }
      );

      formatter = forEachSupportedSystem ({ pkgs, system }: pkgs.nixfmt-rfc-style);

    };
}
