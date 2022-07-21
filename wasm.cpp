#include "casein.hpp"

// https://github.com/WebAssembly/wasi-sdk
// https://developer.mozilla.org/en-US/docs/WebAssembly/Using_the_JavaScript_API

int main() {
  casein_event(casein::events::create_window { nullptr });
}
