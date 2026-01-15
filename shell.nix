{ pkgs ? import <nixpkgs> { } }:
pkgs.callPackage (
    { mkShell, gtk4, gcc, clang, gdb, lldb, gnumake, ninja, pkg-config, cmake }:
    mkShell
    {
        strictDeps = true;

        buildInputs = [
            gtk4.dev
        ];
        
        nativeBuildInputs = [
            gcc
            clang
            gdb
            lldb
            gnumake
            ninja
            pkg-config
            cmake
        ];
    }
) { }