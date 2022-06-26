# casein

C++ boilerplate library for UI-based multiplatform apps

## Usage/install

1. Fetch the code
2. Build:

   ```shell
   $ cmake -S . -B build
   $ cmake --build build
   $ cmake --build --install
   ```

## Requirements

* CMake
* Modern C++ compiler

## Implicit requirements

* Only useful for UI apps (i.e. console apps should be fine with ye old `main`
* Only meanful for multiplatorm
* Expects users to install it via `cmake install` or similar

