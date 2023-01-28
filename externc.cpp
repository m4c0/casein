import casein;

#include "externc.h"

extern "C" void casein_handle(const casein::event & e);

extern "C" void casein_key_down(int key) {
  casein_handle(casein::events::key_down { key });
}
extern "C" void casein_key_up(int key) {
  casein_handle(casein::events::key_up { key });
}
extern "C" void casein_mouse_down(int btn) {
  casein_handle(casein::events::mouse_down { btn });
}
extern "C" void casein_mouse_move(int x, int y) {
  casein_handle(casein::events::mouse_move { x, y });
}
extern "C" void casein_mouse_up(int btn) {
  casein_handle(casein::events::mouse_up { btn });
}
extern "C" void casein_repaint() {
  casein_handle(casein::events::repaint {});
}
extern "C" void casein_create_window(void * ptr) {
  casein_handle(casein::events::create_window { ptr });
}
extern "C" void casein_quit() {
  casein_handle(casein::events::quit {});
}
