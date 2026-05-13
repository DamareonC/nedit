# NEdit

NEdit is a simple GUI text editor that allows you to create, open, and save files. NEdit is written in C and uses the GTK toolkit.

So far, NEdit has only been built and tested on Linux.

## Building from source

### Requirements

* C 23 compiler (GCC or Clang)
* Build system (Make or Ninja; Ninja is used in provided presets)
* pkg-config
* [CMake](https://cmake.org/download/) (3.21 or later)
* [GTK4](https://www.gtk.org/docs/installations/) (for Linux, install the development package)
* [Git](https://git-scm.com/downloads/)

For NixOS, a `shell.nix` file containing all the requirements (except Git) is provided. Run `nix-shell` to load into the development environment.

### Building and Running

* Clone NEdit: `git clone https://github.com/DamareonC/nedit.git`
* Move to NEdit directory: `cd nedit`
* Generate build files: `cmake -B build`*
* Build NEdit: `cmake --build build`
* Run NEdit: `./build/nedit`

*Debug and Release presets are provided, and can be run with `cmake --preset debug` or `cmake --preset release` respectively

### Installing

NEdit can be installed via `cmake --install build`* (may require root privileges). On Linux, NEdit will be located at `/usr/local/bin/nedit` by default.

If you wish to package NEdit (e.g. as tar.gz, deb, rpm, AppImage and more) or create an installer script, run the `cpack` command in the `build`* directory.

*If the Debug or Release presets was used, use `build/debug` or `build/release` (respectively) instead