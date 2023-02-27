import casein;

extern "C" void casein_handle(const casein::event & e);

inline constexpr auto key_for_keycode(int code) {
  switch (code) {
  case 32:
    return casein::K_SPACE;
  case 37:
    return casein::K_LEFT;
  case 38:
    return casein::K_UP;
  case 39:
    return casein::K_RIGHT;
  case 40:
    return casein::K_DOWN;
  default:
    return casein::K_NULL;
  }
}

extern "C" void casein_keycode_down(int key) {
  casein_handle(casein::events::key_down { key_for_keycode(key) });
}
extern "C" void casein_keycode_up(int key) {
  casein_handle(casein::events::key_up { key_for_keycode(key) });
}
