# NEdit (GTK)

NEdit is a simple GUI text editor that allows you create, open, and save files. This implementation of NEdit is written in C and uses the GTK4 toolkit.

So far, NEdit (GTK) has only been built and tested on Linux.

## Building from source

### Requirements

* C 23 compiler (GCC or Clang)
* Build system (Make or Ninja)
* pkg-config
* [CMake](https://cmake.org/download/) (3.21 or later)
* [GTK4](https://www.gtk.org/docs/installations/) (for Linux, install the development package)
* [Git](https://git-scm.com/downloads/)

For NixOS, a `shell.nix` file containing all the requirements (except Git) is provided. Run `nix-shell` to load into the development environment.

### Building and Running

* Clone NEdit: `git clone https://github.com/DamareonC/nedit-gtk.git`
* Move to NEdit directory: `cd nedit-gtk`
* Generate build files: `cmake -B build`
* Build NEdit: `cmake --build build`
* Run NEdit: `./build/nedit`