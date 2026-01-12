{ pkgs ? import <nixpkgs> { } }:
pkgs.callPackage (
    { mkShell, gtk4, gcc, clang, gnumake, ninja, pkg-config, cmake }:
    mkShell
    {
        strictDeps = true;

        buildInputs = [
            gtk4.dev
        ];
        
        nativeBuildInputs = [
            gcc
            clang
            gnumake
            ninja
            pkg-config
            cmake
        ];
    }
) { }