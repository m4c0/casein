import casein;

extern "C" void casein_handle(const casein::event & e);

extern "C" void casein_key_down(casein::keys key) {
  casein_handle(casein::events::key_down { key });
}
extern "C" void casein_key_up(casein::keys key) {
  casein_handle(casein::events::key_up { key });
}
extern "C" void casein_mouse_down(int x, int y, int btn) {
  casein_handle(casein::events::mouse_down { x, y, btn });
}
extern "C" void casein_mouse_move(int x, int y) {
  casein_handle(casein::events::mouse_move { x, y });
}
extern "C" void casein_mouse_up(int x, int y, int btn) {
  casein_handle(casein::events::mouse_up { x, y, btn });
}
extern "C" void casein_repaint() {
  casein_handle(casein::events::repaint {});
}
extern "C" void casein_create_window(void * ptr) {
  casein_handle(casein::events::create_window { static_cast<casein::native_handle_t>(ptr) });
}
extern "C" void casein_quit() {
  casein_handle(casein::events::quit {});
}
extern "C" void casein_resize_window(int w, int h, bool live) {
  casein_handle(casein::events::resize_window { w, h, live });
}
