# Tabula Slint

A tiny C++/Slint desktop app for testing cross-platform GUI builds on macOS and Windows.

The app implements a Trithemius-style Tabula Recta transform using a fixed `ABCDEFGHIJKLMNOPQRSTUVWXYZ` key.

## Build

```sh
cmake --preset debug
cmake --build --preset debug
open ./build/debug/Tabula.app
```

For a smaller optimized binary:

```sh
cmake --preset release
cmake --build --preset release
open ./build/release/Tabula.app
```

On macOS, CMake builds a proper `.app` bundle so Finder launches the GUI directly instead of opening Terminal.

On Windows, open the folder in CLion, select the `debug` or `release` preset, and build. The CMake file copies Slint runtime DLLs next to the executable after build.

## Notes

- CMake first tries `find_package(Slint)`.
- If Slint is not installed, CMake fetches Slint from the official `release/1` branch.
- The cipher shifts alphabetic characters by `0, 1, 2, ... 25`, repeating for the fixed key.
- The UI bundles Glass TTY VT220 by Viacheslav Slavinsky, released under the Unlicense.
