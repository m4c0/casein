module casein;
import :internal;

extern "C" void casein_enable_filedrop(bool);

bool casein::fullscreen {};
casein::native_handle_t casein::native_ptr {};
dotz::vec2 casein::mouse_pos {};
dotz::vec2 casein::mouse_rel {};
float casein::screen_scale_factor = 1.0;
bool casein::keydown_repeating {};
jute::view casein::window_title { "App" };
dotz::vec2 casein::window_size { 1280, 720 };
hai::varray<hai::cstr> casein::dropped_files {};

using fn_t = hai::fn<void>;
static fn_t emap[casein::MAX_EVENT_TYPE] {};
static fn_t emap_g[casein::MAX_EVENT_TYPE][casein::G_MAX] {};
static fn_t emap_k[casein::MAX_EVENT_TYPE][casein::K_MAX] {};
static fn_t emap_m[casein::MAX_EVENT_TYPE][casein::M_MAX] {};

namespace casein {
  void handle(event_type et, hai::fn<void> fn) {
    emap[et] = traits::move(fn);

    if (et == FILES_DROP) casein_enable_filedrop(!!fn);
  }
  void handle(event_type et, keys k, hai::fn<void> fn) { emap_k[et][k] = traits::move(fn); }
  void handle(event_type et, mouse_buttons m, hai::fn<void> fn) { emap_m[et][m] = traits::move(fn); }
  void handle(event_type et, gestures g, hai::fn<void> fn) { emap_g[et][g] = traits::move(fn); }

  void reset_k(event_type et) {
    for (auto &n : emap_k[et]) n = {};
  }
  void reset_m(event_type et) {
    for (auto &n : emap_m[et]) n = {};
  }
  void reset_g(event_type et) {
    for (auto &n : emap_g[et]) n = {};
  }
} // namespace casein

extern "C" void casein_call(casein::event_type et) {
  if (auto &fn = emap[et]) fn();
}
extern "C" void casein_call_g(casein::event_type et, casein::gestures g) {
  if (auto &fn = emap_g[et][g]) fn();
  casein_call(et);
}
extern "C" void casein_call_k(casein::event_type et, casein::keys k) {
  if (auto &fn = emap_k[et][k]) fn();
  casein_call(et);
}
extern "C" void casein_call_m(casein::event_type et, casein::mouse_buttons m) {
  if (auto fn = emap_m[et][m]) fn();
  casein_call(et);
}
