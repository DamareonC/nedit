# NEdit

NEdit is a simple GUI text editor that allows you to create, open, and save files. NEdit is written in C and uses the GTK toolkit.

So far, NEdit has only been built and tested on Linux.

## Building from source

### Requirements

* C 23 compiler (GCC or Clang; Clang is currently recommended for the Debug preset)
* Build system (Make or Ninja; Ninja Multi-Config is used in provided preset)
* [CMake](https://cmake.org/download/) (4.1 or later)
* [GTK4](https://www.gtk.org/docs/installations/) (for Linux, install the development package)
* [Git](https://git-scm.com/downloads/)
* Ccache (Optional; speeds up build times)

### Building and Running

* Clone NEdit: `git clone https://github.com/DamareonC/nedit.git`
* Move to NEdit directory: `cd nedit`
* Generate build files: `cmake --preset ninja-mc` (to use ccache, also pass in: -DCMAKE_C_COMPILER_LAUNCHER=ccache)
* Build NEdit: `cmake --build build` (Debug build is default; pass in `--config Release` for Release build)
* Run NEdit: `./build/Debug/nedit` or `./build/Release/nedit`

### Installing

NEdit can be installed via `cmake --install build --config Release --strip` (may require root privileges). On Linux, NEdit will be located at `/usr/local/bin/nedit` by default (this can be changed with the `--prefix` flag, e.g. `--prefix /usr` puts the binary at `/usr/bin/nedit`).

If you wish to package NEdit (e.g. as tar.gz, deb, rpm, AppImage and more) or create an installer script, run the `cpack -C Release` command in the `build` directory.