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
    in
    {
      packages = forEachSupportedSystem (
        { pkgs, system }:
        rec {
          default = clip4;
          clip4 = pkgs.libsForQt5.callPackage ./build.nix { };
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
