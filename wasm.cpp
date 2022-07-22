#include "casein.hpp"

// https://github.com/WebAssembly/wasi-sdk
// https://developer.mozilla.org/en-US/docs/WebAssembly/Using_the_JavaScript_API

extern "C" void casein_key_down() {
  casein_event(casein::events::key_down { 0 });
}
extern "C" void casein_key_up() {
  casein_event(casein::events::key_up { 0 });
}
extern "C" void casein_repaint() {
  casein_event(casein::events::repaint {});
}
extern "C" void casein_create_window() {
  casein_event(casein::events::create_window { nullptr });
}
int main() {
}
