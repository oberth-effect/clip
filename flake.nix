{
  description = "Cologne Laue Indexation Program";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";

  outputs =
    { self, nixpkgs }:
    let
      supportedSystems = [
        "x86_64-linux"
        # "aarch64-linux"
        # "x86_64-darwin"
        # "aarch64-darwin"
      ];
      forEachSupportedSystem =
        f: nixpkgs.lib.genAttrs supportedSystems (system: f { pkgs = import nixpkgs { inherit system; }; });
    in
    {
      packages = forEachSupportedSystem (
        { pkgs }:
        {
          default = pkgs.libsForQt5.callPackage ./build.nix { };
        }
      );

      formatter = forEachSupportedSystem ({ pkgs }: pkgs.nixfmt-rfc-style);

    };
}
