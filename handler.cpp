module casein;
import silog;

extern "C" casein::native_handle_t * casein_native_ptr = &casein::native_ptr;
extern "C" casein::point * casein_mouse_pos = &casein::mouse_pos;
extern "C" casein::point * casein_mouse_rel = &casein::mouse_rel;

using fn_t = void (*)();
static fn_t emap[casein::MAX_EVENT_TYPE] {};
static fn_t emap_g[casein::MAX_EVENT_TYPE][casein::G_MAX] {};
static fn_t emap_k[casein::MAX_EVENT_TYPE][casein::K_MAX] {};
static fn_t emap_m[casein::MAX_EVENT_TYPE][casein::M_MAX] {};

namespace casein {
  void handle(event_type et, void (*fn)()) {
    emap[et] = fn;
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
