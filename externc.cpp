module casein;
import jute;

extern "C" casein::native_handle_t * casein_native_ptr = &casein::native_ptr;
extern "C" dotz::vec2 * casein_mouse_pos = &casein::mouse_pos;
extern "C" dotz::vec2 * casein_mouse_rel = &casein::mouse_rel;
extern "C" float * casein_screen_scale_factor = &casein::screen_scale_factor;
extern "C" bool * casein_keydown_repeating = &casein::keydown_repeating;
extern "C" dotz::vec2 * casein_window_size = &casein::window_size;
extern "C" bool * casein_fullscreen = &casein::fullscreen;
extern "C" bool * casein_window_live_resize = &casein::window_live_resize;

extern "C" void casein_enable_filedrop(bool);
extern "C" void casein_add_drop(const char * str, unsigned len) {
  jute::view v { str, len };
  casein::dropped_files.push_back_doubling(v.cstr());
}
extern "C" void casein_clear_drops() {
  casein::dropped_files.truncate(0);
}

extern "C" void casein_window_title(const char ** ptr, unsigned * len) {
  *ptr = casein::window_title.begin();
  *len = casein::window_title.size();
}
