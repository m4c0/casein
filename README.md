# casein

C++ boilerplate library for UI-based multiplatform apps

## Usage/install

1. Fetch the code
2. Fetch [ecow][ecow] in the same directory as this one
3. Build:

   ```shell
   $ clang++ -std=c++20 -I../ecow build.cpp -o build
   $ ./build
   ```

## Requirements

* Clang 15 or above

## Implicit requirements

* Only useful for UI apps (i.e. console apps should be fine with ye old `main`
* Only meanful for multiplatorm
* Expects users to use `ecow` as a build system

## License

MIT, with exception of the code inside `glue`, which has a different copyright
(as described in the file `NOTICE` inside said folder).

