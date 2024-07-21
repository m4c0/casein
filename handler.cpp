module casein;
import jute;
import silog;

extern "C" void casein_exit(int);
void casein::exit(int code) {
  casein_exit(code);
}

extern "C" void casein_set_title(const char *);
void casein::set_title(const char * title) {
  casein_set_title(title);
}

extern "C" void casein_enter_fullscreen();
extern "C" void casein_leave_fullscreen();
bool in_fullscreen {};
bool casein::is_fullscreen() {
  return in_fullscreen;
}
void casein::set_fullscreen(bool f) {
  if (in_fullscreen == f) return;
  in_fullscreen = f;
  if (f) {
    casein_enter_fullscreen();
  } else {
    casein_leave_fullscreen();
  }
}

casein::native_handle_t casein::native_ptr;
dotz::vec2 casein::mouse_pos;
dotz::vec2 casein::mouse_rel;
float casein::screen_scale_factor = 1.0;
bool casein::keydown_repeating;
dotz::vec2 casein::window_size;
hai::varray<hai::cstr> casein::dropped_files {};

extern "C" casein::native_handle_t * casein_native_ptr = &casein::native_ptr;
extern "C" dotz::vec2 * casein_mouse_pos = &casein::mouse_pos;
extern "C" dotz::vec2 * casein_mouse_rel = &casein::mouse_rel;
extern "C" float * casein_screen_scale_factor = &casein::screen_scale_factor;
extern "C" bool * casein_keydown_repeating = &casein::keydown_repeating;
extern "C" dotz::vec2 * casein_window_size = &casein::window_size;

extern "C" void casein_enable_filedrop(bool);
extern "C" void casein_add_drop(const char * str, unsigned len) {
  jute::view v { str, len };
  casein::dropped_files.push_back_doubling(v.cstr());
}
extern "C" void casein_clear_drops() {
  casein::dropped_files.truncate(0);
}

using fn_t = void (*)();
static fn_t emap[casein::MAX_EVENT_TYPE] {};
static fn_t emap_g[casein::MAX_EVENT_TYPE][casein::G_MAX] {};
static fn_t emap_k[casein::MAX_EVENT_TYPE][casein::K_MAX] {};
static fn_t emap_m[casein::MAX_EVENT_TYPE][casein::M_MAX] {};

namespace casein {
  void handle(event_type et, void (*fn)()) {
    emap[et] = fn;

    if (et == FILES_DROP) {
      casein_enable_filedrop(fn != nullptr);
    }
  }
  void handle(event_type et, keys k, void (*fn)()) {
    emap_k[et][k] = fn;
  }
  void handle(event_type et, mouse_buttons m, void (*fn)()) {
    emap_m[et][m] = fn;
  }
  void handle(event_type et, gestures g, void (*fn)()) {
    emap_g[et][g] = fn;
  }

  void reset_k(event_type et) {
    for (auto & n : emap_k[et])
      n = {};
  }
  void reset_m(event_type et) {
    for (auto & n : emap_m[et])
      n = {};
  }
  void reset_g(event_type et) {
    for (auto & n : emap_g[et])
      n = {};
  }
}

extern "C" void casein_call(casein::event_type et) {
  if (auto fn = emap[et]) {
    fn();
  }
}
extern "C" void casein_call_g(casein::event_type et, casein::gestures g) {
  if (auto fn = emap_g[et][g]) {
    fn();
  }
  casein_call(et);
}
extern "C" void casein_call_k(casein::event_type et, casein::keys k) {
  if (auto fn = emap_k[et][k]) {
    fn();
  }
  casein_call(et);
}
extern "C" void casein_call_m(casein::event_type et, casein::mouse_buttons m) {
  if (auto fn = emap_m[et][m]) {
    fn();
  }
  casein_call(et);
}
