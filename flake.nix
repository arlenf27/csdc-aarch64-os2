{
  description = "AArch64 Bare-Metal (aarch64-none-elf) GNU Toolchain Flake";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};

        # Download and unpack the binary tarball
        armGnuToolchain = pkgs.stdenv.mkDerivation rec {
          pname = "arm-gnu-toolchain-aarch64-none-elf";
          version = "15.3.rel1";

          src = pkgs.fetchurl {
            url = "https://gitlab.arm.com/api/v4/projects/tooling%2Fgnu-toolchains-for-arm/packages/generic/gnu-toolchain/${version}/arm-gnu-toolchain-${version}-darwin-arm64-aarch64-none-elf.tar.xz";
            # Replaced with real hash via `nix store prefetch-file`
            hash = "sha256-HZrAxFS6VVwO4nLGL8+Co2zMHHyRc8H2sNArihTFl4k=";
          };

          # The archive extracts into a top-level directory matching the archive name
          sourceRoot = ".";

          nativeBuildInputs = [
            pkgs.xar
          ]; # Needed if running patch checks

          # Prevent default Nix build phases (since these are pre-compiled binaries)
          dontBuild = true;
          dontConfigure = true;

          installPhase = ''
            runHook preInstall

            mkdir -p $out
            # Copy all bin, lib, include, and target folders into the nix package store
            cp -r arm-gnu-toolchain-${version}-darwin-arm64-aarch64-none-elf/* $out/

            runHook postInstall
          '';

          # Fix dynamic library paths and macOS security signatures if necessary
          postFixup = pkgs.lib.optionalString pkgs.stdenv.hostPlatform.isDarwin ''
            if command -v codesign &> /dev/null; then
              find $out/bin -type f -exec codesign --force --sign - {} \; 2>/dev/null || true
            fi
          '';

          meta = with pkgs.lib; {
            description = "Arm GNU Toolchain for AArch64 bare-metal target (aarch64-none-elf)";
            homepage = "https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads";
            platforms = [ "aarch64-darwin" ]; # Binary is targeted for macOS Apple Silicon
          };
        };
      in
      {
        packages.default = armGnuToolchain;

        devShells.default = pkgs.mkShell {
          packages = [
            armGnuToolchain
            pkgs.qemu
          ];

          shellHook = ''
            export CC=aarch64-none-elf-gcc
            export CXX=aarch64-none-elf-g++
            export AS=aarch64-none-elf-as
            export LD=aarch64-none-elf-ld
            export OBJCOPY=aarch64-none-elf-objcopy

            echo "AArch64 Bare-Metal GNU Toolchain (v15.3.rel1) Loaded."
            echo "Target GCC: $(aarch64-none-elf-gcc --version | head -n1)"
          '';
        };
      }
    );
}
