# SPDX-FileCopyrightText: (C) 2022 Claudio Cambra <claudio.cambra@gmail.com>
# SPDX-License-Identifier: BSD-2-Clause

{
  description = "A flake for kalendar";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    with flake-utils.lib;
    eachSystem [ "aarch64-linux" "x86_64-linux" ] (system:
        let
          pkgs = import nixpkgs {
            inherit system;
          };

          nativeBuildInputs = with pkgs; [
            cmake
            extra-cmake-modules
            qt5.wrapQtAppsHook
          ];

          buildInputs = with pkgs; [
            qt5.qtbase
            qt5.qtsvg
          ];

          packages.default = with pkgs; stdenv.mkDerivation rec {
            inherit nativeBuildInputs buildInputs;
            pname = "clip";
            version = "dev";
            src = ./.;
            propagatedUserEnvPkgs = [
            ];
            postFixup = ''
            '';
          };

          apps.default = mkApp {
            name = "clip4";
            drv = packages.default;
          };

        in {
          inherit packages apps;
          devShell = pkgs.mkShell {
            inherit buildInputs;
            nativeBuildInputs = with pkgs; nativeBuildInputs ++[
              clang-tools
              libclang.python
              ninja
              gdb
            ];
            name = "clip-shell";
            shellHook = ''
            '';
          };
        }
    );
}
